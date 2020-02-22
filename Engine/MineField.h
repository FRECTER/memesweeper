#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class MineField {
private:
	class Tile {
	public:
		enum class State {
			Hidden,
			Flag,
			Open,
			BombWin
		};
	public:
		void SpawnMine();
		bool CheckMine() const;
		void Draw(const Vec2& screenPos, Graphics& gfx) const;
		State GetState() const {
			return state;
		}
		void Open();
		bool Opened() const;
		void Flag();
		bool Flagged() const;
		void SetMineCount(int mineCount);
		int GetMineCount() const;
		void SetBombWin();
	private:
		State state = State::Hidden;
		bool hasMine = false;
		int adjMineNum = -1;
	};
public:
	MineField(int mineNum);
	void Draw(Graphics& gfx) const;
	Rect BoardRect() const;
	int GetWidth() const {
		return width;
	}
	int GetHeight() const {
		return height;
	}
	void OpenTile(const Vec2& gridPos);
	void FlagTile(const Vec2& gridPos);
	int CountAdjMine(const Vec2& gridPos);
	bool CheckWon(int TotalMineNum) const;
	bool CheckLost() const;
	void RevealBombsLost();
	void RevealBombsWon();
	void DrawCross(Graphics& gfx);
private:
	Tile& TileAtPos(const Vec2& pos);
	const Tile& TileAtPos(const Vec2& pos) const;
private:
	static constexpr int width = 30;
	static constexpr int height = 30;
	Vec2 TL = Vec2((Graphics::ScreenWidth - width * SpriteCodex::tileSize) / 2,
				   (Graphics::ScreenHeight - height * SpriteCodex::tileSize) / 2);
	Tile field[width * height];
};