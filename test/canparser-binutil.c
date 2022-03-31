#include "canparser-binutil.h"

#ifdef __DEF_CANPARSER__

canparser_rx_t canparser_rx;

#endif // __DEF_CANPARSER__

uint32_t canparser_Receive(canparser_rx_t* _m, const uint8_t* _d, uint32_t _id, uint8_t dlc_)
{
 uint32_t recid = 0;
 if ((_id >= 0x1U) && (_id < 0x4U)) {
  if (_id == 0x1U) {
   recid = Unpack_LIFEPO4_CELLS_1_CanParser(&(_m->LIFEPO4_CELLS_1), _d, dlc_);
  } else {
   if (_id == 0x2U) {
    recid = Unpack_LIFEPO4_CELLS_2_CanParser(&(_m->LIFEPO4_CELLS_2), _d, dlc_);
   } else if (_id == 0x3U) {
    recid = Unpack_LIFEPO4_CELLS_3_CanParser(&(_m->LIFEPO4_CELLS_3), _d, dlc_);
   }
  }
 } else {
  if (_id == 0x4U) {
   recid = Unpack_LIFEPO4_GENERAL_CanParser(&(_m->LIFEPO4_GENERAL), _d, dlc_);
  } else {
   if (_id == 0x4U) {
    recid = Unpack_MPPT_GENERAL_CanParser(&(_m->MPPT_GENERAL), _d, dlc_);
   } else if (_id == 0x4U) {
    recid = Unpack_HEARTBEAT_CanParser(&(_m->HEARTBEAT), _d, dlc_);
   }
  }
 }

 return recid;
}

