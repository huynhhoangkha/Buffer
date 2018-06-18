//Buffer library by Huynh Hoang Kha
//This implement stack/queue buffers
//Using linked list and array
#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <cstdint>
#include <string>
#include "ByteStack.h"
#include "ByteQueue.h"
using namespace std;

enum ExceptionErrorCode {
	NEGATIVE_CAPACITY,
	NEGATIVE_SIZE,
	SIZE_BIGGER_THAN_CAPACITY,
	NEGATIVE_INDEX,
	OUT_OF_RANGE_INDEX,
	NOT_SET_ENDIAN,
	POP_FROM_EMPTY_STACK,
	ACCESS_TO_EMPTY_STACK,
	PUSH_TO_FULL_STACK
};

enum Endian {
	NOT_SET,
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

class BufferException {
public:
	int exceptionCode;
	char* msgPtr;
	BufferException(int code, const char* message);
	BufferException(const BufferException &obj);
	~BufferException();
	string getMessage();
	int getCode();
};

class Buffer {
public:
	virtual int getCapacity();					//Return buffer's capacity
	virtual int getSize();						//Return number of bytes stored in the buffer
	virtual bool isEmpty();						//Return true if the buffer is empty
	virtual bool isFull();						//Return true if the buffer is full
	virtual void clean() = 0;					//Clean the buffer's content
	//Methods that will throw exception when error occur (in the case of invalid index/offset)
	virtual uint8_t getByte(int index);			//Return the byte at index
	virtual char getChar(int index);			//Return the byte at index as a character
	virtual string getString() = 0;				//Return the whole data as std::string object
	virtual int getInt(int offset) = 0;			//Return an 4-byte integer starting from the offset index byte
	virtual float getFloat(int offset) = 0;		//Return an 4-byte float starting from the offset index byte
	virtual long getLong(int offset) = 0;		//Return an 8-byte long starting from the offset index byte
	virtual double getDouble(int offset) = 0;	//Return an 8-byte double starting from the offset index byte
	//Methods that return false when error occur (in the case of invalid index/offset), output value via a pointer
	virtual uint8_t& operator[](int index) = 0;								//Return reference to the byte at index
	virtual bool getByte(int index, uint8_t* outputByte);					//Return the byte at index
	virtual bool getChar(int index, char* outputChar);						//Return the byte at index as a character
	virtual bool getInt(int offset, int* outputInt) = 0;					//Return an 4-byte integer starting from the offset index byte
	virtual bool getFloat(int offset, float* outputFloat) = 0;				//Return an 4-byte float starting from the offset index byte
	virtual bool getLong(int offset, long* outputLong) = 0;					//Return an 8-byte long starting from the offset index byte
	virtual bool getDouble(int offset, double* outputDouble) = 0;			//Return an 8-byte double starting from the offset index byte
	virtual bool getMemoryBlock(void* memPtr, int offset, int size) = 0;	//Copy 'size' bytes from buffer into a memory block pointed by memPtr
	virtual bool writeInt(int offset, int data) = 0;			//Write an int value into the buffer at 'offset' position
	virtual bool writeFloat(int offset, float data) = 0;		//Write an float value into the buffer at 'offset' position
	virtual bool writeLong(int offset, long data) = 0;			//Write an long value into the buffer at 'offset' position
	virtual bool writeDouble(int offset, double data) = 0;		//Write an double value into the buffer at 'offset' position
protected:
	int capacity;								//Buffer's capacity
	int size;									//Number of bytes stored in the buffer	
	Endian endian;								//System endianness, should be NOT_SET by default
};

class ArrayBuffer :public Buffer {
protected:
	uint8_t* arrayPointer;
public:
	//Construct this ArrayStackBuffer with the size 'capacity'
	ArrayBuffer(int capacity, Endian systemEndian);
	//Construct this ArrayStackBuffer with the size 'capacity' and then copy 'dataSize' byte(s) from memory block pointed by memPtr into buffer.
	ArrayBuffer(void* memPtr, int capacity, int dataSize, Endian systemEndian);
	ArrayBuffer(string inputString, Endian systemEndian);
	ArrayBuffer(int capacity, string inputString, Endian systemEndian);
	//Free up all allocated memory
	~ArrayBuffer();
	void clean();									//Clean the buffer's content
	//Methods that will throw exception when error occur (in the case of invalid index/offset)
	uint8_t& operator[](int index);					//Return reference to the byte at index
	string getString();								//Return the whole data as std::string object
	int getInt(int offset);							//Return an 4-byte integer starting from the offset index byte
	float getFloat(int offset);						//Return an 4-byte float starting from the offset index byte
	long getLong(int offset);						//Return an 8-byte long starting from the offset index byte
	double getDouble(int offset);					//Return an 8-byte double starting from the offset index byte
	//Methods that return false when error occur (in the case of invalid index/offset), output value via a pointer
	bool getInt(int offset, int* outputInt);					//Return an 4-byte integer starting from the offset index byte
	bool getFloat(int offset, float* outputFloat);				//Return an 4-byte float starting from the offset index byte
	bool getLong(int offset, long* outputLong);					//Return an 8-byte long starting from the offset index byte
	bool getDouble(int offset, double* outputDouble);			//Return an 8-byte double starting from the offset index byte
	bool getMemoryBlock(void* memPtr, int offset, int size);	//Copy 'size' bytes from buffer into a memory block pointed by memPtr
	bool writeInt(int offset, int data);			//Write an int value into the buffer at 'offset' position
	bool writeFloat(int offset, float data);		//Write an float value into the buffer at 'offset' position
	bool writeLong(int offset, long data);			//Write an long value into the buffer at 'offset' position
	bool writeDouble(int offset, double data);		//Write an double value into the buffer at 'offset' position
	template <typename T> bool writePrimity(int offset, T data);		//write any primitive object into buffer at 'offset' position
	template <typename T> bool getPrimity(int offset, T* outputObject);	//Get any primitive object, return result via an object pointer
};

template<typename T>
inline bool ArrayBuffer::writePrimity(int offset, T data) {
	if (offset < 0 || offset + sizeof(T) > this->capacity || offset > this->size) return false;
	if (this->endian == NOT_SET) {
		BufferException bE(NOT_SET_ENDIAN, "Please set Endian to be BIG_ENDIAN or LITTLE_ENDIAN according to your system.");
		throw bE;
	}
	if (this->endian == BIG_ENDIAN) {
		uint8_t* ptr = (uint8_t*)&data;
		for (int i = 0; i < sizeof(T); i++) this->arrayPointer[offset++] = *(ptr++);
	}
	else if (this->endian == LITTLE_ENDIAN) {
		uint8_t* ptr = ((uint8_t*)&data) + sizeof(T) - 1;
		for (int i = 0; i < sizeof(T); i++) this->arrayPointer[offset++] = *(ptr--);
	}
	if (offset > this->size) this->size = offset;
	return true;
}

template<typename T>
inline bool ArrayBuffer::getPrimity(int offset, T * outputObject) {
	if (this->endian == NOT_SET || offset < 0 || offset + sizeof(T) > this->size) return false;
	uint8_t* ptr = (uint8_t*)outputObject;
	if (this->endian == BIG_ENDIAN) for (int i = 0; i < sizeof(T); i++) *(ptr++) = this->arrayPointer[offset + i];
	else if (this->endian == LITTLE_ENDIAN) for (int i = sizeof(T) - 1; i >= 0; i--) *(ptr++) = this->arrayPointer[offset + i];
	return true;
}

class StackArrayBuffer : public ArrayBuffer, public ByteStack {
public:
	//Construct this ArrayStackBuffer with the size 'capacity'
	StackArrayBuffer(int capacity, Endian systemEndian) :ArrayBuffer(capacity, systemEndian) {};
	//Construct this ArrayStackBuffer with the size 'capacity' and then copy 'dataSize' byte(s) from memory block pointed by memPtr into buffer.
	StackArrayBuffer(void* memPtr, int capacity, int dataSize, Endian systemEndian): ArrayBuffer(memPtr, capacity, dataSize, systemEndian) {};
	//Construct this ArrayStackBuffer to be enough to store the input string
	StackArrayBuffer(string inputString, Endian systemEndian) :ArrayBuffer(inputString, systemEndian) {};
	//Construct this ArrayStackBuffer with the size 'capacity' and then store input string into it.
	StackArrayBuffer(int capacity, string inputString, Endian systemEndian) :ArrayBuffer( capacity, inputString, systemEndian) {};
	//Destructor: Unallocate all memory.
	~StackArrayBuffer();
	//Methods that will throw exception when error occur (in the case of empty/full stack errors)
	uint8_t popByte();		//Return the byte at top of stack and then remove it from stack
	char popChar();			//Return the byte at top of stack as a character and then remove it from stack
	uint8_t topByte();		//Return the byte at top of stack without removing it from stack
	char topChar();			//Return the byte at top of stack as a character without removing it from stack
	//Methods that return false when error occur (in the case of empty/full stack errors), output value via a pointer
	bool popByte(uint8_t* outputByte);	//Return the byte at top of stack and then remove it from stack
	bool popChar(char* outputChar);		//Return the byte at top of stack as a character and then remove it from stack
	bool topByte(uint8_t* outputByte);	//Return the byte at top of stack without removing it from stack
	bool topChar(char* outputChar);		//Return the byte at top of stack as a character without removing it from stack
	bool pushByte(uint8_t dataByte);	//Push a byte to stack, return true if insertion was OK
	bool pushChar(char dataChar);		//Push a character to stack, return true if insertion was OK
};

class QueueArrayBuffer :public ArrayBuffer, public ByteQueue {
	int firstIndex, lastIndex;
public:
	//Construct this ArrayStackBuffer with the size 'capacity'
	QueueArrayBuffer(int capacity, Endian systemEndian);
	//Construct this ArrayStackBuffer with the size 'capacity' and then copy 'dataSize' byte(s) from memory block pointed by memPtr into buffer.
	QueueArrayBuffer(void* memPtr, int capacity, int dataSize, Endian systemEndian);
	//Construct this ArrayStackBuffer to be enough to store the input string
	QueueArrayBuffer(string inputString, Endian systemEndian);
	//Construct this ArrayStackBuffer with the size 'capacity' and then store input string into it.
	QueueArrayBuffer(int capacity, string inputString, Endian systemEndian);
	//Destructor: Unallocate all memory.
	~QueueArrayBuffer();
	//Methods that will throw exception when error occur (in the case of empty/full queue errors)
	uint8_t deQueueByte();					//Return the first-joined byte in the queue and then remove it from the queue
	char deQueueChar();						//Return the first-joined byte in the queue as a character and then remove it from the queue
	//Methods that return false when error occur (in the case of empty/full queue errors), output value via a pointer
	bool enQueueByte(uint8_t dataByte);		//Push a byte to the queue, return true if insertion was OK
	bool enQueueChar(char dataChar);		//Push a character to the queue, return true if insertion was OK
	bool deQueueByte(uint8_t* outputByte);	//Return the first-joined byte in the queue and then remove it from the queue
	bool deQueueChar(char* outputChar);		//Return the first-joined byte in the queue as a character and then remove it from the queue
};
#endif // !_BUFFER_H_
