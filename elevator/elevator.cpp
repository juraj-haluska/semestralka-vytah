#include "elevator.h"
#include "proxyswitch.h"
#include "math.h"

void Elevator::checkButtons(packet_t * packet) {
  if (packet->dataLength < 1) return;
  if (packet->data[0] != D_BTN_PRESS) return;
  const uint8_t addr = packet->peerAddr;
  
  //! panel A
  if (addr == (BTN_PANEL_A | BTN_P)) {
    ledPanelA->setLed(LED_P);
    floorQueue.push(FLOOR_P);
  }
  if (addr == (BTN_PANEL_A | BTN_1)) {
    ledPanelA->setLed(LED_1);
    floorQueue.push(FLOOR_1);
  }
  if (addr == (BTN_PANEL_A | BTN_2)) {
    ledPanelA->setLed(LED_2);
    floorQueue.push(FLOOR_2);
  }
  if (addr == (BTN_PANEL_A | BTN_3)) {
    ledPanelA->setLed(LED_3);
    floorQueue.push(FLOOR_3);
  }
  if (addr == (BTN_PANEL_A | BTN_4)) {
    ledPanelA->setLed(LED_4);
    floorQueue.push(FLOOR_4);
  }

  //! panel B
  if (addr == (BTN_PANEL_B | BTN_P)) {
    ledPanelB->setLed(LED_P);
    floorQueue.push(FLOOR_P);
  }
  if (addr == (BTN_PANEL_B | BTN_1)) {
    ledPanelB->setLed(LED_1);
    floorQueue.push(FLOOR_1);
  }
  if (addr == (BTN_PANEL_B | BTN_2)) {
    ledPanelB->setLed(LED_2);
    floorQueue.push(FLOOR_2);
  }
  if (addr == (BTN_PANEL_B | BTN_3)) {
    ledPanelB->setLed(LED_3);
    floorQueue.push(FLOOR_3);
  }
  if (addr == (BTN_PANEL_B | BTN_4)) {
    ledPanelB->setLed(LED_4);
    floorQueue.push(FLOOR_4);
  }
}

void Elevator::checkProximity(packet_t *packet) {
  if (packet->dataLength < 1) return;
  if (packet->peerAddr == PROXY_P) {
    proxy.addr = PROXY_P;
    proxy.proxy = packet->data[0];
  }
  if (packet->peerAddr == PROXY_1) {
    proxy.addr = PROXY_1;
    proxy.proxy = packet->data[0];
  }
  if (packet->peerAddr == PROXY_2) {
    proxy.addr = PROXY_2;
    proxy.proxy = packet->data[0];
  }
  if (packet->peerAddr == PROXY_3) {
    proxy.addr = PROXY_3;
    proxy.proxy = packet->data[0];
  }
  if (packet->peerAddr == PROXY_4) {
    proxy.addr = PROXY_4;
    proxy.proxy = packet->data[0];
  }

  if (state == STATE_FULLSPD || state == STATE_HALFSPD || state == STATE_SLOWSPD) {
    if (way < 0) {
      display->setDirectionDown(proxy.addr & 0x0F);
    } else {
      display->setDirectionUp(proxy.addr & 0x0F);
    }
  }
}

//! execution loop of elevator controller
void Elevator::execute() {
  cabin->checkFreeFall();
  watchdog->watchdogCheck();
  switch (state) {
    case STATE_IDLE: idle(); break;
    case STATE_START: start(); break;
    case STATE_FULLSPD: fullSpd(); break;
    case STATE_HALFSPD: halfSpd(); break;
    case STATE_SLOWSPD: slowSpd(); break;
    case STATE_STOP: stop(); break;
    case STATE_BOARD: board(); break;
  };
}

void Elevator::idle() {
  int floor = floorQueue.pop();
  //! transit to moving state if floor request available
  if (floor != -1) {
    requestedFloor = floor;
    engine->requestEncoderCount();
    help.printf("idle->start\r\n");
    toStart();
  }
}

void Elevator::start() {
  //! current position from encoder
  if (!engine->isEncoderCountValid()) {
    engine->requestEncoderCount();
    return;
  }
  //! actual position of cabin
  int actual = (int) engine->getLastEncoderCount();
  //! desired position of cabin
  int desired = (int) ((4 - requestedFloor) * -250.0f);

  //! decide which way to go
  way = 1;
  if (desired < actual) {
    way *= -1;
  }

  //! close cabin
  cabin->lock();

  //! proximity check first
  //! check if cabin is in proximty of some floor
  if (proxy.proxy == PROXY_NARROW || proxy.proxy == PROXY_WIDE) {
    //! check distance from desired floor
    int distance = requestedFloor - (proxy.addr & 0x0F);
    if (abs(distance) > 1) {
      help.printf("start->move\r\n");
      toFullSpd();
    } else if (abs(distance) == 1) {
      help.printf("start->break1\r\n");
      toHalfSpd();
    } else {
      //! current floor
      if (proxy.proxy == PROXY_NARROW) {
        help.printf("start->board\r\n");
        toBoard();
      } else {
        help.printf("start->break2\r\n");
        toSlowSpd();
      }
    }
  } else {
    //! cabin is not in proximity
    if (abs(desired - actual) >= 250) {
      help.printf("start->move\r\n");
      toFullSpd();
    } else {
      help.printf("start->break1\r\n");
      toHalfSpd();
    }
  }
}

void Elevator::fullSpd() {
  if (proxy.proxy == PROXY_NARROW) {
    int distance = requestedFloor - (proxy.addr & 0x0F);
    if (abs(distance) <= 1) {
      help.printf("move->break1\r\n");
      toHalfSpd();
    }
  }
}

void Elevator::halfSpd() {
  if (((proxy.addr & 0x0F ) == requestedFloor) && (proxy.proxy == PROXY_WIDE)) {
    help.printf("break1->break2\r\n");
    toSlowSpd();
  }
}

void Elevator::slowSpd() {
  if (((proxy.addr & 0x0F ) == requestedFloor) && (proxy.proxy == PROXY_NARROW)) {
    help.printf("break2->stop\r\n");
    toStop();
  }
}

void Elevator::stop() {
  if (timer.read_ms() >= STOP_DELAY) {
    timer.stop();
    help.printf("stop->board\r\n");
    toBoard();
  }
}

void Elevator::board() {
  if (timer.read_ms() >= BOARD_DELAY) {
    timer.stop();
    help.printf("board->idle\r\n");
    toIdle();
  }
}

void Elevator::toIdle() {
  state = STATE_IDLE;
}

void Elevator::toStart() {
  state = STATE_START;
}

void Elevator::toFullSpd() {
  engine->move(way * SPEED_FULL);
  state = STATE_FULLSPD;
}

void Elevator::toHalfSpd() {
  engine->move(way * SPEED_HALF); 
  state = STATE_HALFSPD;
}

void Elevator::toSlowSpd() {
  engine->move(way * SPEED_SLOW);
  state = STATE_SLOWSPD;
}

void Elevator::toStop() {
  engine->stop();
  state = STATE_STOP;
  timer.reset();
  timer.start();
}

void Elevator::toBoard() {
  ledPanelA->unsetLed(requestedFloor);
  ledPanelB->unsetLed(requestedFloor);
  display->setDirectionNone(requestedFloor);
  cabin->unlock();
  state = STATE_BOARD;
  timer.reset();
  timer.start();
}