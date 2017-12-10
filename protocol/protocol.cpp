#include "protocol.h"
#include "mbed.h"

extern Serial help;

void Protocol::onReceived() {
  //help.printf("on received called\r\n");

  // first byte in buffer must by START_BYTE
  if (byteBuffer.size() >= 1 && byteBuffer[0] != START_BYTE) {
    byteBuffer.clear();
  }
  
  uint8_t received = serial.getc();
  //help.printf("byte received: %x\r\n", received);

  // add received byte to buffer
  byteBuffer.push_back(received);

  // check if ACK was received
  if (byteBuffer.size() == sizeof(ACK)) {
    uint8_t match = 0;
    for (uint8_t i = 0; i < byteBuffer.size(); i++) {
      if (byteBuffer[i] == ACK[i]) {
        match++;
      }
    }
    if (match == byteBuffer.size()) {
      // ack received
      byteBuffer.clear();
      callback(NULL);
    }
  }
}