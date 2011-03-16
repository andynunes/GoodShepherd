/*
 *  main.cpp
 *  GoodShephard
 *
 *  Created by Andy on 2/19/11.
 *  Copyright 2011 Andy Nunes. All rights reserved.
 *
 */

#include "main.h"

Shephard* saint;

void received_packet(HttpPacket *packet);

int main(int argc, const char* argv[]) {
   
   //default pcap capture filter
   string filter("tcp port 80");
   //default pcap interface name
   string iface("en1");
   //default pcap capture file name
   string pcap_file_name("");
   
   for (int cur_arg=1; cur_arg<argc; cur_arg++) {
		//check for verbose option
		if (strcmp(argv[cur_arg], "-v")==0) {
			verbose = true;
		}
      else if (strcmp(argv[cur_arg], "-offline")==0) {
         if (++cur_arg<argc) {
            DEBUG_STRING("Running in offline mode with file %s\n", argv[cur_arg]);
            offline = true;
            iface.assign(argv[cur_arg]);
         }
         else {
            EXIT_USAGE
         }
      }
   }

   Shephard* Jesus = new Shephard();
   saint = Jesus;

   try {
      HttpSniffer sniffer(iface, filter, received_packet);
      sniffer.start();
   } catch (exception &e) {
      cerr << e.what() << endl;
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

/*
 * Wrapper for Shephard's received_packet method
 */
void received_packet(HttpPacket *packet) {
   saint->received_packet(packet);
}

