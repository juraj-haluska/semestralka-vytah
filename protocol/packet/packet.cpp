#include "packet.h"

void Packet::addByte(uint8_t byte) {
  data.push_back(byte);
}

uint8_t Packet::getPeerAddr() {
  return peerAddr;
}

std::vector<uint8_t> & Packet::getData() {
  return data;
}