/* NewbBlox - Tetris Piece 
 *
 * Author: Aramande <aramande@hackular.com>
 *
 * Created: 17:09:50 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <vector>

#ifndef PIECE_H
#define PIECE_H

class Piece
{
private:
  static std::vector<std::vector<std::vector<std::vector<SDL_Surface * > > > > tetrominos;
  std::vector<std::vector<SDL_Surface *> > piece;
  int kind;
  int rotation;
  SDL_Surface* getBlock(int nr);
public:
  Piece();
  ~Piece();
  std::vector<std::vector<SDL_Surface *> > getPiece();
  void turn(bool clockwise);
};

#endif /* PIECE_H */
