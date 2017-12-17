#include "cabin.h"
#include "protocol.h"

void Cabin::lock() {
  packet.peerAddr = myAddr;
  dataBuffer[0] = CABIN_LOCK;
  (*protocol).sendPacket(&packet);  
}

void Cabin::unlock() {
  packet.peerAddr = myAddr;
  dataBuffer[0] = CABIN_UNLOCK;
  (*protocol).sendPacket(&packet);  
}

void Cabin::activateBreak() {  
  packet.peerAddr = breakAddr;
  dataBuffer[0] = BREAK_LOCK;
  (*protocol).sendPacket(&packet);   
}

void Cabin::deactivateBreak() {
  packet.peerAddr = breakAddr;
  dataBuffer[0] = BREAK_UNLOCK;
  (*protocol).sendPacket(&packet);  
}

// interrupt routine
void Cabin::freeFallHandler() {
  freeFallDetected = true;
}

void Cabin::checkFreeFall() {
  if (freeFallDetected) {
    freeFallDetected = false;
    activateBreak();
  }
}