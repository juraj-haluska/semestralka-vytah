#ifndef _PROXYSWITCH_H
#define _PROXYSWITCH_H

#include <inttypes.h>

#define PROXY_NONE    0x00
#define PROXY_WIDE    0x01
#define PROXY_NARROW  0x02  

typedef struct {
  uint8_t addr;
  uint8_t proxy;
} proxyswitch_t;

#endif /*_PROXYSWITCH_H */