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