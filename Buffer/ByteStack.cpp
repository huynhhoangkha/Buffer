#include "ByteStack.h"

char ByteStack::popChar() { return (char)this->popByte(); }

int ByteStack::popInt()
{
	return 0;
}

float ByteStack::popFloat()
{
	return 0.0f;
}

long ByteStack::popLong()
{
	return 0;
}

double ByteStack::popDouble()
{
	return 0.0;
}

char ByteStack::topChar()
{
	return 0;
}

int ByteStack::topInt()
{
	return 0;
}

float ByteStack::topFloat()
{
	return 0.0f;
}

long ByteStack::topLong()
{
	return 0;
}

double ByteStack::topDouble()
{
	return 0.0;
}

bool ByteStack::popChar(char * outputChar)
{
	return false;
}

bool ByteStack::popInt(int * outputInt)
{
	return false;
}

bool ByteStack::popFloat(float * outputFloat)
{
	return false;
}

bool ByteStack::popLong(long * outputLong)
{
	return false;
}

bool ByteStack::popDouble(double * outputDouble)
{
	return false;
}

bool ByteStack::topChar(char * outputChar)
{
	return false;
}

bool ByteStack::topInt(int * outputInt)
{
	return false;
}

bool ByteStack::topFloat(float * outputFloat)
{
	return false;
}

bool ByteStack::topLong(long * outputLong)
{
	return false;
}

bool ByteStack::topDouble(double * outputDouble)
{
	return false;
}

bool ByteStack::pushChar(char dataChar)
{
	return false;
}

bool ByteStack::pushInt(int dataInt)
{
	return false;
}

bool ByteStack::pushFloat(float dataFloat)
{
	return false;
}

bool ByteStack::pushLong(long dataLong)
{
	return false;
}

bool ByteStack::pushDouble(double dataDouble)
{
	return false;
}
