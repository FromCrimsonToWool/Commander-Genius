/*
 * CEvent.h
 *
 *  Created on: 03.08.2010
 *      Author: gerstrong
 *
 *  Intended to represent an event.
 *  Doesn't do any handling.
 */

#ifndef CEVENT_H_
#define CEVENT_H_

#include <stdint.h>

struct CEvent { virtual ~CEvent() {} };

struct EventEnterLevel : CEvent {
	uint16_t data;
	EventEnterLevel(uint16_t d) : data(d) {}
};

struct EventExitLevel : CEvent {
	uint16_t levelObject;
	EventExitLevel(uint16_t l) : levelObject(l) {}
};


#endif /* CEVENT_H_ */
