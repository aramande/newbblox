/* NewbBlox - SDL Timer 
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 20:37:42 15 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
  //The clock time when the timer started
  int startTicks;

  //The ticks stored when the timer was paused
  int pausedTicks;

  //The timer status
  bool paused;
  bool started;

public:
  //Initializes variables
  Timer();

  //The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();

  //Gets the timer's time
  int get_ticks();

  //Checks the status of the timer
  bool is_started();
  bool is_paused();
};

#endif /* TIMER_H */
