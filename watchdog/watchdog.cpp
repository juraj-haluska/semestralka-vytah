#include "watchdog.h"
#include "protocol.h"

void Watchdog::watchdogCheck() {
  if (timer.read_ms() >= WATCHDOG_PERIOD) {
    // reset watchdog
    dataBuffer[0] = WATCHDOG_CLEAR;
    (*protocol).sendPacket(&packet);
    timer.reset();
  }
}