#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h> // addrinfo structure
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int get_ip(char* protocol, char* url, char** target) {
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(url, protocol, &hints, &res);

  if (status != 0) {
    fprintf(stderr, "getaddrinfo returned: %s\n", gai_strerror(status));
    return EXIT_FAILURE;
  }

  char ip_address[INET_ADDRSTRLEN];

  for (struct addrinfo *p = res; p != NULL; p = res->ai_next) {
    struct sockaddr_in *myaddr = (struct sockaddr_in*)(p->ai_addr);
    inet_ntop(AF_INET, &myaddr->sin_addr, ip_address, sizeof ip_address);
    *target = malloc(sizeof ip_address);
    strcpy(*target, ip_address);
  }

  freeaddrinfo(res);

  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Enter url in the following format\nhttp://www.google.com\n");
    return EXIT_FAILURE;
  }

  char* full_address = argv[1];
  char *protocol, *address;

  //The following for loop is used to seperate http://www.url.com to "http" and "www.url.com"
  for (int i=0; i<strlen(argv[1]); i++) {
    if (full_address[i] == ':') {
      protocol = malloc((i+1)*sizeof(char));
      strncpy(protocol, full_address, i);
      int len = strlen(full_address) - i;
      address = malloc(len*sizeof(char));
      strcpy(address, full_address + i + 3);
      break;
    }
  }

  char* ip_addr;
  get_ip(protocol, address, &ip_addr);
  printf("IP address of %s: %s\n", argv[1], ip_addr);

  free(protocol);
  free(address);
  free(ip_addr);

  return EXIT_SUCCESS;
}
