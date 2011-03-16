/*
 *  Session.cpp
 *  GoodShephard
 *
 *  Created by Andy on 2/19/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#include "Session.h"

/*
 * Default constructor
 * parameter site: name of primary domain of session
 * parameter cs: map of cookies in session
 */
Session::Session(string site, CookieMap cm) {
   domain = site;
   cookies = cm;
}

/*
 * Stringifier method
 */
string Session::toString() const{
   return this->getDomain() + " + " + this->getCookieString() + "\n";
}

/*
 * Accessor method for domain
 */
string Session::getDomain() const {
   return domain;
}

/*
 * stringifier for cookies
 */
string Session::getCookieString() const {
   string result;
   
   CookieMap::const_iterator it;
   for (it = cookies.begin(); it!=cookies.end(); it++) {
      result += it->first + ": " + it->second + "\n";
   }
   
   return result;
}

/*
 * grab value of cookie out of CookieMap
 */
string Session::getCookieValue(string cookieName) const{
   CookieMap::const_iterator it = cookies.find(cookieName);
   if (it==cookies.end()) {
      return NULL;
   }
   return it->second;
}

/*
 * Greater than comparator overload method
 */
bool Session::operator< (const Session &o) const{
   bool result = !(this->operator==(o));
//   DEBUG_LINE("operator< called:");
//   cerr << "first session: " << o.toString();
//   cerr << "second session: " << this->toString();
//   cerr << "result is " << result << endl;
   return result;
}

/*
 * Equality operator overload
 */
bool Session::operator== (const Session &s1) const{
   if (s1.getDomain()!=this->getDomain()) {
      return false;
   }
   
   CookieMap::const_iterator it;
   for (it = cookies.begin(); it!=cookies.end(); it++) {
      if (it->second!=s1.getCookieValue(it->first)) {
         return false;
      }
   }
   return true;
}
