#include "stdafx.h"
#include "hupai.h"
#include "hupaiDlg.h"

///////////////////////////////////////////////////
//Operation Data(Parameter) Interpreting:
//OT_DIAN:
// m_OprData[] -- DianFinalFan[]
// m_ExtraData[0] -- HuType(Bit)
// m_ExtraData[1] -- HuFinalFan
//
//OT_SETDEFHUTYPE:
// Obsolete. Using OT_CHGPLAYERINFO instead.
//
//OT_SETSUM:
//
//OT_SETNUMPLAYERS:
//
//OT_CHGPLAYERINFO:
// m_OprData[0] -- Player ID
// m_OprData[1] -- Identify which info to change: 
//                 case 1: Change MingGang
//                 case 2: Change AnGang
//                 case 3: Set MenQing
//                 case 4: Set MingGang
//                 case 5: Set AnGang
//                 case 6: Set Sum
//                 case 7: Set DefHuType(Bit)
// m_OprData[2] -- Value to change/set
// m_OprData[3] -- Previous Value
//
//OT_NEWGAME:
// m_OprData[] -- Player's Info [], compressed in Bit
//                &7 : MingGang Count
//                ( &56)>>3 : AnGang Count
//                ( &64)>>6 : If MenQing
//
///////////////////////////////////////////////////

int MainWnd::DoOperation(const Operation &op, bool refresh)
{
	switch(op.m_OprType){
		case Operation::OT_DIAN:
			{
				for(int i = 0; i < m_NumPlayers; i++){
					m_PlayersInfo[i].m_Sum += op.m_OprData[i];
				}
				if(refresh){
					this->Refresh();
				}
				m_PP->m_ListCtrl_HuRcd.InsertItem(m_PP->m_ListCtrl_HuRcd.GetItemCount(), L"");
				for(int i = 0; i < m_NumPlayers; i++){
					CString s;
					if(op.m_OprData[i] == 0)
						s = L"";
					else{
						s.Format(L"%+d", op.m_OprData[i]);
					}
					m_PP->m_ListCtrl_HuRcd.SetItemText(m_PP->m_ListCtrl_HuRcd.GetItemCount()-1, i+1, s);
				}
			}
			break;
		case Operation::OT_SETDEFHUTYPE:
			break;
		case Operation::OT_SETSUM:
			break;
		case Operation::OT_SETNUMPLAYERS:
			break;
		case Operation::OT_CHGPLAYERINFO:
			{
				switch(op.m_OprData[1])
				{
				case 1:
					m_PlayersInfo[op.m_OprData[0]].m_MingGangCnt += op.m_OprData[2];
					break;
				case 2:
					m_PlayersInfo[op.m_OprData[0]].m_AnGangCnt += op.m_OprData[2];
					break;
				case 3:
					m_PlayersInfo[op.m_OprData[0]].m_MenQing = op.m_OprData[2];
					break;
				case 4:
					m_PlayersInfo[op.m_OprData[0]].m_MingGangCnt = op.m_OprData[2];
					break;
				case 5:
					m_PlayersInfo[op.m_OprData[0]].m_AnGangCnt = op.m_OprData[2];
					break;
				default:
					break;
				}
				if(refresh){
					this->Refresh();
				}
			}
			break;
		case Operation::OT_NEWGAME:
			{
				for(int i = 0; i < m_NumPlayers; i++){
					m_PlayersInfo[i].m_AnGangCnt = 0;
					m_PlayersInfo[i].m_MingGangCnt = 0;
					m_PlayersInfo[i].m_MenQing = 1;
				}
				if(refresh){
					this->Refresh();
				}
			}
			break;
		default:
			break;
	}
	return 0;
}

