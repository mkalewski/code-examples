/*
 * Copyright (C) 2016 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./udp-client.c -o ./udp-client
 * Usage:        ./udp-client SERVER PORT
 *
 * Bug reports:  https://dsg.cs.put.poznan.pl/gitlab/mkalewski/ps-2016/issues
 *
 */

#include <netdb.h>
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
  struct hostent* addrent;

  addrent = gethostbyname(argv[1]);
  memset(&caddr, 0, sizeof(caddr));
  caddr.sin_family = AF_INET;
  caddr.sin_addr.s_addr = INADDR_ANY;
  caddr.sin_port = 0;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  memcpy(&saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
  saddr.sin_port = htons(atoi(argv[2]));
  sfd = socket(PF_INET, SOCK_DGRAM, 0);
  bind(sfd, (struct sockaddr*)&caddr, sizeof(caddr));
  strcpy(buf, "Hello server!\n");
  sendto(sfd, buf, strlen(buf)+1, 0, (struct sockaddr*)&saddr, sizeof(saddr));
  sl = sizeof(saddr);
  rc = recvfrom(sfd, buf, 128, 0, (struct sockaddr*)&saddr, &sl); close(sfd);
  write(1, buf, rc);
  return EXIT_SUCCESS;
}
