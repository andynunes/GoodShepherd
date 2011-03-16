/*
 *  Shephard.cpp
 *  GoodShephard
 *
 *  Created by Andy on 2/20/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#include "Handlers.h"

/*
 * Default constructor for Handler
 */
Handler::Handler(string domainName, CookieNameSet cookies, logout_callback cb) {
   domain = domainName;
   cookieNames = cookies;
   callback = cb;
}

/*
 * Accessor method for domain
 */
string Handler::getDomain() {
   return domain;
}

/*
 * Accessor method for callback
 */
logout_callback Handler::getCallback() {
   return callback;
}

/*
 * Accessor method for cookieNames
 */
CookieNameSet Handler::getCookieNames() {
   return cookieNames;
}

/*
 * Greater than comparator overload method
 */
bool Handler::operator< (const Handler &o) const{
   return this->domain < o.domain;
}


/* ------------HandlerList implementation------------------ */

/*
 * Default constructor for our list of handlers
 */
HandlerList::HandlerList() {
   createAndAddHandler("facebook.com", handler_facebook, 2, "xs", "c_user");
   createAndAddHandler("yahoo.com", handler_yahoo, 2, "T", "Y");
   createAndAddHandler("amazon.com", handler_amazon, 1, "x-main");
}

logout_callback HandlerList::getCallback(string name) {
   HandlerMap::iterator it;
   //find Handler by name
   it = myHandlers.find(name);
   //get cookie names from that Handler
   return (it->second).getCallback();
}

/*
 * Accessor to just cookie names of a Handler in this list
 * parameter name: name of Handler to get cookies from
 * returns a Set of strings holding all cookie names in that Handler
 */
CookieNameSet HandlerList::getCookieNames(string name) {   
   HandlerMap::iterator it;
   //find Handler by name
   it = myHandlers.find(name);
   //if handler not found, return empty set!
   if (it==myHandlers.end()) {
      return CookieNameSet();
   }
   //get cookie names from that Handler
   return (it->second).getCookieNames();
}

/*
 * Adder method for individual handlers
 */
void HandlerList::createAndAddHandler(string domainName, logout_callback cb, uint32_t count, ... ) {
   CookieNameSet cookies;
   pair<HandlerMap::iterator, bool> ret;
   
   //use variable argument list
   va_list listPointer;
   va_start(listPointer, count);
   //insert all variable arguments into set of cookies
   for (uint32_t i=0; i<count; i++) {
      cookies.insert(string(va_arg(listPointer, char *)));
   }
   //create new handler with cookies and callback provided
   Handler* temp = new Handler(domainName, cookies, cb);
   
   //add newly created handler to local set of handlers
   ret = myHandlers.insert(HandlerPair(domainName, *temp));
   
   //check whether handler was successfully added
   if (ret.second==false) {
      cerr << "Attempted to add a duplicate handler..." << endl;
   }
}

int handler_amazon(CookieMap) {
   cout << "Amazon logout handler called! Yay for sidejacking!" << endl;
   return EXIT_SUCCESS;
}

int handler_facebook(CookieMap) {
   cout << "Facebook logout handler called! Yay for sidejacking!" << endl;
   return EXIT_SUCCESS;
}

int handler_yahoo(CookieMap) {
   cout << "Yahoo logout handler called! Yay for sidejacking!" << endl;
   return EXIT_SUCCESS;
}