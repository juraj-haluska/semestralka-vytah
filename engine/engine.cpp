#include "engine.h"
#include "protocol.h"

void Engine::move(int32_t speed) {
  dataBuffer[0] = CMD_MOVE;
  dataBuffer[1] = *(((uint8_t *) &speed) + 0);
  dataBuffer[2] = *(((uint8_t *) &speed) + 1);
  dataBuffer[3] = *(((uint8_t *) &speed) + 2);
  dataBuffer[4] = *(((uint8_t *) &speed) + 3);
  packet.dataLength = 5;
  (*protocol).sendPacket(&packet);  
}

void Engine::stop() {
  dataBuffer[0] = CMD_STOP;
  packet.dataLength = 1;
  (*protocol).sendPacket(&packet);  
}

void Engine::requestEncoderCount() {  
  dataBuffer[0] = CMD_REQUEST;
  packet.dataLength = 1;
  (*protocol).sendPacket(&packet); 
}

double Engine::getLastEncoderCount() {
  return encoderCount;
}

void Engine::handlePacket(packet_t *packet) {
  if (packet->peerAddr != myAddr) return;
  if (packet->data[0] == CMD_REQUEST && packet->dataLength >= 9) {
    encoderCount = *((double *) (&(packet->data[1])));
  }
}