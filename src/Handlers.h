/*
 *  Handlers.h
 *  GoodShephard
 *
 *  Created by Andy on 2/20/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#ifndef GOODSHEPHARD_HANDLERS_H
#define GOODSHEPHARD_HANDLERS_H

#include <set>
#include <map>
#include <string>
#include <iostream>

using namespace std;

typedef map<string, string> CookieMap;
typedef set<string> CookieNameSet;
typedef int (*logout_callback) (CookieMap);

class Handler {
public:
   Handler(string domainName, CookieNameSet cookies, logout_callback cb);
   string getDomain();
   CookieNameSet getCookieNames();
   logout_callback getCallback();
   bool operator< (const Handler &o) const;
   
private:
   string domain;
   CookieNameSet cookieNames;
   logout_callback callback;
};

typedef map<string, Handler> HandlerMap;
typedef pair<string, Handler> HandlerPair;

class HandlerList {
public:
   HandlerList();
   logout_callback getCallback(string name);
   CookieNameSet getCookieNames(string name);
   
private:
   void createAndAddHandler(string domainName, logout_callback cb, uint32_t count, ... );
   HandlerMap myHandlers;
};

int handler_amazon(CookieMap);
int handler_facebook(CookieMap);
int handler_yahoo(CookieMap);
#endif
