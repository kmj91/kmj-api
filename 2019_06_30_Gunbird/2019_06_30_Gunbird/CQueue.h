#pragma once

class CQueue{
public :
	CQueue();
	CQueue(int Size);
	//CQueue(CQueue &copy);	//얕은 복사
	CQueue(const CQueue &copy);	//깊은 복사
	~CQueue();

	bool Enqueue(int Data);
	bool Dequeue(int *pOut);
	bool Peek(int *pOut);

	int	GetUseSize();		// 현재 사용중인 개수
	int	GetQueueSize();		// 스택의 전체 개수
	bool isEmpty();		// 데이터가 있는지 없는지 true / false
	void Release();

protected :
	int * _queue;
	int _rear;
	int _front;
	int _queueSize;
};