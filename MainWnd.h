#pragma once

#include "Recorder.h"
// MainWnd

class ChupaiDlg;

#define MAXPLAYER 4

struct PlayerInfo
{
	int m_Srl;
	int m_Sum, m_DefFan, m_MingGangCnt, m_AnGangCnt;
	bool m_MenQing;
	int m_Hu;
	static int m_PlusFan;
	CRect m_WholeRect, m_NameRect, m_FanRect, m_MingGangRect, m_MingGangWrdRect, m_MingGangCntRect, m_AnGangRect, m_AnGangWrdRect, m_AnGangCntRect, 
		m_MenRect, m_HuRect, m_HuLeftRect, m_HuRightRect;
	CString m_Name;
	PlayerInfo() {int PlayerInfo::*unto = (int PlayerInfo::*)&PlayerInfo::m_Name; memset(this, 0, *(int*)&unto); m_Name.Empty(); m_MenQing = 1;}
};

#define BASEHU 1
#define PENGHU 2
#define QINGHU 4
#define QINGPENGHU 8
#define HUAZHU 16
#define USERDEFHU 32
#define ERRBITHU 64
#define ERRSRLHU 128
#define ERRIDBHU 256

#define SRL2BITHU(srl) ((srl)==0?BASEHU:((srl)==1?PENGHU:((srl)==2?QINGHU:((srl)==3?QINGPENGHU:(srl)==4?HUAZHU:((srl)==5?USERDEFHU:ERRBITHU)))))
#define BIT2SRLHU(bit) ((bit)==BASEHU?0:((bit)==PENGHU?1:((bit)==QINGHU?2:((bit)==QINGPENGHU?3:((bit)==HUAZHU?4:((bit)==USERDEFHU?5:ERRSRLHU))))))
#define BIT2STRHU(bit) ((bit)==BASEHU?L"基本和":((bit)==PENGHU?L"碰碰和":((bit)==QINGHU?L"清一色":((bit)==QINGPENGHU?L"清碰":((bit)==HUAZHU?L"花猪":((bit)==USERDEFHU?L"自定义":L"错误类型"))))))
#define SRL2STRHU(srl) BIT2STRHU(SRL2BITHU(srl))


#define BASEFAN 1
#define PENGFAN 4
#define QINGFAN 6
#define QINGPENGFAN 12

#define PLUSFAN 2
#define MINGFAN 1
#define ANFAN 2
#define MENZIFAN 1

typedef std::vector<PlayerInfo> PLAYERINFOARR;

struct State
{
	int m_At, m_InnerAt, m_FirstClick;
	State() {m_At = -1; m_InnerAt = 0; m_FirstClick = -1;}
};


class MainWnd : public CWnd
{
	DECLARE_DYNAMIC(MainWnd)
private:  ///disable, no implementation
	MainWnd();

public:
	MainWnd(ChupaiDlg* pp);
	virtual ~MainWnd();
	static  unsigned int  KDraw(void *pp);
	ChupaiDlg* m_PP;
	HANDLE m_hMutexLetGo;
	CRITICAL_SECTION m_csCheck, m_csCrit, m_csLoadData, m_csLoadImportantData;
	bool m_kGoing, m_NeedRedraw, m_Released, m_FirstOnShow, m_NeedCalPlayerRects, m_LButtonDown, m_RButtonDown;
	bool m_FastSet;
	CWinThread *m_KDrawThread;

	CDC m_dcmCrit, m_dcmDraw1, m_dcmPreDraw, m_dcmBitmap;
	CBitmap m_bmpCrit, m_bmpDraw1, m_bmpPreDraw, m_bmpBitmap;
	CBitmap *m_defbmpCrit, *m_defbmpDraw1, *m_defbmpPreDraw, *m_defbmpBitmap;
	int MAXX, MAXY;
	CRect m_WndRect, m_GoPrevRect, m_GoNextRect, m_NewGameRect, m_ChaRect;
	int m_NumPlayers, m_TypeFan[4], m_PlusFan, m_MingFan, m_AnFan, m_MenZiFan;

	State m_CurStat;
	PLAYERINFOARR m_PlayersInfo;

	void Refresh();
	void NewGame();
	void Cha();
	void Reset();

	RecordNode m_CurRcdNode;
	int GoPrev();
	int GoNext();
	Recorder m_Rcder;
	int DoOperation(const Operation& op, bool refresh = 1);
	int UndoOperation(const Operation& op, bool refresh = 1);
	int DoRcdNode(const RecordNode& node, bool refresh = 1);
	int UndoRcdNode(const RecordNode& node, bool refresh = 1);
	CString AnnounceOperation(const Operation& op);
	CString AnnounceRcdNode(const RecordNode& node);
	static int CheckBitHu(int bithu, int bithulst[]);

	static void DrawFrame(CDC* pdc, const CRect& r, unsigned int color);
	static void DrawGradFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor);
	static void DrawPenFrame(CDC* pdc, const CRect& r, unsigned int color);
	static void DrawGradPenFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor);
	static void _drawGradFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor, bool usepen);
	
protected:
	void GetPointAt(const CPoint& point, int& at, int& innerat) const;
	static int DivWhole(const CRect& whole, int num, RECTARR& rectlst);
	static int DivPlayerRect(const CRect& playerrect, RECTARR& rectlst);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};


