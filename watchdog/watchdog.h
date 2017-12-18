/**
* @brief This header file is declares watchdog subsystem.
*
* @author Juraj Haluska
*
*/

#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include "protocol.h"

//! watchdog clear period
#define WATCHDOG_PERIOD  800   

//! watchdog commands
#define WATCHDOG_RESET   0x01
#define WATCHDOG_CLEAR   0x02

/**
 *  @brief Class that controlls watchdog subsystem.
 */  
class Watchdog {
private:
  //! private attributes
  static const int maxDataLength = 1;       /**< maximal length of dataBuffer */  
  uint8_t myAddr;                           /**< address of motor */ 
  Protocol * protocol;                      /**< pointer to global protocol */ 
  Timer timer;                              /**< watchdog timer */ 
  uint8_t dataBuffer[maxDataLength];        /**< data buffer */ 
  packet_t packet;                          /**< packet */ 
public:
  //! create watchdog
  Watchdog(uint8_t _myAddr, Protocol *_protocol): myAddr(_myAddr), protocol(_protocol) {
    //! initialize packet struct
    packet.peerAddr = myAddr;
    packet.data = dataBuffer;
    packet.dataLength = maxDataLength;
    packet.dynamic = false;

    //! start watchdog timer
    timer.start();
  };

  /**
   * @brief Check if watchdog has elapsed WATCHDOG_PERIOD time.
   * 
   * If yes, will send watchdog clear request.
   */
  void watchdogCheck();
};

#endif /* _WATCHDOG_H */