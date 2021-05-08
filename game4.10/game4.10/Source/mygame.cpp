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
	ofs.open("./set.txt");
	if (!ofs.is_open()) {
		GotoGameState(GAME_STATE_OVER);
	}
	ofs << "0";
	ofs.close();
}

void CGameStateSelect::OnBeginState() {
	
}

void CGameStateSelect::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

}

void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point) {
	ofs.open("./set.txt");
	if (!ofs.is_open()) {
		GotoGameState(GAME_STATE_OVER);
	}
	if (point.x > SIZE_X-btnAudio_open.Width() && point.y < btnAudio_open.Height()) {
		swAudio++;
	}
	if (point.x > SIZE_X - btnStartGame.Width() && point.y > 400 && point.y < 400 + btnStartGame.Height()) {
		ofs.close();
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	if (point.x > 113 && point.x < 113 + upgrade[0].Width() && point.y>150 && point.y < 150 + upgrade[0].Height()) {
		select = 0;
		ofs << "0";
	}
	if (point.x > 113 && point.x < 113 + upgrade[1].Width() && point.y>227 && point.y < 227 + upgrade[1].Height()) {
		select = 1;
		ofs << "1";
	}
	if (point.x > 113 && point.x < 113 + upgrade[2].Width() && point.y>305 && point.y < 305 + upgrade[2].Height()) {
		select = 2;
		ofs << "2";
	}
	if (point.x > 520 && point.x < 520 + upgrade[3].Width() && point.y>150 && point.y < 150 + upgrade[3].Height()) {
		select = 3;
		ofs << "3";
	}
	if (point.x > 520 && point.x < 520 + upgrade[4].Width() && point.y>227 && point.y < 227 + upgrade[4].Height()) {
		select = 4;
		ofs << "4";
	}
	if (point.x > 520 && point.x < 520 + upgrade[5].Width() && point.y>305 && point.y < 305 + upgrade[5].Height()) {
		select = 5;
		ofs << "5";
	}
	ofs.close();
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
	translating = player[s].Width() + 50;
	ifs.open("./set.txt", ios::in);
	if (!ifs.is_open()) {
		GotoGameState(GAME_STATE_OVER);
	}
	getline(ifs, c);
	ifs.close();
	//TRACE("c = %d\n",c[0]-'0');
	//s = c - '0';
	//
	// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// 移動背景圖的座標
	//

	//-----------------------------------------------------偵測底部----------------------------------------------------------------
	if (abs(ground.Left()) + translating > 0 && abs(ground.Left()) + translating < 1275) { bottom = 415; }
	if (abs(ground.Left()) + translating > 1275 && abs(ground.Left()) + translating < 2158) { bottom = int(-0.23*(abs(ground.Left())+translating-1275)+430); }
	if (abs(ground.Left()) + translating > 2158 && abs(ground.Left()) + translating < 2362) { bottom = 482; }
	if (abs(ground.Left()) + translating > 2362) { bottom = 415; }
	//-----------------------------------------------------偵測底部----------------------------------------------------------------

	//-----------------------------------------------------偵測障礙物----------------------------------------------------------------
	background.SetTopLeft(background.Left()-map_speed, 0);
	ground.SetTopLeft(ground.Left() - map_speed, 0);
	if (JUMP_STATE == false) {
		max_hight = bottom - player[s].Height() - 125;
	}
	if (JUMP_STATE == true) {
		if (player[s].Top() > max_hight && UP_STATE==true) {
			player[s].SetTopLeft(50, player[s].Top() - dump_speed);
		}
		else if(player[s].Top()+player[s].Height()< bottom){
			UP_STATE = false;
			player[s].SetTopLeft(50, player[s].Top() + dump_speed);
		}
		else {
			player[s].SetTopLeft(50, bottom - player[s].Height());
			JUMP_STATE = false;
			CONTINUE_JUMP = true;
			max_hight = bottom - player[s].Height() - 125;
			IS_FUNC = true;
		}
	}
	if (abs(ground.Left())>(640-translating) && abs(ground.Left())<(730-translating) && player[s].Top() + player[s].Height() > 360) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	if (abs(ground.Left()) > (1275 - translating) && abs(ground.Left()) < (1790 - translating)) {
		player[s].SetTopLeft(50, player[s].Top()-3);
	}
	if (abs(ground.Left()) + translating > 1275 && abs(ground.Left()) + translating < 2158) {
		IS_FUNC = false;
	}
	if (abs(ground.Left()) + translating > 2158 && abs(ground.Left()) + translating < 2170) {
		IS_FUNC = true;
	}
	if (abs(ground.Left()) > (2158 - translating) && abs(ground.Left()) < (2362 - translating) && player[s].Top() + player[s].Height() > 450) {
		if (UP_STATE == false) {
			GotoGameState(GAME_STATE_OVER);
		}
	}
	attack.SetTopLeft(player[s].Left()+player[s].Width()-100, player[s].Top()-10);
	player[s].OnMove();
	if (IS_FUNC == true) {
		if ((player[s].Top() + player[s].Height()) < bottom) {
			player[s].SetTopLeft(50, player[s].Top() + 15);
		}
	}
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
	s = 5;
	IS_FUNC = true;
	max_hight = 200;
	bottom = 0;
	map_speed = 10;
	dump_speed = 15;
	CONTINUE_JUMP = true;
	JUMP_STATE = false;
	UP_STATE = false;
	ATTACH_STATE = false;
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
	ground.LoadBitmap(IDB_BITMAP163, RGB(255, 255, 255));
	attack.LoadBitmap(IDB_BITMAP160, RGB(255, 255, 255));
	for (int i = 0; i < 6; i++) {
		player[i].SetTopLeft(50, SIZE_Y - player[i].Height() - 40);
	}
	background.SetTopLeft(0, 0);
	ground.SetTopLeft(0, 0);
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
		GotoGameState(GAME_STATE_SELECT);
	}
		
	if (nChar == KEY_RIGHT) {
		GotoGameState(GAME_STATE_SELECT);
	}
		
	if (nChar == KEY_UP) {
		IS_FUNC = false;
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
		GotoGameState(GAME_STATE_SELECT);
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
	if (ATTACH_STATE == true) {
		attack.ShowBitmap();
		ATTACH_STATE = false;
	}
	player[s].OnShow();
}
}