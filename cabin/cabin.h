#ifndef _CABIN_H
#define _CABIN_H

#include "protocol.h"

#define CABIN_LOCK      0x00
#define CABIN_UNLOCK    0x01
#define BREAK_UNLOCK    0x00
#define BREAK_LOCK      0x01

class Cabin {
private:
  uint8_t myAddr;
  uint8_t breakAddr;
  Protocol *protocol;
public:
  Cabin(uint8_t _myAddr, uint8_t _breakAddr, Protocol *_protocol): myAddr(_myAddr), breakAddr(_breakAddr), protocol(_protocol) {};
  void lock();
  void unlock();
  void activateBreak();
  void deactivateBreak();
};

#endif /* _CABIN_H */