#pragma once


// MainWnd

class ChupaiDlg;
typedef std::vector<CRect> RECTARR;

struct PlayerInfo
{
	int m_Srl;
	int m_Sum, m_DefFan, m_MingGangCnt, m_AnGangCnt;
	bool m_MenQing;
	int m_Hu;
	static int m_PlusFan;
	CRect m_WholeRect, m_NameRect, m_FanRect, m_MingGangRect, m_MingGangWrdRect, m_MingGangCntRect, m_AnGangRect, m_AnGangWrdRect, m_AnGangCntRect, 
		m_MenRect, m_HuRect;
	CString m_Name;
	PlayerInfo() {int PlayerInfo::*unto = (int PlayerInfo::*)&PlayerInfo::m_Name; memset(this, 0, *(int*)&unto); m_Name.Empty(); m_MenQing = 1;}
};

#define BASEHU 1
#define PENGHU 2
#define QINGHU 4
#define QINGPENGHU 8

#define BASEFAN 1
#define PENGFAN 4
#define QINGFAN 6
#define QINGPENGFAN 12

#define DEFPLUSFAN 2
#define MINGFAN 1
#define ANFAN 2

typedef std::vector<PlayerInfo> PLAYERINFOARR;

struct State
{
	int m_At, m_InnerAt, m_FirstClick;
	State() {m_At = -1; m_InnerAt = 0; m_FirstClick = -1;}
};

class MainWnd : public CWnd
{
	DECLARE_DYNAMIC(MainWnd)

public:
	MainWnd();
	MainWnd(ChupaiDlg* pp);
	virtual ~MainWnd();
	static  unsigned int  KDraw(void *pp);
	ChupaiDlg* m_PP;
	HANDLE m_hMutexLetGo;
	CRITICAL_SECTION m_csCheck, m_csCrit, m_csLoadData, m_csLoadImportantData;
	bool m_kGoing, m_NeedRedraw, m_Released, m_FirstOnShow, m_NeedCalPlayerRects, m_LButtonDown, m_RButtonDown;
	CWinThread *m_KDrawThread;
//	int a;
	CDC m_dcmCrit, m_dcmDraw1, m_dcmPreDraw;
	CBitmap m_bmpCrit, m_bmpDraw1, m_bmpPreDraw;
	CBitmap *m_defbmpCrit, *m_defbmpDraw1, *m_defbmpPreDraw;
	int MAXX, MAXY;
	CRect m_WndRect;
	int m_NumPlayers;
//	int m_CurrentAt;
	State m_CurStat;
	PLAYERINFOARR m_PlayersInfo;

	void Refresh();
	
	static void DrawFrame(CDC* pdc, const CRect& r, unsigned int color);
	static void DrawGradFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor);
	static void DrawPenFrame(CDC* pdc, const CRect& r, unsigned int color);
	static void DrawGradPenFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor);
	static void _drawGradFrame(CDC* pdc, const CRect& outerrect, int width, unsigned int outercolor, unsigned int innercolor, bool usepen);
	
protected:
	void GetPointAt(const CPoint& point, int& at, int& innerat);
	static int DivWhole(const CRect& whole, int num, RECTARR& rectlst);
	static CRect _setRect(const CRect& whole, double rl, double rt, double rr, double rb);
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


