#pragma once
#include "Vec2.h"

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
	private:
		State state = State::Hidden;
		bool hasMine = false;
	};
public:
	MineField(int mineNum);
private:
	Tile& TileAtPos(const Vec2& pos);
private:
	static constexpr int width = 10;
	static constexpr int height = 10;
	Tile field[width * height];
};