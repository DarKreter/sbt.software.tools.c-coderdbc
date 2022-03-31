#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "dbccodeconf.h"

#include "canparser.h"

// This version definition comes from main driver version and
// can be compared in user code space for strict compatibility test
#define VER_CANPARSER_MAJ (0U)
#define VER_CANPARSER_MIN (0U)

typedef struct
{
  LIFEPO4_CELLS_1_t LIFEPO4_CELLS_1;
  LIFEPO4_CELLS_2_t LIFEPO4_CELLS_2;
  LIFEPO4_CELLS_3_t LIFEPO4_CELLS_3;
  LIFEPO4_GENERAL_t LIFEPO4_GENERAL;
  MPPT_GENERAL_t MPPT_GENERAL;
  HEARTBEAT_t HEARTBEAT;
} canparser_rx_t;

// There is no any TX mapped massage.

uint32_t canparser_Receive(canparser_rx_t* m, const uint8_t* d, uint32_t msgid, uint8_t dlc);

#ifdef __DEF_CANPARSER__

extern canparser_rx_t canparser_rx;

#endif // __DEF_CANPARSER__

#ifdef __cplusplus
}
#endif
