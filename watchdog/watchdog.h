#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include "protocol.h"

#define WATCHDOG_PERIOD  800   
#define WATCHDOG_RESET   0x01
#define WATCHDOG_CLEAR   0x02

class Watchdog {
private:
  static const int maxDataLength = 1;
  uint8_t myAddr;
  Protocol * protocol;
  Timer timer;
  uint8_t dataBuffer[maxDataLength];
  packet_t packet;
public:
  Watchdog(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) {
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dataLength = maxDataLength;
    packet.dynamic = false;    
    timer.start();
  };
  void watchdogCheck();
};

#endif /* _WATCHDOG_H */