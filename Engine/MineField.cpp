#include "MineField.h"
#include <assert.h>
#include <random>
#include <algorithm>

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
			SpriteCodex::DrawTileChoose(screenPos, adjMineNum, gfx);
		else
			SpriteCodex::DrawTileBombRed(screenPos, gfx);
		break;
	case State::BombWin:
		SpriteCodex::DrawTileBomb(screenPos, gfx);
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

void MineField::Tile::SetMineCount(int mineCount) {
	assert(adjMineNum == -1);
	adjMineNum = mineCount;
}

int MineField::Tile::GetMineCount() const {
	return adjMineNum;
}

void MineField::Tile::SetBombWin() {
	state = State::BombWin;
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
	for (Vec2 gPos = { 0,0 }; gPos.y < height; gPos.y++) {
		for (gPos.x = 0; gPos.x < width; gPos.x++)
			TileAtPos(gPos).SetMineCount(CountAdjMine(gPos));
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
	if (TileAtPos(gridPos).GetState() == Tile::State::Hidden) {
		TileAtPos(gridPos).Open();
		if (TileAtPos(gridPos).GetMineCount() == 0) {
			const int xMin = std::max(0, gridPos.x - 1);
			const int xMax = std::min(width - 1, gridPos.x + 1);
			const int yMin = std::max(0, gridPos.y - 1);
			const int yMax = std::min(height - 1, gridPos.y + 1);
			for (Vec2 pos = { xMin,yMin }; pos.y <= yMax; pos.y++) {
				for (pos.x = xMin; pos.x <= xMax; pos.x++) {
					if (pos != gridPos)
						OpenTile(pos);
				}
			}
		}
	}
}

void MineField::FlagTile(const Vec2& gridPos) {
	if (TileAtPos(gridPos).GetState() == Tile::State::Hidden)
		TileAtPos(gridPos).Flag();
	else if (TileAtPos(gridPos).GetState() == Tile::State::Flag)
		TileAtPos(gridPos).Flag();
}

int MineField::CountAdjMine(const Vec2& gridPos) {
	const int xMin = std::max(0, gridPos.x - 1);
	const int xMax = std::min(width - 1, gridPos.x + 1);
	const int yMin = std::max(0, gridPos.y - 1);
	const int yMax = std::min(height - 1, gridPos.y + 1);

	int count = 0;
	for (Vec2 pos = { xMin,yMin }; pos.y <= yMax; pos.y++) {
		for (pos.x = xMin; pos.x <= xMax; pos.x++) {
			if (TileAtPos(pos).CheckMine())
				count++;
		}
	}
	return count;
}

bool MineField::CheckWon(int totalMineNum) const {
	int countFlaggedMines = 0;
	int countOpened = 0;
	int countFlagged = 0;
	for (const Tile& tiles : field) {
		if (tiles.Flagged() && tiles.CheckMine())
			countFlaggedMines++;
		if (tiles.Flagged())
			countFlagged++;
		if (tiles.Opened())
			countOpened++;
	}
	if ((countFlaggedMines == totalMineNum && countFlagged == totalMineNum) || countOpened == width * height - totalMineNum)
		return true;
	else
		return false;
}

bool MineField::CheckLost() const {
	for (const Tile& tiles : field) {
		if (tiles.CheckMine() && tiles.Opened())
			return true;
	}
	return false;
}

void MineField::RevealBombsLost() {
	for (int i = 0; i < width * height; i++) {
		if (field[i].Flagged() && field[i].CheckMine())
			field[i].Flag();
		if (field[i].CheckMine() && !field[i].Opened())
			field[i].Open();
	}
}

void MineField::RevealBombsWon() {
	for (int i = 0; i < width * height; i++) {
		if (field[i].Flagged())
			field[i].Flag();
		if (field[i].CheckMine())
			field[i].SetBombWin();
	}
}

void MineField::DrawCross(Graphics& gfx) {
	for (int i = 0; i < width * height; i++) {
		if (field[i].Flagged() && !field[i].CheckMine())
			SpriteCodex::DrawTileCross(Vec2(i % width, i / width) * SpriteCodex::tileSize + TL, gfx);
	}
}

MineField::Tile& MineField::TileAtPos(const Vec2& pos) {
	return field[pos.y * width + pos.x];
}

const MineField::Tile& MineField::TileAtPos(const Vec2& pos) const {
	return field[pos.y * width + pos.x];
}