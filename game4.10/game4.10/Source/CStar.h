#pragma once


namespace game_framework {
	class CStar
	{
		private:
			CMovingBitmap star;
			bool IS_SHOW;
		public:
			bool get_IS_Show();
			void set_IS_Show(bool);
			void LoadBitmap();
			void onMove(int);
			void SetTopLeft(int, int);
			void ShowBitmap();
			int Left();
			int Height();
			int Width();
			int Top();
	};
}

