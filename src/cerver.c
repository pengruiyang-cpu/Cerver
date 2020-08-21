// inti Cerver - cerver.c
// 2020 / 08 / 21 - rain - create


//#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<errno.h>


#define IS_FAILED(x) (x == -1 ? 1 : 0)

void accept_request(int accept_handle);
int getword(char *s, char *buffer, int index);

int main(int argc, char const *argv[]){
  // socket handle
  int socket_handle = socket(AF_INET, SOCK_STREAM, 0);

  if (IS_FAILED (socket_handle)){
    printf("fatal error: socket init failed (%d: %m)\n", errno);
    exit(-1);
  };

  // bind IP address and PORT

  // get PORT
  int port;

  // argc == 2: ./cerver 80
  if (argc == 2)
    port = atoi(argv[1]);
  else
    // default at 80
    port = 80;

  struct sockaddr_in server;
  // clean it
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (IS_FAILED(bind(socket_handle, (const struct sockaddr *)&server, sizeof(struct sockaddr)))){
    printf("fatal error: bind port failed (%d: %m)\n", errno);
    exit(-1);
  };

  if (IS_FAILED (listen (socket_handle, 10))){
    printf("fatal error: listen failed (%d: %m)\n", errno);
    exit(-1);
  };

  int sin_size = sizeof(struct sockaddr_in);
  struct sockaddr_in client_address;
  int client_length = sizeof(struct sockaddr_in);
  int accept_handle;
  if (IS_FAILED((accept_handle = accept (socket_handle, (struct sockaddr *) &client_address, (socklen_t *)&client_length)))){
    printf("fatal error: accept request failed (%d: %m)\n", errno);
    exit(-1);
  };

  // accept request
  for (;;){
    accept_request(accept_handle);
    break;
  }

  close(socket_handle);
  close(accept_handle);
  return 0;
}


void accept_request(int accept_handle){
  // get request header
  char request_header[1024] = {0};
  int length;
  length = recv(accept_handle, request_header, sizeof(request_header), 0);
  // display it
  printf("%s", request_header);

  // get request method
  char request_method[1024] = {0};
  getword(request_header, request_method, 0);
  // don't use printf!!! SEGMENT FAULT
  fprintf(stderr, "request method: %s\n", request_method);

  // get filename
  char path[1024] = {0};
  getword(request_header, path, strlen(request_method)+1);
  fprintf(stderr, "request path: %s\n", path);
  fprintf(stderr, "line 111");
  return;
}

// other function
// get a word from s1
int getword(char *s, char *buffer, int index){
  int i;
  for (i = 0, s += index;*s != ' ';i++, s++){
    buffer[i] = *s;
  };
  buffer[i] = '\0';
  return i;
}
