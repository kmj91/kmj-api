#pragma once
/*---------------------------------------------------------------

procademy MemoryPool.

메모리 풀 클래스 (오브젝트 풀)
특정 데이타(구조체,클래스,변수)를 일정량 할당 후 나눠쓴다.

- 사용법.

procademy::CObjectPool<DATA> MemPool(300, FALSE);
DATA *pData = MemPool.Alloc();

pData 사용

MemPool.Free(pData);


----------------------------------------------------------------*/
//#ifndef  __PROCADEMY_MEMORY_POOL__
//#define  __PROCADEMY_MEMORY_POOL__
#include <assert.h>
#include <new.h>
#include <malloc.h>
#include <intrin.h>		//_InterlockedCompareExchange

template <class DATA>
class CObjectPool
{
private:
	template <class DATA>
	friend class CObjectPoolTLS;

	enum en_CMemoryPool {
		HASH_CODE = 77,
		SUPPLEMENT_HASH_CODE = 88
	};

	// 128 비트 주소를 담을 수있는 구조체
	__declspec(align(16)) typedef struct _LARGE_INTEGER_128 {
		__int64 Int[2];
	} LARGE_INTEGER_128, *PLARGE_INTEGER_128;

	/* **************************************************************** */
	// 각 블럭 앞에 사용될 노드 구조체.
	/* **************************************************************** */
	struct st_BLOCK_NODE
	{
		st_BLOCK_NODE()
		{
			chHashCode = SUPPLEMENT_HASH_CODE;
			stpNextBlock = NULL;
		}
		char chHashCode;
		st_BLOCK_NODE *stpNextBlock;
	};

public:

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 최대 블럭 개수.
	//				(bool) 생성자 호출 여부.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CObjectPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CObjectPool();

private:
	// TLS에서 호출 생성자
	CObjectPool(int iChunkNum, int iBlockNum, void * pYobidashi , bool bPlacementNew = false);

public:
	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return m_lAllocCount + m_lSupplementBlockCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_lUseCount + m_lSupplementUseCount; }

private:
	// 초기화
	void InitBlock();
	// 추가 할당
	DATA * SupplementBlock();
	// TLS 초기화
	void InitBlockTLS();
	// Chunk
	DATA	*AllocTLS(void);
	// 추가 할당 TLS
	DATA * SupplementBlockTLS();


private:
	char * m_chObjectMemory;
	volatile long m_lAllocCount;
	volatile long m_lUseCount;
	bool m_bPlacementOption;
	//st_BLOCK_NODE * m_stTop[2];
	volatile LARGE_INTEGER_128 m_stTop;
	volatile long m_lSupplementBlockCount;
	volatile long m_lSupplementUseCount;
	volatile LARGE_INTEGER_128 m_stSupplementBlockTop;
	long m_lChunkBlockCount;
	void * m_pYobidashi;
};




//생성자
template <class DATA>
CObjectPool<DATA>::CObjectPool(int iBlockNum, bool bPlacementNew) {
	if (iBlockNum < 0) {
		iBlockNum = 0;
	}

	m_lAllocCount = iBlockNum;
	m_lUseCount = 0;
	m_lSupplementBlockCount = 0;
	m_lSupplementUseCount = 0;
	m_stTop.Int[0] = NULL;
	m_stTop.Int[1] = NULL;
	m_stSupplementBlockTop.Int[0] = NULL;
	m_stSupplementBlockTop.Int[1] = NULL;

	//if (bPlacementNew) {
	//	m_bPlacementOption = true;
	//}
	//else {
	//	m_bPlacementOption = false;
	//}

	m_bPlacementOption = bPlacementNew;

	if (iBlockNum) {
		//초기화
		InitBlock();
	}
}

//파괴자
template <class DATA>
CObjectPool<DATA>::~CObjectPool() {
	int iSize;
	int iCnt;
	void * temp;

	free(m_chObjectMemory);

	iSize = m_lSupplementBlockCount - m_lSupplementUseCount;

	for (iCnt = 0; iCnt < iSize; iCnt++) {
		temp = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
		m_stSupplementBlockTop.Int[0] = (__int64)((st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0])->stpNextBlock;
		free(temp);
	}
}

