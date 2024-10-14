// Copyright © 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>

#ifndef _WIN32
#include <cstring>
#endif

// Check for spaces
static bool number_of_words_ge(const char* s, int max_words)
{
  int wc = 0;
  while (*s) {
    while (*s && std::isspace(*s))
      ++s;
    while (*s && !std::isspace(*s))
      ++s;
    ++wc;
    if (wc >= max_words)
      return true;
  }
  return false;
}

static bool line_is_empty(const char* s) {
  while (*s && isspace(*s))
    s++;
  return *s == '\0';
}

static bool last_chars_are_dashes(const char* s, int min_number_of_dashes)
{
  const char * const sstart = s;
  s += std::strlen(s);

  while (--s > sstart)
    if (!isspace(*s))
      break;

  if (s <= sstart)
    return false;

  int number_of_dashes = 0;
  do
    if (*s == '-')
      ++number_of_dashes;
    else
      break;
  while (--s >= sstart);

  return number_of_dashes >= min_number_of_dashes;
}

static bool getline_in_a_loop(std::istream &istr, std::string &text)
{
  std::getline(istr, text);

  // input loop
  while (!istr.fail()) {       

    // skip empty lines
    if (!line_is_empty(text.c_str())) {
      if (number_of_words_ge(text.c_str(), 3)) {
        // print the line if it was not typed in
        if (&istr != &std::cin) {
          std::cout << "Prompt: " << std::endl;
          std::cout << text << std::endl;
        }   
        break; // break out of loop if 3+ words
      }
      if (!last_chars_are_dashes(text.c_str(), 5)) {
        // print the line if it was not typed in
        if (&istr != &std::cin) {
          std::cout << "Prompt: " << std::endl;
          std::cout << text << std::endl;
        }
        break; // break out of loop if last word does not end wih dashes
      }
    }
    // otherwise get another line of input
    std::getline(istr, text);    
  }

  // return false if no more input
  return !istr.fail();
}
