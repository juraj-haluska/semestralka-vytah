#ifndef _ELEVATOR_H
#define _ELEVATOR_H

#include "protocol.h"
#include "display.h"
#include "ledpanel.h"
#include "cabin.h"
#include "engine.h"
#include "proxyswitch.h"
#include "watchdog.h"

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

/* proximity switches */
#define PROXY_P 0xE0
#define PROXY_1 0xE1
#define PROXY_2 0xE2
#define PROXY_3 0xE3
#define PROXY_4 0xE4

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
#define WATCHDOG      0XFE

/* data */
#define D_BTN_PRESS   0xFF

/* elevator states */
#define STATE_IDLE    0x00
#define STATE_START   0x01
#define STATE_FULLSPD 0x02
#define STATE_HALFSPD 0x03
#define STATE_SLOWSPD 0x04
#define STATE_STOP    0x05     
#define STATE_BOARD   0x06

/* used motor speed levels by controller */
#define SPEED_FULL    100
#define SPEED_HALF    50
#define SPEED_SLOW    25

/* time constants in ms */
#define BOARD_DELAY   1500
#define STOP_DELAY    1000 

class Elevator {
private:
  Display *display;  
  LedPanel *ledPanelA;
  LedPanel *ledPanelB;
  Cabin *cabin;
  Engine *engine;
  proxyswitch_t proxy;
  FloorQueue floorQueue;
  Watchdog *watchdog;
  
  // state informations
  int state;
  int requestedFloor;
  int way;

  // timer for tasks
  Timer timer;

  // private state machine functions
  void idle();
  void start();
  void fullSpd();
  void halfSpd();
  void slowSpd();
  void stop();
  void board();

  // transient functions between states
  void toIdle();
  void toStart();
  void toFullSpd();
  void toHalfSpd();
  void toSlowSpd();
  void toStop();
  void toBoard();
public:
  Elevator(Display *_display,
    LedPanel *_ledPanelA,
    LedPanel *_ledPanelB,
    Cabin *_cabin,
    Engine *_engine,
    Watchdog *_watchdog
  ): display(_display),
    ledPanelA(_ledPanelA),
    ledPanelB(_ledPanelB),
    cabin(_cabin),
    engine(_engine),
    watchdog(_watchdog) {
      state = STATE_IDLE;
  };

  // functions
  void checkButtons(packet_t *packet);
  void checkProximity(packet_t *packet);
  void execute();
};

#endif /* _ELEVATOR_H */