// 생성자
// TLS에서 호출 생성자
template <class DATA>
CObjectPool<DATA>::CObjectPool(int iChunkNum, int iBlockNum, void * pYobidashi, bool bPlacementNew) {
	if (iChunkNum < 0) {
		iChunkNum = 0;
	}
	
	if (iBlockNum < 0) {
		iBlockNum = 100;
	}

	m_lChunkBlockCount = iBlockNum;
	m_pYobidashi = pYobidashi;

	m_lAllocCount = iChunkNum;
	m_lUseCount = 0;
	m_lSupplementBlockCount = 0;
	m_lSupplementUseCount = 0;
	m_stTop.Int[0] = NULL;
	m_stTop.Int[1] = NULL;
	m_stSupplementBlockTop.Int[0] = NULL;
	m_stSupplementBlockTop.Int[1] = NULL;

	if (bPlacementNew) {
		m_bPlacementOption = true;
	}
	else {
		m_bPlacementOption = false;
	}

	if (iChunkNum) {
		//초기화
		InitBlockTLS();
	}
}

//사용
template <class DATA>
DATA * CObjectPool<DATA>::Alloc(void) {
	long lLocalAllocCount;
	long lLocalUseCount;
	long lLocalSupplementBlockCount;
	long lLocalSupplementUseCount;
	DATA * pData;
	//st_BLOCK_NODE * pLocalTop;
	LARGE_INTEGER_128 pLocalTop;
	LARGE_INTEGER_128 pCompareTop;
	//LARGE_INTEGER_128 popTop;
	st_BLOCK_NODE * popTop;

	lLocalAllocCount = m_lAllocCount;
	lLocalUseCount = m_lUseCount;
	lLocalSupplementBlockCount = m_lSupplementBlockCount;
	lLocalSupplementUseCount = m_lSupplementUseCount;

	if (lLocalAllocCount > lLocalUseCount) {
		while (1) {
			//pLocalTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			//m_stTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			//popTop.Int[0] = (__int64)pLocalTop;
			//popTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			popTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			if (popTop == NULL) {
				continue;
			}
			//m_stTop.Int[1] = (__int64)popTop->stpNextBlock;
			pLocalTop.Int[0] = (__int64)popTop;
			pLocalTop.Int[1] = (__int64)popTop->stpNextBlock;
			pCompareTop.Int[0] = pLocalTop.Int[1];
			if (popTop->stpNextBlock != NULL) {
				pCompareTop.Int[1] = (__int64)popTop->stpNextBlock->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}


			//if (_InterlockedCompareExchange128(m_stTop.Int, (__int64)popTop->stpNextBlock->stpNextBlock, (__int64)popTop->stpNextBlock, pLocalTop.Int) == 1) {
			if (_InterlockedCompareExchange128(m_stTop.Int, pCompareTop.Int[1], pCompareTop.Int[0], pLocalTop.Int) == 1) {
				//++m_iUseCount;
				_InterlockedIncrement(&m_lUseCount);
				//popTop = (st_BLOCK_NODE *)pLocalTop.Int[0];
				//pData = (DATA *)popTop.Int[0] + 1;
				pData = (DATA *)((st_BLOCK_NODE *)popTop + 1);
				// 생성자 호출
				if (m_bPlacementOption) {
					new (pData) DATA;
				}
				return pData;
			}
		}
	}


	if (lLocalSupplementBlockCount > lLocalSupplementUseCount) {
		while (1) {
			//pLocalTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			//if (pLocalTop->stpNextBlock != NULL) {
			//	m_stSupplementBlockTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			//}
			//else {
			//	m_stSupplementBlockTop.Int[1] = NULL;
			//}
			//popTop.Int[0] = (__int64)pLocalTop;
			//popTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			popTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			if (popTop == NULL) {
				continue;
			}
			//m_stSupplementBlockTop.Int[1] = (__int64)popTop->stpNextBlock;
			pLocalTop.Int[0] = (__int64)popTop;
			pLocalTop.Int[1] = (__int64)popTop->stpNextBlock;
			pCompareTop.Int[0] = pLocalTop.Int[1];
			if (popTop->stpNextBlock != NULL) {
				pCompareTop.Int[1] = (__int64)popTop->stpNextBlock->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}
			

			if (_InterlockedCompareExchange128(m_stSupplementBlockTop.Int, pCompareTop.Int[1], pCompareTop.Int[0], pLocalTop.Int) == 1) {
				//++m_iSupplementUseCount;
				_InterlockedIncrement(&m_lSupplementUseCount);
				//popTop = (st_BLOCK_NODE *)pLocalTop.Int[0];
				//pData = (DATA *)popTop.Int[0] + 1;
				pData = (DATA *)((st_BLOCK_NODE *)popTop + 1);
				// 생성자 호출
				if (m_bPlacementOption) {
					new (pData) DATA;
				}
				return pData;
			}
		}
	}

	
	
	//새로 만듬
	return SupplementBlock();
}

