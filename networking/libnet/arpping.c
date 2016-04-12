/*
 * Copyright (C) 2016 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./arpping.c -o ./arpping -lnet -lpcap
 * Usage:        ./arpping HOST
 * NOTE:         This program requires root privileges.
 *
 */

#include <libnet.h>
#include <linux/if_ether.h>
#include <pcap.h>
#include <signal.h>
#include <stdlib.h>

struct arphdr {
  u_int16_t ftype;
  u_int16_t ptype;
  u_int8_t  flen;
  u_int8_t  plen;
  u_int16_t opcode;
  u_int8_t  sender_mac_addr[6];
  u_int8_t  sender_ip_addr[4];
  u_int8_t  target_mac_addr[6];
  u_int8_t  target_ip_addr[4];
};

libnet_t *ln;
pcap_t* handle;
char perrbuf[PCAP_ERRBUF_SIZE];
char nerrbuf[LIBNET_ERRBUF_SIZE];

void cleanup() {
  pcap_close(handle);
  libnet_destroy(ln);
}

void stop(int signo) {
  exit(EXIT_SUCCESS);
}

int tdiff(struct timeval t1, struct timeval t2) {
  return (t2.tv_sec - t1.tv_sec) * 1000000
         + ((int)t2.tv_usec - (int)t1.tv_usec);
}

int main(int argc, char** argv) {
  int cap, us;
  u_int32_t target_ip_addr, src_ip_addr;
  u_int8_t bcast_hw_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
           zero_hw_addr[6]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  struct timeval now;
  struct libnet_ether_addr* src_hw_addr;
  struct pcap_pkthdr* pheader = NULL;
  struct ethhdr* fheader;
  struct arphdr* aheader;
  const u_char* frame;

  atexit(cleanup);
  signal(SIGINT, stop);
  ln = libnet_init(LIBNET_LINK, NULL, nerrbuf);
  src_ip_addr = libnet_get_ipaddr4(ln);
  src_hw_addr = libnet_get_hwaddr(ln);
  target_ip_addr = libnet_name2addr4(ln, argv[1], LIBNET_RESOLVE);
  libnet_autobuild_arp(
    ARPOP_REQUEST,                   /* operation type       */
    src_hw_addr->ether_addr_octet,   /* sender hardware addr */
    (u_int8_t*) &src_ip_addr,        /* sender protocol addr */
    zero_hw_addr,                    /* target hardware addr */
    (u_int8_t*) &target_ip_addr,     /* target protocol addr */
    ln);                             /* libnet context       */
  libnet_autobuild_ethernet(
    bcast_hw_addr,                   /* ethernet destination */
    ETHERTYPE_ARP,                   /* ethertype            */
    ln);                             /* libnet context       */
  handle = pcap_create(libnet_getdevice(ln), perrbuf);
  pcap_activate(handle);
  while(1) {
    libnet_write(ln);
    gettimeofday(&now, NULL);
    printf("REQ %s\n", argv[1]);
    while(1) {
      cap = pcap_next_ex(handle, &pheader, &frame);
      if (cap != 1)
        continue;
      fheader = (struct ethhdr*) frame;
      if (ntohs(fheader->h_proto) != ETH_P_ARP)
        continue;
      us = tdiff(now, pheader->ts);
      aheader = (struct arphdr*) (frame + ETH_HLEN);
      if (ntohs(aheader->opcode) == ARPOP_REPLY &&
          memcmp(aheader->sender_ip_addr,
                 (u_int8_t*) &target_ip_addr, 4) == 0) {
        printf("REP %s | %02x:%02x:%02x:%02x:%02x:%02x (%d.%04d s)\n",
               argv[1], aheader->sender_mac_addr[0],
               aheader->sender_mac_addr[1], aheader->sender_mac_addr[2],
               aheader->sender_mac_addr[3], aheader->sender_mac_addr[4],
               aheader->sender_mac_addr[5], us / 1000000, us % 1000000);
        sleep(1);
        break;
      }
    }
  }
  return EXIT_SUCCESS;
}
