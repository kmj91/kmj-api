#pragma once
// 2018.09.07			기명준

#include "CObjectPool.h"

#include <Windows.h>
//#include <assert.h>
//#include <new.h>
//#include <malloc.h>
//#include <intrin.h>		//_InterlockedCompareExchange

template <class DATA>
class CObjectPoolTLS
{
private:
	// CObjectPool -> Check 덩어리 -> CObjectPoolTLS
	
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
		bool hasConstructorCalled;			// 생성자가 호출되었는지 확인하기 위한 변수
		bool hasFreed;						// Alloc 으로 사용 후 Free 반납을 할 때 중복으로 해제를 구분하기 위한 변수
		st_CHUNK_NODE *stpNextBlock;
		Chunk<DATA> *pChunk;				// 같은 타입의 다른 오브젝트 풀에서 교차 해제를 방지하기 위한 변수
	};

	template <class DATA>
	class Chunk {
	public:
		// CObjectPool에서 처음 이후 호출하는 생성자
		Chunk() {

		}

		// CObjectPool에서 처음 호출하는 생성자
		Chunk(int iBlockCount, bool bPlacementNew, void * pYobidashi) {
			m_lAllocCount = iBlockCount;
			//m_lUseCount = iBlockCount;
			m_lFreeCount = iBlockCount;
			m_pYobidashi = pYobidashi;
			//m_bPlacementOption = CObjectPoolTLS<DATA>::m_bPLACEMENT;
			m_bPlacementOption = bPlacementNew;
			CreateDataList();
		}

		~Chunk() {
			// 소멸자 호출
			ReleaseDataList();
			// 메모리 해제
			free(m_chObjectChunk);
		}

		bool Alloc(DATA ** pPopTop) {
			*pPopTop = (DATA *)m_stTop;

			// 생성자 호출 옵션
			if (m_bPlacementOption) {
				// 생성자 호출 플래그 true
				m_stTop->hasConstructorCalled = true;
				// 메모리 사용, Free 될때 까지 false
				m_stTop->hasFreed = false;
				// 다음 블록으로
				m_stTop = m_stTop->stpNextBlock;
				// 생성자 호출
				*pPopTop = (new ((st_CHUNK_NODE *)*pPopTop + 1) DATA);

				if (m_stTop == 0) {
					return true;
				}
				return false;
			}
			else {
				// 메모리 사용, Free 될때 까지 false
				m_stTop->hasFreed = false;
				// 다음 블록으로
				m_stTop = m_stTop->stpNextBlock;
				// 생성자 호출하지 않음
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

			// 생성자 호출 옵션
			if (m_bPlacementOption) {
				// 여기서 생성자를 호출하지 않습니다. Alloc을 할 때 마다 생성자 호출
				for (cnt = 0; cnt < allocCount; ++cnt) {
					stNode.chHashCode = CHUNK_HASH_CODE;
					// 생성자 호출 플래그 false
					stNode.hasConstructorCalled = false;
					// 메모리 해제 플래그
					stNode.hasFreed = true;
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
				// 생성자 호출을 이곳에서 한번만 해줍니다.
				for (cnt = 0; cnt < allocCount; ++cnt) {
					stNode.chHashCode = CHUNK_HASH_CODE;
					// 생성자 호출 플래그 true
					stNode.hasConstructorCalled = true;
					// 메모리 해제 플래그
					stNode.hasFreed = true;
					stNode.stpNextBlock = (st_CHUNK_NODE *)(m_chObjectChunk + iDataSize + iNodeSize);
					stNode.pChunk = this;
					*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
					m_chObjectChunk = m_chObjectChunk + iNodeSize;
					// 생성자 호출
					new (m_chObjectChunk) DATA;
					m_chObjectChunk = m_chObjectChunk + iDataSize;
				}
				stNode.chHashCode = CHUNK_HASH_CODE;
				stNode.stpNextBlock = NULL;
				stNode.pChunk = this;
				*(st_CHUNK_NODE *)m_chObjectChunk = stNode;
				m_chObjectChunk = m_chObjectChunk + iNodeSize;
				// 생성자 호출
				new (m_chObjectChunk) DATA;
			}

			//포인터 원위치
			m_chObjectChunk = (char *)m_stTop;
		}

		// 리스트 해제
		void ReleaseDataList() {
			int iNodeSize;
			st_CHUNK_NODE * stNode;
			char* m_chpObject;

			iNodeSize = sizeof(st_CHUNK_NODE);
			stNode = m_chObjectChunk;

			// 노드의 끝까지 반복
			while (stNode != NULL) {
				// 생성자 호출 확인
				if (stNode->hasConstructorCalled) {
					// 생성자를 호출함
					m_chpObject = (char*)stNode;
					m_chpObject = m_chpObject + iNodeSize;
					// 파괴자 호출
					(DATA*)m_chpObject->~DATA();
				}
				// 다음 블록으로
				stNode = stNode->stpNextBlock;
			}
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
	CObjectPoolTLS(int iChunkNum = 0, int iBlockNum = 100, bool bPlacementNew = false);
	virtual	~CObjectPoolTLS();

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
	int		GetAllocCount(void) { return m_ObjectPool->GetAllocCount() * iChunkBlockCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_ObjectPool->GetUseCount() * iChunkBlockCount; }

private:
	CObjectPool<Chunk<DATA>> * m_ObjectPool;
	int iChunkBlockCount;
	unsigned long m_TLSindex;
	bool m_bPlacementOption;

};















//생성자
template <class DATA>
CObjectPoolTLS<DATA>::CObjectPoolTLS(int iChunkNum, int iBlockNum, bool bPlacementNew) {
	if (iChunkNum < 0) {
		iChunkNum = 0;
	}

	if (iBlockNum < 0) {
		iBlockNum = 100;
	}

	iChunkBlockCount = iBlockNum;

	// 생성자 호출 옵션
	m_bPlacementOption = bPlacementNew;

	// TLS 인덱스 할당
	m_TLSindex = TlsAlloc();

	//-------------------------------------------------
	// 2020.04.02
	// 청크는 실제 객체의 생성자 호출이 아니기 때문에
	// 생성자 호출 옵션이 필요없습니다.
	//-------------------------------------------------
	// 청크 풀 
	m_ObjectPool = new CObjectPool<Chunk<DATA>>(iChunkNum, iBlockNum, this, false);
}

//파괴자
template <class DATA>
CObjectPoolTLS<DATA>::~CObjectPoolTLS() {
	int iSize;
	int iCnt;
	void * temp;

	delete m_ObjectPool;
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
DATA * CObjectPoolTLS<DATA>::Alloc(void) {
	Chunk<DATA> *pChunk;
	DATA *pData;

	pChunk = (Chunk<DATA> *)TlsGetValue(m_TLSindex);

	// Chunk가 없으면 새로 할당받는다
	if (pChunk == 0) {
		//REALLOC :
		pChunk = m_ObjectPool->AllocTLS();
		TlsSetValue(m_TLSindex, (LPVOID)pChunk);
	}

	// 마지막 블록일 경우 새로 Chunk를 할당 받는다
	if (pChunk->Alloc(&pData)) {
		pChunk = m_ObjectPool->AllocTLS();
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
bool CObjectPoolTLS<DATA>::Free(DATA *pData) {
	st_CHUNK_NODE * pNewTop = (st_CHUNK_NODE *)pData - 1;

	if (pNewTop->chHashCode != CHUNK_HASH_CODE) {
		throw;
	}

	// 해당 데이터가 생성된 오브젝트 풀이 아니면 잘못 해제된 상황
	if (pNewTop->pChunk->m_pYobidashi != this) {
		throw;
	}

	// 이미 해제 했는데 다시 해제되는 것을 방지
	if (pNewTop->hasFreed) {
		return false;
	}

	//-------------------------------------------------
	// 2020.04.01
	// Placement New 를 통해 생성자 호출을 했을 경우
	// 반드시 파괴자 호출도 수동으로 해주어야 합니다
	//-------------------------------------------------
	if (m_bPlacementOption)
	{
		// 파괴자 호출
		pData->~DATA();
		// 생성자 호출 플래그 초기화
		pNewTop->hasConstructorCalled = false;
	}

	//-----------------------------------------------------------------------------------
	// 2020.04.02
	// 한번 해제했던 메모리를 다시 중복으로 해제하지 못하게 막습니다.
	// 만약 막지 않으면 청크 내의 메모리 해제 카운트 m_lFreeCount 값이 잘못 계산됩니다.
	//-----------------------------------------------------------------------------------
	pNewTop->hasFreed = true;

	// 청크내의 오브젝트가 전부 반납 되면 청크 반납
	if (InterlockedDecrement(&pNewTop->pChunk->m_lFreeCount) == 0) {
		pNewTop->pChunk->InitChunk();
		m_ObjectPool->Free(pNewTop->pChunk);
	}

	return true;
}
