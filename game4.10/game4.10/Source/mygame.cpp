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
		swAudio = 0;
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
		CAudio::Instance()->Load(AUDIO_OPEN, "sounds\\opening.mp3");
		CAudio::Instance()->Load(AUDIO_SELECT, "sounds\\pickWeapon.mp3");
		CAudio::Instance()->Load(AUDIO_RUN, "sounds\\battleMusic.mp3");
		CAudio::Instance()->Load(AUDIO_DEAD, "sounds\\dead.mp3");
		CAudio::Instance()->Load(AUDIO_EAT_POINT, "sounds\\eating.mp3");
		CAudio::Instance()->Load(AUDIO_EAT_STAR, "sounds\\eatbig.mp3");
		CAudio::Instance()->Load(AUDIO_SLASH, "sounds\\slash.mp3");
		CAudio::Instance()->Load(AUDIO_MONSTERDIE, "sounds\\monsterDie.mp3");
		CAudio::Instance()->Load(AUDIO_JUMP, "sounds\\jump.mp3");
		CAudio::Instance()->Load(AUDIO_OVER, "sounds\\sad.mp3");

		if (MyRead("audio.txt") == 1) {
			CAudio::Instance()->Play(AUDIO_OPEN, true);
		}
	}

	void CGameStateInit::MyWrite(string file, int c) {
		ofstream ofs;
		ofs.open(file);
		if (!ofs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ofs << c;
		ofs.close();
	}

	int CGameStateInit::MyRead(string file) {
		int temp;
		ifstream ifs;
		ifs.open(file);
		if (!ifs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ifs >> temp;
		ifs.close();
		return temp;
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_PREPARE);						// 切換至GAME_STATE_PREPARE
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (point.x > SIZE_X - btnAudio_open.Width() && point.y < btnAudio_open.Height()) {
			if (swAudio == 0) {
				swAudio = 1;
				CAudio::Instance()->Play(AUDIO_OPEN, true);
			}
			else {
				swAudio = 0;
				CAudio::Instance()->Stop(AUDIO_OPEN);
			}
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
		logo1.SetTopLeft(0, 0);
		logo2.SetTopLeft(0, 0);
		green_effect.SetTopLeft(150, 200);
		btnAudio_open.SetTopLeft(SIZE_X - btnAudio_open.Width(), 0);
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
		if (swAudio == 1) {
			btnAudio_open.ShowBitmap();
			MyWrite("audio.txt", 1);
		}
		else {
			btnAudio_close.ShowBitmap();
			MyWrite("audio.txt", 0);
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
		swAudio = MyRead("audio.txt");
		background.LoadBitmap(IDB_BITMAP88);
		btnStartGame.LoadBitmap(IDB_BITMAP43, RGB(0, 255, 0));
		btnAudio_open.LoadBitmap(IDB_BITMAP55, RGB(0, 255, 0));
		btnAudio_close.LoadBitmap(IDB_BITMAP56, RGB(0, 255, 0));
	}

	void CGameStatePrepare::OnBeginState() {
		
	}

	void CGameStatePrepare::MyWrite(string file, int c) {
		ofstream ofs;
		ofs.open(file);
		if (!ofs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ofs << c;
		ofs.close();
	}

	int CGameStatePrepare::MyRead(string file) {
		int temp;
		ifstream ifs;
		ifs.open(file);
		if (!ifs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ifs >> temp;
		ifs.close();
		return temp;
	}

	void CGameStatePrepare::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

	}

	void CGameStatePrepare::OnLButtonDown(UINT nFlags, CPoint point) {
		if (swAudio == 0) {
			swAudio = 1;
			CAudio::Instance()->Play(AUDIO_OPEN, true);
		}
		else {
			swAudio = 0;
			CAudio::Instance()->Stop(AUDIO_OPEN);
		}
		if (point.x > SIZE_X - btnStartGame.Width() && point.y > 241 && point.y < 241 + btnStartGame.Height()) {
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
		if (swAudio == 1) {
			btnAudio_open.ShowBitmap();
			MyWrite("audio.txt", 1);
		}
		else {
			btnAudio_close.ShowBitmap();
			MyWrite("audio.txt", 0);
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	CGameStateSelect::CGameStateSelect(CGame *g) : CGameState(g) {

	}

	void CGameStateSelect::OnInit() {
		swAudio = sMyRead("audio.txt");
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
		for (int i = 0; i < 6; i++) { block[i].LoadBitmap(IDB_BITMAP186); }
		for (int i = 0; i < 6; i++) { IS_SHOW[i] = true; }
		IS_SHOW[0] = false;
		sMyWrite("./set.txt", 0);
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 10; j++) {
				switch (j) {
				case 0:
					money[i][j].LoadBitmap(IDB_BITMAP189, RGB(0, 255, 0));
					break;
				case 1:
					money[i][j].LoadBitmap(IDB_BITMAP190, RGB(0, 255, 0));
					break;
				case 2:
					money[i][j].LoadBitmap(IDB_BITMAP191, RGB(0, 255, 0));
					break;
				case 3:
					money[i][j].LoadBitmap(IDB_BITMAP192, RGB(0, 255, 0));
					break;
				case 4:
					money[i][j].LoadBitmap(IDB_BITMAP193, RGB(0, 255, 0));
					break;
				case 5:
					money[i][j].LoadBitmap(IDB_BITMAP194, RGB(0, 255, 0));
					break;
				case 6:
					money[i][j].LoadBitmap(IDB_BITMAP195, RGB(0, 255, 0));
					break;
				case 7:
					money[i][j].LoadBitmap(IDB_BITMAP196, RGB(0, 255, 0));
					break;
				case 8:
					money[i][j].LoadBitmap(IDB_BITMAP197, RGB(0, 255, 0));
					break;
				case 9:
					money[i][j].LoadBitmap(IDB_BITMAP198, RGB(0, 255, 0));
					break;
				default:
					break;
				}
			}
		}
		for (int i = 0; i < 10; i++) { money[0][i].SetTopLeft(130, 20); }
		for (int i = 0; i < 10; i++) { money[1][i].SetTopLeft(100, 20); }
		for (int i = 0; i < 10; i++) { money[2][i].SetTopLeft(70, 20); }
		for (int i = 0; i < 10; i++) { money[3][i].SetTopLeft(40, 20); }
		for (int i = 0; i < 10; i++) { money[4][i].SetTopLeft(10, 20); }
	}

	void CGameStateSelect::OnBeginState() {
		CAudio::Instance()->Stop(AUDIO_OPEN);
		CAudio::Instance()->Stop(AUDIO_RUN);
		CAudio::Instance()->Stop(AUDIO_OVER);
		if (sMyRead("audio.txt")==1) {
			CAudio::Instance()->Play(AUDIO_SELECT, true);
		}
	}

	void CGameStateSelect::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		const char KEY_SPACE = 0x20; // keyboard空白建
		if (nChar == KEY_SPACE) {
			sMyWrite("money.txt", sMyRead("money.txt") + 500);
		}
	}

	void CGameStateSelect::sMyWrite(string file, int c) {
		ofstream ofs;
		ofs.open(file);
		if (!ofs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ofs << c;
		ofs.close();
	}

	int CGameStateSelect::sMyRead(string file) {
		int temp;
		ifstream ifs;
		ifs.open(file);
		if (!ifs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ifs >> temp;
		ifs.close();
		return temp;
	}

	void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point) {
		if (swAudio == 0) {
			swAudio = 1;
			CAudio::Instance()->Play(AUDIO_SELECT, true);
		}
		else {
			swAudio = 0;
			CAudio::Instance()->Stop(AUDIO_SELECT);
		}
		if (point.x > SIZE_X - btnStartGame.Width() && point.y > 400 && point.y < 400 + btnStartGame.Height()) {
			GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		}
		if (point.x > 113 && point.x < 113 + upgrade[0].Width() && point.y>150 && point.y < 150 + upgrade[0].Height()) {
			if (sMyRead("./money.txt") >= 0 || IS_SHOW[0] == false) {
				if (IS_SHOW[0] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 0);
					IS_SHOW[0] = false;
					select = 0;
					sMyWrite("./set.txt", 0);
				}
				else {
					select = 0;
					sMyWrite("./set.txt", 0);
				}
			}
		}
		if (point.x > 113 && point.x < 113 + upgrade[1].Width() && point.y>227 && point.y < 227 + upgrade[1].Height()) {
			if (sMyRead("./money.txt") >= 300 || IS_SHOW[1] == false) {
				if (IS_SHOW[1] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 300);
					IS_SHOW[1] = false;
					select = 1;
					sMyWrite("./set.txt", 1);
				}
				else {
					select = 1;
					sMyWrite("./set.txt", 1);
				}
			}
		}
		if (point.x > 113 && point.x < 113 + upgrade[2].Width() && point.y>305 && point.y < 305 + upgrade[2].Height()) {
			if (sMyRead("./money.txt") >= 600 || IS_SHOW[2] == false) {
				if (IS_SHOW[2] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 600);
					IS_SHOW[2] = false;
					select = 2;
					sMyWrite("./set.txt", 2);
				}
				else {
					select = 2;
					sMyWrite("./set.txt", 2);
				}
			}
		}
		if (point.x > 520 && point.x < 520 + upgrade[3].Width() && point.y>150 && point.y < 150 + upgrade[3].Height()) {
			if (sMyRead("./money.txt") >= 900 || IS_SHOW[3] == false) {
				if (IS_SHOW[3] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 900);
					IS_SHOW[3] = false;
					select = 3;
					sMyWrite("./set.txt", 3);
				}
				else {
					select = 3;
					sMyWrite("./set.txt", 3);
				}
			}
		}
		if (point.x > 520 && point.x < 520 + upgrade[4].Width() && point.y>227 && point.y < 227 + upgrade[4].Height()) {
			if (sMyRead("./money.txt") >= 1200 || IS_SHOW[4] == false) {
				if (IS_SHOW[4] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 1200);
					IS_SHOW[4] = false;
					select = 4;
					sMyWrite("./set.txt", 4);
				}
				else {
					select = 4;
					sMyWrite("./set.txt", 4);
				}
			}
		}
		if (point.x > 520 && point.x < 520 + upgrade[5].Width() && point.y>305 && point.y < 305 + upgrade[5].Height()) {
			if (sMyRead("./money.txt") >= 1500 || IS_SHOW[5] == false) {
				if (IS_SHOW[5] == true) {
					sMyWrite("./money.txt", sMyRead("./money.txt") - 1500);
					IS_SHOW[5] = false;
					select = 5;
					sMyWrite("./set.txt", 5);
				}
				else {
					select = 5;
					sMyWrite("./set.txt", 5);
				}
			}
		}
	}

	void CGameStateSelect::OnShow() {
		background.SetTopLeft(0, 0);
		btnStartGame.SetTopLeft(SIZE_X - btnStartGame.Width(), 400);
		btnAudio_open.SetTopLeft(SIZE_X - btnAudio_open.Width(), 10);
		btnAudio_close.SetTopLeft(SIZE_X - btnAudio_open.Width(), 10);
		upgrade[0].SetTopLeft(113, 150);
		upgrade[1].SetTopLeft(113, 227);
		upgrade[2].SetTopLeft(113, 305);
		upgrade[3].SetTopLeft(520, 150);
		upgrade[4].SetTopLeft(520, 227);
		upgrade[5].SetTopLeft(520, 305);
		block[0].SetTopLeft(113, 150);
		block[1].SetTopLeft(113, 227);
		block[2].SetTopLeft(113, 305);
		block[3].SetTopLeft(520, 150);
		block[4].SetTopLeft(520, 227);
		block[5].SetTopLeft(520, 305);
		background.ShowBitmap();
		player[select].SetTopLeft(330, 260);
		frame.SetTopLeft(upgrade[select].Left(), upgrade[select].Top());

		btnStartGame.ShowBitmap();
		if (swAudio == 1) {
			btnAudio_open.ShowBitmap();
			sMyWrite("audio.txt", 1);
		}
		else {
			btnAudio_close.ShowBitmap();
			sMyWrite("audio.txt", 0);
		}
		frame.ShowBitmap();
		for (int i = 0; i < 6; i++) {
			upgrade[i].ShowBitmap();
		}
		player[select].SetDelayCount(1);
		player[select].OnMove();
		player[select].OnShow();
		for (int i = 0; i < 6; i++) { 
			if (IS_SHOW[i] == true) {
				block[i].ShowBitmap();
			}
		}
		for (int i = 0; i <= log10(sMyRead("money.txt")); i++) {
			int index = sMyRead("money.txt") / int(pow(10, i)) % 10;
			money[i][index].ShowBitmap();
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g) : CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		background.SetTopLeft(0, 0);
		btnAgain.SetTopLeft(290, 350);
		/*counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);*/
	}

	void CGameStateOver::OnBeginState()
	{
		//counter = 30 * 5; // 5 seconds
		if (MyRead("audio.txt") == 1) {
			CAudio::Instance()->Play(AUDIO_DEAD);
			CAudio::Instance()->Play(AUDIO_OVER, true);
		}
		CAudio::Instance()->Stop(AUDIO_RUN);
	}

	void CGameStateOver::OnInit()
	{
		background.LoadBitmap(IDB_BITMAP171);
		btnAgain.LoadBitmap(IDB_BITMAP172);
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
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 10; j++) {
				switch (j) {
				case 0:
					map_score[i][j].LoadBitmap(IDB_BITMAP189, RGB(0, 255, 0));
					break;
				case 1:
					map_score[i][j].LoadBitmap(IDB_BITMAP190, RGB(0, 255, 0));
					break;
				case 2:
					map_score[i][j].LoadBitmap(IDB_BITMAP191, RGB(0, 255, 0));
					break;
				case 3:
					map_score[i][j].LoadBitmap(IDB_BITMAP192, RGB(0, 255, 0));
					break;
				case 4:
					map_score[i][j].LoadBitmap(IDB_BITMAP193, RGB(0, 255, 0));
					break;
				case 5:
					map_score[i][j].LoadBitmap(IDB_BITMAP194, RGB(0, 255, 0));
					break;
				case 6:
					map_score[i][j].LoadBitmap(IDB_BITMAP195, RGB(0, 255, 0));
					break;
				case 7:
					map_score[i][j].LoadBitmap(IDB_BITMAP196, RGB(0, 255, 0));
					break;
				case 8:
					map_score[i][j].LoadBitmap(IDB_BITMAP197, RGB(0, 255, 0));
					break;
				case 9:
					map_score[i][j].LoadBitmap(IDB_BITMAP198, RGB(0, 255, 0));
					break;
				default:
					break;
				}
			}
		}
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 10; j++) {
				switch (j) {
				case 0:
					max_score[i][j].LoadBitmap(IDB_BITMAP189, RGB(0, 255, 0));
					break;
				case 1:
					max_score[i][j].LoadBitmap(IDB_BITMAP190, RGB(0, 255, 0));
					break;
				case 2:
					max_score[i][j].LoadBitmap(IDB_BITMAP191, RGB(0, 255, 0));
					break;
				case 3:
					max_score[i][j].LoadBitmap(IDB_BITMAP192, RGB(0, 255, 0));
					break;
				case 4:
					max_score[i][j].LoadBitmap(IDB_BITMAP193, RGB(0, 255, 0));
					break;
				case 5:
					max_score[i][j].LoadBitmap(IDB_BITMAP194, RGB(0, 255, 0));
					break;
				case 6:
					max_score[i][j].LoadBitmap(IDB_BITMAP195, RGB(0, 255, 0));
					break;
				case 7:
					max_score[i][j].LoadBitmap(IDB_BITMAP196, RGB(0, 255, 0));
					break;
				case 8:
					max_score[i][j].LoadBitmap(IDB_BITMAP197, RGB(0, 255, 0));
					break;
				case 9:
					max_score[i][j].LoadBitmap(IDB_BITMAP198, RGB(0, 255, 0));
					break;
				default:
					break;
				}
			}
		}
		for (int i = 0; i < 10; i++) { map_score[0][i].SetTopLeft(170, 250); }
		for (int i = 0; i < 10; i++) { map_score[1][i].SetTopLeft(140, 250); }
		for (int i = 0; i < 10; i++) { map_score[2][i].SetTopLeft(110, 250); }
		for (int i = 0; i < 10; i++) { map_score[3][i].SetTopLeft(80, 250); }
		for (int i = 0; i < 10; i++) { map_score[4][i].SetTopLeft(50, 250); }
		for (int i = 0; i < 10; i++) { max_score[0][i].SetTopLeft(715, 250); }
		for (int i = 0; i < 10; i++) { max_score[1][i].SetTopLeft(685, 250); }
		for (int i = 0; i < 10; i++) { max_score[2][i].SetTopLeft(655, 250); }
		for (int i = 0; i < 10; i++) { max_score[3][i].SetTopLeft(625, 250); }
		for (int i = 0; i < 10; i++) { max_score[4][i].SetTopLeft(595, 250); }
	}

	void CGameStateOver::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		if (point.x > 290 && point.x < 290 + btnAgain.Width() && point.y > 350 && point.y < 350 + btnAgain.Height()) {
			GotoGameState(GAME_STATE_SELECT);
		}
	}

	void CGameStateOver::OnShow()
	{
		background.ShowBitmap();
		btnAgain.ShowBitmap();
		for (int i = 0; i <= log10(MyRead("record.txt")); i++) {
			int index = MyRead("record.txt") / int(pow(10, i)) % 10;
			map_score[i][index].ShowBitmap();
		}
		if (MyRead("record.txt") >= MyRead("champion.txt")) {
			MyWrite("champion.txt", MyRead("record.txt"));
		}
		for (int i = 0; i <= log10(MyRead("champion.txt")); i++) {
			int index = MyRead("champion.txt") / int(pow(10, i)) % 10;
			max_score[i][index].ShowBitmap();
		}
		/*CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC*/
	}

	void CGameStateOver::MyWrite(string file, int c) {
		ofstream ofs;
		ofs.open(file);
		if (!ofs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ofs << c;
		ofs.close();
	}

	int CGameStateOver::MyRead(string file) {
		int temp;
		ifstream ifs;
		ifs.open(file);
		if (!ifs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ifs >> temp;
		ifs.close();
		return temp;
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
		CAudio::Instance()->Stop(AUDIO_SELECT);
		if (MyRead("audio.txt") == 1) {
			CAudio::Instance()->Play(AUDIO_RUN, true);
		}
		s = MyRead("./set.txt");
		BOUNCE_STATE = false;
		IS_MONSTERDIE = false;
		IS_FUNC = true;
		distance = 50;
		AUTO_JUMP = true;
		DRAG_STATE = false;
		CONTINUE_JUMP = true;
		JUMP_STATE = false;
		UP_STATE = false;
		ATTACH_STATE = false;
		max_hight = 300;
		bottom = 0;
		map_speed = 10;
		jump_speed = 25;
		dump_speed = 6;
		acceleration = 1;
	}

	int CGameStateRun::MyRead(string file) {
		int temp;
		ifstream ifs;
		ifs.open(file);
		if (!ifs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ifs >> temp;
		ifs.close();
		return temp;
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		translating = player[s].Width() + distance;
		if (abs(ground.Left()) > 37800) {
			GotoGameState(GAME_STATE_END);
		}
		if (MyRead("flag.txt")==1 && cheat==0) {
			int temp = MyRead("record.txt");
			count_point = MyRead("./money.txt");
			for (int i = 0; i < 9; i++) {
				if (temp >= record_point[i][0] && temp < record_point[i + 1][0]) {
					player[s].SetTopLeft(distance, record_point[i][1] - player[s].Height());
					for (int j = 0; j < total_star; j++) {
						cstar[j].SetTopLeft(cstar[j].Left() + abs(record_point[i][0]- abs(background.Left())), cstar[j].Top());
					}
					for (int j = 0; j < total_enemy; j++) {
						enemy[j].SetTopLeft(enemy[j].Left() + abs(record_point[i][0] - abs(background.Left())), enemy[j].Top());
					}
					background.SetTopLeft(0 - record_point[i][0], 0);
					ground.SetTopLeft(0 - record_point[i][0], 0);
					background2.SetTopLeft(13176 - record_point[i][0], 0);
					ground2.SetTopLeft(13176 - record_point[i][0], 0);
					background3.SetTopLeft(23402 - record_point[i][0], 0);
					ground3.SetTopLeft(23402 - record_point[i][0], 0);
					MyWrite("flag.txt", 0);
					break;
				}
			}
		}
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
		if (abs(ground.Left()) + translating > 1281 && abs(ground.Left()) + translating < 2157) { bottom = int(-0.18*(abs(ground.Left()) + translating - 1271) + 430) - 30; }
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
		if (abs(ground.Left()) + translating > 5037 && abs(ground.Left()) + translating < 5885) { bottom = int(-0.17*(abs(ground.Left()) + translating - 5037) + 420) - 15; }
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
		if (abs(ground.Left()) + translating > 13176 && abs(ground.Left()) + translating < 14086) { bottom = 380; }
		if (abs(ground.Left()) + translating > 14086 && abs(ground.Left()) + translating < 15636) { bottom = 482; }
		if (abs(ground.Left()) + translating > 15636 && abs(ground.Left()) + translating < 16476) { bottom = 390; }
		if (abs(ground.Left()) + translating > 16476 && abs(ground.Left()) + translating < 16676) { bottom = 482; }
		if (abs(ground.Left()) + translating > 16676 && abs(ground.Left()) + translating < 16806) { bottom = 345; }
		if (abs(ground.Left()) + translating > 16806 && abs(ground.Left()) + translating < 17031) { bottom = 482; }
		if (abs(ground.Left()) + translating > 17031 && abs(ground.Left()) + translating < 17161) { bottom = 345; }
		if (abs(ground.Left()) + translating > 17161 && abs(ground.Left()) + translating < 17356) { bottom = 482; }
		if (abs(ground.Left()) + translating > 17356 && abs(ground.Left()) + translating < 18081) { bottom = 390; }
		if (abs(ground.Left()) + translating > 18081 && abs(ground.Left()) + translating < 18236) { bottom = 482; }
		if (abs(ground.Left()) + translating > 18236 && abs(ground.Left()) + translating < 18356) { bottom = 390; }
		if (abs(ground.Left()) + translating > 18356 && abs(ground.Left()) + translating < 18521) { bottom = 482; }
		if (abs(ground.Left()) + translating > 18521 && abs(ground.Left()) + translating < 19746) { bottom = 390; }
		if (abs(ground.Left()) + translating > 19746 && abs(ground.Left()) + translating < 21206) { bottom = 482; }
		if (abs(ground.Left()) + translating > 21206 && abs(ground.Left()) + translating < 23402) { bottom = 390; }
		if (abs(ground.Left()) + translating > 23402 && abs(ground.Left()) + translating < 25377) { bottom = 380; }
		if (abs(ground.Left()) + translating > 25377 && abs(ground.Left()) + translating < 25507) { bottom = 320; }
		if (abs(ground.Left()) + translating > 25507 && abs(ground.Left()) + translating < 25672) { bottom = 380; }
		if (abs(ground.Left()) + translating > 25672 && abs(ground.Left()) + translating < 25802) { bottom = 320; }
		if (abs(ground.Left()) + translating > 25802 && abs(ground.Left()) + translating < 26042) { bottom = 482; }
		if (abs(ground.Left()) + translating > 26042 && abs(ground.Left()) + translating < 26642) { bottom = 380; }
		if (abs(ground.Left()) + translating > 26642 && abs(ground.Left()) + translating < 26812) { bottom = 482; }
		if (abs(ground.Left()) + translating > 26812 && abs(ground.Left()) + translating < 27422) { bottom = 380; }
		if (abs(ground.Left()) + translating > 27422 && abs(ground.Left()) + translating < 27582) { bottom = int(-0.34*(abs(ground.Left()) + translating - 27422) + 385) - 15; }
		if (abs(ground.Left()) + translating > 27582 && abs(ground.Left()) + translating < 27777) { bottom = 482; }
		if (abs(ground.Left()) + translating > 27777 && abs(ground.Left()) + translating < 27907) { bottom = 355; }
		if (abs(ground.Left()) + translating > 27907 && abs(ground.Left()) + translating < 28577) { bottom = 482; }
		if (abs(ground.Left()) + translating > 28577 && abs(ground.Left()) + translating < 28710) { bottom = 355; }
		if (abs(ground.Left()) + translating > 28710 && abs(ground.Left()) + translating < 29392) { bottom = 482; }
		if (abs(ground.Left()) + translating > 29392 && abs(ground.Left()) + translating < 29522) { bottom = 355; }
		if (abs(ground.Left()) + translating > 29522 && abs(ground.Left()) + translating < 29652) { bottom = 482; }
		if (abs(ground.Left()) + translating > 29652 && abs(ground.Left()) + translating < 30712) { bottom = 375; }
		if (abs(ground.Left()) + translating > 30712 && abs(ground.Left()) + translating < 30842) { bottom = 315; }
		if (abs(ground.Left()) + translating > 30842 && abs(ground.Left()) + translating < 31847) { bottom = 375; }
		if (abs(ground.Left()) + translating > 31847 && abs(ground.Left()) + translating < 32017) { bottom = 482; }
		if (abs(ground.Left()) + translating > 32017 && abs(ground.Left()) + translating < 32147) { bottom = 460; }
		if (abs(ground.Left()) + translating > 32147 && abs(ground.Left()) + translating < 32257) { bottom = 482; }
		if (abs(ground.Left()) + translating > 32257 && abs(ground.Left()) + translating < 32387) { bottom = 435; }
		if (abs(ground.Left()) + translating > 32387 && abs(ground.Left()) + translating < 32482) { bottom = 482; }
		if (abs(ground.Left()) + translating > 32482 && abs(ground.Left()) + translating < 32612) { bottom = 400; }
		if (abs(ground.Left()) + translating > 32612 && abs(ground.Left()) + translating < 32717) { bottom = 482; }
		if (abs(ground.Left()) + translating > 32717 && abs(ground.Left()) + translating < 35227) { bottom = 385; }
		if (abs(ground.Left()) + translating > 35227 && abs(ground.Left()) + translating < 35377) { bottom = 482; }
		if (abs(ground.Left()) + translating > 35377 && abs(ground.Left()) + translating < 35507) { bottom = 365; }
		if (abs(ground.Left()) + translating > 35507 && abs(ground.Left()) + translating < 35627) { bottom = 482; }
		if (abs(ground.Left()) + translating > 35627 && abs(ground.Left()) + translating < 99999) { bottom = 375; }

		bottom += 15;
		//-----------------------------------------------------偵測底部----------------------------------------------------------------

		//-----------------------------------------------------偵測障礙物----------------------------------------------------------------
		background.SetTopLeft(background.Left() - map_speed, 0);
		ground.SetTopLeft(ground.Left() - map_speed, 0);
		background2.SetTopLeft(background2.Left() - map_speed, 0);
		ground2.SetTopLeft(ground2.Left() - map_speed, 0);
		background3.SetTopLeft(background3.Left() - map_speed, 0);
		ground3.SetTopLeft(ground3.Left() - map_speed, 0);
		score_board.SetTopLeft(240, 0);
		MyWrite("record.txt", abs(background.Left()));
		MyWrite("./money.txt", count_point);
		if (UP_STATE == true) {
			IS_FUNC = false;
		}
		if (JUMP_STATE == false) {
			max_hight = bottom - player[s].Height() - 140;
			jump_speed = 25;
			dump_speed = 6;
		}
		if (JUMP_STATE == true) {
			if (player[s].Top() > max_hight && UP_STATE == true) {
				player[s].SetTopLeft(distance, player[s].Top() - jump_speed);
				jump_speed -= acceleration;
			}
			else if (player[s].Top() + player[s].Height() < bottom) {
				UP_STATE = false;
				player[s].SetTopLeft(distance, player[s].Top() + dump_speed);
				dump_speed += acceleration;
			}
			else {
				player[s].SetTopLeft(distance, bottom - player[s].Height());
				JUMP_STATE = false;
				CONTINUE_JUMP = true;
				max_hight = bottom - player[s].Height() - 140;
				IS_FUNC = true;
				jump_speed = 25;
				dump_speed = 6;
			}
		}
		if (abs(ground.Left()) > (632 - translating) && abs(ground.Left()) < (741 - translating) && player[s].Top() + player[s].Height() > 368) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 1281 && abs(ground.Left()) + translating < 2157) {
			if (JUMP_STATE == false) {
				player[s].SetTopLeft(distance, bottom - player[s].Height());
			}
		}
		if (abs(ground.Left()) + translating > 1281 && abs(ground.Left()) + translating < 2157) {
			IS_FUNC = false;
		}
		if (abs(ground.Left()) + translating > 2157 && abs(ground.Left()) + translating < 2181) {
			IS_FUNC = true;
		}
		if (abs(ground.Left()) > (2157 - translating) && abs(ground.Left()) < (2368 - translating) && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) > (2554 - translating) && abs(ground.Left()) < (2763 - translating) && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 3028 && abs(ground.Left()) + translating < 3106 && player[s].Top() + player[s].Height() > 383 && player[s].Left() > 0) {
			player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
		}
		if (abs(ground.Left()) + translating > 3108 && abs(ground.Left()) + translating < 3148 && player[s].Left() < distance) {
			player[s].SetTopLeft(player[s].Left() + map_speed * 2, player[s].Top());
		}
		if (abs(ground.Left()) > (3265 - translating) && abs(ground.Left()) < (3375 - translating) && player[s].Top() + player[s].Height() > 368) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
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
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) > (4633 - translating) && abs(ground.Left()) < (4775 - translating) && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
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
				MyWrite("flag.txt", 1);
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
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 6805 && abs(ground.Left()) + translating < 6925 && player[s].Top() + player[s].Height() > 430) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 7080 && abs(ground.Left()) + translating < 7195 && player[s].Top() + player[s].Height() > 430) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 7350 && abs(ground.Left()) + translating < 7467 && player[s].Top() + player[s].Height() > 430) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
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
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 8590 && abs(ground.Left()) + translating < 8677 && player[s].Top() + player[s].Height() > 298) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
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
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 13989 && abs(ground.Left()) + translating < 14501 && player[s].Top() < 100) {
			JUMP_STATE = false;
			IS_FUNC = false;
			DRAG_STATE = true;
		}
		if (DRAG_STATE == true) {
			if (abs(ground.Left()) + translating > 13989 && abs(ground.Left()) + translating < 15631) {
				int temp = int(0.094*(abs(ground.Left()) + translating - 13989) + 30);
				player[s].SetTopLeft(distance, temp);
			}
		}
		if (abs(ground.Left()) + translating > 15629 && abs(ground.Left()) + translating < 15651) {
			IS_FUNC = true;
			CONTINUE_JUMP = true;
			DRAG_STATE = false;
			UP_STATE = false;
		}
		if (abs(ground.Left()) + translating > 16001 && abs(ground.Left()) + translating < 16091 && player[s].Top() + player[s].Height() > 355) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 16476 && abs(ground.Left()) + translating < 16676 && player[s].Top() + player[s].Height() > 420) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 16806 && abs(ground.Left()) + translating < 17031 && player[s].Top() + player[s].Height() > 420) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 17161 && abs(ground.Left()) + translating < 17356 && player[s].Top() + player[s].Height() > 420) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 18081 && abs(ground.Left()) + translating < 18236 && player[s].Top() + player[s].Height() > 480) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 18356 && abs(ground.Left()) + translating < 18521 && player[s].Top() + player[s].Height() > 480) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 19746 && abs(ground.Left()) + translating < 21206 && player[s].Top() + player[s].Height() > 430) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 19636 && abs(ground.Left()) + translating < 20001 && player[s].Top() < 100) {
			JUMP_STATE = false;
			IS_FUNC = false;
			DRAG_STATE = true;
		}
		if (DRAG_STATE == true) {
			if (abs(ground.Left()) + translating > 19636 && abs(ground.Left()) + translating < 21206) {
				int temp = int(0.094*(abs(ground.Left()) + translating - 19636) + 30);
				player[s].SetTopLeft(distance, temp);
			}
		}
		if (abs(ground.Left()) + translating > 21206 && abs(ground.Left()) + translating < 21231) {
			IS_FUNC = true;
			CONTINUE_JUMP = true;
			DRAG_STATE = false;
			UP_STATE = false;
		}
		if (abs(ground.Left()) + translating > 25377 && abs(ground.Left()) + translating < 25507 && player[s].Top() + player[s].Height() > 350 && player[s].Left() > 0) {
			player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
		}
		if (abs(ground.Left()) + translating > 25459 && abs(ground.Left()) + translating < 25491 && player[s].Left() < distance) {
			player[s].SetTopLeft(player[s].Left() + map_speed * 2, player[s].Top());
		}
		if (abs(ground.Left()) + translating > 25507 && abs(ground.Left()) + translating < 25672 && player[s].Top() + player[s].Height() > 360) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 25802 && abs(ground.Left()) + translating < 26042 && player[s].Top() + player[s].Height() > 470) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 26332 && abs(ground.Left()) + translating < 26422 && player[s].Top() + player[s].Height() > 350) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 26642 && abs(ground.Left()) + translating < 26812 && player[s].Top() + player[s].Height() > 470) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 27422 && abs(ground.Left()) + translating < 27582 && JUMP_STATE==false) {
			player[s].SetTopLeft(player[s].Left(), bottom - player[s].Height());
		}
		if (abs(ground.Left()) + translating > 27582 && abs(ground.Left()) + translating < 27777 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 27907 && abs(ground.Left()) + translating < 28197 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 28197 && abs(ground.Left()) + translating < 28297 && player[s].Top() + player[s].Height() > 455) {
			BOUNCE_STATE = true;
		}
		if (abs(ground.Left()) + translating > 29002 && abs(ground.Left()) + translating < 29097 && player[s].Top() + player[s].Height() > 455) {
			BOUNCE_STATE = true;
		}
		if (BOUNCE_STATE == true) {
			if (player[s].Top() + player[s].Height() >= (bottom-5)) {
				IS_FUNC = true;
				BOUNCE_STATE = false;
			}
			JUMP_STATE = true;
			UP_STATE = false;
			IS_FUNC = false;
			player[s].SetTopLeft(player[s].Left(), player[s].Top() - 47);
			
			
		}
		if (abs(ground.Left()) + translating > 28292 && abs(ground.Left()) + translating < 28582 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 28712 && abs(ground.Left()) + translating < 29002 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 29097 && abs(ground.Left()) + translating < 29397 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 29522 && abs(ground.Left()) + translating < 29652 && player[s].Top() + player[s].Height() > 450) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 29652 && abs(ground.Left()) + translating < 29737 && player[s].Top() + player[s].Height() > 345) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 30412 && abs(ground.Left()) + translating < 30497 && player[s].Top() + player[s].Height() > 345) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 30709 && abs(ground.Left()) + translating < 30842 && player[s].Top() + player[s].Height() > 350 && player[s].Left() > 0) {
			player[s].SetTopLeft(player[s].Left() - map_speed, player[s].Top());
		}
		if (abs(ground.Left()) + translating > 30781 && abs(ground.Left()) + translating < 30821 && player[s].Left() < distance) {
			player[s].SetTopLeft(player[s].Left() + map_speed * 2, player[s].Top());
		}
		if (abs(ground.Left()) + translating > 31262 && abs(ground.Left()) + translating < 31442 && player[s].Top() + player[s].Height() > 345) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 31847 && abs(ground.Left()) + translating < 32017 && player[s].Top() + player[s].Height() > 475) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 32147 && abs(ground.Left()) + translating < 32257 && player[s].Top() + player[s].Height() > 470) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 32387 && abs(ground.Left()) + translating < 32482 && player[s].Top() + player[s].Height() > 470) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 32612 && abs(ground.Left()) + translating < 32717 && player[s].Top() + player[s].Height() > 470) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 35227 && abs(ground.Left()) + translating < 35377 && player[s].Top() + player[s].Height() > 455) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		if (abs(ground.Left()) + translating > 35507 && abs(ground.Left()) + translating < 35627 && player[s].Top() + player[s].Height() > 455) {
			if (UP_STATE == false) {
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
		}

		attack.SetTopLeft(player[s].Left() + player[s].Width() - 100, player[s].Top() - 10);
		player[s].OnMove();

		// ====== enemy =======
		for (int i = 0; i < total_enemy; i++) {
			enemy[i].SetTopLeft(enemy[i].Left() - map_speed, enemy[i].Top());
			if (player[s].Left() + player[s].Width() >= enemy[i].Left() && player[s].Left() <= enemy[i].Left() + enemy[i].Width() && player[s].Top() + player[s].Height() >= enemy[i].Top() && player[s].Top() <= enemy[i].Top() + enemy[i].Height() && IS_ALIVE[i]==true) {
				IS_ALIVE[i] = false;
				MyWrite("flag.txt", 1);
				GotoGameState(GAME_STATE_OVER);
			}
			if ((enemy[i].Left() - (player[s].Left() + player[s].Width()))*(enemy[i].Left() - (player[s].Left() + player[s].Width())) + (enemy[i].Top() + enemy[i].Height() - player[s].Top())*(enemy[i].Top() + enemy[i].Height() - player[s].Top()) < 250000 && i < 10) {
				if (enemy[i].Left() > player[s].Left() + player[s].Width()) {
					enemy[i].SetTopLeft(enemy[i].Left() - 10, enemy[i].Top());
				}
				if (enemy[i].Top() + enemy[i].Height() < player[s].Top() && enemy[i].Top() + enemy[i].Height() < bottom) {
					enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() + 10);
				}
				if (enemy[i].Top() > player[s].Top() + player[s].Height()) {
					enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() - 10);
				}
			}
			if ((enemy[i].Left() - (player[s].Left() + player[s].Width()))*(enemy[i].Left() - (player[s].Left() + player[s].Width())) + (enemy[i].Top() + enemy[i].Height() - player[s].Top())*(enemy[i].Top() + enemy[i].Height() - player[s].Top()) < 250000 && i >= 17 && i < 21) {
				if (enemy[i].Left() > player[s].Left() + player[s].Width()) {
					enemy[i].SetTopLeft(enemy[i].Left() - 5, enemy[i].Top());
				}
			}
			if (i == 21) {
				if (MONSTER_UP[i] == true) {
					if (enemy[i].Top() > 150) {
						enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() - 5);
					}
					else {
						MONSTER_UP[i] = false;
					}
				}
				else {
					enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() + 5);
					if (enemy[i].Top() + enemy[i].Height() > 400) {
						MONSTER_UP[i] = true;
					}
				}
			}
			if (i >= 22 && i<=27) {
				if (MONSTER_UP[i] == true) {
					if (enemy[i].Top() > 150) {
						enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() - 5);
					}
					else {
						MONSTER_UP[i] = false;
					}
				}
				else {
					enemy[i].SetTopLeft(enemy[i].Left(), enemy[i].Top() + 5);
					if (enemy[i].Top() + enemy[i].Height() > 430) {
						MONSTER_UP[i] = true;
					}
				}
			}
			enemy[i].OnMove();
		}
		// ====== enemy =======

		// ====== drop down =======
		if (IS_FUNC == true) {
			if ((player[s].Top() + player[s].Height()) < bottom) {
				player[s].SetTopLeft(distance, player[s].Top() + 10);
			}
		}
		// ====== drop down =======
		//===========偵測point碰撞===========
		for (int i = 0; i < total_star; i++) {
			if (player[s].Left() + player[s].Width() - 30 >= cstar[i].Left() && player[s].Left() - 30 <= cstar[i].Left() + cstar[i].Width() && player[s].Top() + player[s].Height() - 30 >= cstar[i].Top() && player[s].Top() - 30 <= cstar[i].Top() + cstar[i].Height()) {
				if (cstar[i].get_IS_Show() == true && i < 103) {
					if (MyRead("audio.txt") == 1) {
						CAudio::Instance()->Play(AUDIO_EAT_POINT);
					}
					count_point+=6; 
				}
				if (cstar[i].get_IS_Show() == true && i >= 103) { 
					if (MyRead("audio.txt") == 1) {
						CAudio::Instance()->Play(AUDIO_EAT_STAR);
					}
					count_point+=11; 
				}
				cstar[i].set_IS_Show(false);
			}
		}
		//===========偵測point碰撞===========
		//-----------------------------------------------------偵測障礙物----------------------------------------------------------------
	}

	void CGameStateRun::MyWrite(string file, int c) {
		ofstream ofs;
		ofs.open(file);
		if (!ofs.is_open()) {
			GotoGameState(GAME_STATE_OVER);
		}
		ofs << c;
		ofs.close();
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		// 等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		MyWrite("flag.txt", 1);
		MyWrite("record.txt", 0);
		MyWrite("money.txt", 0);
		//MyWrite("champion.txt", 0);
		cheat = 0;
		s = 0;
		total_star = 145;
		total_enemy = 28;
		total_is_alive = 28;
		total_monster_up = 28;
		count_point = 0;
		BOUNCE_STATE = false;
		IS_MONSTERDIE = false;
		IS_FUNC = true;
		distance = 50;
		AUTO_JUMP = true;
		DRAG_STATE = false;
		CONTINUE_JUMP = true;
		JUMP_STATE = false;
		UP_STATE = false;
		ATTACH_STATE = false;
		max_hight = 300;
		bottom = 0;
		map_speed = 10;
		jump_speed = 25;
		dump_speed = 6;
		acceleration = 1;
		score_board.LoadBitmap(IDB_BITMAP63, RGB(0, 255, 0));
		point_board.LoadBitmap(IDB_BITMAP53, RGB(0, 255, 0));
		point_board.SetTopLeft(0, 50);
		// ====================================== enemy ======================================
		for (int i = 0; i < total_enemy; i++) {
			MONSTER_UP[i] = true;
		}
		for (int i = 0; i < total_enemy; i++) {
			if (i < 17) {
				for (int j = 329; j < 331; j++) {
					enemy[i].AddBitmap(j, RGB(255, 255, 255));
				}
			}
			if (i >= 17 && i < 21) {
				enemy[i].AddBitmap(IDB_BITMAP179, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP180, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP181, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP182, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP183, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP184, RGB(255, 255, 255));
				enemy[i].AddBitmap(IDB_BITMAP185, RGB(255, 255, 255));
			}
			if (i >= 21) {
				for (int j = 331; j < 335; j++) {
					enemy[i].AddBitmap(j, RGB(255, 0, 0));
				}
			}
		}
		for (int i = 0; i < total_enemy; i++) {
			if (i < 21) {
				enemy[i].SetDelayCount(2);
			}
			else {
				enemy[i].SetDelayCount(3);
			}
		}
		enemy[0].SetTopLeft(6800, 100); // 追尾型敵人
		enemy[1].SetTopLeft(8390, 170);
		enemy[2].SetTopLeft(10590, 90);
		enemy[3].SetTopLeft(18396, 100);
		enemy[4].SetTopLeft(26402, 100);
		enemy[5].SetTopLeft(30560, 150);
		enemy[6].SetTopLeft(29862, 80);
		enemy[7].SetTopLeft(32342, 170);
		enemy[8].SetTopLeft(34352, 220);
		enemy[9].SetTopLeft(37102, 150);

		enemy[10].SetTopLeft(14331, 125); //定點型敵人
		enemy[11].SetTopLeft(14580, 145);
		enemy[12].SetTopLeft(14821, 155);
		enemy[13].SetTopLeft(15121, 190);
		enemy[14].SetTopLeft(19950, 120);
		enemy[15].SetTopLeft(20230, 150);
		enemy[16].SetTopLeft(20800, 200);

		enemy[17].SetTopLeft(4450, 300); //前進型敵人
		enemy[18].SetTopLeft(9300, 270);
		enemy[19].SetTopLeft(19176, 280);
		enemy[20].SetTopLeft(36300, 280);

		enemy[21].SetTopLeft(16876, 300); //上下型敵人
		enemy[22].SetTopLeft(18116, 420);
		enemy[23].SetTopLeft(18402, 420);
		enemy[24].SetTopLeft(25871, 420);
		enemy[25].SetTopLeft(28017, 420);
		enemy[26].SetTopLeft(29191, 420);
		enemy[27].SetTopLeft(35267, 420);
		for (int i = 0; i < total_enemy; i++) {
			enemy[i].SetTopLeft(enemy[i].Left() - cheat, enemy[i].Top());
		}
		// ====================================== enemy ======================================
		for (int i = 0; i < total_is_alive; i++) {
			IS_ALIVE[i] = true;
		}
		for (int i = 0; i < total_star; i++) {
			if (i < 103) {
				cstar[i].LoadBitmap();
			}
			else {
				cstar[i].LoadBitmapStar();
			}
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
		background2.LoadBitmap(IDB_BITMAP167);
		background3.LoadBitmap(IDB_BITMAP169);
		ground.LoadBitmap(IDB_BITMAP163, RGB(255, 255, 255));
		ground2.LoadBitmap(IDB_BITMAP166, RGB(255, 255, 255));
		ground3.LoadBitmap(IDB_BITMAP168, RGB(255, 255, 255));
		attack.LoadBitmap(IDB_BITMAP160, RGB(255, 255, 255));
		//set map_score[][] loading picture
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 10; j++) {
				switch (j) {
				case 0:
					map_score[i][j].LoadBitmap(IDB_BITMAP189, RGB(0, 255, 0));
					break;
				case 1:
					map_score[i][j].LoadBitmap(IDB_BITMAP190, RGB(0, 255, 0));
					break;
				case 2:
					map_score[i][j].LoadBitmap(IDB_BITMAP191, RGB(0, 255, 0));
					break;
				case 3:
					map_score[i][j].LoadBitmap(IDB_BITMAP192, RGB(0, 255, 0));
					break;
				case 4:
					map_score[i][j].LoadBitmap(IDB_BITMAP193, RGB(0, 255, 0));
					break;
				case 5:
					map_score[i][j].LoadBitmap(IDB_BITMAP194, RGB(0, 255, 0));
					break;
				case 6:
					map_score[i][j].LoadBitmap(IDB_BITMAP195, RGB(0, 255, 0));
					break;
				case 7:
					map_score[i][j].LoadBitmap(IDB_BITMAP196, RGB(0, 255, 0));
					break;
				case 8:
					map_score[i][j].LoadBitmap(IDB_BITMAP197, RGB(0, 255, 0));
					break;
				case 9:
					map_score[i][j].LoadBitmap(IDB_BITMAP198, RGB(0, 255, 0));
					break;
				default:
					break;
				}
			}
		}
		//set score_point[][] loading picture
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				switch (j) {
				case 0:
					score_point[i][j].LoadBitmap(IDB_BITMAP189, RGB(0, 255, 0));
					break;
				case 1:
					score_point[i][j].LoadBitmap(IDB_BITMAP190, RGB(0, 255, 0));
					break;
				case 2:
					score_point[i][j].LoadBitmap(IDB_BITMAP191, RGB(0, 255, 0));
					break;
				case 3:
					score_point[i][j].LoadBitmap(IDB_BITMAP192, RGB(0, 255, 0));
					break;
				case 4:
					score_point[i][j].LoadBitmap(IDB_BITMAP193, RGB(0, 255, 0));
					break;
				case 5:
					score_point[i][j].LoadBitmap(IDB_BITMAP194, RGB(0, 255, 0));
					break;
				case 6:
					score_point[i][j].LoadBitmap(IDB_BITMAP195, RGB(0, 255, 0));
					break;
				case 7:
					score_point[i][j].LoadBitmap(IDB_BITMAP196, RGB(0, 255, 0));
					break;
				case 8:
					score_point[i][j].LoadBitmap(IDB_BITMAP197, RGB(0, 255, 0));
					break;
				case 9:
					score_point[i][j].LoadBitmap(IDB_BITMAP198, RGB(0, 255, 0));
					break;
				default:
					break;
				}
			}
		}
		for (int i = 0; i < 10; i++) { map_score[0][i].SetTopLeft(430, 50); }
		for (int i = 0; i < 10; i++) { map_score[1][i].SetTopLeft(400, 50); }
		for (int i = 0; i < 10; i++) { map_score[2][i].SetTopLeft(370, 50); }
		for (int i = 0; i < 10; i++) { map_score[3][i].SetTopLeft(340, 50); }
		for (int i = 0; i < 10; i++) { map_score[4][i].SetTopLeft(310, 50); }
		for (int i = 0; i < 10; i++) { score_point[0][i].SetTopLeft(120, 60); }
		for (int i = 0; i < 10; i++) { score_point[1][i].SetTopLeft(90, 60); }
		for (int i = 0; i < 10; i++) { score_point[2][i].SetTopLeft(60, 60); }
		for (int i = 0; i < 10; i++) { score_point[3][i].SetTopLeft(30, 60); }
		for (int i = 0; i < 6; i++) {
			player[i].SetTopLeft(distance, SIZE_Y - player[i].Height() - 40);
		}
		background.SetTopLeft(0 - cheat, 0);
		ground.SetTopLeft(0 - cheat, 0);
		background2.SetTopLeft(13176 - cheat, 0);
		ground2.SetTopLeft(13176 - cheat, 0);
		background3.SetTopLeft(23402 - cheat, 0);
		ground3.SetTopLeft(23402 - cheat, 0);
		// ---------------------------------------------------初始化POINT位置----------------------------------------------------------------
		//genaral point
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
		cstar[15].SetTopLeft(4685, 300);
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
		cstar[43].SetTopLeft(14281, 125);
		cstar[44].SetTopLeft(14421, 135);
		cstar[45].SetTopLeft(14600, 145);
		cstar[46].SetTopLeft(14701, 155);
		cstar[47].SetTopLeft(15416, 225);
		cstar[48].SetTopLeft(15696, 340);
		cstar[49].SetTopLeft(15796, 340);
		cstar[50].SetTopLeft(15896, 340);
		cstar[51].SetTopLeft(16021, 180);
		cstar[52].SetTopLeft(16021, 240);
		cstar[53].SetTopLeft(16021, 300);
		cstar[54].SetTopLeft(16176, 340);
		cstar[55].SetTopLeft(16296, 340);
		cstar[56].SetTopLeft(16701, 300);
		cstar[57].SetTopLeft(16751, 300);
		cstar[58].SetTopLeft(17046, 300);
		cstar[59].SetTopLeft(17101, 300);
		cstar[60].SetTopLeft(17496, 340);
		cstar[61].SetTopLeft(17836, 340);
		cstar[62].SetTopLeft(18671, 340);
		cstar[63].SetTopLeft(18721, 340);
		cstar[64].SetTopLeft(19256, 340);
		cstar[65].SetTopLeft(19306, 340);
		cstar[66].SetTopLeft(19906, 130);
		cstar[67].SetTopLeft(20046, 142);
		cstar[68].SetTopLeft(20186, 154);
		cstar[69].SetTopLeft(20326, 166);
		cstar[70].SetTopLeft(20466, 178);
		cstar[71].SetTopLeft(20606, 190);
		cstar[72].SetTopLeft(21086, 235);
		cstar[73].SetTopLeft(25420, 250);
		cstar[74].SetTopLeft(26357, 270);
		cstar[75].SetTopLeft(27102, 215);
		cstar[76].SetTopLeft(27102, 265);
		cstar[77].SetTopLeft(27102, 315);
		cstar[78].SetTopLeft(28202, 395);
		cstar[79].SetTopLeft(28242, 395);
		cstar[80].SetTopLeft(29002, 395);
		cstar[81].SetTopLeft(29042, 395);
		cstar[82].SetTopLeft(29852, 320);
		cstar[83].SetTopLeft(29902, 320);
		cstar[84].SetTopLeft(29952, 320);
		cstar[85].SetTopLeft(30532, 320);
		cstar[86].SetTopLeft(30582, 320);
		cstar[87].SetTopLeft(30632, 320);
		cstar[88].SetTopLeft(31472, 320);
		cstar[89].SetTopLeft(31522, 320);
		cstar[90].SetTopLeft(31572, 320);
		cstar[91].SetTopLeft(32057, 415);
		cstar[92].SetTopLeft(32297, 380);
		cstar[93].SetTopLeft(32522, 345);
		cstar[94].SetTopLeft(33652, 320);
		cstar[95].SetTopLeft(33852, 320);
		cstar[96].SetTopLeft(34052, 320);
		cstar[97].SetTopLeft(34252, 320);
		cstar[98].SetTopLeft(34452, 320);
		cstar[99].SetTopLeft(34652, 320);
		cstar[100].SetTopLeft(34852, 320);
		cstar[101].SetTopLeft(36027, 310);
		cstar[102].SetTopLeft(36527, 310);
		//special point
		cstar[103].SetTopLeft(660, 315);
		cstar[104].SetTopLeft(2035, 195);
		cstar[105].SetTopLeft(2970, 370);
		cstar[106].SetTopLeft(3045, 310);
		cstar[107].SetTopLeft(3125, 370);
		cstar[108].SetTopLeft(3800, 160);
		cstar[109].SetTopLeft(3800, 310);
		cstar[110].SetTopLeft(4685, 200);
		cstar[111].SetTopLeft(6220, 180);
		cstar[112].SetTopLeft(6295, 180);
		cstar[113].SetTopLeft(8010, 250);
		cstar[114].SetTopLeft(8620, 240);
		cstar[115].SetTopLeft(10600, 240);
		cstar[116].SetTopLeft(10660, 240);
		cstar[117].SetTopLeft(13980, 265);
		cstar[118].SetTopLeft(14500, 140);
		cstar[119].SetTopLeft(14961, 185);
		cstar[120].SetTopLeft(15326, 215);
		cstar[121].SetTopLeft(16701, 250);
		cstar[122].SetTopLeft(16751, 250);
		cstar[123].SetTopLeft(17046, 250);
		cstar[124].SetTopLeft(17101, 250);
		cstar[125].SetTopLeft(18270, 330);
		cstar[126].SetTopLeft(20116, 148);
		cstar[127].SetTopLeft(20701, 190);
		cstar[128].SetTopLeft(20946, 225);
		cstar[129].SetTopLeft(25380, 278);
		cstar[130].SetTopLeft(25420, 200);
		cstar[131].SetTopLeft(25455, 278);
		cstar[132].SetTopLeft(26082, 325);
		cstar[133].SetTopLeft(26697, 235);
		cstar[134].SetTopLeft(26697, 285);
		cstar[135].SetTopLeft(26902, 315);
		cstar[136].SetTopLeft(27262, 315);
		cstar[137].SetTopLeft(28202, 345);
		cstar[138].SetTopLeft(28242, 345);
		cstar[139].SetTopLeft(29002, 345);
		cstar[140].SetTopLeft(29042, 345);
		cstar[141].SetTopLeft(33952, 320);
		cstar[142].SetTopLeft(34352, 320);
		cstar[143].SetTopLeft(35139, 300);
		cstar[144].SetTopLeft(35416, 300);
		for (int i = 0; i < total_star; i++) {
			cstar[i].SetTopLeft(cstar[i].Left() - cheat, cstar[i].Top());
		}
		// ---------------------------------------------------初始化POINT位置----------------------------------------------------------------
		
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_SPACE = 0x20; // keyboard空白建
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		if (nChar == KEY_SPACE) {
			ATTACH_STATE = true;
			for (int i = 0; i < total_enemy; i++) {
				if (attack.Left() + attack.Width() >= enemy[i].Left() && attack.Left() <= enemy[i].Left() + enemy[i].Width() && attack.Top() + attack.Height() >= enemy[i].Top() && attack.Top() <= enemy[i].Top() + enemy[i].Height()) {
					if (MyRead("audio.txt") == 1) {
						CAudio::Instance()->Play(AUDIO_MONSTERDIE);
					}
					IS_MONSTERDIE = true;
					IS_ALIVE[i] = false;
					count_point += 100;
				}
				else {
					IS_MONSTERDIE = false;
				}
			}
			if (IS_MONSTERDIE == false) {
				if (MyRead("audio.txt") == 1) {
					CAudio::Instance()->Play(AUDIO_SLASH);
				}
			}
		}

		if (nChar == KEY_LEFT) {
			int temp = MyRead("record.txt");
			for (int i = 0; i < 9; i++) {
				if (temp >= record_point[i][0] && temp < record_point[i + 1][0]) {
					player[s].SetTopLeft(distance, record_point[i][1] - player[s].Height());
					for (int j = 0; j < total_star; j++) {
						cstar[j].SetTopLeft(cstar[j].Left() + abs(record_point[i][0] - abs(background.Left())), cstar[j].Top());
					}
					for (int j = 0; j < total_enemy; j++) {
						enemy[j].SetTopLeft(enemy[j].Left() + abs(record_point[i][0] - abs(background.Left())), enemy[j].Top());
					}
					background.SetTopLeft(0 - record_point[i][0], 0);
					ground.SetTopLeft(0 - record_point[i][0], 0);
					background2.SetTopLeft(13176 - record_point[i][0], 0);
					ground2.SetTopLeft(13176 - record_point[i][0], 0);
					background3.SetTopLeft(23402 - record_point[i][0], 0);
					ground3.SetTopLeft(23402 - record_point[i][0], 0);
					MyWrite("flag.txt", 0);
					break;
				}
			}
		}

		if (nChar == KEY_RIGHT) {
			int temp = MyRead("record.txt");
			for (int i = 0; i < 9; i++) {
				if (temp >= record_point[i][0] && temp < record_point[i + 1][0]) {
					player[s].SetTopLeft(distance, record_point[i+1][1] - player[s].Height());
					for (int j = 0; j < total_star; j++) {
						cstar[j].SetTopLeft(cstar[j].Left() - abs(record_point[i+1][0] - abs(background.Left())), cstar[j].Top());
					}
					for (int j = 0; j < total_enemy; j++) {
						enemy[j].SetTopLeft(enemy[j].Left() - abs(record_point[i+1][0] - abs(background.Left())), enemy[j].Top());
					}
					background.SetTopLeft(0 - record_point[i + 1][0], 0);
					ground.SetTopLeft(0 - record_point[i + 1][0], 0);
					background2.SetTopLeft(13176 - record_point[i + 1][0], 0);
					ground2.SetTopLeft(13176 - record_point[i + 1][0], 0);
					background3.SetTopLeft(23402 - record_point[i + 1][0], 0);
					ground3.SetTopLeft(23402 - record_point[i + 1][0], 0);
					MyWrite("flag.txt", 0);
					break;
				}
			}
		}

		if (nChar == KEY_UP) {
			if (MyRead("audio.txt") == 1) {
				CAudio::Instance()->Play(AUDIO_JUMP);
			}
			if (CONTINUE_JUMP == true) {
				if (JUMP_STATE == true) {
					max_hight = player[s].Top() - 140;
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

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		if (nChar == KEY_LEFT) {
			
		}
		if (nChar == KEY_RIGHT) {
			
		}
		if (nChar == KEY_UP) {
			
		}
		if (nChar == KEY_DOWN) {
			
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
		background3.ShowBitmap();
		ground3.ShowBitmap();
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
		//===========顯示ENEMY===========
		for (int i = 0; i < total_enemy; i++) {
			if (IS_ALIVE[i] == true) {
				enemy[i].OnShow();
			}
		}
		//===========顯示ENEMY===========
	}

	CGameStateEnd::CGameStateEnd(CGame *g) : CGameState(g)
	{
	}

	void CGameStateEnd::OnInit() {
		background.LoadBitmap(IDB_BITMAP188);
	}

	void CGameStateEnd::OnBeginState() {
		CAudio::Instance()->Stop(AUDIO_RUN);
	}

	void CGameStateEnd::OnMove() {
		background.SetTopLeft(0, 0);
	}

	void CGameStateEnd::OnShow() {
		background.ShowBitmap();
	}
}