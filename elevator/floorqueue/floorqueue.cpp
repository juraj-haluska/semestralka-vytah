#include "floorqueue.h"

// if queue is full overwrite the oldest element
void FloorQueue::push(int floor) {
  // check if floor is in queue already
  for (int i = 0; i <= front; i++) {
    if (floor == buffer[front]) return;
  }

  // check if queue is full
  if (front == (CAPACITY - 1)) {
    // "remove last element"
    front--;
  }

  // check if queue is not empty
  if (front >= 0) {
    // shift all elements to the right by 1
    for (int i = front; i >= 0; i--) {
      buffer[i + 1] = buffer[i];
    }
  }
  
  // add current floor
  buffer[0] = floor;
  front++;
}

int FloorQueue::pop() {
  if (front < 0) return -1;
  int tmp = buffer[front];
  front--;
  return tmp;
}