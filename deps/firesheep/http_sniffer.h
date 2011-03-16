//
// http_sniffer.h
// Part of the Firesheep project.
//
// Copyright (C) 2010 Eric Butler
//
// Authors:
//   Eric Butler <eric@codebutler.com>
//   Nick kossifidis <mickflemm@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GOODSHEPHARD_HTTP_SNIFFER_H
#define GOODSHEPHARD_HTTP_SNIFFER_H

#include <string>
#include <sys/types.h>
#include "http_packet.h"
#include "Shephard.h"

using namespace std;

extern bool offline;

typedef map <string, HttpPacket *> PacketCacheMap;

class HttpSniffer
{
public:
  HttpSniffer (string first, string filter, http_packet_cb callback);
  void start();

protected:
  string    m_iface;
  string    m_filter;
  string    m_pcap_filename;
  http_packet_cb  m_callback;
  bool    m_wifimon;
  bool    m_offline;
  
  static void got_packet_wrapper(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    HttpSniffer *sniffer = (HttpSniffer *) user;
    sniffer->got_packet(header, packet);
  }
  
private:
  PacketCacheMap m_pending_packets;
  void got_packet(const struct pcap_pkthdr *header, const u_char *packet);
};

#endif
