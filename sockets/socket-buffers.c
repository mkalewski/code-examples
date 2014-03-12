//  Copyright (C) 2013-2014 Michal Kalewski  <mkalewski at cs.put.poznan.pl>
//
//  This program comes with ABSOLUTELY NO WARRANTY.
//  THIS IS FREE SOFTWARE, AND YOU ARE WELCOME TO REDISTRIBUTE IT UNDER THE
//  TERMS AND CONDITIONS OF THE MIT LICENSE.  YOU SHOULD HAVE RECEIVED A COPY
//  OF THE LICENSE ALONG WITH THIS SOFTWARE; IF NOT, YOU CAN DOWNLOAD A COPY
//  FROM HTTP://WWW.OPENSOURCE.ORG.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define ERROR(e) { perror(e); exit(EXIT_FAILURE); }

int getbuffsize(int sfd, int buffname) {
  int s;
  socklen_t slt = (socklen_t)sizeof(s);
  if (getsockopt(sfd, SOL_SOCKET, buffname, (void*)&s, &slt) < 0)
    ERROR("getsockopt")
  return s;
}

void buffsizes(int sfd, int *srb, int *ssb) {
  *srb = getbuffsize(sfd, SO_RCVBUF);
  *ssb = getbuffsize(sfd, SO_SNDBUF);
}

int main(int argc, char **argv) {
  int sfd, srb, ssb;

  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    ERROR("socket()")
  buffsizes(sfd, &srb, &ssb);
  printf("TCP:  RCVBUF = %6d [B]  SNDBUF = %6d [B]\n", srb, ssb);
  close(sfd);

  if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    ERROR("socket()")
  buffsizes(sfd, &srb, &ssb);
  printf("UDP:  RCVBUF = %6d [B]  SNDBUF = %6d [B]\n", srb, ssb);
  close(sfd);

  return EXIT_SUCCESS;
}
