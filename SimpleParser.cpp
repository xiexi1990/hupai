#include "StdAfx.h"
#include "SimpleParser.h"

SimpleParser::SimpleParser(void)
{
}

SimpleParser::~SimpleParser(void)
{
}

int SimpleParser::_GetNumLoc(const CString& str, int& _begin, int& len)
{
	if(str.GetLength() == 0)
		return 3; ///3: string contains nothing
	int firstleft = str.GetLength() + 1, lastright = -1;
	bool nfbefore = 0, nfafter = 0;
	int begin, end;
#define ISFREECHAR(ch) ((ch) == L' ' || (ch) == L'\t' || (ch) == L'\n' || (ch) == L'\r')
#define ISUINT(ch) ((ch) <= L'9' && (ch) >= L'0')
#define ISSIGN(ch) ((ch) == L'+' || (ch) == L'-')
	for(int i = 0; i < str.GetLength(); i++){
		if(str[i] == L'(' || str[i] == L'£¨'){
			firstleft = i;
			break;
		}
		else if(!ISFREECHAR(str[i]))
			nfbefore = 1;
	}
	for(int i = str.GetLength()-1; i >= 0; i--){
		if(str[i] == L')' || str[i] == L'£©'){
			lastright = i;
			break;
		}
		else if(!ISFREECHAR(str[i]))
			nfafter = 1;
	}

	int chkbegin, chkend;
	if(firstleft == str.GetLength() + 1 && lastright == -1){
		chkbegin = 0;
		chkend = str.GetLength();
	}
	else{
		if(nfbefore == 1 && nfafter == 1)
			return 2; ///2: more than one part or recognization failure
		if(firstleft > lastright){
			return 1; ///1: parenthese mismatch
		}

		if(nfbefore == 0 && nfafter == 0)
			return 3; ///3: string contains nothing

		if(nfbefore == 1){
			chkbegin = 0;
			chkend = firstleft;
		}
		else{
			chkbegin = lastright + 1;
			chkend = str.GetLength();
		}
	}
	for(int i = chkbegin; i < chkend; i++){
		if(!ISFREECHAR(str[i])){
			begin = i;
			break;
		}
	}

	for(int i = chkend-1; i >= chkbegin; i--){
		if(!ISFREECHAR(str[i])){
			end = i;
			break;
		}
	}

	if(begin > end)
		return 99; ///99: should be impossible
	if(begin == end){
		if(ISUINT(str[begin])){
			_begin = begin;
			len = 1;
			return 0; ///0: success
		}
		else
			return 2; ///2: more than one part or recognization failure
	}
	else{
		if(!(ISSIGN(str[begin]) || ISUINT(str[begin]))){
			return 2; ///2: more than one part or recognization failure
		}
		for(int i = begin + 1; i <= end; i++){
			if(!(ISUINT(str[i]))){
				return 2; ///2: more than one part or recognization failure
			}
		}
		_begin = begin;
		len = end - begin + 1;
		return 0; ///0: success
	}

}

int SimpleParser::GetNum(const CString& str, int& n)
{
	int begin, len;
	int vld = _GetNumLoc(str, begin, len);
	if(vld != 0)
		return vld;
	swscanf_s(str.Mid(begin, len), L"%d", &n);
	return 0;
}

int SimpleParser::SetNum(int nset, CString& str, bool showplussign)
{
	int begin, len;
	int vld = _GetNumLoc(str, begin, len);
	if(vld != 0)
		return vld;
	CString s;
	if(showplussign){
		s.Format(L"%+d", nset);
	}
	else{
		s.Format(L"%d", nset);
	}
	str = str.Left(begin) + s + str.Mid(begin + len);
	return 0;
}