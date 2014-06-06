#include "stdafx.h"
#include "MainWnd.h"

void MainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	m_dcmPreDraw.FillSolidRect(m_WndRect, 0xffffff);
	if(m_CurStat.m_At != -1){
		if(m_LButtonDown || m_RButtonDown){
			if(m_CurStat.m_At == 10 && m_LButtonDown){
				DrawGradFrame(&m_dcmPreDraw, this->m_GoPrevRect, m_GoPrevRect.Width()*0.1, 0xffc000, 0xfffcf0);
			}
			else if(m_CurStat.m_At == 11 && m_LButtonDown){
				DrawGradFrame(&m_dcmPreDraw, this->m_GoNextRect, m_GoNextRect.Width()*0.1, 0xffc000, 0xfffcf0);
			}
			else if(m_CurStat.m_At == 12 && m_LButtonDown){
				DrawGradFrame(&m_dcmPreDraw, this->m_NewGameRect, m_NewGameRect.Width()*0.1, 0xffc000, 0xfffcf0);
			}
			else if(m_CurStat.m_At == 13 && m_LButtonDown){
				DrawGradFrame(&m_dcmPreDraw, this->m_ChaRect, m_ChaRect.Width()*0.1, 0xffc000, 0xfffcf0);
			}
			else{
				if(m_CurStat.m_FirstClick != -1 && (m_CurStat.m_InnerAt == 0 || m_CurStat.m_InnerAt == 4 || m_CurStat.m_InnerAt == 5 || m_CurStat.m_InnerAt == 6) && m_LButtonDown){
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
					if(m_CurStat.m_InnerAt == 4 || m_CurStat.m_InnerAt == 5 || m_CurStat.m_InnerAt == 6){
						oldrop2 = m_dcmPreDraw.SetROP2(R2_MASKPEN);
						if(m_CurStat.m_InnerAt == 4){
							DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRect, m_PlayersInfo[m_CurStat.m_At].m_HuRect.Height()*0.1, 0xffc000, 0xfffcf0);
							DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRect, -m_PlayersInfo[m_CurStat.m_At].m_HuRect.Height()*0.4, 0xffc000, 0xfffcf0);
						}
						else if(m_CurStat.m_InnerAt == 5){
							DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect.Height()*0.1, 0xffc000, 0xfffcf0);
							DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect, -m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect.Height()*0.4, 0xffc000, 0xfffcf0);
						}
						else if(m_CurStat.m_InnerAt == 6){
							DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect.Height()*0.1, 0xffc000, 0xfffcf0);
							DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect, -m_PlayersInfo[m_CurStat.m_At].m_HuRightRect.Height()*0.4, 0xffc000, 0xfffcf0);
						}
						m_dcmPreDraw.SetROP2(oldrop2);
					}
				}

				if(m_CurStat.m_InnerAt == 0 && m_LButtonDown){
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffc000, 0xfffcf0);
				}
				if(m_CurStat.m_InnerAt == 1){  ///ming gang
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect, -m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect.Width()*0.4, 0xff0000, 0xffc0c0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect.Width()*0.1, 0xff0000, 0xffc0c0);
				}
				else if(m_CurStat.m_InnerAt == 2){  ///an gang
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect, -m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect.Width()*0.4, 0xff0000, 0xffc0c0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect.Width()*0.1, 0xff0000, 0xffc0c0);
				}
				else if(m_CurStat.m_InnerAt == 3){  ///men qing
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MenRect, -m_PlayersInfo[m_CurStat.m_At].m_MenRect.Width()*0.2, 0xff0000, 0xffc0c0);
				}
			}
			
		}
		else{
			if(m_CurStat.m_At == 10){
				DrawGradFrame(&m_dcmPreDraw, this->m_GoPrevRect, m_GoPrevRect.Width()*0.1, 0xffff00, 0xfffff0);
			}
			else if(m_CurStat.m_At == 11){
				DrawGradFrame(&m_dcmPreDraw, this->m_GoNextRect, m_GoNextRect.Width()*0.1, 0xffff00, 0xfffff0);
			}
			else if(m_CurStat.m_At == 12){
				DrawGradFrame(&m_dcmPreDraw, this->m_NewGameRect, m_NewGameRect.Width()*0.1, 0xffff00, 0xfffff0);
			}
			else if(m_CurStat.m_At == 13){
				DrawGradFrame(&m_dcmPreDraw, this->m_ChaRect, m_ChaRect.Width()*0.1, 0xffff00, 0xfffff0);
			}
			else{
				if(m_CurStat.m_FirstClick != -1 && (m_CurStat.m_InnerAt == 4 || m_CurStat.m_InnerAt == 5 || m_CurStat.m_InnerAt == 6)){
					int oldrop2 = m_dcmPreDraw.SetROP2(R2_MASKPEN);
					if(m_CurStat.m_InnerAt == 4){
						DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRect, m_PlayersInfo[m_CurStat.m_At].m_HuRect.Height()*0.1, 0xffff00, 0xfffff0);
						DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRect, -m_PlayersInfo[m_CurStat.m_At].m_HuRect.Height()*0.4, 0xffff00, 0xfffff0);
					}
					else if(m_CurStat.m_InnerAt == 5){
						DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect.Height()*0.1, 0xffff00, 0xfffff0);
						DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect, -m_PlayersInfo[m_CurStat.m_At].m_HuLeftRect.Height()*0.4, 0xffff00, 0xfffff0);
					}
					else if(m_CurStat.m_InnerAt == 6){
						DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect.Height()*0.1, 0xffff00, 0xfffff0);
						DrawGradPenFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_HuRightRect, -m_PlayersInfo[m_CurStat.m_At].m_HuRightRect.Height()*0.4, 0xffff00, 0xfffff0);
					}
					m_dcmPreDraw.SetROP2(oldrop2);
				}
				if(m_CurStat.m_InnerAt == 0){
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
				}
				if(m_CurStat.m_InnerAt == 1){  ///ming gang
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect, -m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect.Width()*0.4, 0xff8000, 0xffe0c0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect, m_PlayersInfo[m_CurStat.m_At].m_MingGangWrdRect.Width()*0.1, 0xff8000, 0xffe0c0);
				}
				else if(m_CurStat.m_InnerAt == 2){  ///an gang
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect, -m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect.Width()*0.4, 0xff8000, 0xffe0c0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect, m_PlayersInfo[m_CurStat.m_At].m_AnGangWrdRect.Width()*0.1, 0xff8000, 0xffe0c0);
				}
				else if(m_CurStat.m_InnerAt == 3){  ///men qing
					DrawGradFrame(&m_dcmPreDraw, this->m_PlayersInfo[m_CurStat.m_At].m_WholeRect, m_PlayersInfo[m_CurStat.m_At].m_WholeRect.Width()*0.1, 0xffff00, 0xfffff0);
					DrawGradFrame(&m_dcmPreDraw, m_PlayersInfo[m_CurStat.m_At].m_MenRect, -m_PlayersInfo[m_CurStat.m_At].m_MenRect.Width()*0.2, 0xff8000, 0xffe0c0);
				}
			}
		}
	}


	EnterCriticalSection(&this->m_csCrit);
	this->m_dcmPreDraw.BitBlt(0, 0, m_WndRect.Width(), m_WndRect.Height(), &this->m_dcmCrit, 0, 0, SRCAND);
	LeaveCriticalSection(&this->m_csCrit);

	dc.BitBlt(0, 0, m_WndRect.Width(), m_WndRect.Height(), &m_dcmPreDraw, 0, 0, SRCCOPY);
}