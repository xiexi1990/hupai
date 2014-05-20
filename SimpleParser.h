#pragma once

class SimpleParser
{
public:
	SimpleParser(void);
	~SimpleParser(void);
	int GetNum(const CString& str, int& n);
	int SetNum(int nset, CString& str, bool showplussign);
protected:
	int _GetNumLoc(const CString& str, int& begin, int& len);
};