//해제
template <class DATA>
bool CObjectPool<DATA>::Free(DATA *pData) {
	st_BLOCK_NODE * pNewTop = (st_BLOCK_NODE *)pData - 1;
	st_BLOCK_NODE * pLocalTop;
	LARGE_INTEGER_128 pCompareTop;

	switch (pNewTop->chHashCode) {
	case HASH_CODE:
		//pNewTop->stpNextBlock = (st_BLOCK_NODE *)m_stTop.Int[0];
		//m_stTop.Int[0] = (__int64)pNewTop;
		//--m_iUseCount;
		_InterlockedDecrement(&m_lUseCount);
		do {
			//pLocalTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			//pNewTop->stpNextBlock = pLocalTop;
			pLocalTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			pCompareTop.Int[0] = (__int64)pLocalTop;
			if (pLocalTop != NULL) {
				pCompareTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}
			pNewTop->stpNextBlock = pLocalTop;
		//} while (_InterlockedCompareExchange64(&m_stTop.Int[0], (__int64)pNewTop, (__int64)pLocalTop) == m_stTop.Int[0]);
		} while (_InterlockedCompareExchange128(m_stTop.Int, (__int64)pNewTop->stpNextBlock, (__int64)pNewTop, pCompareTop.Int) != 1);
		return true;
	case SUPPLEMENT_HASH_CODE:
		//pNewTop->stpNextBlock = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
		//m_stSupplementBlockTop.Int[0] = (__int64)pNewTop;
		//--m_iSupplementUseCount;
		_InterlockedDecrement(&m_lSupplementUseCount);
		do {
			//pLocalTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			//pNewTop->stpNextBlock = pLocalTop;
			pLocalTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			pCompareTop.Int[0] = (__int64)pLocalTop;
			if (pLocalTop != NULL) {
				pCompareTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}
			pNewTop->stpNextBlock = pLocalTop;
		//} while (_InterlockedCompareExchange64(&m_stSupplementBlockTop.Int[0], (__int64)pNewTop, (__int64)pLocalTop) == m_stSupplementBlockTop.Int[0]);
		} while (_InterlockedCompareExchange128(m_stSupplementBlockTop.Int, (__int64)pNewTop->stpNextBlock, (__int64)pNewTop, pCompareTop.Int) != 1);
		return true;
	default:
		//printf("Free() error\n");
		throw;
	}
}


//초기화
template <class DATA>
void CObjectPool<DATA>::InitBlock() {
	int cnt;
	int iDataSize;
	int iNodeSize;
	st_BLOCK_NODE stNode;

	iDataSize = sizeof(DATA);
	iNodeSize = sizeof(st_BLOCK_NODE);
	m_chObjectMemory = (char *)malloc((iDataSize + iNodeSize) * m_lAllocCount);

	m_stTop.Int[0] = (__int64)m_chObjectMemory;

	if (m_bPlacementOption) {
		for (cnt = 0; cnt < m_lAllocCount - 1; cnt++) {
			stNode.chHashCode = HASH_CODE;
			stNode.stpNextBlock = (st_BLOCK_NODE *)(m_chObjectMemory + iDataSize + iNodeSize);
			*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
			m_chObjectMemory = m_chObjectMemory + iNodeSize + iDataSize;
		}
		stNode.chHashCode = HASH_CODE;
		stNode.stpNextBlock = NULL;
		*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
	}
	else {
		for (cnt = 0; cnt < m_lAllocCount - 1; cnt++) {
			stNode.chHashCode = HASH_CODE;
			stNode.stpNextBlock = (st_BLOCK_NODE *)(m_chObjectMemory + iDataSize + iNodeSize);
			*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
			m_chObjectMemory = m_chObjectMemory + iNodeSize;
			new (m_chObjectMemory) DATA;
			m_chObjectMemory = m_chObjectMemory + iDataSize;
		}
		stNode.chHashCode = HASH_CODE;
		stNode.stpNextBlock = NULL;
		*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
		m_chObjectMemory = m_chObjectMemory + iNodeSize;
		new (m_chObjectMemory) DATA;
	}

	//포인터 원위치
	m_chObjectMemory = (char *)m_stTop.Int[0];
	m_stTop.Int[1] = (__int64)((st_BLOCK_NODE *)m_stTop.Int[0])->stpNextBlock;
}

