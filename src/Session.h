/*
 *  Session.h
 *  GoodShephard
 *
 *  Created by Andy on 2/19/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#ifndef GOODSHEPHARD_SESSION_H
#define GOODSHEPHARD_SESSION_H

#include <map>
#include <string>
#include "Handlers.h"

#define DEBUG_VAR(msg) if (verbose) cerr << #msg " = " << msg << endl
#define DEBUG_LINE(msg) if (verbose) cerr << #msg << endl
#define DEBUG_STRING(fmt, ...) if (verbose) fprintf(stderr, fmt, __VA_ARGS__)

using namespace std;

extern bool verbose;

typedef pair<string, string> Cookie;
typedef map<string, string> CookieMap;

class Session {
public:
   Session(string site, CookieMap cm);
   string getDomain() const;
   string getCookieValue(string cookieName) const;
   string getCookieString() const;
   string toString() const;
   bool operator== (const Session &s1) const;
   bool operator< (const Session &o) const;

private:
   string domain;
   CookieMap cookies;
};

#endif
