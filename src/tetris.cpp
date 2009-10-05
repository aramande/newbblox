/* NewbBlox - Grafical Tetris Interface 
 *
 * Author: Aramande <aramande@hackular.com>
 *
 * Created: 16:57:03 13 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x
#define FPS

#include <sstream>
#include <config.h>
#include <sys/stat.h>
#include "tetris.h"
#include "debug.h"
#include "timer.h"

using namespace std;

//The size of the screen 
const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 480; 
const int SCREEN_BPP = 32; 
const int FRAMES_PER_SECOND = 15;

Tetris::Tetris()
{
  Debug::debug("Initializing SDL screen.");
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
    Debug::error("SDL could not initialize.");
  } 
  if( TTF_Init() == -1 ) { 
    Debug::error("SDL_ttf could not initialize.");
  } 
  SDL_WM_SetCaption("NewbBlox", NULL);
  drawBackground();
  if(SDL_Flip(screen) == -1){
    Debug::error("Could not flip screen.");
  }
  matrix.assign(11, vector<SDL_Surface *>());
  for(int i=0; i<11; i++)
    matrix[i].assign(19, static_cast<SDL_Surface*>(NULL));

  points=0;
  level=1;
  lines=5;
}
Tetris::~Tetris(){}

/**
 * This starts the whole game. 
 */
void Tetris::run()
{
  frames = 0; 

  Timer fps; 
  Timer update; 
  update.start(); 
  fps.start(); 
  x=4;
  y=-4;
  SDL_Event event;
  bool quit = false;
  if(SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL+1) == -1){
    Debug::warning("Could not set KeyRepeat to enabled.");
  }
  while(!quit) { 
    piece = new Piece();
    do{
      if(SDL_PollEvent(&event)){
	if(event.type == SDL_QUIT){
	  quit = true;
	  return;
	}
	if(event.type == SDL_KEYDOWN){
	  switch(event.key.keysym.sym){
	  case SDLK_UP:
	    clearPiece();
	    rotatePiece();
	    break; 
	  case SDLK_DOWN: 
	    if(!isColliding(0,1)){
	      clearPiece();
	      movePiece(2);
	    }
	    break; 
	  case SDLK_LEFT: 
	    if(!isColliding(-1,0)){
	      clearPiece();
	      movePiece(0);
	    }
	    break; 
	  case SDLK_RIGHT: 
	    if(!isColliding(1,0)){
	      clearPiece();
	      movePiece(1);
	    }
	    break; 
	  case SDLK_SPACE:
	    clearPiece();
	    movePiece(3);
	  }
	  //drawBackground();	
	  //drawBoard();
	  if(!isColliding(0,1) && frames % 60-(level*2) == 0){
	    clearPiece();
	    movePiece(2); // 2 for down
	  }
	  drawPiece();
	  if(SDL_Flip(screen) == -1){
	    Debug::warning("Could not flip screen.");
	  }
	}
      }
      if(frames % 60-(level*2) == 0){
	if(!isColliding(0,1)){
	  clearPiece();
	  movePiece(2); // 2 for down
	}
	else{
	  break;
	}
	drawPiece();
	if(SDL_Flip(screen) == -1){
	  Debug::warning("Could not flip screen.");
	}
      }
#ifdef FPS
      if(update.get_ticks() > 1000){
	stringstream caption; 
	caption << "NewbBlox - Avg FPS: " << frames / ( fps.get_ticks() / 1000.f );
	SDL_WM_SetCaption( caption.str().c_str(), NULL ); 
	update.start(); 
      }
#endif
      frames++;
      int currTime = SDL_GetTicks();
      int timeElapsed = currTime - prevTime;
      if(timeElapsed < FRAMES_PER_SECOND)
	{
	  // Not enough time has elapsed. Let's limit the frame rate
	  SDL_Delay(FRAMES_PER_SECOND - timeElapsed);
	  currTime = SDL_GetTicks();
	  timeElapsed = currTime - prevTime;
	}
      prevTime = currTime;
      // SDL_Delay((frames/(fps.get_ticks()/200.f)));
    }while(!quit);
    if(!storePiece()){
      drawPiece();
      if(SDL_Flip(screen) == -1){
	Debug::warning("Could not flip screen.");
      }
      SDL_WM_SetCaption("GAME OVER", NULL);
      SDL_Delay(10000);
      quit = true;
    }
    x = 4;
    y = -4;
  }
}

