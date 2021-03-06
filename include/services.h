/*
 * services.h
 *
 *  Created on: Aug 20, 2017
 *      Author: Kofemolka
 */

#ifndef SERVICES_SERVICES_H_
#define SERVICES_SERVICES_H_

#include "afx.h"
#include "ble_types.h"

static const ble_uuid128_t m_base_uuid128 =
{
   {
       0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
       0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
   }
};

#define BLE_UUID_OBD_SERVICE            0xC8D0

typedef enum {
	ID_CHAR_SPEED = 0,
	ID_CHAR_TEMP,
	TOTAL_CHARS
} CHAR_ID_t;

#endif /* SERVICES_SERVICES_H_ */
