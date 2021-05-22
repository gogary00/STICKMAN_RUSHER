#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CStar.h"

namespace game_framework {
	bool CStar::get_IS_Show() {
		return IS_SHOW;
	}

	void CStar::set_IS_Show(bool b) {
		IS_SHOW = b;
	}

	void CStar::LoadBitmap() {
		star.LoadBitmap(IDB_BITMAP30, RGB(0, 255, 0));
	}

	void CStar::onMove(int speed) {
		star.SetTopLeft(star.Left() - speed, star.Top());
	}

	void CStar::SetTopLeft(int x, int y) {
		star.SetTopLeft(x, y);
	}

	void CStar::ShowBitmap() {
		star.ShowBitmap();
	}

	int CStar::Left() {
		return star.Left();
	}

	int CStar::Height() {
		return star.Height();
	}

	int CStar::Width() {
		return star.Width();
	}
	int CStar::Top() {
		return star.Top();
	}

}
