/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	field(totalMineNum)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel() {
	gameWon = field.CheckWon(totalMineNum);
	gameLost = field.CheckLost();
	if (!gameWon && !gameLost) {
		while (!wnd.mouse.IsEmpty()) {
			const Mouse::Event e = wnd.mouse.Read();
			if (e.GetType() == Mouse::Event::Type::LPress) {
				Vec2 click(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
				const Rect rect = field.BoardRect();
				if (click.x >= rect.left && click.x < rect.right + field.GetWidth() * SpriteCodex::tileSize &&
					click.y >= rect.top && click.y < rect.bottom + field.GetHeight() * SpriteCodex::tileSize)
					field.OpenTile((click - Vec2(rect.left, rect.top)) / SpriteCodex::tileSize);
			}
			else if (e.GetType() == Mouse::Event::Type::RPress) {
				Vec2 click(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
				const Rect rect = field.BoardRect();
				if (click.x >= rect.left && click.x < rect.right + field.GetWidth() * SpriteCodex::tileSize &&
					click.y >= rect.top && click.y < rect.bottom + field.GetHeight() * SpriteCodex::tileSize)
					field.FlagTile((click - Vec2(rect.left, rect.top)) / SpriteCodex::tileSize);
			}
		}
	}
	else if (gameWon)
		field.RevealBombsWon();
	else if (gameLost)
		field.RevealBombsLost();
}

void Game::ComposeFrame() {
	field.Draw(gfx);
	if (gameLost)
		field.DrawCross(gfx);
}
