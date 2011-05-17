/*
 *  Shephard.cpp
 *  GoodShephard
 *
 *  Created by Andy on 2/20/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#include "Shephard.h"

Shephard::~Shephard() {
   cache.clear();
}

void Shephard::received_packet(HttpPacket *packet) {
   pair<UserSet::iterator, bool> check;
   CookieNameSet cs;
   logout_callback callback;
   int callback_return;
   SessionCache::const_iterator thisCacheEntry;
   
   //retrieve primary domain from URL
   string primary = Shephard::get_primary_domain(packet->host());
   //check for error condition from get_primary_domain
   if (primary.empty()) {
      cerr << "Invalid URL: " << primary << endl;
      //skip packet
      return;
   }
   
   DEBUG_STRING("primary domain: %s", primary.c_str());
   
   //check primary domain against list of handler URL's
   cs = handlers.getCookieNames(primary);
   
   //if no match in handlers, skip packet
   if (cs.empty()) {
      return;
   }
   
   DEBUG_STRING("to: %s\n", packet->to().c_str());
   DEBUG_STRING("from: %s\n", packet->from().c_str());
   DEBUG_STRING("host: %s\n", packet->host().c_str());
   DEBUG_STRING("user-agent: %s\n", packet->user_agent().c_str());
   DEBUG_STRING("cookies: %s\n", packet->cookies().c_str());
   
   //we have a handler, parse cookies and create new cookie map
   CookieMap cm = parseCookies(packet->cookies(), cs);
   
   //if no relevant cookies found, packet is of no use
   if (cm.empty()) {
      DEBUG_LINE("No cookies matching handler found, skipping packet...");
      DEBUG_LINE("------------------------</Shephard>-------------------------");
      return;
   }
   printCookieMap(cm);
   
   //then create new Session from domain and cookies from handler
   Session sesh(primary, cm);
   
   //chop off port number and semi-colon of from field leaving only IP
   string ip = (packet->from().substr(0, packet->from().find_first_of(':')));
   
   //create new User for this packet
   User user(packet->user_agent(), ip);
   
   //insert this new User into the UserSet
   check = users.insert(user);
   
   //SessionUserPair.first = sesh;
   //SessionUserPair.second = user;
   
   thisCacheEntry = cache.find(sesh);
   
   //TODO: make seperate method to check for collisions
   //that returns true if ANY cookie values match
   
   //if session is already in cache
   if (thisCacheEntry != cache.end()) {
      //if user associated with this session is different than that
      //in cache, this 'should be' a sidejacking attempt
      if (thisCacheEntry->second != user) {
         cout << "Sidejacking attempt!" << endl;
         
         //acquire callback function
         callback = handlers.getCallback(primary);
         
         //call callback function
         callback_return = callback(cm);
         
         if (callback_return!=EXIT_SUCCESS) {
            cerr << "callback for " << primary << " failed." << endl;
         }
      }
   }
   //session is not already in cache
   else {
      //insert this session and user into the cache (map)
      cache.insert(pair<Session, User>(sesh, user));
      //cache.insert(SessionUserPair); 
      DEBUG_LINE("Inserted new entry into cache:\n");
      DEBUG_STRING("session: %s\n", sesh.toString().c_str());
      DEBUG_STRING("user: %s\n", user.toString().c_str());
   }
   
   printSessionCache();
   
   //cerr << "Users: " << users.size() << endl;
   
   DEBUG_LINE("------------------------</Shephard>-------------------------");
}

void Shephard::printSessionCache() {
   SessionCache::const_iterator it = cache.begin();
   
   DEBUG_LINE("------------Session Cache:---------------");
   for (; it != cache.end(); it++ ) {
      Session sesh = it->first;
      User user = it->second;
      DEBUG_STRING("domain: %s\n", sesh.getDomain().c_str());
      DEBUG_STRING("cookie: %s\n", sesh.getCookieString().c_str());
      DEBUG_STRING("user: %s & %s\n", user.getIP().c_str(), user.getUserAgent().c_str());
   }
}

void Shephard::printCookieMap(CookieMap cm) {
   CookieMap::const_iterator it = cm.begin();
   
   if (it==cm.end()) {
      DEBUG_LINE("cookie map is empty!");
   }
   for (; it != cm.end(); it++ ) {
      //cerr << it->first << "=" << it->second << endl;
      DEBUG_STRING("%s=%s\n", (it->first).c_str(), (it->second).c_str());
   }
}

/*
 * parses cookie into new CookieMap using return by value
 * param cs: set of cookie names relevant to this session
 * TODO: allocate cookie map dynamically and return by reference...
 *    this would require deallocating everything
 */
CookieMap Shephard::parseCookies(string cookies, CookieNameSet& cs) {
   CookieMap cm;
   size_t found_semi;
   size_t last_semi = 0;

   //parse cookies into CookieMap using semi-colons
   do {
      //look for semi-colon in cookie string, starting at last found semi-colon
      found_semi = cookies.find(';', last_semi+1);
      //only continue if semi-colon found (if not, break from while)
      if (found_semi!=string::npos) {
         //take just this pair to avoid mistakes in going past this pair
         string whole_pair(cookies.substr(last_semi, found_semi - last_semi));
         insertCookiePair(whole_pair, cm, cs);
      }
      //if no more semi-colons, check for last pair
      else if (last_semi < cookies.size()) {
         //get the rest of the string after the last semi-colon
         string whole_pair(cookies.substr(last_semi+1, cookies.size()));
         insertCookiePair(whole_pair, cm, cs);
      }
      //record location of last seen semi-colon
      last_semi = found_semi;
   } while (found_semi != string::npos);

   //DEBUG_STRING("Leaving parseCookies with cm of size: %d\n", (int)cm.size());
   return cm;
}

/*
 * parses a pair of cookie name and value and inserts the pair into the map
 * only if the cookie name is in the set cs
 * param cs: set of cookie names relevant to this session
 */
void Shephard::insertCookiePair(string whole_pair, CookieMap& cm, CookieNameSet& cs) {
   //find equals character in this pair
   size_t found_equal = whole_pair.find('=', 0);
   //if equals character found
   if (found_equal != string::npos) {
      size_t space, semi;
      string key(whole_pair.substr(0, found_equal));
      string val(whole_pair.substr(found_equal+1, string::npos));
      //get rid of previous trailing semi-colon and space if there
      if ((space = key.find(' ', 0))!=string::npos) {
         key.erase(0, space+1);
      }
      if ((semi = key.find(';', 0))!=string::npos) {
         key.erase(0, semi+1);
      }
      
      //only add cookie if it's in the CookieNameSet
      if (cs.find(key)!=cs.end()) {
         //add pair to CookieMap
         cm[key] = val;
         //DEBUG_STRING("Added %s=%s\n", key.c_str(), val.c_str());
      }
   }
}

Shephard::Shephard() {
   //register all handlers
}

string Shephard::get_primary_domain(string domain) {
   //instantiate local string variable
   string primary = domain;
   //strip down URL to primary domain
   size_t last_dot = primary.rfind('.');
   //if URL contains no periods, it's malformed - skip the packet
   if (last_dot==string::npos) {
      //TODO: test this condition by hand-crafting bad packet
      return NULL;
   }
   //find second to last period if it exists
   size_t penultimate_dot = primary.rfind('.', last_dot-1);
   //if it exists, use everything after it as the primary domain string
   if (penultimate_dot!=string::npos) {
      primary = primary.substr(penultimate_dot+1);
   }
   return primary;
}
