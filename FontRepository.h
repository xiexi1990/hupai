#pragma once

typedef std::vector<CFont*> PFONTARR;

class FontRepository
{
public:
	bool QueryHeight(int height);
	CFont* GetHeight(int height);
	bool AddHeight(int height);
	CFont* AddGetHeight(int height);

	FontRepository(void);
	~FontRepository(void);
protected:
	bool _QueryHeight(int height);
	CFont* _GetHeight(int height);
	bool _AddHeight(int height);
	
	CCriticalSection m_CS;
	PFONTARR m_pFtArr;
	INTARR m_FtSizArr;
	LOGFONT m_LogFont;
};
