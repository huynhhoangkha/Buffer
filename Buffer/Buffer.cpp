//Buffer library by Huynh Hoang Kha
//This implement stack/queue buffers
//Using linked list and array
#pragma warning (disable: 4996)
#pragma warning (disable: 4018) //This warning is in control!
#include "Buffer.h"

#pragma region BufferException implementation
//------------------------------------------------------------------------------------------------------------
//Section: BufferException implementation
BufferException::BufferException(int code, const char* message) {
	this->exceptionCode = code;
	this->msgPtr = NULL;
	while (this->msgPtr == NULL) this->msgPtr = new char[strlen(message) + 1];
	strcpy(this->msgPtr, message);
}

BufferException::BufferException(const BufferException & obj) {
	this->exceptionCode = obj.exceptionCode;
	this->msgPtr = NULL;
	while (this->msgPtr == NULL) this->msgPtr = new char[strlen(obj.msgPtr) + 1];
	strcpy(this->msgPtr, obj.msgPtr);
}

BufferException::~BufferException() { delete[] this->msgPtr;}
string BufferException::getMessage() { return string(msgPtr);}
int BufferException::getCode() { return this->exceptionCode;}
//Endsection: BufferException implementation
#pragma endregion BufferException implementation

#pragma region Some buffer methods implementation
//------------------------------------------------------------------------------------------------------------
//Section: Some buffer methods implementation
int Buffer::getCapacity() {return this->capacity;}
int Buffer::getSize() {return this->size;}
bool Buffer::isEmpty() {return (this->size == 0);}
bool Buffer::isFull() {return (this->size == this->capacity);}
uint8_t Buffer::getByte(int index) { return (*this)[index]; }
char Buffer::getChar(int index) { return (char)((*this)[index]); }
bool Buffer::getByte(int index, uint8_t *outputByte) {
	if (index < 0 || index + 1 > this->size) return false;
	*outputByte = (*this)[index];
	return true;
}
bool Buffer::getChar(int index, char *outputChar) { return getByte(index, (uint8_t*)outputChar); }
//Endsection: Some buffer methods implementation
#pragma endregion Some buffer methods implementation

#pragma region ArrayBuffer implementation
//------------------------------------------------------------------------------------------------------------
//Section: ArrayBuffer implementation
ArrayBuffer::ArrayBuffer(int capacity, Endian systemEndian) {
	if (capacity < 0) {
		BufferException bE(NEGATIVE_CAPACITY, "Invalid buffer capacity. It's can not be negative.!");
		throw bE;
	}
	this->endian = systemEndian;
	this->arrayPointer = NULL;
	while (this->arrayPointer == NULL) this->arrayPointer = new uint8_t[capacity];
	this->capacity = capacity;
	this->size = 0;
	memset((void*)this->arrayPointer, '\0', capacity);
}

ArrayBuffer::ArrayBuffer(void * memPtr, int capacity, int dataSize, Endian systemEndian) {
	if (capacity < 0) {
		BufferException bE(NEGATIVE_CAPACITY, "Invalid buffer capacity. It's can not be negative.!");
		throw bE;
	}
	if (dataSize < 0){
		BufferException bE(NEGATIVE_SIZE, "Invalid buffer size. It's can not be negative.!");
		throw bE;
	}
	if (dataSize > capacity) {
		BufferException bE(SIZE_BIGGER_THAN_CAPACITY, "Invalid buffer size. It's can not be lagger than capacity.!");
		throw bE;
	}
	this->endian = systemEndian;
	this->arrayPointer = NULL;
	while (this->arrayPointer == NULL) this->arrayPointer = new uint8_t[capacity];
	this->capacity = capacity;
	this->size = dataSize;
	memset((void*)this->arrayPointer, '\0', capacity);
	memcpy((void*)this->arrayPointer, memPtr, dataSize);
}

ArrayBuffer::ArrayBuffer(string inputString, Endian systemEndian) {
	if (inputString.length() == 0) {
		BufferException bE(EMPTY_INITIALIZATION_STRING, "Cannot initialize a buffer with a null string");
		throw bE;
	}
	this->capacity = (this->size = inputString.length());
	this->arrayPointer = NULL;
	while (this->arrayPointer == NULL) this->arrayPointer = new uint8_t[this->capacity];
	for (int i = 0; i < this->size; i++) this->arrayPointer[i] = inputString[i];
	this->endian = systemEndian;
}

