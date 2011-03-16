/*
 *  User.cpp
 *  GoodShephard
 *
 *  Created by Andy on 2/19/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#include "User.h"

/*
 * Default constructor
 */
User::User(string agent, string pi) {
   UserAgent = agent;
   ip = pi;
}

/*
 * Greater than comparator overload method
 */
bool User::operator< (const User &o) const{
   return this->ip < o.ip;
}

/*
 * Equality overload method
 */
bool User::operator== (const User &o) const{
   return ((this->ip==o.ip) && (this->UserAgent==o.UserAgent));
}

/*
 * Non-equality overload method
 */
bool User::operator!= (const User &o) const{
   return !(this->operator== (o));
}

/*
 * Accessor method for UserAgent
 */
string User::getUserAgent() {
   return UserAgent;
}

/*
 * Accessor method for ip
 */
string User::getIP() {
   return ip;
}

/*
 * Stringifier method
 */
string User::toString() {
   return UserAgent + " + " + ip + "\n";
}