//추가 할당
template <class DATA>
DATA * CObjectPool<DATA>::SupplementBlock() {
	int iDataSize;
	int iNodeSize;
	//st_BLOCK_NODE stNode;
	char * chBlockMemory;
	//char * chRetval;

	iDataSize = sizeof(DATA);
	iNodeSize = sizeof(st_BLOCK_NODE);
	chBlockMemory = (char *)malloc(iDataSize + iNodeSize);
	int len = _msize(chBlockMemory);

	//chRetval = chBlockMemory + iNodeSize;

	//*(st_BLOCK_NODE *)chBlockMemory = stNode;
	((st_BLOCK_NODE *)chBlockMemory)->chHashCode = SUPPLEMENT_HASH_CODE;
	((st_BLOCK_NODE *)chBlockMemory)->stpNextBlock = NULL;
	chBlockMemory = chBlockMemory + iNodeSize;
	new (chBlockMemory) DATA;

	//++m_iSupplementBlockCount;
	//++m_iSupplementUseCount;
	_InterlockedIncrement(&m_lSupplementBlockCount);
	_InterlockedIncrement(&m_lSupplementUseCount);
	//return (DATA *)chRetval;
	return (DATA *)chBlockMemory;
}


// TLS 초기화
template <class DATA>
void CObjectPool<DATA>::InitBlockTLS() {
	int cnt;
	int iDataSize;
	int iNodeSize;
	st_BLOCK_NODE stNode;

	iDataSize = sizeof(DATA);
	iNodeSize = sizeof(st_BLOCK_NODE);
	m_chObjectMemory = (char *)malloc((iDataSize + iNodeSize) * m_lAllocCount);

	m_stTop.Int[0] = (__int64)m_chObjectMemory;

	if (m_bPlacementOption) {
		for (cnt = 0; cnt < m_lAllocCount - 1; cnt++) {
			stNode.chHashCode = HASH_CODE;
			stNode.stpNextBlock = (st_BLOCK_NODE *)(m_chObjectMemory + iDataSize + iNodeSize);
			*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
			m_chObjectMemory = m_chObjectMemory + iNodeSize + iDataSize;
		}
		stNode.chHashCode = HASH_CODE;
		stNode.stpNextBlock = NULL;
		*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
	}
	else {
		for (cnt = 0; cnt < m_lAllocCount - 1; cnt++) {
			stNode.chHashCode = HASH_CODE;
			stNode.stpNextBlock = (st_BLOCK_NODE *)(m_chObjectMemory + iDataSize + iNodeSize);
			*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
			m_chObjectMemory = m_chObjectMemory + iNodeSize;
			new (m_chObjectMemory) DATA(m_lChunkBlockCount, m_bPlacementOption, m_pYobidashi);
			m_chObjectMemory = m_chObjectMemory + iDataSize;
		}
		stNode.chHashCode = HASH_CODE;
		stNode.stpNextBlock = NULL;
		*(st_BLOCK_NODE *)m_chObjectMemory = stNode;
		m_chObjectMemory = m_chObjectMemory + iNodeSize;
		new (m_chObjectMemory) DATA(m_lChunkBlockCount, m_bPlacementOption, m_pYobidashi);
	}

	//포인터 원위치
	m_chObjectMemory = (char *)m_stTop.Int[0];
	m_stTop.Int[1] = (__int64)((st_BLOCK_NODE *)m_stTop.Int[0])->stpNextBlock;
}


