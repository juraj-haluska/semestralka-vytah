#include "packet.h"

void Packet::addByte(uint8_t byte) {
  data.push_back(byte);
}

uint8_t Packet::getRecvAddr() {
  return recvAddr;
}

std::vector<uint8_t> const& Packet::getData() {
  return data;
}