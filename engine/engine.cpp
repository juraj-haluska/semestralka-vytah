#include "engine.h"
#include "protocol.h"

void Engine::move(int32_t speed) {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = CMD_MOVE;
  packet->data[1] = (uint8_t) (0xFF & (speed >> 8 * 3));
  packet->data[2] = (uint8_t) (0xFF & (speed >> 8 * 2));
  packet->data[3] = (uint8_t) (0xFF & (speed >> 8 * 1));
  packet->data[4] = (uint8_t) (0xFF & speed);
  packet->dataLength = 5;
  
  (*protocol).getOutMailbox().put(packet);    
}

void Engine::stop() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = CMD_STOP;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);    
}

void Engine::requestEncoderCount() {
  packet_t *packet = (*protocol).getOutMailbox().alloc();
  
  packet->peerAddr = myAddr;
  packet->data[0] = CMD_REQUEST;
  packet->dataLength = 1;
  
  (*protocol).getOutMailbox().put(packet);  
}