#include "StdAfx.h"
#include "FontRepository.h"

FontRepository::FontRepository(void)
{
	memset(&m_LogFont, 0, sizeof LOGFONT);
	wcscpy_s(m_LogFont.lfFaceName, L"Arial");
	m_LogFont.lfWeight = FW_HEAVY;
}

FontRepository::~FontRepository(void)
{
	for(unsigned int i = 0; i < this->m_pFtArr.size(); i++){
		m_pFtArr[i]->DeleteObject();
		delete m_pFtArr[i];
	}
}

bool FontRepository::_QueryHeight(int height) const
{
	for(unsigned int i = 0; i < this->m_FtSizArr.size(); i++){
		if(height == m_FtSizArr[i])
			return 1;
	}
	return 0;
}

bool FontRepository::QueryHeight(int height)
{
	m_CS.Lock();
	bool rt = _QueryHeight(height);
	m_CS.Unlock();
	return rt;
}


CFont* FontRepository::_GetHeight(int height) const
{
	for(unsigned int i = 0; i < m_FtSizArr.size(); i++){
		if(height == m_FtSizArr[i])
			return m_pFtArr[i];
	}
	return 0;
}

CFont* FontRepository::GetHeight(int height)
{
	m_CS.Lock();
	CFont *rt = _GetHeight(height);
	m_CS.Unlock();
	return rt;
}

bool FontRepository::_AddHeight(int height)
{
	if(_QueryHeight(height))
		return 0;
	CFont *pfont;
	pfont = new CFont;
	m_LogFont.lfHeight = height;
	pfont->CreateFontIndirectW(&m_LogFont);
	this->m_pFtArr.push_back(pfont);
	this->m_FtSizArr.push_back(height);
	return 1;
}

bool FontRepository::AddHeight(int height)
{
	m_CS.Lock();
	bool rt = _AddHeight(height);
	m_CS.Unlock();
	return rt;
}

CFont* FontRepository::AddGetHeight(int height)
{
	m_CS.Lock();
	_AddHeight(height);
	CFont *rt = _GetHeight(height);
	m_CS.Unlock();
	return rt;
}