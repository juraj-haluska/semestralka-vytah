#ifndef _ENGINE_H
#define _ENGINE_H

#include "protocol.h"

#define CMD_STOP    0x01
#define CMD_MOVE    0x02
#define CMD_REQUEST 0x03

class Engine {
private:
  static const int maxDataLength = 5;
  uint8_t myAddr;
  Protocol * protocol;
  uint8_t dataBuffer[maxDataLength];
  packet_t packet;
  double encoderCount;
public:
  Engine(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) {
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dynamic = false;    
  };
  void stop();
  void move(int32_t speed);
  void requestEncoderCount();
  double getLastEncoderCount();
  void handlePacket(packet_t *packet);
};

#endif /* _ENGINE_H */