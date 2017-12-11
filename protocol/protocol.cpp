#include "protocol.h"
#include "mbed.h"

extern Serial help;

void Protocol::onReceived() {
  // first byte in buffer must by START_BYTE
  if (byteBuffer.size() >= PACKET_SB_POS && byteBuffer[PACKET_SB_POS - 1] != START_BYTE) {
    byteBuffer.clear();
  }
  
  const uint8_t received = serial.getc();

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
      return;
    }
  }

  static uint8_t expectedDataLength = 0;
  static uint8_t actualDataLength = 0;
  static uint8_t crc = 0;

  // check if length byte is on top of the buffer
  if (byteBuffer.size() == PACKET_DL_POS) {
    expectedDataLength = byteBuffer[PACKET_DL_POS - 1];
    actualDataLength = 0;
    crc = 0;

    help.printf("expected length: %d\r\n", expectedDataLength);

    // precalculate crc from addresses
    crc = CRC8_TAB[crc ^ byteBuffer[PACKET_RA_POS - 1]];
    crc = CRC8_TAB[crc ^ byteBuffer[PACKET_TA_POS - 1]];
    return;
  }

  if (byteBuffer.size() > PACKET_DL_POS && actualDataLength < expectedDataLength) {
    // calculate crc of data byte
    crc = CRC8_TAB[crc ^ byteBuffer.back()];
    actualDataLength++;
    return;
  }

  // +1 for crc
  if (actualDataLength >= expectedDataLength) {
    // packet complete
    help.printf("packet received\r\n");
    byteBuffer.clear();
  }
}