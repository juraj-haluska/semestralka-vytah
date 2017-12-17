#ifndef _CABIN_H
#define _CABIN_H

#include "protocol.h"

#define CABIN_LOCK      0x01
#define CABIN_UNLOCK    0x00
#define BREAK_UNLOCK    0x00
#define BREAK_LOCK      0x01

class Cabin {
private:
  static const int maxDataLength = 1;
  uint8_t myAddr;
  uint8_t breakAddr;
  uint8_t dataBuffer[maxDataLength];
  packet_t packet;
  Protocol *protocol;
  InterruptIn * freeFall;
  void freeFallHandler();
  bool freeFallDetected;
public:
  Cabin(uint8_t _myAddr, uint8_t _breakAddr, Protocol *_protocol, InterruptIn * _freeFall): 
    myAddr(_myAddr), 
    breakAddr(_breakAddr), 
    protocol(_protocol),
    freeFall(_freeFall)
    {
      packet.data = dataBuffer;
      packet.dataLength = maxDataLength;
      packet.dynamic = false;

      freeFallDetected = false;

      // activate external interrupt
      freeFall->fall(callback(this, &Cabin::freeFallHandler));
  };
  void lock();
  void unlock();
  void activateBreak();
  void deactivateBreak();
  void checkFreeFall();
};

#endif /* _CABIN_H */