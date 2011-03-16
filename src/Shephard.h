/*
 *  Shephard.h
 *  GoodShephard
 *
 *  Created by Andy on 2/20/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#ifndef GOODSHEPHARD_SHEPHARD_H
#define GOODSHEPHARD_SHEPHARD_H

#include <set>
#include "User.h"
#include "Session.h"
#include "http_packet.h"
#include "http_sniffer.h"

#define DEBUG_VAR(msg) if (verbose) cerr << #msg " = " << msg << endl
#define DEBUG_LINE(msg) if (verbose) cerr << #msg << endl
#define DEBUG_STRING(fmt, ...) if (verbose) fprintf(stderr, fmt, __VA_ARGS__)

using namespace std;

extern bool verbose;

typedef map <Session, User> SessionCache;
typedef set <User> UserSet;

class Shephard {
public:
   Shephard();
   ~Shephard();
   void received_packet(HttpPacket *packet);
   
private:
   HandlerList handlers;
   SessionCache cache;
   UserSet users;
   void printSessionCache();
   static CookieMap parseCookies(string cookies, CookieNameSet& cs);
   static string get_primary_domain(string domain);
   static void printCookieMap(CookieMap cm);
   static void insertCookiePair(string whole_pair, CookieMap& cm, CookieNameSet& cs);
};

#endif
