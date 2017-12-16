#include "cabin.h"
#include "protocol.h"

void Cabin::lock() {
  packet.peerAddr = myAddr;
  dataBuffer[0] = CABIN_LOCK;
  (*protocol).getOutQueue().put(&packet);  
}

void Cabin::unlock() {
  packet.peerAddr = myAddr;
  dataBuffer[0] = CABIN_UNLOCK;
  (*protocol).getOutQueue().put(&packet);  
}

void Cabin::activateBreak() {  
  packet.peerAddr = breakAddr;
  dataBuffer[0] = BREAK_LOCK;
  (*protocol).getOutQueue().put(&packet);   
}

void Cabin::deactivateBreak() {
  packet.peerAddr = breakAddr;
  dataBuffer[0] = BREAK_UNLOCK;
  (*protocol).getOutQueue().put(&packet);
}