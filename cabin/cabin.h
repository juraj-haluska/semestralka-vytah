#ifndef _CABIN_H
#define _CABIN_H

#include "protocol.h"

#define CABIN_LOCK      0x00
#define CABIN_UNLOCK    0x01

class Cabin {
private:
  uint8_t myAddr;
  Protocol *protocol;
public:
  Cabin(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) {};
  void lock();
  void unlock();
};

#endif /* _CABIN_H */