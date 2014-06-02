#include "stdafx.h"
#include "hupai.h"
#include "hupaiDlg.h"

unsigned int MainWnd::KDraw(void *pp)
{
#define PP ((MainWnd*)pp)
	int a = 0, entercnt = 0;
	CString tmpstr;
	bool needredraw = 0, firstinit = 1, needcalrects = 0, lbuttondown = 0;
	PLAYERINFOARR pinfos;
	int nplayers;
	State currentstate;
	CRect wndrect, goprevrect, gonextrect, newgamerect;
	LOGFONT logfont;
	CFont font, *deffont;
	memset(&logfont, 0, sizeof LOGFONT);
	wcscpy(logfont.lfFaceName, L"黑体");
	logfont.lfWeight = FW_HEAVY;
//	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;

	while(1){
	//	WaitForSingleObject(PP->m_hMutexLetGo, INFINITE);
		if(needredraw == 0)
			PP->m_KDrawThread->SuspendThread();
		EnterCriticalSection(&PP->m_csCheck);
		PP->m_kGoing = true;
		PP->m_NeedRedraw = false;
		PP->m_Released = false;
		LeaveCriticalSection(&PP->m_csCheck);
		
		EnterCriticalSection(&PP->m_csLoadImportantData);
		needcalrects = PP->m_NeedCalPlayerRects;
		PP->m_NeedCalPlayerRects = 0;
		LeaveCriticalSection(&PP->m_csLoadImportantData);

		EnterCriticalSection(&PP->m_csLoadData);
		wndrect = PP->m_WndRect;
		nplayers = PP->m_NumPlayers;
		if(!needcalrects){
			pinfos = PP->m_PlayersInfo;
		}
		currentstate = PP->m_CurStat;
		lbuttondown = PP->m_LButtonDown;
		LeaveCriticalSection(&PP->m_csLoadData);

		if(firstinit){
			firstinit = 0;
			PP->m_dcmDraw1.SetBkMode(TRANSPARENT);
		}
		
		entercnt++;
#if 0
		{
			CString s;
			s.Format(L"Enter KDraw %d !", entercnt);
			PP->m_PP->SetWindowTextW(s);
		}
#endif

		if(needcalrects){
			RECTARR raplayers, rasingle;
			CRect rect = wndrect;
			rect.left++;
			rect.top++;
			rect.right--;
			rect.bottom--;
			DivWhole(rect, nplayers, raplayers);
			EnterCriticalSection(&PP->m_csLoadData);
			for(int i = 0; i < nplayers; i++){
				PP->m_PlayersInfo[i].m_WholeRect = raplayers[i];
				DivPlayerRect(raplayers[i], rasingle);
				PP->m_PlayersInfo[i].m_NameRect = rasingle[0];
				PP->m_PlayersInfo[i].m_FanRect = rasingle[1];
				PP->m_PlayersInfo[i].m_MenRect = rasingle[2];
				PP->m_PlayersInfo[i].m_HuRect = rasingle[3];
				PP->m_PlayersInfo[i].m_MingGangRect = rasingle[4];
				PP->m_PlayersInfo[i].m_MingGangWrdRect = rasingle[5];
				PP->m_PlayersInfo[i].m_MingGangCntRect = rasingle[6];
				PP->m_PlayersInfo[i].m_AnGangRect = rasingle[7];
				PP->m_PlayersInfo[i].m_AnGangWrdRect = rasingle[8];
				PP->m_PlayersInfo[i].m_AnGangCntRect = rasingle[9];

			}
			pinfos = PP->m_PlayersInfo;
			PP->m_GoPrevRect = raplayers[4];
			PP->m_GoNextRect = raplayers[5];
			PP->m_NewGameRect = raplayers[6];
			LeaveCriticalSection(&PP->m_csLoadData);
			goprevrect = raplayers[4];
			gonextrect = raplayers[5];
			newgamerect = raplayers[6];
		}

		PP->m_dcmDraw1.FillSolidRect(&wndrect, 0xffffff);
	//	if(!(lbuttondown && currentstate.m_InnerAt == 0)){
		if(!(lbuttondown)){
			for(int i = 0; i < nplayers; i++){
				if(currentstate.m_FirstClick == i){
					DrawGradFrame(&PP->m_dcmDraw1, pinfos[i].m_WholeRect, -pinfos[i].m_WholeRect.Width()*0.2, 0xffc000, 0xffffff);
				}
			}
		}
		//	int oldrop2 = m_dcmDraw1.SetROP2(R2_MASKPEN);
		for(int i = 0; i < nplayers; i++){
			if(pinfos[i].m_MenQing){
				PP->m_dcmDraw1.FillSolidRect(&pinfos[i].m_MenRect, 0x80ff80);
			}
			else{
				PP->m_dcmDraw1.FillSolidRect(&pinfos[i].m_MenRect, 0xffe0e0);
			}
		}
	//	m_dcmDraw1.SetROP2(oldrop2);
		for(int i = 0; i < nplayers; i++){	
			DrawFrame(&PP->m_dcmDraw1, pinfos[i].m_WholeRect, 0);
		}

		DrawFrame(&PP->m_dcmDraw1, goprevrect, 0);
		DrawFrame(&PP->m_dcmDraw1, gonextrect, 0);
		DrawFrame(&PP->m_dcmDraw1, newgamerect, 0);
		logfont.lfHeight = goprevrect.Height() * 0.6;
		font.CreateFontIndirectW(&logfont);
		deffont = PP->m_dcmDraw1.SelectObject(&font);
		PP->m_dcmDraw1.DrawText(L"撤销Ctrl+Z", &goprevrect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		PP->m_dcmDraw1.DrawText(L"重复Ctrl+Y", &gonextrect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		PP->m_dcmDraw1.DrawText(L"新局 N", &newgamerect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		PP->m_dcmDraw1.SelectObject(deffont);
		font.DeleteObject();

		//// create font - draw text - delete font
		///1st player name and sum
		logfont.lfHeight = pinfos[0].m_NameRect.Height() *0.6;
		font.CreateFontIndirectW(&logfont);
		deffont = PP->m_dcmDraw1.SelectObject(&font);
		
		for(int i = 0; i < nplayers; i++){			
			PP->m_dcmDraw1.DrawText(pinfos[i].m_Name, &pinfos[i].m_NameRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
			if(pinfos[i].m_Sum == 0)
				tmpstr = L"0";
			else
				tmpstr.Format(L"%+d", pinfos[i].m_Sum);
			PP->m_dcmDraw1.DrawText(tmpstr, &pinfos[i].m_FanRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		}

		PP->m_dcmDraw1.SelectObject(deffont);
		font.DeleteObject();

		///2nd minggang and angang
		logfont.lfHeight = pinfos[0].m_MingGangCntRect.Height() *0.8;
		font.CreateFontIndirectW(&logfont);
		deffont = PP->m_dcmDraw1.SelectObject(&font);
		PP->m_dcmDraw1.SetTextColor(0xff0000);
		for(int i = 0; i < nplayers; i++){			
	//		DrawFrame(&PP->m_dcmDraw1, pinfos[i].m_MingGangRect, 0);
	//		DrawFrame(&PP->m_dcmDraw1, pinfos[i].m_MingGangWrdRect, 0);
			PP->m_dcmDraw1.DrawText(L"明", &pinfos[i].m_MingGangWrdRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
			if(pinfos[i].m_MingGangCnt){
				tmpstr.Format(L"%d", pinfos[i].m_MingGangCnt);
				PP->m_dcmDraw1.DrawText(tmpstr, &pinfos[i].m_MingGangCntRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
			}
			PP->m_dcmDraw1.DrawText(L"暗", &pinfos[i].m_AnGangWrdRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
			if(pinfos[i].m_AnGangCnt){
				tmpstr.Format(L"%d", pinfos[i].m_AnGangCnt);
				PP->m_dcmDraw1.DrawText(tmpstr, &pinfos[i].m_AnGangCntRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
			}
		}
		PP->m_dcmDraw1.SetTextColor(0);
		PP->m_dcmDraw1.SelectObject(deffont);
		font.DeleteObject();


		
		EnterCriticalSection(&PP->m_csCrit);
		PP->m_dcmCrit.BitBlt(0, 0, wndrect.Width(), wndrect.Height(), &PP->m_dcmDraw1, 0, 0, SRCCOPY);
		LeaveCriticalSection(&PP->m_csCrit);

		PP->Invalidate(0);
		EnterCriticalSection(&PP->m_csCheck);
		needredraw = PP->m_NeedRedraw;
		if(needredraw == 0)
			PP->m_kGoing = 0;
		LeaveCriticalSection(&PP->m_csCheck);
		
	//	AfxMessageBox(_T("hey!"));
	}
	return 0;
}