//  Copyright (C) 2013-2014 Michal Kalewski  <mkalewski at cs.put.poznan.pl>
//
//  This program comes with ABSOLUTELY NO WARRANTY.
//  THIS IS FREE SOFTWARE, AND YOU ARE WELCOME TO REDISTRIBUTE IT UNDER THE
//  TERMS AND CONDITIONS OF THE MIT LICENSE.  YOU SHOULD HAVE RECEIVED A COPY
//  OF THE LICENSE ALONG WITH THIS SOFTWARE; IF NOT, YOU CAN DOWNLOAD A COPY
//  FROM HTTP://WWW.OPENSOURCE.ORG.

#include <stdlib.h>
#include <stdio.h>

int _atoi(const char *a) {
  int i = 0, s = *a == '-' ? -1 : 1;
  if (s == -1)
    a++;
  while(*a) {
    i = (i<<3) + (i<<1) + (int)((*a) - '0');
    a++;
  }
  return s * i;
}

/* sample proposal */

int main(int argc, char** argv) {
  if (argc > 1)
    printf("%d\n", _atoi(argv[1]));
  return EXIT_SUCCESS;
}