ArrayBuffer::ArrayBuffer(int capacity, string inputString, Endian systemEndian) {
	int inputStringLength = inputString.length();
	if (capacity < 0) {
		BufferException bE(NEGATIVE_CAPACITY, "Invalid buffer capacity. It's can not be negative.!");
		throw bE;
	}
	if (inputStringLength >= capacity) {
		this->capacity = capacity;
		this->arrayPointer = NULL;
		while (this->arrayPointer == NULL) this->arrayPointer = new uint8_t[capacity];
		memset((void*)this->arrayPointer, '\0', capacity);
		for (this->size = 0; this->size + 1 < capacity; this->size++) {
			this->arrayPointer[this->size] = inputString[this->size];
		}
	}
	else {
		this->size = inputStringLength;
		this->capacity = capacity;
		this->arrayPointer = NULL;
		while (this->arrayPointer == NULL) this->arrayPointer = new uint8_t[capacity];
		memset((void*)this->arrayPointer, '\0', capacity);
		for (int i = 0; i < this->size; i++) {
			this->arrayPointer[i] = inputString[i];
		}
	}
	this->endian = systemEndian;
}

ArrayBuffer::~ArrayBuffer() {
	if (this->arrayPointer != NULL) {
		delete[] this->arrayPointer;
		this->arrayPointer = NULL;
	}
}

void ArrayBuffer::clean() {
	this->size = 0;
	memset((void*)this->arrayPointer, '\0', this->capacity);
}

uint8_t & ArrayBuffer::operator[](int index) {
	if (index < 0) {
		BufferException bE(NEGATIVE_INDEX, "Index can not be negative.");
		throw bE;
	}
	if (index + 1 > this->size) {
		BufferException bE(OUT_OF_RANGE_INDEX, "Index out of range.");
		throw bE;
	}
	return this->arrayPointer[index];
}

string ArrayBuffer::getString() {
	string result;
	for (int i = 0; i < this->size; i++) result.push_back((char)this->arrayPointer[i]);
	return result;
}

