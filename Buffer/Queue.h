#pragma once
#ifndef _QUEUE_H_
#define _QUEUE_H_

template <class T>
class Queue {
public:
	virtual bool deQueue(T* dataPtr) = 0;
	virtual bool enQueue(T data) = 0;
};

#endif // !_QUEUE_H_
