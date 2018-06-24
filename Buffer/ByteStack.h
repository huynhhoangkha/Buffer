//Byte stack interface by Huynh Hoang Kha
#pragma once
#ifndef _BYTE_STACK_H_
#define _BYTE_STACK_H_
#include <cstdint>
#include"Endian.h"

class ByteStack{
public:
	//Methods that will throw exception when error occur (in the case of empty/full stack errors)
	virtual uint8_t popByte() = 0;					//Return the byte at top of stack and then remove it from stack
	char popChar();									//Return the byte at top of stack as a character and then remove it from stack
	virtual int popInt();							//Return an int at top of stack and then remove it from stack
	virtual float popFloat();						//Return a float at top of stack and then remove it from stack
	virtual long popLong();							//Return a long at top of stack and then remove it from stack
	virtual double popDouble();						//Return a double at top of stack and then remove it from stack
	virtual uint8_t topByte() = 0;					//Return the byte at top of stack without removing it from stack
	char topChar();									//Return the byte at top of stack as a character without removing it from stack
	virtual int topInt();							//Return the an int at top of stack without removing it from stack
	virtual float topFloat();						//Return the a float at top of stack without removing it from stack
	virtual long topLong();							//Return the a long at top of stack without removing it from stack
	virtual double topDouble();						//Return the a double at top of stack without removing it from stack
	//Methods that return false when error occur (in the case of empty/full stack errors), output value via a pointer
	virtual bool popByte(uint8_t* outputByte) = 0;	//Return the byte at top of stack and then remove it from stack
	bool popChar(char* outputChar);					//Return the byte at top of stack as a character and then remove it from stack
	virtual bool popInt(int* outputInt);			//Return an int at top of stack and then remove it from stack
	virtual bool popFloat(float* outputFloat);		//Return a float at top of stack and then remove it from stack
	virtual bool popLong(long* outputLong);			//Return a long at top of stack and then remove it from stack
	virtual bool popDouble(double* outputDouble);	//Return a double at top of stack and then remove it from stack
	virtual bool topByte(uint8_t* outputByte) = 0;	//Return the byte at top of stack without removing it from stack
	bool topChar(char* outputChar);					//Return the byte at top of stack as a character without removing it from stack
	virtual bool topInt(int* outputInt);			//Return the an int at top of stack without removing it from stack
	virtual bool topFloat(float* outputFloat);		//Return the a float at top of stack without removing it from stack
	virtual bool topLong(long* outputLong);			//Return the a long at top of stack without removing it from stack
	virtual bool topDouble(double* outputDouble);	//Return the a double at top of stack without removing it from stack
	virtual bool pushByte(uint8_t dataByte) = 0;	//Push a byte to stack, return true if insertion was OK
	bool pushChar(char dataChar);					//Push a character to stack, return true if insertion was OK
	virtual bool pushInt(int dataInt);				//Push an int to stack, return true if insertion was OK
	virtual bool pushFloat(float dataFloat);		//Push a float to stack, return true if insertion was OK
	virtual bool pushLong(long dataLong);			//Push a long to stack, return true if insertion was OK
	virtual bool pushDouble(double dataDouble);		//Push a double to stack, return true if insertion was OK
};
#endif // !_BYTE_STACK_H_
