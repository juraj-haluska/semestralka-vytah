#include "protocol.h"
#include "mbed.h"

void Protocol::onReceived() {
  callback(NULL);
}