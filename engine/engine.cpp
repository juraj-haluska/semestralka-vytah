#include "engine.h"
#include "protocol.h"

void Engine::move(int32_t speed) {
  dataBuffer[0] = CMD_MOVE;
  dataBuffer[1] = (uint8_t) (0xFF & (speed >> 8 * 3));
  dataBuffer[2] = (uint8_t) (0xFF & (speed >> 8 * 2));
  dataBuffer[3] = (uint8_t) (0xFF & (speed >> 8 * 1));
  dataBuffer[4] = (uint8_t) (0xFF & speed);
  packet.dataLength = 5;
  (*protocol).getOutQueue().put(&packet);   
}

void Engine::stop() {
  dataBuffer[0] = CMD_STOP;
  packet.dataLength = 1;
  (*protocol).getOutQueue().put(&packet);    
}

void Engine::requestEncoderCount() {  
  dataBuffer[0] = CMD_REQUEST;
  packet.dataLength = 1;
  (*protocol).getOutQueue().put(&packet);    
}