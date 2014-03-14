//  Copyright (C) 2013-2014 Michal Kalewski  <mkalewski at cs.put.poznan.pl>
//
//  This program comes with ABSOLUTELY NO WARRANTY.
//  THIS IS FREE SOFTWARE, AND YOU ARE WELCOME TO REDISTRIBUTE IT UNDER THE
//  TERMS AND CONDITIONS OF THE MIT LICENSE.  YOU SHOULD HAVE RECEIVED A COPY
//  OF THE LICENSE ALONG WITH THIS SOFTWARE; IF NOT, YOU CAN DOWNLOAD A COPY
//  FROM HTTP://WWW.OPENSOURCE.ORG.

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  union {
    short s;
    char c[sizeof(short)];
  } un;

  if (sizeof(short) != 2) {
    printf("sizeof(short) = %d\n", (int)sizeof(short));
    return EXIT_SUCCESS;
  }

  un.s = 0x0102;
  if (un.c[0] == 1 && un.c[1] == 2)
    printf("big-endian\n");
  else if (un.c[0] == 2 && un.c[1] == 1)
    printf("little-endian\n");
  else
    printf("unknown\n");

  return EXIT_SUCCESS;
}
