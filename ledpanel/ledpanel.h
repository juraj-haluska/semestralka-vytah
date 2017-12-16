#ifndef _LED_PANEL
#define _LED_PANEL

#include "protocol.h"

#define LED_OFF 0x00
#define LED_ON  0x01

class LedPanel {
private:
  static const int maxDataLength = 1;
  uint8_t myBaseAddr;
  Protocol *protocol;
  uint8_t dataBuffer[maxDataLength];
  packet_t packet;
public:
  LedPanel(uint8_t _myBaseAddr, Protocol *_protocol): myBaseAddr(_myBaseAddr), protocol(_protocol) {
    packet.data = dataBuffer;
    packet.dynamic = false;
    packet.dataLength = maxDataLength;
  };
  void setLed(int led);
  void unsetLed(int led);
};


#endif /* _LED_PANEL */