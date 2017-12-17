#ifndef _ELEVATOR_H
#define _ELEVATOR_H

#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"

#include "floorqueue.h"

extern Serial help;

/* buttons */
#define BTN_P   0x00
#define BTN_1   0x01
#define BTN_2   0x02
#define BTN_3   0x03
#define BTN_4   0x04

/* leds */
#define LED_P   0x00
#define LED_1   0x01
#define LED_2   0x02
#define LED_3   0x03
#define LED_4   0x04

/* floors */
#define FLOOR_P 0x00
#define FLOOR_1 0x01
#define FLOOR_2 0x02
#define FLOOR_3 0x03
#define FLOOR_4 0x04

/* objects */
#define DISPLAY       0x30
#define LED_PANEL_A   0x10
#define LED_PANEL_B   0x20
#define BTN_PANEL_A   0xC0
#define BTN_PANEL_B   0xB0
#define CABIN         0xF0
#define EMERGENCY     0x0F
#define MOTOR         0xF1

/* data */
#define D_BTN_PRESS   0xFF

/* L1 STATES */
#define STATE_IDLE    0x00
#define STATE_MOVING  0x01
#define STATE_BOARD   0x02

class Elevator {
private:
  Display *display;  
  LedPanel *ledPanelA;
  LedPanel *ledPanelB;
  Cabin *cabin;
  Engine *engine;
  FloorQueue floorQueue;
  
  // state informations
  int state;

  // private functions
  void idle();
  void moving();
  void boarding();
public:
  Elevator(Display *_display,
    LedPanel *_ledPanelA,
    LedPanel *_ledPanelB,
    Cabin *_cabin,
    Engine *_engine
  ): display(_display),
    ledPanelA(_ledPanelA),
    ledPanelB(_ledPanelB),
    cabin(_cabin),
    engine(_engine) {
      state = STATE_IDLE;
  };

  // functions
  void checkButtons(packet_t * packet);
  int getNext();
  void execute();
};

#endif /* _ELEVATOR_H */