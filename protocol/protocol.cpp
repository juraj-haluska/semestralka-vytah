#include "protocol.h"
#include "mbed.h"

// constructor
Protocol::Protocol( RawSerial& _serial, uint8_t _addr, void (*_callback) (const Packet * packet)):
  serial(_serial),
  addr(_addr),
  callback(_callback) {;;
    serial.attach(&(this->onByteReceived), SerialBase::RxIrq);
};

void Protocol::onByteReceived() {

}