int ArrayBuffer::getInt(int offset) {
	int data = 0;
	if (this->endian == NOT_SET) {
		BufferException bE(NOT_SET_ENDIAN, "Please set Endian to be BIG_ENDIAN or LITTLE_ENDIAN according to you system.");
		throw bE;
	}
	if (offset < 0) {
		BufferException bE(NEGATIVE_INDEX, "Index can not be negative.");
		throw bE;
	}
	if (offset + sizeof(int) > this->size) {
		BufferException bE(OUT_OF_RANGE_INDEX, "Index out of range.");
		throw bE;
	}
	if (this->getPrimity(offset, &data)) return data;
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

float ArrayBuffer::getFloat(int offset) {
	float data = 0;
	if (this->endian == NOT_SET) {
		BufferException bE(NOT_SET_ENDIAN, "Please set Endian to be BIG_ENDIAN or LITTLE_ENDIAN according to you system.");
		throw bE;
	}
	if (offset < 0) {
		BufferException bE(NEGATIVE_INDEX, "Index can not be negative.");
		throw bE;
	}
	if (offset + sizeof(float) > this->size) {
		BufferException bE(OUT_OF_RANGE_INDEX, "Index out of range.");
		throw bE;
	}
	if (this->getPrimity(offset, &data)) return data;
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

long ArrayBuffer::getLong(int offset) {
	long data = 0;
	if (this->endian == NOT_SET) {
		BufferException bE(NOT_SET_ENDIAN, "Please set Endian to be BIG_ENDIAN or LITTLE_ENDIAN according to you system.");
		throw bE;
	}
	if (offset < 0) {
		BufferException bE(NEGATIVE_INDEX, "Index can not be negative.");
		throw bE;
	}
	if (offset + sizeof(double) > this->size) {
		BufferException bE(OUT_OF_RANGE_INDEX, "Index out of range.");
		throw bE;
	}
	if (this->getPrimity(offset, &data)) return data;
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

double ArrayBuffer::getDouble(int offset) {
	double data = 0;
	if (this->endian == NOT_SET) {
		BufferException bE(NOT_SET_ENDIAN, "Please set Endian to be BIG_ENDIAN or LITTLE_ENDIAN according to you system.");
		throw bE;
	}
	if (offset < 0) {
		BufferException bE(NEGATIVE_INDEX, "Index can not be negative.");
		throw bE;
	}
	if (offset + sizeof(double) > this->size) {
		BufferException bE(OUT_OF_RANGE_INDEX, "Index out of range.");
		throw bE;
	}
	if (this->getPrimity(offset, &data)) return data;
	else {
		BufferException bE(UNKNOWN_EXCEPTION, "Something wrong when calling getPrimity method.");
		throw bE;
	}
}

bool ArrayBuffer::getInt(int offset, int* outputInt) {return this->getPrimity(offset, outputInt);}
bool ArrayBuffer::getFloat(int offset, float* outputFloat) {return this->getPrimity(offset, outputFloat);}
bool ArrayBuffer::getLong(int offset, long * outputLong) { return this->getPrimity(offset, outputLong); }
bool ArrayBuffer::getDouble(int offset, double* outputDouble) {return this->getPrimity(offset, outputDouble);}

bool ArrayBuffer::getMemoryBlock(void * memPtr, int offset, int size) {
	if (offset < 0 || offset + size > this->size) return false;
	memcpy(memPtr, (void*)this->arrayPointer, size);
	return true;
}

bool ArrayBuffer::writeInt(int offset, int data) {return this->writePrimity(offset, data);}
bool ArrayBuffer::writeFloat(int offset, float data) { return this->writePrimity(offset, data); }
bool ArrayBuffer::writeLong(int offset, long data) { return this->writePrimity(offset, data); }
bool ArrayBuffer::writeDouble(int offset, double data) { return this->writePrimity(offset, data); }
//Endsection: ArrayBuffer implementation
#pragma endregion ArrayBuffer implementation

#pragma region StackArrayBuffer implementation
//------------------------------------------------------------------------------------------------------------
//Section: StackArrayBuffer implementation
StackArrayBuffer::~StackArrayBuffer() {
	if (this->arrayPointer != NULL) {
		delete[] this->arrayPointer;
		this->arrayPointer = NULL;
	}
}
//Endsection: StackArrayBuffer implementation
#pragma endregion StackArrayBuffer implementation

#pragma region QueueArrayBuffer implementation
//------------------------------------------------------------------------------------------------------------
//Section: QueueArrayBuffer implementation
#define INITIALIZE_QUEUE_ARRAY_INDEX this->firstIndex = 0; this->lastIndex = -1;
QueueArrayBuffer::QueueArrayBuffer(int capacity, Endian systemEndian) : ArrayBuffer(capacity, systemEndian) { INITIALIZE_QUEUE_ARRAY_INDEX }
QueueArrayBuffer::QueueArrayBuffer(void * memPtr, int capacity, int dataSize, Endian systemEndian) : ArrayBuffer(memPtr, capacity, dataSize, systemEndian) { INITIALIZE_QUEUE_ARRAY_INDEX }
QueueArrayBuffer::QueueArrayBuffer(string inputString, Endian systemEndian) : ArrayBuffer(inputString, systemEndian) { INITIALIZE_QUEUE_ARRAY_INDEX }
QueueArrayBuffer::QueueArrayBuffer(int capacity, string inputString, Endian systemEndian) :ArrayBuffer(capacity, inputString, systemEndian) { INITIALIZE_QUEUE_ARRAY_INDEX }

QueueArrayBuffer::~QueueArrayBuffer() {
	if (this->arrayPointer != NULL) {
		delete[] this->arrayPointer;
		this->arrayPointer = NULL;
	}
}

string QueueArrayBuffer::getString() {
	string result;
	int tempIdx = this->firstIndex;
	for (int i = 0; i < this->size; i++) {
		result.push_back((char)this->arrayPointer[tempIdx]);
		tempIdx = (tempIdx + 1) % this->capacity;
	}
	return result;
}
//Endsection: QueueArrayBuffer implementation
#pragma endregion QueueArrayBuffer implementation