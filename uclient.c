     1  #define SRV_IP "999.999.999.999"
     2  /* diep(), #includes and #defines like in the server */
     3
     4  int main(void)
     5  {
     6    struct sockaddr_in si_other;
     7    int s, i, slen=sizeof(si_other);
     8    char buf[BUFLEN];
     9
    10    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    11      diep("socket");
    12
    13    memset((char *) &si_other, 0, sizeof(si_other));
    14    si_other.sin_family = AF_INET;
    15    si_other.sin_port = htons(PORT);
    16    if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
    17      fprintf(stderr, "inet_aton() failed\n");
    18      exit(1);
    19    }
    20
    21    for (i=0; i<NPACK; i++) {
    22      printf("Sending packet %d\n", i);
    23      sprintf(buf, "This is packet %d\n", i);
    24      if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
    25        diep("sendto()");
    26    }
    27
    28    close(s);
    29    return 0;
    30  }
