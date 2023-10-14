#include <stdio.h>
#include <stdint.h>
#include <string.h>

//The following libraries are for getaddrinfo and the addrinfo structure
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h> // To import inet_ntop

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Enter atleast one URL");
    return 1;
  }
  const char* domain = argv[1];
  const char* protocol = "http";
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(domain, protocol, &hints, &res);

  if (status == 0) {
    char ip4[INET_ADDRSTRLEN];
    //memset(ip4, 0, IPLEN);
    struct sockaddr_in* resAddr = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &(resAddr->sin_addr), ip4, sizeof ip4);
    printf("IP address of domain %s is: %s\n", domain, ip4);

    return 0;
  } 

  printf("Failed\n");
  fprintf(stderr, "getaddinfo: %s\n", gai_strerror(status));

  return 2;
}
