/*
 * Copyright (C) 2016 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./udp-server.c -o ./udp-server
 * Usage:        ./udp-server
 *
 * Bug reports:  https://dsg.cs.put.poznan.pl/gitlab/mkalewski/ps-2016/issues
 *
 */

#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int sfd, rc;
  socklen_t sl;
  char buf[128];
  struct sockaddr_in saddr, caddr;

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(1234);
  sfd = socket(PF_INET, SOCK_DGRAM, 0);
  bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
  while(1) {
    memset(&caddr, 0, sizeof(caddr));
    memset(&buf, 0, sizeof(buf));
    sl = sizeof(caddr);
    rc = recvfrom(sfd, buf, 128, 0, (struct sockaddr*)&caddr, &sl);
    sendto(sfd, buf, rc, 0, (struct sockaddr*)&caddr, sl);
  }
  close(sfd);
  return EXIT_SUCCESS;
}
