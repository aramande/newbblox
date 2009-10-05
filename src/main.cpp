/* NewbBlox - Main
 *
 * Author: Aramande <aramande@gmail.com>
 * Created: 16:48:49 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <config.h>
#include <iostream>
#include "tetris.h"
#include "debug.h"

using namespace std;

int main (int argc, char **argv)
{
  try{
    Debug::debug("Loading game.");
    Tetris *game = new Tetris();
    Debug::debug("Running game.");
    game->run();
  }catch(int error){
    return 0;
  }
  return 0;
}

