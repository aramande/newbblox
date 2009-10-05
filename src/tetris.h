/* NewbBlox - Grafical Tetris Interface
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 16:15:32 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "piece.h"

#ifndef TETRIS_H
#define TETRIS_H

class Tetris
{
public:
  Tetris();
  ~Tetris();
  void movePiece(int direction);
  void rotatePiece();
  void run();
private:
  SDL_Surface *background; 
  std::vector<std::vector<SDL_Surface *> > matrix;
  SDL_Surface *screen;
  SDL_Surface *message;
  TTF_Font *font;
  std::string filename, fontname;
  Piece *piece;
  int x, y;
  int frames, prevTime;
  int points, level, lines;

  void clearBoard();
  void clearPiece();
  void clearScoreboard();
  void drawBackground();
  void drawBoard();
  void drawPiece();
  void drawText(std::string, int height_offset);
  bool isColliding(int offset_x, int offset_y);
  bool storePiece();
};

#endif /* TETRIS_H */
