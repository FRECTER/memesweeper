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

void MineField::Tile::Draw(const Vec2& screenPos, Graphics& gfx) const {
	switch (state) {
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flag:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Open:
		if (!hasMine)
			SpriteCodex::DrawTile0(screenPos, gfx);
		else
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		break;
	}
}

void MineField::Tile::Open() {
	assert(state == State::Hidden);
	state = State::Open;
}

bool MineField::Tile::Opened() const {
	return state == State::Open;
}

void MineField::Tile::Flag() {
	assert(!Opened());
	if (state == State::Flag)
		state = State::Hidden;
	else
		state = State::Flag;
}

bool MineField::Tile::Flagged() const {
	return state == State::Flag;
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

void MineField::Draw(Graphics& gfx) const {
	gfx.DrawRect(BoardRect(), SpriteCodex::baseColor);
	for (Vec2 gPos = { 0,0 }; gPos.y < height; gPos.y++) {
		for (gPos.x = 0; gPos.x < width; gPos.x++)
			TileAtPos(gPos).Draw(gPos * SpriteCodex::tileSize + TL, gfx);
	}
}

Rect MineField::BoardRect() const {
	return Rect(TL.x, TL.x + width * SpriteCodex::tileSize, TL.y, TL.y + height * SpriteCodex::tileSize);
}

void MineField::OpenTile(const Vec2& gridPos) {
	if (TileAtPos(gridPos).GetState() == Tile::State::Hidden)
		TileAtPos(gridPos).Open();
}

void MineField::FlagTile(const Vec2& gridPos) {
	if (TileAtPos(gridPos).GetState() == Tile::State::Hidden)
		TileAtPos(gridPos).Flag();
	else if (TileAtPos(gridPos).GetState() == Tile::State::Flag)
		TileAtPos(gridPos).Flag();
}

MineField::Tile& MineField::TileAtPos(const Vec2& pos) {
	return field[pos.y * width + pos.x];
}

const MineField::Tile& MineField::TileAtPos(const Vec2& pos) const {
	return field[pos.y * width + pos.x];
}