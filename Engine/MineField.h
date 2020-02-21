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
			Open
		};
	public:
		void SpawnMine();
		bool CheckMine() const;
		void Draw(const Vec2& screenPos, Graphics& gfx) const;
	private:
		State state = State::Hidden;
		bool hasMine = false;
	};
public:
	MineField(int mineNum);
	void Draw(Graphics& gfx) const;
	Rect BoardRect() const;
private:
	Tile& TileAtPos(const Vec2& pos);
	const Tile& TileAtPos(const Vec2& pos) const;
private:
	static constexpr int width = 10;
	static constexpr int height = 10;
	Vec2 TL = Vec2((Graphics::ScreenWidth - width * SpriteCodex::tileSize) / 2,
				   (Graphics::ScreenHeight - height * SpriteCodex::tileSize) / 2);
	Tile field[width * height];
};