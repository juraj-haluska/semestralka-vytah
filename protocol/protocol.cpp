#include "protocol.h"
#include "mbed.h"

extern Serial help;

Protocol::Protocol(Serial& _serial, uint8_t _myAddr):
serial(_serial),
myAddr(_myAddr) {
  this->newPacket();
  serial.attach(callback(this, &Protocol::onByteReceived), Serial::RxIrq);
};

Mail<packet_t, RECEIVE_SLOTS> & Protocol::getPacketMailbox() {
  return recvMailbox;
}

MemoryPool<uint8_t [PACKET_DATA_LEN], RECEIVE_SLOTS> & Protocol::getDataPool() {
  return dataPool;
}

void Protocol::newPacket() {
  crc = 0;
  for (int i = 0; i < MAX_DATA_LENGTH; i++) {
    recvBuffer[i] = ZERO;
  }
  dataLength = 0;
}

void Protocol::onByteReceived() {
  // first byte in buffer must by START_BYTE
  if (dataLength >= PACKET_SB_POS && recvBuffer[PACKET_SB_POS - 1] != START_BYTE) {
    newPacket();
  }
  
  // add received byte to buffer
  // including last crc byte
  if (dataLength <= MAX_DATA_LENGTH) {
    recvBuffer[dataLength++] = serial.getc();
  } else {
    // error
    newPacket();
  }

  // check if ACK was received
  if (dataLength == sizeof(ACK)) {
    uint8_t match = 0;
    for (uint8_t i = 0; i < dataLength; i++) {
      if (recvBuffer[i] == ACK[i]) {
        match++;
      }
    }
    if (match == dataLength) {
      // ack received
      newPacket();
      event.set(EVENT_ACK);
      return;
    }
  }

  // calculate crc if address's present
  if (dataLength == PACKET_RA_POS) {
    crc = CRC8_TAB[crc ^ recvBuffer[PACKET_RA_POS - 1]];
  }

  if (dataLength == PACKET_TA_POS) {
    crc = CRC8_TAB[crc ^ recvBuffer[PACKET_TA_POS - 1]];
  }

  // check if length byte is in the buffer
  if (dataLength >= PACKET_DL_POS) {
    uint8_t expectedDataLength = recvBuffer[PACKET_DL_POS - 1];
    int actualDataLength = dataLength - PACKET_DL_POS;

    // calculate crc of data
    if ((dataLength > PACKET_DL_POS) && (actualDataLength <= expectedDataLength)) {
      crc = CRC8_TAB[crc ^ recvBuffer[dataLength - 1]];
    }

    // data complete (+1 for crc byte)
    if (actualDataLength >= (expectedDataLength + 1)) {
      // check crc
      if (crc == recvBuffer[dataLength - 1]) {
        // allocate new packet, copy data and add to mailbox
        packet_t *packet = recvMailbox.alloc();
        uint8_t *data = (uint8_t *) dataPool.alloc();

        packet->peerAddr = recvBuffer[PACKET_TA_POS - 1];
        packet->data = data;
        packet->dataLength = expectedDataLength;
        packet->dynamic = true;
        
        for (int i = 0; i < expectedDataLength; i++) {
          packet->data[i] = recvBuffer[PACKET_DL_POS + i];
        }

        recvMailbox.put(packet);

        newPacket();
      } else {
        newPacket();
      }
    }
  }
}

void Protocol::sendPacket(packet_t * packet) {
  volatile uint32_t flag = 0x00;
      
  // packet transmission
  while (flag != EVENT_ACK) {
    uint8_t s_crc = 0; 

    // compute crc of addresses
    s_crc = CRC8_TAB[s_crc ^ packet->peerAddr];
    s_crc = CRC8_TAB[s_crc ^ myAddr];

    // send header
    serial.putc(START_BYTE);
    serial.putc(packet->peerAddr); 
    serial.putc(myAddr); 
    serial.putc(packet->dataLength);
      
    // send data and compute crc
    for (int i = 0; i < packet->dataLength; i++) {
      s_crc = CRC8_TAB[s_crc ^ packet->data[i]];
      serial.putc(packet->data[i]);
    }

    // send crc
    serial.putc(s_crc);

    // wait for ack event
    flag = event.wait_all(EVENT_ACK, ACK_TIMEOUT);
  }
  
  // packet successfuly sent
}