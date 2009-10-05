/* NewbBlox - Debugging 
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 15:54:03 14 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <time.h>
#include <iostream>
#include <config.h>
#include <SDL/SDL.h>
#include <sys/time.h>
#include "debug.h"

using namespace std;

void Debug::output(string message, string type)
{
  time_t tim = time(NULL);
  tm *now=localtime(&tim);
  timeval smalltim;
  gettimeofday(&smalltim, NULL);
  printf("%02d:%02d:%02d.%d %s: %s\n", now->tm_hour, now->tm_min, now->tm_sec, (int)(((smalltim.tv_usec%1000000)/1000000.0)*1000000), type.c_str(), message.c_str());
}

void Debug::debug(string message)
{
#ifdef DEBUG
  output(message, "DEBUG");
#endif
}

void Debug::error(string message)
{
  output(message, "ERROR");
  SDL_Quit();
  throw 42;
}

void Debug::warning(string message)
{
#ifdef WARN
  output(message, "WARNING");
#endif
}
