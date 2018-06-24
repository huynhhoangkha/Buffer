#pragma once
#ifndef _STACK_H_
#define _STACK_H_

template <class T>
class Stack {
public:
	virtual T pop() = 0;
	virtual T top() = 0;
	virtual bool pop(T* output) = 0;
	virtual bool top(T* output) = 0;
	virtual bool push(T input) = 0;
};

#endif // !_STACK_H_