//사용
template <class DATA>
DATA * CObjectPool<DATA>::AllocTLS(void) {
	long lLocalAllocCount;
	long lLocalUseCount;
	long lLocalSupplementBlockCount;
	long lLocalSupplementUseCount;
	DATA * pData;
	//st_BLOCK_NODE * pLocalTop;
	LARGE_INTEGER_128 pLocalTop;
	LARGE_INTEGER_128 pCompareTop;
	//LARGE_INTEGER_128 popTop;
	st_BLOCK_NODE * popTop;

	lLocalAllocCount = m_lAllocCount;
	lLocalUseCount = m_lUseCount;
	lLocalSupplementBlockCount = m_lSupplementBlockCount;
	lLocalSupplementUseCount = m_lSupplementUseCount;

	if (lLocalAllocCount > lLocalUseCount) {
		while (1) {
			//pLocalTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			//m_stTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			//popTop.Int[0] = (__int64)pLocalTop;
			//popTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			popTop = (st_BLOCK_NODE *)m_stTop.Int[0];
			if (popTop == NULL) {
				continue;
			}
			//m_stTop.Int[1] = (__int64)popTop->stpNextBlock;
			pLocalTop.Int[0] = (__int64)popTop;
			pLocalTop.Int[1] = (__int64)popTop->stpNextBlock;
			pCompareTop.Int[0] = pLocalTop.Int[1];
			if (popTop->stpNextBlock != NULL) {
				pCompareTop.Int[1] = (__int64)popTop->stpNextBlock->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}


			//if (_InterlockedCompareExchange128(m_stTop.Int, (__int64)popTop->stpNextBlock->stpNextBlock, (__int64)popTop->stpNextBlock, pLocalTop.Int) == 1) {
			if (_InterlockedCompareExchange128(m_stTop.Int, pCompareTop.Int[1], pCompareTop.Int[0], pLocalTop.Int) == 1) {
				//++m_iUseCount;
				_InterlockedIncrement(&m_lUseCount);
				//popTop = (st_BLOCK_NODE *)pLocalTop.Int[0];
				//pData = (DATA *)popTop.Int[0] + 1;
				pData = (DATA *)((st_BLOCK_NODE *)popTop + 1);
				// 생성자 호출
				if (m_bPlacementOption) {
					new (pData) DATA();
				}
				return pData;
			}
		}
	}


	if (lLocalSupplementBlockCount > lLocalSupplementUseCount) {
		while (1) {
			//pLocalTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			//if (pLocalTop->stpNextBlock != NULL) {
			//	m_stSupplementBlockTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			//}
			//else {
			//	m_stSupplementBlockTop.Int[1] = NULL;
			//}
			//popTop.Int[0] = (__int64)pLocalTop;
			//popTop.Int[1] = (__int64)pLocalTop->stpNextBlock;
			popTop = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
			if (popTop == NULL) {
				continue;
			}
			//m_stSupplementBlockTop.Int[1] = (__int64)popTop->stpNextBlock;
			pLocalTop.Int[0] = (__int64)popTop;
			pLocalTop.Int[1] = (__int64)popTop->stpNextBlock;
			pCompareTop.Int[0] = pLocalTop.Int[1];
			if (popTop->stpNextBlock != NULL) {
				pCompareTop.Int[1] = (__int64)popTop->stpNextBlock->stpNextBlock;
			}
			else {
				pCompareTop.Int[1] = NULL;
			}


			if (_InterlockedCompareExchange128(m_stSupplementBlockTop.Int, pCompareTop.Int[1], pCompareTop.Int[0], pLocalTop.Int) == 1) {
				//++m_iSupplementUseCount;
				_InterlockedIncrement(&m_lSupplementUseCount);
				//popTop = (st_BLOCK_NODE *)pLocalTop.Int[0];
				//pData = (DATA *)popTop.Int[0] + 1;
				pData = (DATA *)((st_BLOCK_NODE *)popTop + 1);
				// 생성자 호출
				if (m_bPlacementOption) {
					new (pData) DATA();
				}
				return pData;
			}
		}
	}



	//새로 만듬
	return SupplementBlockTLS();
}

//추가 할당
template <class DATA>
DATA * CObjectPool<DATA>::SupplementBlockTLS() {
	int iDataSize;
	int iNodeSize;
	//st_BLOCK_NODE stNode;
	char * chBlockMemory;
	//char * chRetval;

	iDataSize = sizeof(DATA);
	iNodeSize = sizeof(st_BLOCK_NODE);
	chBlockMemory = (char *)malloc(iDataSize + iNodeSize);
	int len = _msize(chBlockMemory);

	//chRetval = chBlockMemory + iNodeSize;

	//*(st_BLOCK_NODE *)chBlockMemory = stNode;
	((st_BLOCK_NODE *)chBlockMemory)->chHashCode = SUPPLEMENT_HASH_CODE;
	((st_BLOCK_NODE *)chBlockMemory)->stpNextBlock = NULL;
	chBlockMemory = chBlockMemory + iNodeSize;
	new (chBlockMemory) DATA(m_lChunkBlockCount, m_bPlacementOption, m_pYobidashi);

	//++m_iSupplementBlockCount;
	//++m_iSupplementUseCount;
	_InterlockedIncrement(&m_lSupplementBlockCount);
	_InterlockedIncrement(&m_lSupplementUseCount);
	//return (DATA *)chRetval;
	return (DATA *)chBlockMemory;
}

//#endif