void Tetris::clearScoreboard()
{
  SDL_Rect rect;
  rect.x = 307; 
  rect.y = 157;
  rect.h = 90;
  rect.w = 100;
  SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0xED, 0xED, 0xED ) );
}

void Tetris::clearPiece()
{
  SDL_Rect rect;

  for(int i=0; i < piece->getPiece().size(); i++){
    for(int j=0; j < piece->getPiece()[i].size(); j++){
      if(piece->getPiece()[i][j]){
	rect.x = (i+x)*24;
	rect.y = (j+y)*24;
	rect.w = 1*24;
	rect.h = 1*24;
	SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
      }
    }
  }
}

void Tetris::clearBoard()
{
  SDL_Rect rect;

  for(int i=0; i < matrix.size(); i++){
    for(int j=0; j < matrix[i].size(); j++){
      if(matrix[i][j]){
	rect.x = i*24;
	rect.y = j*24;
	rect.w = 1*24;
	rect.h = 1*24;
	SDL_FillRect( screen, &rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
      }
    }
  }
}

/**
 * Moves piece in the direction decided by the argument.
 * 
 * @param  direction  0 for left, 1 for right, 2 for down and 3 for drop
 */
void Tetris::movePiece(int direction){
  switch(direction){
  case 0:
    --x;
    break;
  case 1:
    ++x;
    break;
  case 2:
    ++y;
    break;
  case 3:
    while(!isColliding(0,1)) ++y; 
    break;
  default:
    break;
  }
}

/**
 * Rotates the piece and checks for collision, if collision occured, don't rotate.
 */
void Tetris::rotatePiece()
{
  piece->turn(true);
  if(isColliding(0,0)){
    piece->turn(false);
  }
}

/** 
 * Drawing the background on the window.
 */
void Tetris::drawBackground()
{
  if(filename == ""){
    string _filename = "";
    string datadir = "";
#ifdef DATADIR
    Debug::debug("DATADIR was found and has value: " + static_cast<string>(STRINGIFY(DATADIR)));
    datadir = static_cast<string>(STRINGIFY(DATADIR));
#else
    Debug::error("The global variable DATADIR has not been initialized, did you forget to do `make install`?");
#endif
    struct stat stFileInfo;
    int intStat; 
    
    _filename = "/images/background.png";
    _filename = (datadir + _filename).c_str();
    Debug::debug("Looking up image file '"+_filename+"' instead.");
    intStat = stat(_filename.c_str(),&stFileInfo);
    
    if(intStat != 0) Debug::error("Could not find file: "+_filename+".");
    else Debug::debug("'"+_filename+"' was found.");
    filename = _filename;
  }
  Debug::debug("Drawing background");
  SDL_Rect offset; 
  offset.x = 0; 
  offset.y = 0;
  SDL_BlitSurface(IMG_Load(filename.c_str()), NULL, screen, &offset);
}

/** 
 * Drawing all the pieces on the board stored in matrix.
 */
void Tetris::drawBoard()
{
  for(int i=0; i < matrix.size(); i++){
    for(int j=0; j < matrix[i].size(); j++){
      if(matrix[i][j]){
	SDL_Rect offset; 
	offset.x = i*24; 
	offset.y = j*24;
	
	Debug::debug("Drawing block "+to_string(i)+","+to_string(j)+" at "+to_string(offset.x)+","+to_string(offset.y));
	SDL_BlitSurface(matrix[i][j], NULL, screen, &offset);
      }
    }
  }
}

/** 
 * Drawing the piece on the board, starting with the top left corner (0,0).
 * 
 * @param  x  X coordinate of the piece's upper left corner relative to the matrix
 * @param  y  Y coordinate of the piece's upper left corner relative to the matrix
 */
void Tetris::drawPiece()
{
  for(int i=0; i < piece->getPiece().size(); i++){
    for(int j=0; j < piece->getPiece()[i].size(); j++){
      if(piece->getPiece()[i][j]){
	SDL_Rect offset; 
	offset.x = (i+x)*24; 
	offset.y = (j+y)*24;

	Debug::debug("Drawing block "+to_string(i)+","+to_string(j)+" at "+to_string(offset.x)+","+to_string(offset.y));
	SDL_BlitSurface(piece->getPiece()[i][j], NULL, screen, &offset);
      }
    }
  }
}

/** 
 * Drawing the Points and other stuff on the scoreboard.
 * 
 * @param  
 * @param  
 */
void Tetris::drawText(string text, int height_offset){
  if(fontname == ""){
    string _fontname = "";
    string datadir = "";
#ifdef DATADIR
    Debug::debug("DATADIR was found and has value: " + static_cast<string>(STRINGIFY(DATADIR)));
    datadir = static_cast<string>(STRINGIFY(DATADIR));
#else
    Debug::error("The global variable DATADIR has not been initialized, did you forget to do `make install`?");
#endif
    struct stat stFileInfo;
    int intStat; 
    
    _fontname = "/fonts/tahoma.ttf";
    _fontname = (datadir + _fontname).c_str();
    Debug::debug("Looking up image file '"+_fontname+"'.");
    intStat = stat(_fontname.c_str(),&stFileInfo);
    
    if(intStat != 0) Debug::error("Could not find file: "+_fontname+".");
    else Debug::debug("'"+_fontname+"' was found.");
    fontname = _fontname;
    font = TTF_OpenFont( fontname.c_str(), 16 );
  }
  if(font == NULL) Debug::warning("Failed to load the font: "+to_string(fontname.c_str()));
  SDL_Rect offset; 
  offset.x = 308; 
  offset.y = 158 + height_offset;
  SDL_Color textColor = {0, 0, 0};
  message = TTF_RenderText_Solid( font, text.c_str(), textColor );
  if(message == NULL) Debug::warning("Unknown error with the text renderer.");
  SDL_BlitSurface(message, NULL, screen, &offset);  
}

/**
 * Calculates if any block of the piece collides with a stored block in the matrix.
 * And also checks for collision with ground and walls.
 *
 * @return  false if no collision is detected
 */
bool Tetris::isColliding(int offset_x, int offset_y)
{
  bool ret = false;
  for(int i=0; i<piece->getPiece().size(); i++){
    for(int j=0; j<piece->getPiece()[i].size(); j++){
      if(piece->getPiece()[i][j]){
	if(j+y+offset_y >= 19) ret = true;
	if(i+x+offset_x >= 11) ret = true;
	if(i+x+offset_x < 1) ret = true;
	for(int k=0; k<matrix.size(); k++){
	  for(int l=0; l<matrix[k].size(); l++){
	    if(matrix[k][l]){
	      if(i+x+offset_x == k && j+y+offset_y == l){
		return true;
	      }
	    }
	  }
	}
	if(ret) return ret;
      }
    }
  }
  return ret;
}

/** 
 * Storing piece in the matrix, starting with the top left corner (0,0).
 * 
 * @param  x  X coordinate of the piece's upper left corner relative to the matrix
 * @param  y  Y coordinate of the piece's upper left corner relative to the matrix
 */
bool Tetris::storePiece()
{
  for(int i=0; i<piece->getPiece().size(); i++){
    for(int j=0; j<piece->getPiece()[i].size(); j++){
      if(piece->getPiece()[i][j]){
	if(j+y < 0) return false;
	Debug::debug("Block stored at x:"+to_string(i+x)+" y:"+to_string(j+y));
	matrix[i+x][j+y] = piece->getPiece()[i][j];
      }
    }
  }
  points += 5*level;
  Debug::debug("Piece was stored.");
  clearBoard();
  for (int j = 0; j < 19; j++){
    int i = 1;  
    while (i < 11){  
      if (!matrix[i][j]) break;  
      i++;  
    }  
    
    if (i == 11){
      // Moves all the upper lines one row down  
      for (int pY = j; pY > 0; pY--){  
	for (int pX = 1; pX < 11; pX++){  
	  matrix[pX][pY] = matrix[pX][pY-1];  
	}  
      }
      lines--;
      if(lines <= 0){
	level++; 
	lines += level + 4;
      }
      points += 150*level;
    }
  }
  drawBoard();
  clearScoreboard();
  drawText("Points: "+to_string(points), 0);
  drawText("Level: "+to_string(level), 30);
  drawText("Lines: "+to_string(lines), 60);

  return true;
}
