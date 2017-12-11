#ifndef PACKET_H_
#define PACKET_H_

#include <vector>
#include <inttypes.h>

class Packet {
private:
  uint8_t peerAddr;
  std::vector<uint8_t> data; 
public:
  Packet(uint8_t _peerAddr): peerAddr(_peerAddr) { }
  void addByte(uint8_t byte);
  uint8_t getPeerAddr();
  std::vector<uint8_t> &getData(); 
};

#endif /* PACKET_H_ */