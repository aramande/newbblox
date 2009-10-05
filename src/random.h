
/* NewbBlox - Random Number Generator 
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 13:41:18 16 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef RANDOM_H
#define RANDOM_H

#define CMATH_N 624 

class CRandom { 
   // DATA 
   unsigned int             rseed; 
   unsigned long mt[CMATH_N];       /* the array for the state vector */ 
   int mti;                 /* mti==N+1 means mt[N] is not initialized */ 
   // FUNCTIONS 
public: 
   CRandom(void); 
   unsigned int     Random( unsigned int n ); 
   void   SetRandomSeed(unsigned int n); 
   unsigned int     GetRandomSeed(void); 
   void   Randomize(void); 
}; 

#endif /* RANDOM_H */
