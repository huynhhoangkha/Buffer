//Buffer library by Huynh Hoang Kha
//This implement stack/queue buffers
//Using linked list and array
#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <cstdint>
#include <string>
#include "Stack.h"
#include "Queue.h"
using namespace std;

enum Endian {
	NOT_SET,
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

enum ExceptionErrorCode {
	EMPTY_INITIALIZATION_STRING,
	NEGATIVE_CAPACITY,
	NEGATIVE_SIZE,
	SIZE_BIGGER_THAN_CAPACITY,
	NEGATIVE_INDEX,
	OUT_OF_RANGE_INDEX,
	NOT_SET_ENDIAN,
	POP_FROM_EMPTY_STACK,
	ACCESS_TO_EMPTY_STACK,
	PUSH_TO_FULL_STACK,
	NOT_ENOUGH_DATA_TO_POP,
	NOT_ENOUGH_DATA_TO_TOP,
	NOT_ENOUGH_SPACE_TO_PUSH,
	UNKNOWN_EXCEPTION
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
	char getChar(int index);					//Return the byte at index as a character
	virtual string getString() = 0;				//Return the whole data as std::string object
	virtual int getInt(int offset) = 0;			//Return an 4-byte integer starting from the offset index byte
	virtual float getFloat(int offset) = 0;		//Return an 4-byte float starting from the offset index byte
	virtual long getLong(int offset) = 0;		//Return an 8-byte long starting from the offset index byte
	virtual double getDouble(int offset) = 0;	//Return an 8-byte double starting from the offset index byte
	//Methods that return false when error occur (in the case of invalid index/offset), output value via a pointer
	virtual uint8_t& operator[](int index) = 0;								//Return reference to the byte at index
	virtual bool getByte(int index, uint8_t* outputByte);					//Return the byte at index
	bool getChar(int index, char* outputChar);								//Return the byte at index as a character
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
	Endian endian;								//System endian
};

class ArrayBuffer :public Buffer {
protected:
	uint8_t* arrayPointer;
public:
	//Construct this ArrayStackBuffer with the size 'capacity'
	ArrayBuffer(int capacity, Endian systemEndian);
	//Construct this ArrayStackBuffer with the size 'capacity' and then copy 'dataSize' byte(s) from memory block pointed by memPtr into buffer.
	ArrayBuffer(void* memPtr, int capacity, int dataSize, Endian systemEndian);
	//Construct this buffer to be enough to store the 'inputString'
	ArrayBuffer(string inputString, Endian systemEndian);
	//Construct this buffer with the size 'capacity' and then store the inputString into it.
	ArrayBuffer(int capacity, string inputString, Endian systemEndian);
	//Free up all allocated memory
	~ArrayBuffer();
	virtual void clean();									//Clean the buffer's content
	//Methods that will throw exception when error occur (in the case of invalid index/offset)
	virtual uint8_t& operator[](int index);					//Return reference to the byte at index
	virtual string getString();								//Return the whole data as std::string object
	virtual int getInt(int offset);							//Return an 4-byte integer starting from the offset index byte
	virtual float getFloat(int offset);						//Return an 4-byte float starting from the offset index byte
	virtual long getLong(int offset);						//Return an 8-byte long starting from the offset index byte
	virtual double getDouble(int offset);					//Return an 8-byte double starting from the offset index byte
	//Methods that return false when error occur (in the case of invalid index/offset), output value via a pointer
	virtual bool getInt(int offset, int* outputInt);					//Return an 4-byte integer starting from the offset index byte
	virtual bool getFloat(int offset, float* outputFloat);				//Return an 4-byte float starting from the offset index byte
	virtual bool getLong(int offset, long* outputLong);					//Return an 8-byte long starting from the offset index byte
	virtual bool getDouble(int offset, double* outputDouble);			//Return an 8-byte double starting from the offset index byte
	virtual bool getMemoryBlock(void* memPtr, int offset, int size);	//Copy 'size' bytes from buffer into a memory block pointed by memPtr
	/*
	Be careful when using write methods, they are build base on the writePrimity template,
	and they just generally write data into the data array. The size of the buffer will not
	be updated.
	*/
	virtual bool writeInt(int offset, int data);			//Write an int value into the buffer at 'offset' position
	virtual bool writeFloat(int offset, float data);		//Write an float value into the buffer at 'offset' position
	virtual bool writeLong(int offset, long data);			//Write an long value into the buffer at 'offset' position
	virtual bool writeDouble(int offset, double data);		//Write an double value into the buffer at 'offset' position
	template <typename T> bool writePrimity(int offset, T data);		//write any primitive object into buffer at 'offset' position
	template <typename T> bool getPrimity(int offset, T* outputObject);	//Get any primitive object, return result via an object pointer
};

#pragma region ArrayBuffer templates
template<typename T>
inline bool ArrayBuffer::writePrimity(int offset, T data) {
	if (offset < 0 || offset + sizeof(T) > this->capacity) return false;
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
	return true;
}

template<typename T>
inline bool ArrayBuffer::getPrimity(int offset, T * outputObject) {
	if (this->endian == NOT_SET || offset < 0 || offset + sizeof(T) > this->capacity) return false;
	uint8_t* ptr = (uint8_t*)outputObject;
	if (this->endian == BIG_ENDIAN) for (int i = 0; i < sizeof(T); i++) *(ptr++) = this->arrayPointer[offset + i];
	else if (this->endian == LITTLE_ENDIAN) for (int i = sizeof(T) - 1; i >= 0; i--) *(ptr++) = this->arrayPointer[offset + i];
	return true;
}
#pragma endregion ArrayBuffer templates

#pragma region StackArrayBuffer
class StackArrayBuffer : public ArrayBuffer, public Stack<uint8_t> {
public:
	//Construct this ArrayStackBuffer with the size 'capacity'
	StackArrayBuffer(int capacity, Endian systemEndian) :ArrayBuffer(capacity, systemEndian) {};
	//Construct this ArrayStackBuffer with the size 'capacity' and then copy 'dataSize' byte(s) from memory block pointed by memPtr into buffer.
	StackArrayBuffer(void* memPtr, int capacity, int dataSize, Endian systemEndian) : ArrayBuffer(memPtr, capacity, dataSize, systemEndian) {};
	//Construct this ArrayStackBuffer to be enough to store the input string
	StackArrayBuffer(string inputString, Endian systemEndian) :ArrayBuffer(inputString, systemEndian) {};
	//Construct this ArrayStackBuffer with the size 'capacity' and then store input string into it.
	StackArrayBuffer(int capacity, string inputString, Endian systemEndian) :ArrayBuffer(capacity, inputString, systemEndian) {};
	//Destructor: Unallocate all memory.
	~StackArrayBuffer();
	//Methods that will throw exception when error occur (in the case of empty/full stack errors)
	template <typename T> T pop();		//Return the primity at top of stack and then remove it from stack
	template <typename T> T top();		//Return the primity at top of stack without removing it from stack
	//Methods that return false when error occur (in the case of empty/full stack errors), output value via a pointer
	template <typename T> bool pop(T* output);	//Return the primity at top of stack and then remove it from stack
	template <typename T> bool top(T* output);	//Return the primity at top of stack without removing it from stack
	template <typename T> bool push(T input);	//Push a primity to stack, return true if insertion was OK
	//Implement compulsory methods in the stack interface
	uint8_t pop() { return this->pop<uint8_t>(); }				//implement the 1-byte pop() method from stack interface
	uint8_t top() { return this->top<uint8_t>(); }				//implement the 1-byte top() method from stack interface
	bool pop(uint8_t* output) { return this->pop(output); }		//implement the 1-byte pop() method from stack interface
	bool top(uint8_t* output) { return this->top(output); }		//implement the 1-byte top() method from stack interface
	bool push(uint8_t input) { return this->push(input); }		//implement the 1-byte push() method from stack interface
	//Stack methods implementation for char
	char popChar() { return this->pop<char>(); }				//method to pop a character using: T pop() template
	char topChar() { return this->top<char>(); }				//method to get a top character using: T top() template
	bool popChar(char* output) { return this->pop(output); }	//method to pop a character using: bool pop(T* output) template
	bool topChar(char* output) { return this->top(output); }	//method to get a top character using: bool top(T* output) template
	bool pushChar(char input) { return this->push(input); }		//method to push a character using: bool push(T input) template
	//Stack methods implementation for int						
	int popInt() { return this->pop<int>(); }					//method to pop an int using: T pop() template
	int topInt() { return this->top<int>(); }					//method to get a top int using: T top() template
	bool popInt(int* output) { return this->pop(output); }		//method to pop an int using: bool pop(T* output) template
	bool topInt(int* output) { return this->top(output); }		//method to get a top int using: bool top(T* output) template
	bool pushInt(int input) { return this->push(input); }		//method to push an int using: bool push(T input) template
	//Stack methods implementation for float
	float popFloat() { return this->pop<float>(); }				//method to pop a float using: T pop() template
	float topFloat() { return this->top<float>(); }				//method to get a top float using: T top() template
	bool popFloat(float* output) { return this->pop(output); }	//method to pop a float using: bool pop(T* output) template
	bool topFloat(float* output) { return this->top(output); }	//method to get a top float using: bool top(T* output) template
	bool pushFloat(float input) { return this->push(input); }	//method to push a float using: bool push(T input) template
	//Stack methods implementation for long
	long popLong() { return this->pop<long>(); }				//method to pop a long using: T pop() template
	long topLong() { return this->top<long>(); }				//method to get a top long using: T top() template
	bool popLong(long* output) { return this->pop(output); }	//method to pop a long using: bool pop(T* output) template
	bool topLong(long* output) { return this->top(output); }	//method to get a top long using: bool top(T* output) template
	bool pushLong(long input) { return this->push(input); }		//method to push a long using: bool push(T input) template
	//Stack methods implementation for double
	double popDouble() { return this->pop<double>(); }				//method to pop a double using: T pop() template
	double topDouble() { return this->top<double>(); }				//method to get a top double using: T top() template
	bool popDouble(double* output) { return this->pop(output); }	//method to pop a double using: bool pop(T* output) template
	bool topDouble(double* output) { return this->top(output); }	//method to get a top double using: bool top(T* output) template
	bool pushDouble(double input) { return this->push(input); }		//method to push a double using: bool push(T input) template
};
#pragma endregion StackArrayBuffer

#pragma region StackArrayBuffer templates
template<typename T>
inline T StackArrayBuffer::pop() {
	int offset = this->size - sizeof(T);
	if (offset < 0) {
		BufferException bE(NOT_ENOUGH_DATA_TO_POP, "Data in the stack is not enough to pop");
		throw bE;
	}
	T output;
	if (this->getPrimity(offset, &output)) {
		this->size -= sizeof(T);
		this->arrayPointer[this->size] = '\0';
		return output;
	}
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

template<typename T>
inline T StackArrayBuffer::top() {
	int offset = this->size - sizeof(T);
	if (offset < 0) {
		BufferException bE(NOT_ENOUGH_DATA_TO_POP, "Data in the stack is not enough to pop");
		throw bE;
	}
	T output;
	if (this->getPrimity(offset, &output)) return output;
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

template<typename T>
inline bool StackArrayBuffer::pop(T * output) {
	int offset = this->size - sizeof(T);
	if (offset < 0) return false;
	if (this->getPrimity(offset, output)) {
		this->size -= sizeof(T);
		this->arrayPointer[this->size] = '\0';
		return true;
	}
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

template<typename T>
inline bool StackArrayBuffer::top(T * output) {
	int offset = this->size - sizeof(T);
	if (offset < 0) return false;
	return this->getPrimity(offset, output);
}

template<typename T>
inline bool StackArrayBuffer::push(T dataByte) {
	if (this->capacity - this->size < sizeof(T)) return false;
	return this->writePrimity(this->size, dataByte);
}

#pragma endregion StackArrayBuffer templates

#pragma region QueueArrayBuffer
class QueueArrayBuffer :public ArrayBuffer, public Queue<uint8_t> {
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
	//Override getString method
	string getString();
	//Templates for all queue's methods
	template <typename T> bool enQueue(T input);
	template <typename T> bool deQueue(T* output);
	//Methods must be implemented to complete the Queue<uint8_t> interface
	bool enQueue(uint8_t dataByte) { return this->enQueue(dataByte); };			//Push a byte to the queue, return true if insertion was OK
	bool deQueue(uint8_t* outputDataByte) { return this->deQueue(outputDataByte); };	//Return the first-joined byte in the queue and then remove it from the queue
	//Queue methods for char
	bool enQueueChar(char dataChar) { return this->enQueue(dataChar); };			//Push a char to the queue, return true if insertion was OK
	bool deQueueChar(char* outputDataChar) { return this->deQueue(outputDataChar); };		//Return the first-joined char in the queue and then remove it from the queue
	//Queue methods for int

	//Queue methods for float

	//Queue methods for long

	//Queue methods for double
	
};
#pragma endregion QueueArrayBuffer

#pragma region QueueArrayBuffer templates
template<typename T>
inline bool QueueArrayBuffer::enQueue(T input) {
	if (this->size + sizeof(T) > this->capacity) return false;
	bool status = this->writePrimity(this->lastIndex + 1, input);
	if (status == true) {
		this->lastIndex = (this->lastIndex + sizeof(T)) % this->capacity;
		this->size += sizeof(T);
	}
	return status;
}

template<typename T>
inline bool QueueArrayBuffer::deQueue(T* output) {
	if (this->size < sizeof(T)) return false;
	bool status = this->getPrimity(this->firstIndex, output);
	if (status == true) {
		this->firstIndex = (this->firstIndex + sizeof(T)) % this->capacity;
		this->size = this->size - sizeof(T);
	}
	return status;
}

#pragma endregion QueueArrayBuffer templates
#endif // !_BUFFER_H_