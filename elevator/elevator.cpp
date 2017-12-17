#include "elevator.h"

void Elevator::checkButtons(packet_t * packet) {
  if (packet->dataLength < 1) return;
  if (packet->data[0] != D_BTN_PRESS) return;

  const uint8_t addr = packet->peerAddr;
  // it's possible to do it with less code
  // but for readibility its better this way
  uint8_t panelBase;
  for (int i = 0; i < 2; i++) {
    if (i == 0) panelBase = BTN_PANEL_A;
    if (i == 1) panelBase = BTN_PANEL_B;

    if (addr == (panelBase | BTN_P)) {
      floorQueue.push(FLOOR_P);
    }
    if (addr == (panelBase | BTN_1)) {
      floorQueue.push(FLOOR_1);
    }
    if (addr == (panelBase | BTN_2)) {
      floorQueue.push(FLOOR_2);
    }
    if (addr == (panelBase | BTN_3)) {
      floorQueue.push(FLOOR_3);
    }
    if (addr == (panelBase | BTN_4)) {
      floorQueue.push(FLOOR_4);
    }
  }
}

int Elevator::getNext() {
  return floorQueue.pop();
}