# makefile for CPE 456 Program 2
# Andy Nunes

CC=g++
CFLAGS = -g -Wall -Werror -ansi \
	$(PCAP_CFLAGS) $(BOOST_CPPFLAGS)
TARGET = GoodShephard

all: $(TARGET)

INCLUDES = -I deps/ -I deps/http-parser -I src/ -I deps/firesheep
	
LIBS = -lpcap $(BOOST_FORMAT_LIBS) $(BOOST_STRING_ALGO_LIBS)

SOURCES = src/main.cpp src/Shephard.cpp src/Session.cpp src/User.cpp src/Handlers.cpp deps/firesheep/http_sniffer.cpp deps/firesheep/http_packet.cpp deps/http-parser/http_parser.c

$(TARGET): $(SOURCES) Makefile
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(SOURCES) -o $@

clean:
	rm -rf core* *.o *.gch *.dSYM $(TARGET)
