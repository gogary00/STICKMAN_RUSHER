/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <string>

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	logo1.LoadBitmap(IDB_BITMAP54);
	logo2.LoadBitmap(IDB_BITMAP42, RGB(0, 255, 0));
	green_effect.LoadBitmap(IDB_BITMAP68, RGB(0, 255, 0));
	btnAudio_open.LoadBitmap(IDB_BITMAP55, RGB(0, 255, 0));
	btnAudio_close.LoadBitmap(IDB_BITMAP56, RGB(0, 255, 0));
	btnStartGame.LoadBitmap(IDB_BITMAP43, RGB(0, 255, 0));
	character_body.LoadBitmap(IDB_BITMAP75, RGB(0, 255, 0));
	character_eye.LoadBitmap(IDB_BITMAP74, RGB(0, 255, 0));
	character_sword.LoadBitmap(IDB_BITMAP71, RGB(0, 255, 0));
	character_wing_left.LoadBitmap(IDB_BITMAP69, RGB(0, 255, 0));
	character_scarf1.LoadBitmap(IDB_BITMAP72, RGB(0, 255, 0));
	character_scarf2.LoadBitmap(IDB_BITMAP73, RGB(0, 255, 0));
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE)
		GotoGameState(GAME_STATE_PREPARE);						// 切換至GAME_STATE_PREPARE
	else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if (point.x > SIZE_X - btnAudio_open.Width() && point.y < btnAudio_open.Height()) {
		swAudio++;
	}
	if (point.x < 207 && point.y>300 && point.y < 300 + btnStartGame.Height()) {
		GotoGameState(GAME_STATE_PREPARE);		// 切換至GAME_STATE_PREPARE
	}
}

