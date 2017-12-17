#include "elevator.h"
#include "proxyswitch.h"
#include "math.h"

void Elevator::checkButtons(packet_t * packet) {
  if (packet->dataLength < 1) return;
  if (packet->data[0] != D_BTN_PRESS) return;
  const uint8_t addr = packet->peerAddr;
  
  // panel A
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

  // panel B
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
}

void Elevator::execute() {
  switch (state) {
    case STATE_IDLE: idle(); break;
    case STATE_START: start(); break;
    case STATE_MOVE: move(); break;
    case STATE_BREAK1: break1(); break;
    case STATE_BREAK2: break2(); break;
    case STATE_STOP: stop(); break;
    case STATE_BOARD: board(); break;
  };
}

void Elevator::idle() {
  int floor = floorQueue.pop();
  // transit to moving state if floor request available
  if (floor != -1) {
    requestedFloor = floor;
    engine->requestEncoderCount();
    state = STATE_START;
    help.printf("idle->start\r\n");
  }
}

void Elevator::start() {
  // current position from encoder
  if (!engine->isEncoderCountValid()) {
    engine->requestEncoderCount();
    return;
  }
  // actual position of cabin
  int actual = (int) engine->getLastEncoderCount();
  // desired position of cabin
  int desired = (int) ((4 - requestedFloor) * -250.0f);

  // decide which way to go
  way = 1;
  if (desired < actual) {
    way *= -1;
  }

  // close cabin
  cabin->lock();

  // proximity check first
  // check if cabin is in proximty of some floor
  if (proxy.proxy == PROXY_NARROW || proxy.proxy == PROXY_WIDE) {
    // check distance from desired floor
    int distance = requestedFloor - (proxy.addr & 0x0F);
    if (abs(distance) > 1) {
      state = STATE_MOVE;
      engine->move(way * SPEED_FULL);
      help.printf("start->move\r\n");
    } else if (abs(distance) == 1) {
      state = STATE_BREAK1;
      engine->move(way * SPEED_HALF);
      help.printf("start->break1\r\n");
    } else {
      // current floor
      if (proxy.proxy == PROXY_NARROW) {
        state = STATE_BOARD;
        help.printf("start->board\r\n");
      } else {
        state = STATE_BREAK2;
        engine->move(way * SPEED_SLOW);
        help.printf("start->break2\r\n");
      }
    }
  } else {
    // cabin is not in proximity
    if (abs(desired - actual) >= 250) {
      state = STATE_MOVE;
      engine->move(way * SPEED_FULL);
      help.printf("start->move\r\n");
    } else {
      state = STATE_BREAK1;
      engine->move(way * SPEED_HALF); 
      help.printf("start->break1\r\n"); 
    }
  }
}

void Elevator::move() {
  if (proxy.proxy == PROXY_NARROW) {
    int distance = requestedFloor - (proxy.addr & 0x0F);
    if (distance <= 1) {
      state = STATE_BREAK1;
      engine->move(way * SPEED_HALF);
      help.printf("move->break1\r\n");
    }
  }
}

void Elevator::break1() {
  if (((proxy.addr & 0x0F ) == requestedFloor) && (proxy.proxy == PROXY_WIDE)) {
    state = STATE_BREAK2;
    engine->move(way * SPEED_SLOW);
    help.printf("break1->break2\r\n");
  }
}

void Elevator::break2() {
  if (((proxy.addr & 0x0F ) == requestedFloor) && (proxy.proxy == PROXY_NARROW)) {
    state = STATE_STOP;
    engine->stop();
    help.printf("break2->stop\r\n");
  }
}

void Elevator::stop() {
  state = STATE_BOARD;
  boardingDelay = 0;
  help.printf("stop->board\r\n");
}

void Elevator::board() {
  if (boardingDelay == 0) {
    cabin->unlock();
  }
  if (boardingDelay++ > 400) {
    state = STATE_IDLE;
    help.printf("board->idle\r\n");
  }
}