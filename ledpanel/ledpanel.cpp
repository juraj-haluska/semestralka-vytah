#include "ledpanel.h"
#include "protocol.h"

void LedPanel::setLed(int led) {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myBaseAddr | led;
  packet->data[0] = LED_ON;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);
}

void LedPanel::unsetLed(int led) {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myBaseAddr | led;
  packet->data[0] = LED_OFF;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);  
}