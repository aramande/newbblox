/* NewbBlox - Debugging 
 *
 * Author: Aramande <aramande@gmail.com>
 *
 * Created: 15:50:17 14 May 2009
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <string>
#include <sstream>
#ifndef DEBUG_H
#define DEBUG_H

class Debug
{
  static void output(std::string message, std::string type);
public:
  static void debug(std::string message);
  static void error(std::string message);
  static void warning(std::string message);

};

#endif /* DEBUG_H */

template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}
