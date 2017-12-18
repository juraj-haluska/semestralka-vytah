/**
* @file cabin.h
* @brief This header file contains constants and class for cabin.
*
* @author Juraj Haluska
*
*/

#ifndef _CABIN_H
#define _CABIN_H

#include "protocol.h"

//! cabin commands
#define CABIN_LOCK      0x01
#define CABIN_UNLOCK    0x00
#define BREAK_UNLOCK    0x00
#define BREAK_LOCK      0x01

class Cabin {
private:
  //! private attributes
  static const int maxDataLength = 1;       /*!< maximal length of dataBuffer */  
  uint8_t myAddr;                           /*!< address of cabin lock */  
  uint8_t breakAddr;                        /*!< address of emergency break */ 
  uint8_t dataBuffer[maxDataLength];        /*!< data buffer */ 
  packet_t packet;                          /*!< packet */ 
  Protocol *protocol;                       /*!< pointer to global protocol */ 
  InterruptIn * freeFall;                   /*!< pointer to free fall external interrupt */ 

  //! private functions
  void freeFallHandler();
  bool freeFallDetected;
public:
  Cabin(uint8_t _myAddr, uint8_t _breakAddr, Protocol *_protocol, InterruptIn * _freeFall): 
    myAddr(_myAddr), 
    breakAddr(_breakAddr), 
    protocol(_protocol),
    freeFall(_freeFall)
    {
      packet.data = dataBuffer;
      packet.dataLength = maxDataLength;
      packet.dynamic = false;

      freeFallDetected = false;

      //! activate external interrupt
      freeFall->fall(callback(this, &Cabin::freeFallHandler));
  };
  //! lock the elevator's cabin
  void lock();    
  //! unlock the elevator's cabin                     
  void unlock();
  //! activates emergency break
  void activateBreak();
  //! deactivates emergency break
  void deactivateBreak();
  //! Freefall check
  /**
   * if free fall occured this function will activate
   * emergency break
  */
  void checkFreeFall();
};

#endif /* _CABIN_H */