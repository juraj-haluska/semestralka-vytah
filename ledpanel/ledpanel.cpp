#include "ledpanel.h"
#include "protocol.h"

void LedPanel::setLed(int led) {  
  packet.peerAddr = myBaseAddr | led;
  dataBuffer[0] = LED_ON;
  (*protocol).getOutQueue().put(&packet); 
}

void LedPanel::unsetLed(int led) {  
  packet.peerAddr = myBaseAddr | led;
  packet.data[0] = LED_OFF;
  (*protocol).getOutQueue().put(&packet);
}