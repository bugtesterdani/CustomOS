#include "headers/util.h"
#include <limits.h>

void k_itoa(int value, char* valuestring)
{
  int tenth, min_flag;
  char swap, *p;
  min_flag = 0;

  if (0 > value)
  {
    *valuestring++ = '-';
    value = -INT_MAX > value ? min_flag = INT_MAX : -value;
  }

  p = valuestring;

  do
  {
    tenth = value / 10;
    *p++ = (char)(value - 10 * tenth + '0');
    value = tenth;
  }
  while (value != 0);

  if (min_flag != 0)
  {
    ++*valuestring;
  }
  *p-- = '\0';

  while (p > valuestring)
  {
    swap = *valuestring;
    *valuestring++ = *p;
    *p-- = swap;
  }
}