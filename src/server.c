#include "../include/server.h"
#include <stdio.h>

#define BACKLOG 10


int main() {

  WSADATA wsaData;

 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
 fprintf(stderr, "WSAStartup failed.\n");
 exit(1); }
 if (LOBYTE(wsaData.wVersion) != 2 ||
 HIBYTE(wsaData.wVersion) != 2) {
 fprintf(stderr,"Versiion 2.2 of Winsock is not available.\n");
 WSACleanup();
 exit(2);
  }

  
  FILE *html_data;
  html_data = fopen("index.html", "r");

  char response[1024];
  size_t chars_read = fread( response, 1, sizeof response - 1, html_data );
response[chars_read] = '\0';
  
  char http_header[2048] = "HTTP/1.1 200 OK\r\n";
  strcat(http_header, "Content-Type: text/html\r\n");
  strcat(http_header, "\r\n");
  strcat(http_header, response);

  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;
  
  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  listen(server_socket, BACKLOG);
  int client_socket;
  while(1){
    
    client_socket = accept(server_socket, NULL, NULL);
    send(client_socket, http_header, sizeof(http_header), 0);
    close(client_socket);
  }
   
  return 0;
}
