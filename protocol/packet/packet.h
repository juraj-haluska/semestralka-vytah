#ifndef PACKET_H_
#define PACKET_H_

#include <vector>
#include <inttypes.h>

class Packet {
private:
  uint8_t recvAddr;
  std::vector<uint8_t> data; 
public:
  Packet(uint8_t _recvAddr): recvAddr(_recvAddr) { }
  void addByte(uint8_t byte);
  uint8_t getRecvAddr();
  std::vector<uint8_t> const& getData(); 
};

#endif /* PACKET_H_ */