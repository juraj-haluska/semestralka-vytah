#ifndef _LED_PANEL
#define _LED_PANEL

#include "protocol.h"

#define LED_OFF 0x00
#define LED_ON  0x01

class LedPanel {
private:
  uint8_t myBaseAddr;
  Protocol *protocol;    
public:
  LedPanel(uint8_t _myBaseAddr, Protocol *_protocol): myBaseAddr(_myBaseAddr), protocol(_protocol) {};
  void setLed(int led);
  void unsetLed(int led);
};


#endif /* _LED_PANEL */