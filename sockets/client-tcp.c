#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define ERROR(f) { perror(f); exit(EXIT_FAILURE); }
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
  int sfd, rc;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in saddr;
  struct hostent* addrent;

  if (argc != 3) {
    fprintf(stderr, "usage: %s server port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(!(addrent = gethostbyname(argv[1])))
    ERROR("gethostbyname()")

  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    ERROR("socket()")

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(atoi(argv[2]));
  memcpy(&saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);

  if (connect(sfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
    ERROR("connect()")
  if((rc = read(sfd, buffer, BUFFER_SIZE)) < 0)
    ERROR("read()")
  if (write(1, buffer, rc) < 0)
    ERROR("write()")

  close(sfd);
  return EXIT_SUCCESS;
}
