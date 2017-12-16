#include "cabin.h"
#include "protocol.h"

void Cabin::lock() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = CABIN_LOCK;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);
}

void Cabin::unlock() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = CABIN_UNLOCK;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);
}

void Cabin::activateBreak() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = breakAddr;
  packet->data[0] = BREAK_LOCK;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);    
}

void Cabin::deactivateBreak() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = breakAddr;
  packet->data[0] = BREAK_UNLOCK;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet); 
}