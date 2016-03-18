/*
 * Copyright (C) 2016 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./ethsniff.c -o ./ethsniff
 * Usage:        ./ethsniff INTERFACE
 * NOTE:         This program requires root privileges.
 *
 */

#include <arpa/inet.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int sfd;
struct ifreq ifr;

void cleanup() {
  ifr.ifr_flags &= ~IFF_PROMISC;
  ioctl(sfd, SIOCSIFFLAGS, &ifr);
  close(sfd);
}

void stop(int signo) { exit(EXIT_SUCCESS); }

const static char* etype(unsigned int ethertype) {
  static char buf[16];
  switch (ntohs(ethertype) & 0xFFFFU) {
    case 0x0001: return "802_3";
    case 0x0002: return "AX25";
    case 0x0003: return "ALL";
    case 0x0060: return "LOOP";
    case 0x0800: return "IP";
    case 0x0806: return "ARP";
    case 0x8100: return "8021Q";
    case 0x88A8: return "8021AD";
    default:     snprintf(buf, sizeof(buf), "0x%04x",
                          ntohs(ethertype) & 0xFFFFU);
                 return (const char*) buf;
  }
}

static const char *ptype(const unsigned int pkttype) {
  static char buf[16];
  switch (pkttype) {
    case PACKET_HOST:      return "INCOMING";
    case PACKET_BROADCAST: return "BROADCAST";
    case PACKET_MULTICAST: return "MULTICAST";
    case PACKET_OTHERHOST: return "OTHERHOST";
    case PACKET_OUTGOING:  return "OUTGOING";
    default:               snprintf(buf, sizeof(buf), "0x%02x", pkttype);
                           return (const char*) buf;
  }
}

void printframe(char* frame, int len, struct sockaddr_ll* sall) {
  int i, j;
  unsigned char ch;
  struct ethhdr* fhead;

  fhead = (struct ethhdr*) frame;
  printf("[%dB] %02x:%02x:%02x:%02x:%02x:%02x -> ", (int)len,
         fhead->h_source[0], fhead->h_source[1], fhead->h_source[2],
         fhead->h_source[3], fhead->h_source[4], fhead->h_source[5]);
  printf("%02x:%02x:%02x:%02x:%02x:%02x | ",
         fhead->h_dest[0], fhead->h_dest[1], fhead->h_dest[2],
         fhead->h_dest[3], fhead->h_dest[4], fhead->h_dest[5]);
  printf("%s (%s)\n", etype(sall->sll_protocol), ptype(sall->sll_pkttype));
  for (i = 0; i < len; i += 16) {
    printf("%04X:  ", i);
    for (j = 0; j < 16; j++) {
      ch = (i + j < len) ? frame[i + j] : 0;
      if (i + j < len) printf("%02X ", ch);
      else printf("   ");
    }
    printf(" ");
    for (j = 0; j < 16; j++) {
      ch = (i + j < len) ? frame[i + j] : ' ';
      if ((ch < 0x20) || (ch > 0x7E)) ch = '.';
        printf("%c", ch);
    }
    printf("\n");
  }
  printf( "\n");
}

int main(int argc, char** argv) {
  socklen_t sl;
  ssize_t len;
  char* frame;
  struct sockaddr_ll sall;

  atexit(cleanup);
  signal(SIGINT, stop);
  sfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  strncpy(ifr.ifr_name, argv[1], IFNAMSIZ);
  ioctl(sfd, SIOCGIFFLAGS, &ifr);
  ifr.ifr_flags |= IFF_PROMISC;
  ioctl(sfd, SIOCSIFFLAGS, &ifr);
  while(1) {
    frame = malloc(ETH_FRAME_LEN);
    memset(frame, 0, ETH_FRAME_LEN);
    memset(&sall, 0, sizeof(struct sockaddr_ll));
    sl = sizeof(struct sockaddr_ll);
    len = recvfrom(sfd, frame, ETH_FRAME_LEN, 0, (struct sockaddr*)&sall, &sl);
    printframe(frame, len, &sall);
    free(frame);
  }
}
