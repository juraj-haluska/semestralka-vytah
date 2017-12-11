#include "protocol.h"
#include "mbed.h"

extern Serial help;

Protocol::Protocol( Serial& _serial, uint8_t _addr, void (*_callback) (Packet * packet)):
serial(_serial),
addr(_addr),
clbk(_callback) {
  this->newPacket();
  serial.attach(callback(this, &Protocol::onByteReceived), Serial::RxIrq);
};

void Protocol::newPacket() {
  crc = 0;
  byteBuffer.clear();
}

void Protocol::onByteReceived() {
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
      help.printf("ack\r\n");
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
      // check crc
      if (crc == byteBuffer.back()) {
        Packet * received = new Packet(byteBuffer[PACKET_TA_POS - 1]);
        // copy data
        for (int i = 0; i < expectedDataLength; i++) {
          received->addByte(byteBuffer[PACKET_DL_POS + i]);
        }

        newPacket();
        clbk(received);
      } else {
        newPacket();
      }
    }
  }
}

void Protocol::queuePacket(Packet * packet) {
  packetQueue.push(packet);
}