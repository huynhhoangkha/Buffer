//Byte queue interface by Huynh Hoang Kha
#pragma once
#ifndef _BYTE_QUEUE_H_
#define _BYTE_QUEUE_H_
#include <cstdint>

class ByteQueue {
public:
	//Methods that will throw exception when error occur (in the case of empty/full queue errors)
	virtual uint8_t deQueueByte() = 0;					//Return the first-joined byte in the queue and then remove it from the queue
	virtual char deQueueChar() = 0;						//Return the first-joined byte in the queue as a character and then remove it from the queue
	//Methods that return false when error occur (in the case of empty/full queue errors), output value via a pointer
	virtual bool enQueueByte(uint8_t dataByte) = 0;		//Push a byte to the queue, return true if insertion was OK
	virtual bool enQueueChar(char dataChar) = 0;		//Push a character to the queue, return true if insertion was OK
	virtual bool deQueueByte(uint8_t* outputByte) = 0;	//Return the first-joined byte in the queue and then remove it from the queue
	virtual bool deQueueChar(char* outputChar) = 0;		//Return the first-joined byte in the queue as a character and then remove it from the queue
};
#endif // !_BYTE_QUEUE_H_
