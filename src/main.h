/*
 *  main.h
 *  GoodShephard
 *
 *  Created by Andy on 2/20/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#ifndef GOODSHEPHARD_MAIN_H
#define GOODSHEPHARD_MAIN_H

#include <string>
#include "http_sniffer.h"
#include "http_packet.h"
#include "Shephard.h"

using namespace std;

bool verbose = false;
bool offline = false;

#define DEBUG_VAR(msg) if (verbose) cerr << #msg " = " << msg << endl
#define DEBUG_LINE(msg) if (verbose) cerr << #msg << endl
#define DEBUG_STRING(fmt, ...) if (verbose) fprintf(stderr, fmt, __VA_ARGS__)

#define PRINT_LINE(msg) cout << #msg << endl;
#define OUT_LINE(fmt, ...) fprintf(out, fmt, __VA_ARGS__);

#define EXIT_USAGE PRINT_LINE(usage: \
   argv[0] [-v] [-offline pcap_file]) \
   exit(EXIT_FAILURE);

#endif
