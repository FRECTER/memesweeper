#include "MineField.h"
#include <assert.h>
#include <random>

void MineField::Tile::SpawnMine() {
	assert(!hasMine);
	hasMine = true;
}

bool MineField::Tile::CheckMine() const {
	return hasMine;
}

MineField::MineField(int mineNum) {
	assert(mineNum > 0 && mineNum < width * height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);
	for (int i = 0; i < mineNum; i++) {
		Vec2 spawnPos;
		do
			spawnPos = { xDist(rng),yDist(rng) };
		while (TileAtPos(spawnPos).CheckMine());
		TileAtPos(spawnPos).SpawnMine();
	}
}

MineField::Tile& MineField::TileAtPos(const Vec2& pos) {
	return field[pos.y * width + pos.x];
}