#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "protocol.h"

#define DIRECT_UP    0x01
#define DIRECT_DOWN  0x02
#define DIRECT_NONE  0x03

#define DEFAULT_FLOOR

class Display {
  uint8_t myAddr;
  Protocol *protocol;
  int lastFloor;
public:
  Display(uint8_t _myAddress, Protocol *_protocol): myAddr(_myAddress), protocol(_protocol) {};
  void setDirectionUp(int floor);
  void setDirectionDown(int floor);
  void setDirectionNone(int floor);
  int getLastFloor();
};

#endif /* _DISPLAY_H */