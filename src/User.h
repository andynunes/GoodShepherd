/*
 *  User.h
 *  GoodShephard
 *
 *  Created by Andy on 2/19/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#ifndef GOODSHEPHARD_USER_H
#define GOODSHEPHARD_USER_H

#include <string>

using namespace std;

class User {
public:
   User(string agent, string ip);
   bool operator< (const User &o) const;
   bool operator== (const User &o) const;
   bool operator!= (const User &o) const;
   string getUserAgent();
   string getIP();
   string toString();
   
private:
   string UserAgent;
   string ip;
};

#endif
