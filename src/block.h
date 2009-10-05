/* NewbBlox - Block Sprite
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 01:21:24 13 May 2009
 * 
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef BLOCK_H
#define BLOCK_H

class Block : public SDL_Surface
{
private:
  const char *filename;
public:
  Block(int nr);
  Block();
  ~Block();
  SDL_Surface* getBlock();
};

#endif /* BLOCK_H */