int MainWnd::UndoOperation(const Operation &op, bool refresh)
{
	switch(op.m_OprType){
		case Operation::OT_DIAN:
			{
				for(int i = 0; i < m_NumPlayers; i++){
					m_PlayersInfo[i].m_Sum -= op.m_OprData[i];
				}
				if(refresh){
					this->Refresh();
				}
				m_PP->m_ListCtrl_HuRcd.DeleteItem(m_PP->m_ListCtrl_HuRcd.GetItemCount() - 1);
			}
			break;
		case Operation::OT_SETDEFHUTYPE:
			break;
		case Operation::OT_SETSUM:
			break;
		case Operation::OT_SETNUMPLAYERS:
			break;
		case Operation::OT_CHGPLAYERINFO:
			{
				switch(op.m_OprData[1])
				{
				case 1:
					m_PlayersInfo[op.m_OprData[0]].m_MingGangCnt -= op.m_OprData[2];
					break;
				case 2:
					m_PlayersInfo[op.m_OprData[0]].m_AnGangCnt -= op.m_OprData[2];
					break;
				case 3:
					m_PlayersInfo[op.m_OprData[0]].m_MenQing = op.m_OprData[3];
					break;
				case 4:
					m_PlayersInfo[op.m_OprData[0]].m_MingGangCnt = op.m_OprData[3];
					break;
				case 5:
					m_PlayersInfo[op.m_OprData[0]].m_AnGangCnt = op.m_OprData[3];
					break;
				default:
					break;
				}
				if(refresh){
					this->Refresh();
				}
			}
			break;
		case Operation::OT_NEWGAME:
			{
				for(int i = 0; i < m_NumPlayers; i++){
					m_PlayersInfo[i].m_MingGangCnt = op.m_OprData[i] & 7;
					m_PlayersInfo[i].m_AnGangCnt = (op.m_OprData[i] & 56) >> 3;
					m_PlayersInfo[i].m_MenQing = (op.m_OprData[i] & 64) >> 6;
				}
				if(refresh){
					this->Refresh();
				}
			}
			break;
		default:
			break;
	}
	return 0;
}

CString MainWnd::AnnounceOperation(const Operation &op)
{
	CString str_rt;
	switch(op.m_OprType){
		case Operation::OT_DIAN:
			{
				int plusnum = 0, zeronum = 0, plusloc = -1, minusloc = -1;
				for(int i = 0; i < m_NumPlayers; i++){
					if(op.m_OprData[i] > 0){
						plusnum++;
						plusloc = i;
					}
					else if(op.m_OprData[i] == 0)
						zeronum++;
					else
						minusloc = i;
				}
				if(plusnum == 1 && m_NumPlayers - plusnum - zeronum == 1){
					str_rt.Format(L"%s ���ڸ� %s %s %d ��(%d,%d,%d,%d)", m_PlayersInfo[minusloc].m_Name, m_PlayersInfo[plusloc].m_Name,
						BIT2STRHU(op.m_ExtraData[0]), op.m_ExtraData[1], op.m_OprData[0], op.m_OprData[1],
						op.m_OprData[2], op.m_OprData[3]);
				}
				else if(plusnum == 1 && zeronum == 0){
					str_rt.Format(L"%s ���� %s %d ��(%d,%d,%d,%d)", m_PlayersInfo[plusloc].m_Name, BIT2STRHU(op.m_ExtraData[0]),
						op.m_ExtraData[1], op.m_OprData[0], op.m_OprData[1], op.m_OprData[2], op.m_OprData[3]);
				}
				else{
					str_rt.Format(L"����(%d,%d,%d,%d)", op.m_OprData[0], op.m_OprData[1], op.m_OprData[2], op.m_OprData[3]);
				}
			}
			break;
		case Operation::OT_SETDEFHUTYPE:
			break;
		case Operation::OT_SETSUM:
			break;
		case Operation::OT_SETNUMPLAYERS:
			break;
		case Operation::OT_CHGPLAYERINFO:
			{
				str_rt = m_PlayersInfo[op.m_OprData[0]].m_Name;
				switch(op.m_OprData[1])
				{
				case 1:
					str_rt.Format(L"%s �ı������� %+d", str_rt, op.m_OprData[2]);
					break;
				case 2:
					str_rt.Format(L"%s �ı䰵���� %+d", str_rt, op.m_OprData[2]);
					break;
				case 3:
					if(op.m_OprData[2] == 0)
						str_rt += L" ȡ������";
					else
						str_rt += L" �趨����";				
					break;
				case 4:
					str_rt.Format(L"%s �趨������ %d", str_rt, op.m_OprData[2]);
					break;
				case 5:
					str_rt.Format(L"%s �趨������ %d", str_rt, op.m_OprData[2]);
					break;
				case 6:
					break;
				case 7:
					break;
				default:
					break;
				}
			}
			break;
		case Operation::OT_NEWGAME:
			{
				str_rt = L"�趨�¾�";
			}
			break;
		default:
			break;
	}
	return str_rt;
}