//Byte queue interface by Huynh Hoang Kha
#pragma once
#ifndef _BYTE_QUEUE_H_
#define _BYTE_QUEUE_H_
#include <cstdint>

class ByteQueue {
public:
	//Methods that will throw exception when error occur (in the case of empty/full queue errors)
	virtual uint8_t deQueueByte() = 0;					//Return the first-joined byte in the queue and then remove it from the queue
	char deQueueChar();									//Return the first-joined byte in the queue as a character and then remove it from the queue
	virtual int deQueueInt();							//Return the first-joined int in the queue and then remove it from the queue
	virtual float deQueueFloat();						//Return the first-joined float in the queue and then remove it from the queue
	virtual long deQueueLong();							//Return the first-joined long in the queue and then remove it from the queue
	virtual double deQueueDouble();						//Return the first-joined double in the queue and then remove it from the queue
	//Methods that return false when error occur (in the case of empty/full queue errors), output value via a pointer
	virtual bool deQueueByte(uint8_t* outputByte) = 0;	//Return the first-joined byte in the queue and then remove it from the queue
	bool deQueueChar(char* outputChar);					//Return the first-joined byte in the queue as a character and then remove it from the queue
	virtual bool deQueueInt(int* outputInt);			//Return the first-joined int in the queue and then remove it from the queue
	virtual bool deQueueFloat(float* outputFloat);		//Return the first-joined float in the queue and then remove it from the queue
	virtual bool deQueueLong(long* outputLong);			//Return the first-joined long in the queue and then remove it from the queue
	virtual bool deQueueDouble(double* outputDouble);	//Return the first-joined double in the queue and then remove it from the queue
	virtual bool enQueueByte(uint8_t dataByte) = 0;		//Push a byte to the queue, return true if insertion was OK
	bool enQueueChar(char dataChar);					//Push a character to the queue, return true if insertion was OK
	virtual bool enQueueInt(int dataInt);				//Push an int to the queue, return true if insertion was OK
	virtual bool enQueueFloat(float dataFloat);			//Push a float to the queue, return true if insertion was OK
	virtual bool enQueueLong(long dataLong);			//Push a long to the queue, return true if insertion was OK
	virtual bool enQueueDouble(double dataDouble);		//Push a double to the queue, return true if insertion was OK
};
#endif // !_BYTE_QUEUE_H_
