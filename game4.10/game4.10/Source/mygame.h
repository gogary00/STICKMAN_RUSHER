/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "CStar.h"
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_OPEN,				
		AUDIO_SELECT,		
		AUDIO_RUN,			
		AUDIO_DEAD,
		AUDIO_EAT_POINT,
		AUDIO_EAT_STAR,
		AUDIO_SLASH,
		AUDIO_MONSTERDIE,
		AUDIO_JUMP,
		AUDIO_OVER
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void MyWrite(string, int);
		int MyRead(string);
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int swAudio;
		CMovingBitmap logo1, logo2;
		CMovingBitmap btnAudio_open,btnAudio_close,btnStartGame;
		CMovingBitmap green_effect;
		CMovingBitmap character_body, character_eye,character_sword,character_wing_left, character_wing_right,character_scarf1,character_scarf2;
	};
	/////////////////////////////////////////////////////////////////////////////

	class CGameStatePrepare : public CGameState {
	public:
		CGameStatePrepare(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void MyWrite(string, int);
		int MyRead(string);
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int swAudio;
		CMovingBitmap background;
		CMovingBitmap btnStartGame;
		CMovingBitmap btnAudio_open, btnAudio_close;
	};
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateSelect : public CGameState {
	public:
		CGameStateSelect(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyDown(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void sMyWrite(string, int);
		int sMyRead(string);
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int swAudio,select;
		bool IS_SHOW[6];
		CMovingBitmap background;
		CMovingBitmap btnStartGame;
		CMovingBitmap btnAudio_open, btnAudio_close;
		CMovingBitmap upgrade[6],block[6],frame, money[5][10];
		CAnimation player[6];
	};

	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void MyWrite(string, int);
		int MyRead(string);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:

		int max_hight, s, map_speed, jump_speed, dump_speed, acceleration, translating, bottom, distance, total_star,total_enemy, total_is_alive, total_monster_up, count_point, current, choice[5], cheat, record_point[10][2] = { {0,415},{4800,415},{7540,415},{13401,380},{17416,390},{21714,390},{24002,380},{26892,380},{29802,375},{35852,375} };
		bool JUMP_STATE,UP_STATE,ATTACH_STATE,CONTINUE_JUMP,IS_FUNC,AUTO_JUMP,DRAG_STATE,BOUNCE_STATE,IS_ALIVE[28],IS_MONSTERDIE,MONSTER_UP[28];
		CAnimation player[6],enemy[28];

		CMovingBitmap background,background2, background3,ground,ground2, ground3,attack,map_score[5][10],score_board,score_point[4][10],point_board;
		CStar cstar[145];
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void MyWrite(string, int);
		int MyRead(string);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
		CMovingBitmap background, btnAgain, map_score[5][10], max_score[5][10];
	};

	class CGameStateEnd : public CGameState {
	public:
		CGameStateEnd(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();	
	protected:
		void OnMove();
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap background;
	};
}