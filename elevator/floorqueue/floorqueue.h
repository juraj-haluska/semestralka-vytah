/**
* @brief This header file contains declaration of FloorQueue
* used by Elevator class.
*
* @author Juraj Haluska
*
*/

#ifndef _FLOORQUEUE_H
#define _FLOORQUEUE_H

#include <inttypes.h>

//! capacity of FloorQueue
#define CAPACITY  16

/**
 *  @brief Class that implements FloorQueue data structure
 */  
class FloorQueue {
private:
  int buffer[CAPACITY];     /**< floor buffer */  
  int front;                /**< position of last floor in buffer */  
public:
  //! create floor queue
  FloorQueue() {
    front = -1;
  }

  /**
   * @brief add item to floor queue
   * 
   * If queue is full, oldest element is removed.
   * 
   * @param   floor to add
   */
  void push(int floor);

  /**
   * @brief get item from queue
   * 
   * @return   oldest element in queue
   */
  int pop();
};

#endif /* _FLOORQUEUE_H */