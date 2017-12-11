#include "protocol.h"
#include "mbed.h"

extern Serial help;

void Protocol::newPacket() {
  crc = 0;
  byteBuffer.clear();
}

void Protocol::onReceived() {
  // first byte in buffer must by START_BYTE
  if (byteBuffer.size() >= PACKET_SB_POS && byteBuffer[PACKET_SB_POS - 1] != START_BYTE) {
    newPacket();
  }
  
  // add received byte to buffer
  byteBuffer.push_back(serial.getc());

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
      newPacket();
      callback(NULL);
      return;
    }
  }

  // calculate crc if address's present
  if (byteBuffer.size() == PACKET_RA_POS) {
    crc = CRC8_TAB[crc ^ byteBuffer[PACKET_RA_POS - 1]];
  }

  if (byteBuffer.size() == PACKET_TA_POS) {
    crc = CRC8_TAB[crc ^ byteBuffer[PACKET_TA_POS - 1]];
  }

  // check if length byte is in the buffer
  if (byteBuffer.size() >= PACKET_DL_POS) {
    uint8_t expectedDataLength = byteBuffer[PACKET_DL_POS - 1];
    int actualDataLength = byteBuffer.size() - PACKET_DL_POS;

    // calculate crc of data
    if ((byteBuffer.size() > PACKET_DL_POS) && (actualDataLength <= expectedDataLength)) {
      crc = CRC8_TAB[crc ^ byteBuffer.back()];
    }

    // data complete (+1 for crc byte)
    if (actualDataLength >= (expectedDataLength + 1)) {
      uint8_t tcrc = crc;
      uint8_t rcrc = byteBuffer.back();
      newPacket();
      help.printf("data received:%x:%x\r\n",tcrc, rcrc);
    }

    return;
  }
}