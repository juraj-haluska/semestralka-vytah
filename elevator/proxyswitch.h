/**
* @brief This header file declares structure which represents
* state of proximity switches.
*
* @author Juraj Haluska
*
*/

#ifndef _PROXYSWITCH_H
#define _PROXYSWITCH_H

#include <inttypes.h>

//! proximity sensors commands
#define PROXY_NONE    0x00
#define PROXY_WIDE    0x01
#define PROXY_NARROW  0x02  

/**
 *  @brief proximity state structure.
 */  
typedef struct {
  uint8_t addr;       /**< address of switch*/  
  uint8_t proxy;      /**< proximity  */  
} proxyswitch_t;

#endif /*_PROXYSWITCH_H */