void CGameStateInit::OnShow()
{
	//
	// 貼上logo
	//
	logo1.SetTopLeft(0,0);
	logo2.SetTopLeft(0, 0);
	green_effect.SetTopLeft(150,200);
	btnAudio_open.SetTopLeft(SIZE_X-btnAudio_open.Width(), 0);
	btnAudio_close.SetTopLeft(SIZE_X - btnAudio_close.Width(), 0);
	btnStartGame.SetTopLeft(0, 300);
	character_wing_left.SetTopLeft(300, 300);
	character_scarf1.SetTopLeft(580, 160);
	character_scarf2.SetTopLeft(580, 210);
	character_body.SetTopLeft(520, 60);
	character_eye.SetTopLeft(520, 50);
	character_sword.SetTopLeft(420, 220);

	logo1.ShowBitmap();
	green_effect.ShowBitmap();
	//character_wing_left.ShowBitmap();
	logo2.ShowBitmap();
	btnStartGame.ShowBitmap();
	character_scarf1.ShowBitmap();
	character_scarf2.ShowBitmap();
	character_sword.ShowBitmap();
	character_body.ShowBitmap();
	character_eye.ShowBitmap();
	if (swAudio%2 == 0) {
		btnAudio_open.ShowBitmap();
	}
	else {
		btnAudio_close.ShowBitmap();
	}
	//
	// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
	//

	/*CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	if (ENABLE_GAME_PAUSE)
		pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC*/
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Prepare
CGameStatePrepare::CGameStatePrepare(CGame *g) : CGameState(g) {

}

void CGameStatePrepare::OnInit() {
	background.LoadBitmap(IDB_BITMAP88);
	btnStartGame.LoadBitmap(IDB_BITMAP43, RGB(0, 255, 0));
	btnAudio_open.LoadBitmap(IDB_BITMAP55, RGB(0, 255, 0));
	btnAudio_close.LoadBitmap(IDB_BITMAP56, RGB(0, 255, 0));
}

void CGameStatePrepare::OnBeginState() {
	
}

void CGameStatePrepare::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStatePrepare::OnLButtonDown(UINT nFlags, CPoint point) {
	if (point.x < btnAudio_open.Width() && point.y < btnAudio_open.Height()) {
		swAudio++;
	}
	if (point.x > SIZE_X-btnStartGame.Width() && point.y>241 && point.y < 241 + btnStartGame.Height()) {
		GotoGameState(GAME_STATE_SELECT);		// 切換至GAME_STATE_SELECT
	}
}

void CGameStatePrepare::OnShow() {
	background.SetTopLeft(0, 0);
	btnStartGame.SetTopLeft(SIZE_X - btnStartGame.Width(), 241);
	btnAudio_open.SetTopLeft(10, 10);
	btnAudio_close.SetTopLeft(10, 10);

	background.ShowBitmap();
	btnStartGame.ShowBitmap();
	if (swAudio % 2 == 0) {
		btnAudio_open.ShowBitmap();
	}
	else {
		btnAudio_close.ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////

CGameStateSelect::CGameStateSelect(CGame *g) : CGameState(g) {
	
}

void CGameStateSelect::OnInit() {
	swAudio = 0;
	select = 0;
	background.LoadBitmap(IDB_BITMAP84);
	btnStartGame.LoadBitmap(IDB_BITMAP43, RGB(0, 255, 0));
	btnAudio_open.LoadBitmap(IDB_BITMAP55, RGB(0, 255, 0));
	btnAudio_close.LoadBitmap(IDB_BITMAP56, RGB(0, 255, 0));
	upgrade[0].LoadBitmap(IDB_BITMAP90, RGB(0, 255, 0));
	upgrade[1].LoadBitmap(IDB_BITMAP156, RGB(255, 255, 255));
	upgrade[2].LoadBitmap(IDB_BITMAP92, RGB(0, 255, 0));
	upgrade[3].LoadBitmap(IDB_BITMAP93, RGB(0, 255, 0));
	upgrade[4].LoadBitmap(IDB_BITMAP94, RGB(0, 255, 0));
	upgrade[5].LoadBitmap(IDB_BITMAP95, RGB(0, 255, 0));
	frame.LoadBitmap(IDB_BITMAP158);
	player[0].AddBitmap(IDB_BITMAP122, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP125, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP127, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP128, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP131, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP132, RGB(0, 255, 0));
	for (int i = 0; i < 4; i++) {
		player[1].AddBitmap(293+i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 6; i++) {
		player[2].AddBitmap(297 + i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 5; i++) {
		player[3].AddBitmap(268 + i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 4; i++) {
		player[4].AddBitmap(289 + i, RGB(0, 255, 0));
	}
	player[5].AddBitmap(IDB_BITMAP119, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP120, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP121, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP123, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP124, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP126, RGB(0, 255, 0));
	MyWrite('0');
}

void CGameStateSelect::OnBeginState() {
	
}

void CGameStateSelect::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStateSelect::MyWrite(char c) {
	ofstream ofs;
	ofs.open("./set.txt");
	if (!ofs.is_open()) {
		GotoGameState(GAME_STATE_OVER);
	}
	ofs << c;
	ofs.close();
}

void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point) {
	if (point.x > SIZE_X-btnAudio_open.Width() && point.y < btnAudio_open.Height()) {
		swAudio++;
	}
	if (point.x > SIZE_X - btnStartGame.Width() && point.y > 400 && point.y < 400 + btnStartGame.Height()) {
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	if (point.x > 113 && point.x < 113 + upgrade[0].Width() && point.y>150 && point.y < 150 + upgrade[0].Height()) {
		select = 0;
		MyWrite('0');
	}
	if (point.x > 113 && point.x < 113 + upgrade[1].Width() && point.y>227 && point.y < 227 + upgrade[1].Height()) {
		select = 1;
		MyWrite('1');
	}
	if (point.x > 113 && point.x < 113 + upgrade[2].Width() && point.y>305 && point.y < 305 + upgrade[2].Height()) {
		select = 2;
		MyWrite('2');
	}
	if (point.x > 520 && point.x < 520 + upgrade[3].Width() && point.y>150 && point.y < 150 + upgrade[3].Height()) {
		select = 3;
		MyWrite('3');
	}
	if (point.x > 520 && point.x < 520 + upgrade[4].Width() && point.y>227 && point.y < 227 + upgrade[4].Height()) {
		select = 4;
		MyWrite('4');
	}
	if (point.x > 520 && point.x < 520 + upgrade[5].Width() && point.y>305 && point.y < 305 + upgrade[5].Height()) {
		select = 5;
		MyWrite('5');
	}
}

void CGameStateSelect::OnShow() {
	background.SetTopLeft(0, 0);
	btnStartGame.SetTopLeft(SIZE_X - btnStartGame.Width(), 400);
	btnAudio_open.SetTopLeft(SIZE_X-btnAudio_open.Width(), 10);
	btnAudio_close.SetTopLeft(SIZE_X - btnAudio_open.Width(), 10);
	upgrade[0].SetTopLeft(113, 150);
	upgrade[1].SetTopLeft(113, 227);
	upgrade[2].SetTopLeft(113, 305);
	upgrade[3].SetTopLeft(520, 150);
	upgrade[4].SetTopLeft(520, 227);
	upgrade[5].SetTopLeft(520, 305);
	background.ShowBitmap();
	player[select].SetTopLeft(330, 260);
	frame.SetTopLeft(upgrade[select].Left(), upgrade[select].Top());

	btnStartGame.ShowBitmap();
	if (swAudio % 2 == 0) {
		btnAudio_open.ShowBitmap();
	}
	else {
		btnAudio_close.ShowBitmap();
	}
	frame.ShowBitmap();
	for (int i = 0; i < 6; i++) {
		upgrade[i].ShowBitmap();
	}
	player[select].SetDelayCount(1);
	player[select].OnMove();
	player[select].OnShow();
}

/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g) : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
	
}

void CGameStateRun::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	ifs.open("./set.txt");
	if (!ifs.is_open()) {
		GotoGameState(GAME_STATE_OVER);
	}
	ifs >> s;
	ifs.close();
	translating = player[s].Width() + distance;
	//
	// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// ---------------------------------------------------移動POINT----------------------------------------------------------------
	for (int i = 0; i < total_star; i++) {
		cstar[i].SetTopLeft(cstar[i].Left() - map_speed, cstar[i].Top());
	}
	// ---------------------------------------------------移動POINT----------------------------------------------------------------
	//
	//-----------------------------------------------------偵測底部----------------------------------------------------------------
	if (abs(ground.Left()) + translating > 0 && abs(ground.Left()) + translating < 1281) { bottom = 415; }
	if (abs(ground.Left()) + translating > 1281 && abs(ground.Left()) + translating < 2157) { bottom = int(-0.23*(abs(ground.Left())+translating-1275)+430)-15; }
	if (abs(ground.Left()) + translating > 2157 && abs(ground.Left()) + translating < 2369) { bottom = 482; }
	if (abs(ground.Left()) + translating > 2369 && abs(ground.Left()) + translating < 2555) { bottom = 415; }
	if (abs(ground.Left()) + translating > 2555 && abs(ground.Left()) + translating < 2762) { bottom = 482; }
	if (abs(ground.Left()) + translating > 2762 && abs(ground.Left()) + translating < 3028) { bottom = 415; }
	if (abs(ground.Left()) + translating > 3028 && abs(ground.Left()) + translating < 3108) { bottom = 360; }
	if (abs(ground.Left()) + translating > 3108 && abs(ground.Left()) + translating < 3748) { bottom = 415; }
	if (abs(ground.Left()) + translating > 3748 && abs(ground.Left()) + translating < 3885) { bottom = 350; }
	if (abs(ground.Left()) + translating > 3885 && abs(ground.Left()) + translating < 4635) { bottom = 415; }
	if (abs(ground.Left()) + translating > 4635 && abs(ground.Left()) + translating < 4777) { bottom = 482; }
	if (abs(ground.Left()) + translating > 4777 && abs(ground.Left()) + translating < 5037) { bottom = 415; }
	if (abs(ground.Left()) + translating > 5037 && abs(ground.Left()) + translating < 5885) { bottom = int(-0.17*(abs(ground.Left()) + translating - 5037) + 420)-15; }
	if (abs(ground.Left()) + translating > 5885 && abs(ground.Left()) + translating < 6057) { bottom = 482; }
	if (abs(ground.Left()) + translating > 6057 && abs(ground.Left()) + translating < 6205) { bottom = 285; }
	if (abs(ground.Left()) + translating > 6205 && abs(ground.Left()) + translating < 6340) { bottom = 220; }
	if (abs(ground.Left()) + translating > 6340 && abs(ground.Left()) + translating < 6491) { bottom = 285; }
	if (abs(ground.Left()) + translating > 6491 && abs(ground.Left()) + translating < 6650) { bottom = 482; }
	if (abs(ground.Left()) + translating > 6650 && abs(ground.Left()) + translating < 6805) { bottom = 285; }
	if (abs(ground.Left()) + translating > 6805 && abs(ground.Left()) + translating < 6925) { bottom = 482; }
	if (abs(ground.Left()) + translating > 6925 && abs(ground.Left()) + translating < 7080) { bottom = 285; }
	if (abs(ground.Left()) + translating > 7080 && abs(ground.Left()) + translating < 7195) { bottom = 482; }
	if (abs(ground.Left()) + translating > 7195 && abs(ground.Left()) + translating < 7350) { bottom = 285; }
	if (abs(ground.Left()) + translating > 7350 && abs(ground.Left()) + translating < 7467) { bottom = 482; }
	if (abs(ground.Left()) + translating > 7467 && abs(ground.Left()) + translating < 7767) { bottom = 415; }
	if (abs(ground.Left()) + translating > 7767 && abs(ground.Left()) + translating < 8450) { bottom = int(-0.17*(abs(ground.Left()) + translating - 7767) + 400) - 15; }
	if (abs(ground.Left()) + translating > 8450 && abs(ground.Left()) + translating < 8755) { bottom = int(0.31*(abs(ground.Left()) + translating - 8450) + 280) - 15; }
	if (abs(ground.Left()) + translating > 8755 && abs(ground.Left()) + translating < 9380) { bottom = 376; }
	if (abs(ground.Left()) + translating > 9380 && abs(ground.Left()) + translating < 10250) { bottom = int(-0.07*(abs(ground.Left()) + translating - 9380) + 380) - 15; }
	if (abs(ground.Left()) + translating > 10250 && abs(ground.Left()) + translating < 13176) { bottom = 315; }
	if (abs(ground.Left()) + translating > 13176 && abs(ground.Left()) + translating < 14286) { bottom = 380; }
	if (abs(ground.Left()) + translating > 14286 && abs(ground.Left()) + translating < 15836) { bottom = 482; }
	if (abs(ground.Left()) + translating > 15836 && abs(ground.Left()) + translating < 99999) { bottom = 390; }
	bottom += 15;
	//-----------------------------------------------------偵測底部----------------------------------------------------------------

	//-----------------------------------------------------偵測障礙物----------------------------------------------------------------
	background.SetTopLeft(background.Left()-map_speed, 0);
	ground.SetTopLeft(ground.Left() - map_speed, 0);
	background2.SetTopLeft(background2.Left() - map_speed, 0);
	ground2.SetTopLeft(ground2.Left() - map_speed, 0);
	score_board.SetTopLeft(240, 0);
	if (UP_STATE == true) {
		IS_FUNC = false;
	}
	if (JUMP_STATE == false) {
		max_hight = bottom - player[s].Height() - 125;
	}
	if (JUMP_STATE == true) {
		if (player[s].Top() > max_hight && UP_STATE==true) {
			player[s].SetTopLeft(distance, player[s].Top() - dump_speed);
		}
		else if(player[s].Top()+player[s].Height()< bottom){
			UP_STATE = false;
			player[s].SetTopLeft(distance, player[s].Top() + dump_speed);
		}
		else {
			player[s].SetTopLeft(distance, bottom - player[s].Height());
			JUMP_STATE = false;
			CONTINUE_JUMP = true;
			max_hight = bottom - player[s].Height() - 125;
			IS_FUNC = true;
		}
	}
	if (abs(ground.Left())>(632-translating) && abs(ground.Left())<(741-translating) && player[s].Top() + player[s].Height() > 368) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (1281 - translating) && abs(ground.Left()) < (1850 - translating)) {
		player[s].SetTopLeft(distance, player[s].Top()-3);
	}
	if (abs(ground.Left()) + translating > 1281 && abs(ground.Left()) + translating < 2157) {
		IS_FUNC = false;
	}
	if (abs(ground.Left()) + translating > 2157 && abs(ground.Left()) + translating < 2170) {
		IS_FUNC = true;
	}
	if (abs(ground.Left()) > (2157 - translating) && abs(ground.Left()) < (2368 - translating) && player[s].Top() + player[s].Height() > 450) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (2554 - translating) && abs(ground.Left()) < (2763 - translating) && player[s].Top() + player[s].Height() > 450) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 3028 && abs(ground.Left()) + translating < 3106 && player[s].Top() + player[s].Height() > 383 && player[s].Left() > 0) {
		player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
	}
	if (abs(ground.Left()) + translating > 3108 && abs(ground.Left()) + translating < 3148 && player[s].Left() < distance) {
		player[s].SetTopLeft(player[s].Left() + map_speed*2, player[s].Top());
	}
	if (abs(ground.Left()) > (3265 - translating) && abs(ground.Left()) < (3375 - translating) && player[s].Top() + player[s].Height() > 368) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 3749 && abs(ground.Left()) + translating < 3883 && player[s].Top() + player[s].Height() > 380 && player[s].Left() > 0) {
		player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
	}
	if (abs(ground.Left()) + translating > 3553 && abs(ground.Left()) + translating < 3638 && player[s].Left() < distance) {
		player[s].SetTopLeft(player[s].Left() + map_speed * 2, player[s].Top());
	}
	if (abs(ground.Left()) > (4573 - translating) && abs(ground.Left()) < (4633 - translating) && player[s].Top() + player[s].Height() > 390) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (4633 - translating) && abs(ground.Left()) < (4775 - translating) && player[s].Top() + player[s].Height() > 450) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (5037 - translating) && abs(ground.Left()) < (5460 - translating)) {
		player[s].SetTopLeft(distance, player[s].Top() - 3);
	}
	if (abs(ground.Left()) + translating > 5037 && abs(ground.Left()) + translating < 5885) {
		IS_FUNC = false;
	}
	if (abs(ground.Left()) + translating > 5885 && abs(ground.Left()) + translating < 5895) {
		IS_FUNC = true;
	}
	if (abs(ground.Left()) + translating > 5885 && abs(ground.Left()) + translating < 6057 && player[s].Top() + player[s].Height() > 415) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 6205 && abs(ground.Left()) + translating < 6340 && player[s].Top() + player[s].Height() > 240 && player[s].Left() > 0) {
		player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
	}
	if (abs(ground.Left()) + translating > 6340 && abs(ground.Left()) + translating < 6420 && player[s].Left() < distance) {
		player[s].SetTopLeft(player[s].Left() + map_speed * 2, player[s].Top());
	}
	if (abs(ground.Left()) + translating > 6491 && abs(ground.Left()) + translating < 6650 && player[s].Top() + player[s].Height() > 430) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 6805 && abs(ground.Left()) + translating < 6925 && player[s].Top() + player[s].Height() > 430) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 7080 && abs(ground.Left()) + translating < 7195 && player[s].Top() + player[s].Height() > 430) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 7350 && abs(ground.Left()) + translating < 7467 && player[s].Top() + player[s].Height() > 430) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (7767 - translating) && abs(ground.Left()) < (8450 - translating)) {
		player[s].SetTopLeft(distance, player[s].Top() - 2);
	}
	if (abs(ground.Left()) + translating > 7767 && abs(ground.Left()) + translating < 8754) {
		IS_FUNC = false;
	}
	if (abs(ground.Left()) + translating > 8754 && abs(ground.Left()) + translating < 8764) {
		IS_FUNC = true;
	}
	if (abs(ground.Left()) > (8450 - translating) && abs(ground.Left()) < (8755 - translating)) {
		player[s].SetTopLeft(distance, player[s].Top() + 3);
	}
	if (abs(ground.Left()) + translating > 7991 && abs(ground.Left()) + translating < 8081 && player[s].Top() + player[s].Height() > 314) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 8590 && abs(ground.Left()) + translating < 8677 && player[s].Top() + player[s].Height() > 298) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 9380 && abs(ground.Left()) + translating < 10250) {
		player[s].SetTopLeft(distance, player[s].Top() - 1);
	}
	if (abs(ground.Left()) + translating > 9380 && abs(ground.Left()) + translating < 10250) {
		IS_FUNC = false;
	}
	if (abs(ground.Left()) + translating > 10250 && abs(ground.Left()) + translating < 10261) {
		IS_FUNC = true;
	}
	if (abs(ground.Left()) + translating > 14086 && abs(ground.Left()) + translating < 15576 && player[s].Top() + player[s].Height() > 425) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) + translating > 14279 && abs(ground.Left()) + translating < 14301 && player[s].Top()<120) {
		JUMP_STATE = false;
		IS_FUNC = false;
		DRAG_STATE = true;
	}
	if (DRAG_STATE == true) {
		if (abs(ground.Left()) + translating > 13936 && abs(ground.Left()) + translating < 15831) {
			int temp = int(0.094*(abs(ground.Left()) + translating - 13936) + 30);
			player[s].SetTopLeft(distance, temp);
		}
	}
	if (abs(ground.Left()) + translating > 15829 && abs(ground.Left()) + translating < 15841) {
		IS_FUNC = true;
		CONTINUE_JUMP = true;
		DRAG_STATE = false;
		UP_STATE = false;
	}

	attack.SetTopLeft(player[s].Left()+player[s].Width()-100, player[s].Top()-10);
	player[s].OnMove();

	// ====== drop down =======
	if (IS_FUNC == true) {
		if ((player[s].Top() + player[s].Height()) < bottom) {
			player[s].SetTopLeft(distance, player[s].Top() + 10);
		}
	}
	// ====== drop down =======
	//===========偵測point碰撞===========
	for (int i = 0; i < total_star; i++) {
		if (player[s].Left()+player[s].Width()-30 >= cstar[i].Left() && player[s].Left() - 30 <= cstar[i].Left()+ cstar[i].Width() && player[s].Top()+ player[s].Height() - 30 >= cstar[i].Top() && player[s].Top() - 30 <= cstar[i].Top()+ cstar[i].Height()) {
			if (cstar[i].get_IS_Show() == true) { count_point++; }
			cstar[i].set_IS_Show(false);
		}
	}
	//===========偵測point碰撞===========
	//-----------------------------------------------------偵測障礙物----------------------------------------------------------------
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//
	s = 0;
	total_star = 43;
	count_point = 0;
	IS_FUNC = true;
	distance = 50;
	AUTO_JUMP = true;
	DRAG_STATE = false;
	max_hight = 200;
	bottom = 0;
	map_speed = 10;
	dump_speed = 15;
	CONTINUE_JUMP = true;
	JUMP_STATE = false;
	UP_STATE = false;
	ATTACH_STATE = false;
	score_board.LoadBitmap(IDB_BITMAP63, RGB(0, 255, 0));
	point_board.LoadBitmap(IDB_BITMAP53, RGB(0, 255, 0));
	point_board.SetTopLeft(0, 50);
	for (int i = 0; i < total_star; i++) {
		cstar[i].LoadBitmap();
	}
	for (int i = 0; i < total_star; i++) {
		cstar[i].set_IS_Show(true);
	}
	player[0].AddBitmap(IDB_BITMAP122, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP125, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP127, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP128, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP131, RGB(0, 255, 0));
	player[0].AddBitmap(IDB_BITMAP132, RGB(0, 255, 0));
	for (int i = 0; i < 4; i++) {
		player[1].AddBitmap(293 + i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 6; i++) {
		player[2].AddBitmap(297 + i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 5; i++) {
		player[3].AddBitmap(268 + i, RGB(0, 255, 0));
	}
	for (int i = 0; i < 4; i++) {
		player[4].AddBitmap(289 + i, RGB(0, 255, 0));
	}
	player[5].AddBitmap(IDB_BITMAP119, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP120, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP121, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP123, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP124, RGB(0, 255, 0));
	player[5].AddBitmap(IDB_BITMAP126, RGB(0, 255, 0));
	for (int i = 0; i < 6; i++) {
		player[i].SetDelayCount(3);
	}
	background.LoadBitmap(IDB_BITMAP164);
	background2.LoadBitmap(IDB_BITMAP165);
	ground.LoadBitmap(IDB_BITMAP163, RGB(255, 255, 255));
	ground2.LoadBitmap(IDB_BITMAP166, RGB(255, 255, 255));
	attack.LoadBitmap(IDB_BITMAP160, RGB(255, 255, 255));
	//set map_score[][] loading picture
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			switch (j) {
				case 0:
					map_score[i][j].LoadBitmap(IDB_0, RGB(0, 0, 0));
					break;
				case 1:
					map_score[i][j].LoadBitmap(IDB_1, RGB(0, 0, 0));
					break;
				case 2:
					map_score[i][j].LoadBitmap(IDB_2, RGB(0, 0, 0));
					break;
				case 3:
					map_score[i][j].LoadBitmap(IDB_3, RGB(0, 0, 0));
					break;
				case 4:
					map_score[i][j].LoadBitmap(IDB_4, RGB(0, 0, 0));
					break;
				case 5:
					map_score[i][j].LoadBitmap(IDB_5, RGB(0, 0, 0));
					break;
				case 6:
					map_score[i][j].LoadBitmap(IDB_6, RGB(0, 0, 0));
					break;
				case 7:
					map_score[i][j].LoadBitmap(IDB_7, RGB(0, 0, 0));
					break;
				case 8:
					map_score[i][j].LoadBitmap(IDB_8, RGB(0, 0, 0));
					break;
				case 9:
					map_score[i][j].LoadBitmap(IDB_9, RGB(0, 0, 0));
					break;
				default:
					break;
			}
		}
	}
	//set score_point[][] loading picture
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 10; j++) {
			switch (j) {
			case 0:
				score_point[i][j].LoadBitmap(IDB_0, RGB(0, 0, 0));
				break;
			case 1:
				score_point[i][j].LoadBitmap(IDB_1, RGB(0, 0, 0));
				break;
			case 2:
				score_point[i][j].LoadBitmap(IDB_2, RGB(0, 0, 0));
				break;
			case 3:
				score_point[i][j].LoadBitmap(IDB_3, RGB(0, 0, 0));
				break;
			case 4:
				score_point[i][j].LoadBitmap(IDB_4, RGB(0, 0, 0));
				break;
			case 5:
				score_point[i][j].LoadBitmap(IDB_5, RGB(0, 0, 0));
				break;
			case 6:
				score_point[i][j].LoadBitmap(IDB_6, RGB(0, 0, 0));
				break;
			case 7:
				score_point[i][j].LoadBitmap(IDB_7, RGB(0, 0, 0));
				break;
			case 8:
				score_point[i][j].LoadBitmap(IDB_8, RGB(0, 0, 0));
				break;
			case 9:
				score_point[i][j].LoadBitmap(IDB_9, RGB(0, 0, 0));
				break;
			default:
				break;
			}
		}
	}
	for (int i = 0; i < 10; i++) { map_score[0][i].SetTopLeft(430, 50); }
	for (int i = 0; i < 10; i++) { map_score[1][i].SetTopLeft(410, 50); }
	for (int i = 0; i < 10; i++) { map_score[2][i].SetTopLeft(390, 50); }
	for (int i = 0; i < 10; i++) { map_score[3][i].SetTopLeft(370, 50); }
	for (int i = 0; i < 10; i++) { map_score[4][i].SetTopLeft(350, 50); }
	for (int i = 0; i < 10; i++) { score_point[0][i].SetTopLeft(100, 70); }
	for (int i = 0; i < 10; i++) { score_point[1][i].SetTopLeft(80, 70); }
	for (int i = 0; i < 10; i++) { score_point[2][i].SetTopLeft(60, 70); }
	for (int i = 0; i < 6; i++) {
		player[i].SetTopLeft(distance, SIZE_Y - player[i].Height() - 40);
	}
	background.SetTopLeft(0, 0);
	ground.SetTopLeft(0, 0);
	background2.SetTopLeft(13376, 0);
	ground2.SetTopLeft(13376, 0);
	// ---------------------------------------------------初始化POINT位置----------------------------------------------------------------
	cstar[0].SetTopLeft(350, 370);
	cstar[1].SetTopLeft(430, 370);
	cstar[2].SetTopLeft(510, 370);
	cstar[3].SetTopLeft(660, 260);
	cstar[4].SetTopLeft(880, 370);
	cstar[5].SetTopLeft(960, 370);
	cstar[6].SetTopLeft(1040, 370);
	cstar[7].SetTopLeft(2440, 250);
	cstar[8].SetTopLeft(2440, 310);
	cstar[9].SetTopLeft(2440, 370);
	cstar[10].SetTopLeft(2800, 370);
	cstar[11].SetTopLeft(3800, 210);
	cstar[12].SetTopLeft(3800, 260);
	cstar[13].SetTopLeft(4300, 370);
	cstar[14].SetTopLeft(4685, 250);
	cstar[15].SetTopLeft(4685, 290);
	cstar[16].SetTopLeft(6255, 150);
	cstar[17].SetTopLeft(6680, 200);
	cstar[18].SetTopLeft(6705, 180);
	cstar[19].SetTopLeft(6705, 220);
	cstar[20].SetTopLeft(6730, 200);
	cstar[21].SetTopLeft(6960, 200);
	cstar[22].SetTopLeft(6985, 180);
	cstar[23].SetTopLeft(6985, 220);
	cstar[24].SetTopLeft(7010, 200);
	cstar[25].SetTopLeft(7225, 200);
	cstar[26].SetTopLeft(7250, 180);
	cstar[27].SetTopLeft(7250, 220);
	cstar[28].SetTopLeft(7275, 200);
	cstar[29].SetTopLeft(7570, 350);
	cstar[30].SetTopLeft(7930, 310);
	cstar[31].SetTopLeft(8340, 240);
	cstar[32].SetTopLeft(8530, 250);
	cstar[33].SetTopLeft(8930, 320);
	cstar[34].SetTopLeft(10045, 240);
	cstar[35].SetTopLeft(10360, 250);
	cstar[36].SetTopLeft(10420, 255);
	cstar[37].SetTopLeft(10480, 260);
	cstar[38].SetTopLeft(10540, 265);
	cstar[39].SetTopLeft(10720, 265);
	cstar[40].SetTopLeft(10780, 260);
	cstar[41].SetTopLeft(10840, 255);
	cstar[42].SetTopLeft(10900, 250);
	// ---------------------------------------------------初始化POINT位置----------------------------------------------------------------
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT) {

	}
		
	if (nChar == KEY_RIGHT) {

	}
		
	if (nChar == KEY_UP) {

	}
		
	if (nChar == KEY_DOWN) {

	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_SPACE = 0x20; // keyboard空白建
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_SPACE) {
		ATTACH_STATE = true;
	}

	if (nChar == KEY_LEFT) {
		GotoGameState(GAME_STATE_OVER);
	}
		
	if (nChar == KEY_RIGHT) {
		GotoGameState(GAME_STATE_OVER);
	}
		
	if (nChar == KEY_UP) {
		if (CONTINUE_JUMP == true) {
			if (JUMP_STATE == true) {
				max_hight = player[s].Top()-125;
				CONTINUE_JUMP = false;
			}
			JUMP_STATE = true;
			UP_STATE = true;
		}
	}
		
	if (nChar == KEY_DOWN) {
		GotoGameState(GAME_STATE_OVER);
	}
		
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

void CGameStateRun::OnShow()
{
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	//
	//
	//  貼上背景圖、撞擊數、球、擦子、彈跳的球
	//
	//TRACE("s is %d\n", s);

	background.ShowBitmap();
	ground.ShowBitmap();
	background2.ShowBitmap();
	ground2.ShowBitmap();
	if (ATTACH_STATE == true) {
		attack.ShowBitmap();
		ATTACH_STATE = false;
	}
	for (int i = 0; i < total_star; i++) {
		if (cstar[i].get_IS_Show() == true) {
			cstar[i].ShowBitmap();
		}
	}
	player[s].OnShow();
	score_board.ShowBitmap();
	//===========顯示目前距離===========
	current = abs(background.Left());
	int index = 0;
	for (int i = 0; i <= log10(current); i++) {
		index = current / int(pow(10, i)) % 10;
		map_score[i][index].ShowBitmap();
	}
	//===========顯示目前距離===========
	//===========顯示POINT分數===========
	point_board.ShowBitmap();
	for (int i = 0; i <= log10(count_point); i++) {
		index = count_point / int(pow(10, i)) % 10;
		score_point[i][index].ShowBitmap();
	}
	//===========顯示POINT分數===========
}
}