#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "protocol.h"

#define DIRECT_UP    0x01
#define DIRECT_DOWN  0x02
#define DIRECT_NONE  0x03

class Display {
private:
  static const int maxDataLength = 2;
  uint8_t myAddr;
  Protocol *protocol;
  int lastFloor;
  uint8_t dataBuffer[maxDataLength];
  packet_t packet;
public:
  Display(uint8_t _myAddress, Protocol *_protocol): myAddr(_myAddress), protocol(_protocol) {
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dataLength = maxDataLength;
    packet.dynamic = false;    
  };
  void setDirectionUp(int floor);
  void setDirectionDown(int floor);
  void setDirectionNone(int floor);
  int getLastFloor();
};

#endif /* _DISPLAY_H */