#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	class CMyStrCmp
	{
	public:
		CMyStrCmp() {}
		CMyStrCmp(const TCHAR* pString)
			:m_pString(pString)
		{}
		~CMyStrCmp() {}
	public:
		template<class T>
		bool operator()(T& rObj)
		{
			return !lstrcmp(rObj.first, m_pString);

		}
	private:
		const TCHAR* m_pString;

	};



	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}

	template <typename T>
	DWORD Safe_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if (dwRefCnt == 0)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T>
	DWORD Safe_Release_Delete(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if (dwRefCnt == 0)
			{
				delete pointer;
				pointer = NULL;
			}
		}
		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destroy(T& pointer)
	{
		if (NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

	
	///////////////////////////////////Functor///////////////////////////////////////

	class CDeleteObj
	{
	public:
		explicit CDeleteObj() {}
		~CDeleteObj(void) {}

	public:
		template<typename T>
		void operator()(T& pInstance)
		{
			if (nullptr != pInstance)
			{
				delete pInstance;
				pInstance = nullptr;
			}
		}
	};

	class CDeleteMap
	{
	public:
		explicit CDeleteMap() {}
		~CDeleteMap(void) {}
	public:
		template<typename T>
		void operator()(T& Pair)
		{
			if (nullptr != Pair.second)
			{
				delete Pair.second;
				Pair.second = nullptr;
			}
		}
	};

	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag)
			: m_pTag(pTag) { }

		~CTagFinder() {}

	public:
		template<typename T>
		bool operator()(T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if (0 == iResult)
				return true;

			return false;
		}

	private:
		const TCHAR*			m_pTag;
	};
}

#endif // Engine_Function_h__
