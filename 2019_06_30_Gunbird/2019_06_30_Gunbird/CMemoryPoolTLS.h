#pragma once
// 2018.09.07			기명준

#include "CMemoryPool.h"

#include <Windows.h>
//#include <assert.h>
//#include <new.h>
//#include <malloc.h>
//#include <intrin.h>		//_InterlockedCompareExchange

template <class DATA>
class CMemoryPoolTLS
{
private:
	// CMemoryPool -> Check 덩어리 -> CMemoryPoolTLS
	
	template <class DATA>
	class Chunk;

	enum en_Chunk {
		CHUNK_HASH_CODE = 99,
		//ALLOC_COUNT = 100
	};

	// 각 블럭 앞에 사용될 노드 구조체.
	struct st_CHUNK_NODE
	{
		char chHashCode;
		st_CHUNK_NODE *stpNextBlock;
		Chunk<DATA> *pChunk;
	};

	template <class DATA>
	class Chunk {
	public:
		// CMemoryPool에서 처음 이후 호출하는 생성자
		Chunk() {

		}

		// CMemoryPool에서 처음 호출하는 생성자
		Chunk(int iBlockCount, bool bPlacementNew, void * pYobidashi) {
			m_lAllocCount = iBlockCount;
			//m_lUseCount = iBlockCount;
			m_lFreeCount = iBlockCount;
			m_pYobidashi = pYobidashi;
			//m_bPlacementOption = CMemoryPoolTLS<DATA>::m_bPLACEMENT;
			m_bPlacementOption = bPlacementNew;
			CreateDataList();
		}

		~Chunk() {
			free(m_chObjectChunk);
		}

		//DATA *Alloc() {
		//	st_CHUNK_NODE *popTop;

		//	--m_lUseCount;

		//	if (m_lUseCount == 0) {
		//		return NULL;
		//	}

		//	//if (m_stTop == 0) {
		//	//	return NULL;
		//	//}

		//	popTop = m_stTop;

		//	if (m_bPlacementOption) {
		//		// 다음 블록으로
		//		m_stTop = m_stTop->stpNextBlock;
		//		// 생성자 호출
		//		return (new ((DATA *)((st_CHUNK_NODE *)popTop + 1)) DATA);
		//	}
		//	else {
		//		// 다음 블록으로
		//		m_stTop = m_stTop->stpNextBlock;
		//		// 생성자 호출하지 않음
		//		return (DATA *)((st_CHUNK_NODE *)popTop + 1);
		//	}
		//}

		bool Alloc(DATA ** pPopTop) {
			*pPopTop = (DATA *)m_stTop;

			if (m_bPlacementOption) {
				// 다음 블록으로
				m_stTop = m_stTop->stpNextBlock;
				// 생성자 호출
				//return (new ((DATA *)((st_CHUNK_NODE *)popTop + 1)) DATA);
				*pPopTop = (new ((st_CHUNK_NODE *)*pPopTop + 1) DATA);

				if (m_stTop == 0) {
					return true;
				}
				return false;
			}
			else {
				// 다음 블록으로
				m_stTop = m_stTop->stpNextBlock;
				// 생성자 호출하지 않음
				//return (DATA *)((st_CHUNK_NODE *)popTop + 1);
				*pPopTop = (DATA *)((st_CHUNK_NODE *)*pPopTop + 1);

				if (m_stTop == 0) {
					return true;
				}
				return false;
			}
		}

		// 초기화
		void InitChunk() {
			m_lFreeCount = m_lAllocCount;
			m_stTop = (st_CHUNK_NODE *)m_chObjectChunk;;
		}

	private:
		// 리스트 생성
		void CreateDataList() {
			int cnt;
			int allocCount = m_lAllocCount - 1;
			int iDataSize;
			int iNodeSize;
			st_CHUNK_NODE stNode;

			iDataSize = sizeof(DATA);
			iNodeSize = sizeof(st_CHUNK_NODE);
			m_chObjectChunk = (char *)malloc((iDataSize + iNodeSize) * m_lAllocCount);

			m_stTop = (st_CHUNK_NODE *)m_chObjectChunk;

			if (m_bPlacementOption) {
				for (cnt = 0; cnt < allocCount; cnt++) {
					stNode.chHashCode = CHUNK_HASH_CODE;
					stNode.stpNextBlock = (st_CHUNK_NODE *)(m_chObjectChunk + iDataSize + iNodeSize);
					stNode.pChunk = this;
					*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
					m_chObjectChunk = m_chObjectChunk + iNodeSize + iDataSize;
				}
				stNode.chHashCode = CHUNK_HASH_CODE;
				stNode.stpNextBlock = NULL;
				stNode.pChunk = this;
				*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
			}
			else {
				for (cnt = 0; cnt < allocCount; cnt++) {
					stNode.chHashCode = CHUNK_HASH_CODE;
					stNode.stpNextBlock = (st_CHUNK_NODE *)(m_chObjectChunk + iDataSize + iNodeSize);
					stNode.pChunk = this;
					*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
					m_chObjectChunk = m_chObjectChunk + iNodeSize;
					new (m_chObjectChunk) DATA;
					m_chObjectChunk = m_chObjectChunk + iDataSize;
				}
				stNode.chHashCode = CHUNK_HASH_CODE;
				stNode.stpNextBlock = NULL;
				stNode.pChunk = this;
				*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
				m_chObjectChunk = m_chObjectChunk + iNodeSize;
				new (m_chObjectChunk) DATA;
			}

			//포인터 원위치
			m_chObjectChunk = (char *)m_stTop;
		}

