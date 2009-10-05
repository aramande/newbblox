/* NewbBlox - Block Sprite
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 01:27:08 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

#include <sstream>
#include <config.h>
#include <iostream>
#include <sys/stat.h> 
#include "block.h"
#include "debug.h"


using namespace std;

Block::Block(int nr)
{
  string datadir = "";
#ifdef DATADIR
  Debug::debug("DATADIR was found and has value: " + (string)STRINGIFY(DATADIR));
  datadir = (string)STRINGIFY(DATADIR);
#endif
  struct stat stFileInfo;
  int intStat; 

  char str [50];
  sprintf(str, "data/images/%d.bmp", nr);
  filename = str; // Set path to image used

  Debug::debug("Looking up image file '"+(string)filename+"'.");
  intStat = stat(filename,&stFileInfo);
  if(intStat != 0) 
  {
    Debug::warning("Could not find file: "+(string)filename+".");
    sprintf(str, "/images/%d.bmp", nr);
    filename = str;
    filename = (datadir + (string)filename).c_str();
    Debug::debug("Looking up image file '"+(string)filename+"' instead.");
    intStat = stat(filename,&stFileInfo);
    if(intStat != 0) Debug::error("Could not find file: "+(string)filename+".");
    else Debug::debug("'"+(string)filename+"' was found.");
  }else Debug::debug("'"+(string)filename+"' was found.");  
}
Block::Block(){}
Block::~Block(){}
SDL_Surface* Block::getBlock()
{
  return IMG_Load(filename);
}
