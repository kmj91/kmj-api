#pragma once
#ifndef _CQUEUE_
#define _CQUEUE_

#include <string.h>

class CQueue{
public :
	enum en_CQueue {
		DEFAULT_SIZE = 100
	};

public :
	CQueue() {
		_queue = new int[DEFAULT_SIZE];
		_rear = 0;
		_front = 0;
		_queueSize = DEFAULT_SIZE;	//배열사이즈
	}

	CQueue(int Size) {
		_queue = new int[Size];
		_rear = 0;
		_front = 0;
		_queueSize = Size;	//배열사이즈
	}

	//깊은 복사
	CQueue(const CQueue& copy) : _rear(copy._rear), _front(copy._front), _queueSize(copy._queueSize) {
		_queue = new int[copy._queueSize];
		for (int i = 0; i < copy._queueSize; i++) {
			_queue[i] = copy._queue[i];
		}
	}

	~CQueue() {
		delete[]_queue;
	}

	bool Enqueue(int Data) {
		//배열이 다 찼는가?
		if ((_rear + 1) % _queueSize == _front) {
			return false;
		}
		_queue[_rear] = Data;
		_rear = (_rear + 1) % _queueSize;
		return true;
	}

	bool Dequeue(int* pOut) {
		//비어있는가?
		if (_rear == _front) {
			return false;
		}
		*pOut = _queue[_front];
		_front = (_front + 1) % _queueSize;
		return true;
	}

	bool Peek(int* pOut) {
		//비어있는가?
		if (_rear == _front) {
			return false;
		}
		*pOut = _queue[_front];
		return true;
	}

	// 현재 사용중인 개수
	int	GetUseSize() {
		if (_rear > _front) {
			return _rear - _front;
		}
		else if (_rear < _front) {
			return (_queueSize - _front) + _rear;
		}
		else {
			return 0;
		}
	}
	
	// 스택의 전체 개수
	int	GetQueueSize() {
		return _queueSize;
	}

	// 데이터가 있는지 없는지 true / false
	bool isEmpty() {
		if ((_rear + 1) % _queueSize == _front) {
			return false;
		}
		else {
			return true;
		}
	}

	void Release()
	{
		_rear = 0;
		_front = 0;
	}

protected :
	int * _queue;
	int _rear;
	int _front;
	int _queueSize;
};



#endif