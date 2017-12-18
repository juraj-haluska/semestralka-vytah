/**
* @file floorqueue.h
* @brief This header file contains declaration of FloorQueue
* used by Elevator class.
*
* @author Juraj Haluska
*
*/

#ifndef _FLOORQUEUE_H
#define _FLOORQUEUE_H

#include <inttypes.h>

#define CAPACITY  16

class FloorQueue {
private:
  int buffer[CAPACITY];
  int front;
public:
  FloorQueue() {
    front = -1;
  }
  void push(int floor);
  int pop();
};

#endif /* _FLOORQUEUE_H */