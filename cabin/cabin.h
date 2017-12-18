/**
* @brief This header file contains cabin commands and cabin class.
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

/**
 *  @brief Class that controlls elevator's cabin.
 */  
class Cabin {
private:
  //! private attributes
  static const int maxDataLength = 1;       /**< maximal length of dataBuffer */  
  uint8_t myAddr;                           /**< address of cabin lock */  
  uint8_t breakAddr;                        /**< address of emergency break */ 
  uint8_t dataBuffer[maxDataLength];        /**< data buffer */ 
  packet_t packet;                          /**< packet */ 
  Protocol *protocol;                       /**< pointer to global protocol */ 
  InterruptIn * freeFall;                   /**< pointer to free fall external interrupt */ 

  //! private functions 

  //! ISR for free fall 
  void freeFallHandler();

  //! true if free fall has occured
  bool freeFallDetected;         
public:
  //! create cabin
  Cabin(uint8_t _myAddr, uint8_t _breakAddr, Protocol *_protocol, InterruptIn * _freeFall): 
    myAddr(_myAddr), 
    breakAddr(_breakAddr), 
    protocol(_protocol),
    freeFall(_freeFall)
    {
      //! initialize packet struct
      packet.data = dataBuffer;
      packet.dataLength = maxDataLength;
      packet.dynamic = false;

      //! clear free fall flag
      freeFallDetected = false;

      //! activate external interrupt
      freeFall->fall(callback(this, &Cabin::freeFallHandler));
  };
  
  /**
   * @brief lock the elevator's cabin
   */
  void lock(); 

  /**
   * @brief unlock the elevator's cabin
   */                  
  void unlock();

  /**
   * @brief activates emergency break
   */
  void activateBreak();
 
  /**
   * @brief deactivates emergency break
   */
  void deactivateBreak();

  /**
   * @brief this function does freefall check
   * 
   * if free fall occured this function will activate
   * emergency break
  */
  void checkFreeFall();
};

#endif /* _CABIN_H */