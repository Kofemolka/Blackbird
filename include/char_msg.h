/*
 * char_msg.h
 *
 *  Created on: Aug 21, 2017
 *      Author: Kofemolka
 */

#ifndef INCLUDE_CHAR_MSG_H_
#define INCLUDE_CHAR_MSG_H_

#include "afx.h"

typedef struct {
	uint16_t charId; //zero based ID from CHAR_ID_t enum
	uint16_t value;
} char_msg_t;


#endif /* INCLUDE_CHAR_MSG_H_ */
