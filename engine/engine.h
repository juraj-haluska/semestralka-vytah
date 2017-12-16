#ifndef _ENGINE_H
#define _ENGINE_H

#include "protocol.h"

#define CMD_STOP    0x01
#define CMD_MOVE    0x02
#define CMD_REQUEST 0x03

class Engine {
private:
  uint8_t myAddr;
  Protocol * protocol;

public:
  Engine(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) { };
  void stop();
  void move(int32_t speed);
  void requestEncoderCount();
};

#endif /* _ENGINE_H */