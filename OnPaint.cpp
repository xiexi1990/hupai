#include "stdafx.h"
#include "MainWnd.h"

void MainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	m_dcmPreDraw.FillSolidRect(m_WndRect, 0xffffff);
	if(m_CurStat.m_At != -1){
		if(m_LButtonDown || m_RButtonDown){
			if(m_CurStat.m_FirstClick != -1 && m_CurStat.m_InnerAt == 0 && m_LButtonDown){
				int oldrop2 = m_dcmPreDraw.SetROP2(R2_MASKPEN);
				if(m_CurStat.m_At == m_CurStat.m_FirstClick){
					for(int i = 0; i < this->m_NumPlayers; i++){
						if(i == m_CurStat.m_At){
							DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_WholeRect, -m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.2, 0x00ff00, 0xffffff);
						}
						else{
							DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[i].m_WholeRect, -m_PlayersInfo[i].m_WholeRect.Width()*0.2, 0xff0000, 0xffffff);
						}
					}
				}
				else{
					DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_WholeRect, -m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.2, 0x00ff00, 0xffffff);
					DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_FirstClick].m_WholeRect, -m_PlayersInfo[m_CurStat.m_FirstClick].m_WholeRect.Width()*0.2, 0xff0000, 0xffffff);
				}
				m_dcmPreDraw.SetROP2(oldrop2);
			
				for(int i = 0; i < m_NumPlayers; i++){
					m_dcmPreDraw.FillSolidRect(&m_PlayersInfo[i].m_WholeRect, 0xffffff);
				}
			}

			if(m_CurStat.m_InnerAt == 0 && m_LButtonDown){
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffc000, 0xfffcf0);
			}
			if(m_CurStat.m_InnerAt == 1){  ///ming gang
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect, -m_PlayersInfo[m_CurStat.m_At].m_MingGangRect.Width()*0.4, 0xff0000, 0xffc0c0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect.Width()*0.1, 0xff0000, 0xffc0c0);
			}
			else if(m_CurStat.m_InnerAt == 2){  ///an gang
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect, -m_PlayersInfo[m_CurStat.m_At].m_AnGangRect.Width()*0.4, 0xff0000, 0xffc0c0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect.Width()*0.1, 0xff0000, 0xffc0c0);
			}
			else if(m_CurStat.m_InnerAt == 3){  ///men qing
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MenRect, -m_PlayersInfo[m_CurStat.m_At].m_MenRect.Width()*0.2, 0xff0000, 0xffc0c0);
			}
			
		}
		else{
			if(m_CurStat.m_InnerAt == 0){
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
			}
			if(m_CurStat.m_InnerAt == 1){  ///ming gang
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect, -m_PlayersInfo[m_CurStat.m_At].m_MingGangRect.Width()*0.4, 0xff8000, 0xffe0c0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect, m_PlayersInfo[m_CurStat.m_At].m_MingGangRect.Width()*0.1, 0xff8000, 0xffe0c0);
			}
			else if(m_CurStat.m_InnerAt == 2){  ///an gang
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect, -m_PlayersInfo[m_CurStat.m_At].m_AnGangRect.Width()*0.4, 0xff8000, 0xffe0c0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect, m_PlayersInfo[m_CurStat.m_At].m_AnGangRect.Width()*0.1, 0xff8000, 0xffe0c0);
			}
			else if(m_CurStat.m_InnerAt == 3){
				DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MenRect, -m_PlayersInfo[m_CurStat.m_At].m_MenRect.Width()*0.2, 0xff8000, 0xffe0c0);
			}
		}
	}


	EnterCriticalSection(&this->m_csCrit);
	this->m_dcmPreDraw.BitBlt(0, 0, m_WndRect.Width(), m_WndRect.Height(), &this->m_dcmCrit, 0, 0, SRCAND);
	LeaveCriticalSection(&this->m_csCrit);

	dc.BitBlt(0, 0, m_WndRect.Width(), m_WndRect.Height(), &m_dcmPreDraw, 0, 0, SRCCOPY);
}