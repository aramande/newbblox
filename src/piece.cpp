/* NewbBlox - Tetris Piece 
 *
 * Author: Aramande <aramande@hackular.com>
 *
 * Created: 17:19:34 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x
#define SURNULL static_cast<SDL_Surface *>(NULL)

#include <time.h>
#include <string>
#include <sstream>
#include <config.h>
#include <sys/stat.h> 
#include "random.h"
#include "piece.h"
#include "debug.h"

using namespace std;

std::vector<std::vector<std::vector<std::vector<SDL_Surface * > > > > Piece::tetrominos;

Piece::Piece()
{
  int rots;
  if(tetrominos.size() == 0){
    vector<vector<vector<vector<SDL_Surface * > > > > _tetrominos;
    _tetrominos.assign(8, vector<vector<vector<SDL_Surface * > > >());
    for(int i=1; i<8; i++){
      if(i%3 == 1) rots = 4; // T-block, L-Block, Reverse-L-Block
      else if(i%3 == 2 || i == 3) rots = 2; // Z-block, S-block, I-block
      else if(i%3 == 0 && i != 3) rots = 1; // Square-block
      //Order: L, Z, I, R-L, S, Sq, T 
      _tetrominos[i].assign(rots, vector<vector<SDL_Surface * > >());
      for(int j=0; j<rots; j++){
	_tetrominos[i][j].assign(4, vector<SDL_Surface * >());
	for(int k=0; k<4; k++){
	  _tetrominos[i][j][k].assign(4, static_cast<SDL_Surface *>(NULL));
	}
      }
    }
    _tetrominos[1][0][1][1] = _tetrominos[1][0][1][2] = _tetrominos[1][0][1][3] = _tetrominos[1][0][2][3] = 
      _tetrominos[1][1][1][3] = _tetrominos[1][1][1][2] = _tetrominos[1][1][2][2] = _tetrominos[1][1][3][2] = 
      _tetrominos[1][2][1][1] = _tetrominos[1][2][2][1] = _tetrominos[1][2][2][2] = _tetrominos[1][2][2][3] = 
      _tetrominos[1][3][1][2] = _tetrominos[1][3][2][2] = _tetrominos[1][3][3][2] = _tetrominos[1][3][3][1] = getBlock(1);
    
    _tetrominos[2][0][1][2] = _tetrominos[2][0][1][3] = _tetrominos[2][0][2][1] = _tetrominos[2][0][2][2] = 
      _tetrominos[2][1][0][1] = _tetrominos[2][1][1][1] = _tetrominos[2][1][1][2] = _tetrominos[2][1][2][2] = getBlock(2);

    _tetrominos[3][0][2][0] = _tetrominos[3][0][2][1] = _tetrominos[3][0][2][2] = _tetrominos[3][0][2][3] = 
      _tetrominos[3][1][0][2] = _tetrominos[3][1][1][2] = _tetrominos[3][1][2][2] = _tetrominos[3][1][3][2] = getBlock(3);
    
    _tetrominos[4][0][2][1] = _tetrominos[4][0][2][2] = _tetrominos[4][0][2][3] = _tetrominos[4][0][1][3] =
      _tetrominos[4][1][0][1] = _tetrominos[4][1][0][2] = _tetrominos[4][1][1][2] = _tetrominos[4][1][2][2] =  
      _tetrominos[4][2][1][3] = _tetrominos[4][2][1][2] = _tetrominos[4][2][1][1] = _tetrominos[4][2][2][1] =
      _tetrominos[4][3][0][2] = _tetrominos[4][3][1][2] = _tetrominos[4][3][2][2] = _tetrominos[4][3][2][3] = getBlock(4);

    _tetrominos[5][0][0][2] = _tetrominos[5][0][1][2] = _tetrominos[5][0][1][1] = _tetrominos[5][0][2][1] = 
      _tetrominos[5][1][2][2] = _tetrominos[5][1][2][3] = _tetrominos[5][1][1][2] = _tetrominos[5][1][1][1] = getBlock(5);

    _tetrominos[6][0][1][1] = _tetrominos[6][0][1][2] = _tetrominos[6][0][2][1] = _tetrominos[6][0][2][2] = getBlock(6);

    _tetrominos[7][0][1][2] = _tetrominos[7][0][2][2] = _tetrominos[7][0][3][2] = _tetrominos[7][0][2][3] =
      _tetrominos[7][1][1][2] = _tetrominos[7][1][2][2] = _tetrominos[7][1][2][1] = _tetrominos[7][1][2][3] =  
      _tetrominos[7][2][1][2] = _tetrominos[7][2][2][2] = _tetrominos[7][2][2][1] = _tetrominos[7][2][3][2] =
      _tetrominos[7][3][2][3] = _tetrominos[7][3][2][2] = _tetrominos[7][3][2][1] = _tetrominos[7][3][3][2] = getBlock(7);
    
      
    tetrominos = _tetrominos;
  }
  CRandom r; 
  r.Randomize(); 
  kind = r.Random(7)+1;
  r.Randomize();
  rotation = r.Random(4);
  /// Empty the vector
  piece.assign(4, vector<SDL_Surface *>());
  for(int i=0; i<4; i++)
    piece[i].assign(4, static_cast<SDL_Surface*>(NULL));

  Debug::debug("Making new Piece.");
  piece = tetrominos[kind][rotation % tetrominos[kind].size()];
}
Piece::~Piece(){}

/**
 * Gets the appropriate image for the block. The color ranges from 1 to and including 7.
 * 
 * @param  nr  Color value. 1 is red, 2 is blue, ... //fill out with numbers
 *
 * @return  An SDL surface to blit if successful.
 */
SDL_Surface* Piece::getBlock(int nr)
{
  if(nr < 1 || nr > 7) Debug::error("Block initialized out of range. Range is 1-7.");
  string datadir = "";
  string filename = "";
#ifdef DATADIR
  Debug::debug("DATADIR was found and has value: " + static_cast<string>(STRINGIFY(DATADIR)));
  datadir = static_cast<string>(STRINGIFY(DATADIR));
#else
    Debug::error("The global variable DATADIR has not been initialized, did you forget to do `make install`?");
#endif
  struct stat stFileInfo;
  int intStat; 

  char str [50];

  sprintf(str, "/images/%d.png", nr);
  filename = str;
  filename = (datadir + filename).c_str();

  Debug::debug("Looking up image file '"+filename+"'.");
  intStat = stat(filename.c_str(),&stFileInfo);
  if(intStat != 0) Debug::error("Could not find file: "+(string)filename+".");
  else Debug::debug("'"+filename+"' was found.");

  return IMG_Load(filename.c_str());
}

vector<vector<SDL_Surface *> > Piece::getPiece()
{
  return piece;
}

/**
 * Turns the piece
 */
void Piece::turn(bool clockwise)
{
  if(clockwise) ++rotation;
  else --rotation;
  if(rotation == -1) rotation = 3;
  piece = tetrominos[kind][rotation % tetrominos[kind].size()];
  Debug::debug("Rotating piece to state " + to_string(rotation % tetrominos[kind].size()));
}