	private:
		bool m_bPlacementOption;
		char * m_chObjectChunk;
		st_CHUNK_NODE * m_stTop;
		long m_lAllocCount;
		//long m_lUseCount;
	public:
		long m_lFreeCount;
		void * m_pYobidashi;
	};

public:

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 최대 블럭 개수.
	//				(bool) 생성자 호출 여부.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPoolTLS(int iChunkNum = 0, int iBlockNum = 100, bool bPlacementNew = false);
	virtual	~CMemoryPoolTLS();

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
	int		GetAllocCount(void) { return m_MemoryPool->GetAllocCount() * iChunkBlockCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_MemoryPool->GetUseCount() * iChunkBlockCount; }

private:
	CMemoryPool<Chunk<DATA>> * m_MemoryPool;
	int iChunkBlockCount;
	unsigned long m_TLSindex;
	bool m_bPlacementOption;

};















//생성자
template <class DATA>
CMemoryPoolTLS<DATA>::CMemoryPoolTLS(int iChunkNum, int iBlockNum, bool bPlacementNew) {
	if (iChunkNum < 0) {
		iChunkNum = 0;
	}

	if (iBlockNum < 0) {
		iBlockNum = 100;
	}

	iChunkBlockCount = iBlockNum;

	if (bPlacementNew) {
		//#define __PLACEMENT_NEW_OPTION__
		m_bPlacementOption = true;
	}
	else {
		m_bPlacementOption = false;
	}

	// TLS 인덱스 할당
	m_TLSindex = TlsAlloc();

	m_MemoryPool = new CMemoryPool<Chunk<DATA>>(iChunkNum, iBlockNum, this, bPlacementNew);
}

//파괴자
template <class DATA>
CMemoryPoolTLS<DATA>::~CMemoryPoolTLS() {
	int iSize;
	int iCnt;
	void * temp;

	delete m_MemoryPool;
	TlsFree(m_TLSindex);

	//free(m_chObjectMemory);

	//iSize = m_lSupplementBlockCount - m_lSupplementUseCount;

	//for (iCnt = 0; iCnt < iSize; iCnt++) {
	//	temp = (st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0];
	//	m_stSupplementBlockTop.Int[0] = (__int64)((st_BLOCK_NODE *)m_stSupplementBlockTop.Int[0])->stpNextBlock;
	//	free(temp);
	//}
}

//사용
template <class DATA>
DATA * CMemoryPoolTLS<DATA>::Alloc(void) {
	Chunk<DATA> *pChunk;
	DATA *pData;

	pChunk = (Chunk<DATA> *)TlsGetValue(m_TLSindex);

	// Chunk가 없으면 새로 할당받는다
	if (pChunk == 0) {
		//REALLOC :
		pChunk = m_MemoryPool->AllocTLS();
		TlsSetValue(m_TLSindex, (LPVOID)pChunk);
	}

	// 마지막 블록일 경우 새로 Chunk를 할당 받는다
	if (pChunk->Alloc(&pData)) {
		pChunk = m_MemoryPool->AllocTLS();
		TlsSetValue(m_TLSindex, (LPVOID)pChunk);
	}

	//pData = pChunk->Alloc();

	// alloc 받은 값이 널이면
	//if (pData == 0) {
	//	goto REALLOC;
	//}

	return pData;
}

//해제
template <class DATA>
bool CMemoryPoolTLS<DATA>::Free(DATA *pData) {
	st_CHUNK_NODE * pNewTop = (st_CHUNK_NODE *)pData - 1;

	if (pNewTop->chHashCode != CHUNK_HASH_CODE) {
		throw;
	}

	if (pNewTop->pChunk->m_pYobidashi != this) {
		throw;
	}

	if (InterlockedDecrement(&pNewTop->pChunk->m_lFreeCount) == 0) {
		pNewTop->pChunk->InitChunk();
		m_MemoryPool->Free(pNewTop->pChunk);
	}

	return true;
}
