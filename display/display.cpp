#include "display.h"
#include "protocol.h"

void Display::setDirectionUp(int floor) {  
  lastFloor = floor;
  
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = DIRECT_UP;
  packet->data[1] = floor + '0';
  packet->dataLength = 2;
  
  (*protocol).getOutMailbox().put(packet);
}

void Display::setDirectionDown(int floor) {
  lastFloor = floor;
  
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = DIRECT_DOWN;
  packet->data[1] = floor + '0';
  packet->dataLength = 2;
  
  (*protocol).getOutMailbox().put(packet);
}

void Display::setDirectionNone(int floor) {
  lastFloor = floor;
  
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = DIRECT_NONE;
  packet->data[1] = floor + '0';
  packet->dataLength = 2;
  
  (*protocol).getOutMailbox().put(packet);  
}

int Display::getLastFloor() {
    return lastFloor;    
}
