#include "stdafx.h"
#include "CQueue.h"
#include <string.h>
#define DEFAULT_SIZE 100

CQueue::CQueue() {
	_queue = new int[DEFAULT_SIZE];
	_rear = 0;
	_front = 0;
	_queueSize = DEFAULT_SIZE;	//배열사이즈
}

CQueue::CQueue(int Size) {
	_queue = new int[Size];
	_rear = 0;
	_front = 0;
	_queueSize = Size;	//배열사이즈
}

//얕은 복사
//CQueue::CQueue(CQueue &copy) : _queue(copy._queue), _rear(copy._rear), _front(copy._front), _queueSize(copy._queueSize) {}

//깊은 복사
CQueue::CQueue(const CQueue &copy) : _rear(copy._rear), _front(copy._front), _queueSize(copy._queueSize) {
	_queue = new int[copy._queueSize];
	for (int i = 0; i < copy._queueSize; i++) {
		_queue[i] = copy._queue[i];
	}
}

CQueue::~CQueue() {
	delete []_queue;
}

bool CQueue::Enqueue(int Data) {
	//배열이 다 찼는가?
	if ((_rear + 1) % 10 == _front) {
		return false;
	}
	_queue[_rear] = Data;
	_rear = (_rear + 1) % _queueSize;
	return true;
}

bool CQueue::Dequeue(int *pOut) {
	//비어있는가?
	if (_rear == _front) {
		return false;
	}
	*pOut = _queue[_front];
	_front = (_front + 1) % _queueSize;
	return true;
}

bool CQueue::Peek(int *pOut) {
	//비어있는가?
	if (_rear == _front) {
		return false;
	}
	*pOut = _queue[_front];
	return true;
}

int	CQueue::GetUseSize() {
	if (_rear > _front) {
		return _rear - _front;
	}
	else if(_rear < _front) {
		return (_queueSize - _front) + _rear;
	}
	else {
		return 0;
	}
}

int	CQueue::GetQueueSize() {
	return _queueSize;
}

bool CQueue::isEmpty() {
	if ((_rear + 1) % 10 == _front) {
		return false;
	}
	else {
		return true;
	}
}

void CQueue::Release()
{
	_rear = 0;
	_front = 0;
}
