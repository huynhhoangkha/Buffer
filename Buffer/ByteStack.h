//Byte stack interface by Huynh Hoang Kha
#pragma once
#ifndef _BYTE_STACK_H_
#define _BYTE_STACK_H_
#include <cstdint>

class ByteStack {
public:
	//Methods that will throw exception when error occur (in the case of empty/full stack errors)
	virtual uint8_t popByte() = 0;					//Return the byte at top of stack and then remove it from stack
	virtual char popChar() = 0;						//Return the byte at top of stack as a character and then remove it from stack
	virtual uint8_t topByte() = 0;					//Return the byte at top of stack without removing it from stack
	virtual char topChar() = 0;						//Return the byte at top of stack as a character without removing it from stack
	//Methods that return false when error occur (in the case of empty/full stack errors), output value via a pointer
	virtual bool popByte(uint8_t* outputByte) = 0;	//Return the byte at top of stack and then remove it from stack
	virtual bool popChar(char* outputChar) = 0;		//Return the byte at top of stack as a character and then remove it from stack
	virtual bool topByte(uint8_t* outputByte) = 0;	//Return the byte at top of stack without removing it from stack
	virtual bool topChar(char* outputChar) = 0;		//Return the byte at top of stack as a character without removing it from stack
	virtual bool pushByte(uint8_t dataByte) = 0;	//Push a byte to stack, return true if insertion was OK
	virtual bool pushChar(char dataChar) = 0;		//Push a character to stack, return true if insertion was OK
};
#endif // !_BYTE_STACK_H_
