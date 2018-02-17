#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "fastcgi.hpp"
#include "type.hpp"

void halt(const char *s, int errnum) {
  fprintf(stderr, "%s: %s\n", s, strerror(errnum));
  exit(1);
}

char *get_content(const std::string &, std::string);
int main(){
  params_type params;
  int servfd, connfd;
  int ret, i;
  struct sockaddr_in servaddr, cliaddr;
  socklen_t slen, clen;

  struct fcgi_header header, headerBuf;
  struct FCGI_BeginRequestBody brBody;
  struct FCGI_EndRequestBody erBody;

  ssize_t rdlen;
  int requestId, contentLen;
  unsigned char paddingLen;
  int paramNameLen, paramValueLen;

  char buf[BUFLEN];

  unsigned char c;
  unsigned char lenbuf[3];
  char *paramName, *paramValue;

  char *htmlHead, *htmlBody;

  servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (servfd == -1) {
    halt("socket", errno);
  }
  slen = clen = sizeof(struct sockaddr_in);
  bzero(&servaddr, slen);

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9001);
  servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


  int iSockOptVal = 1;
  if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1) {
    halt("setcockopt", errno);
  }

  ret = bind(servfd, (struct sockaddr *)&servaddr, slen);
  if (ret == -1) {
    halt("bind", errno);
  }

  ret = listen(servfd, 16);
  if (ret == -1) {
    halt("listen", errno);
  }


  while (true) {
    bzero(&cliaddr, clen);
    connfd = accept(servfd, (struct sockaddr *)&cliaddr, &clen);
    if (connfd == -1) {
      halt("accept", errno);
    }

    params.clear();
    //fcntl(connfd, F_SETFL, O_NONBLOCK); // 设置socket为非阻塞

    bool ok = false;
    while (true) {
      if (ok) break;
      bzero(&header, HEAD_LEN);
      printf("                 reading header...\n");
      rdlen = read(connfd, &header, HEAD_LEN);

      if (rdlen == -1) {
        halt("read", errno);
      }

      headerBuf = header;
      requestId = (header.requestIdB1 << 8) + header.requestIdB0;
      contentLen = (header.contentLengthB1 << 8) + header.contentLengthB0;
      paddingLen = header.paddingLength;

      printf("version = %d, type = %d, requestId = %d, contentLen = %d, paddingLength = %d\n", header.version, header.type, requestId, contentLen, paddingLen);

      switch (header.type) {
        case FCGI_BEGIN_REQUEST:
          printf("--- begin request ---\n");
          bzero(&brBody, sizeof(brBody));
          read(connfd, &brBody, sizeof(brBody));
          break;
        case FCGI_PARAMS:
          if (contentLen == 0) {
            printf("--- FCGI_PARAMS_END ---\n");
          }

          while (contentLen > 0) {
            rdlen = read(connfd, &c, 1);
            contentLen -= rdlen;
            if ((c & 0x80) != 0) {
              rdlen = read(connfd, lenbuf, 3);
              contentLen -= rdlen;
              paramNameLen = ((c & 0x7f) << 24) + (lenbuf[0] << 16) + (lenbuf[1] << 8) + lenbuf[2];
            } 
            else {
              paramNameLen = c;
            }

            rdlen = read(connfd, &c, 1);
            contentLen -= rdlen;
            if ((c & 0x80) != 0) {
              rdlen = read(connfd, lenbuf, 3);
              contentLen -= rdlen;
              paramValueLen = ((c & 0x7f) << 24) + (lenbuf[0] << 16) + (lenbuf[1] << 8) + lenbuf[2];
            }
            else {
              paramValueLen = c;
            }

            paramName = (char *)malloc(paramNameLen + 1);
            rdlen = read(connfd, paramName, paramNameLen);
            paramName[paramNameLen] = '\0';
            contentLen -= rdlen;

            paramValue = (char *)malloc(paramValueLen + 1);
            rdlen = read(connfd, paramValue, paramValueLen);
            paramValue[paramValueLen] = '\0';
            contentLen -= rdlen;

            printf("%s = %s\n", paramName, paramValue);

            params[std::string(paramName)] = std::string(paramValue);
            free(paramName); free(paramValue);
          }

          if (paddingLen > 0) {
            rdlen = read(connfd, buf, paddingLen);
            contentLen -= rdlen;
          }
          break;

        case FCGI_STDIN:
          if(contentLen == 0) {
            printf("--- FCGI_STDIN_END ---\n");
            ok = true;
            break;
           }

          if (contentLen > 0) {
            while (contentLen > 0) {
              if (contentLen > BUFLEN) {
                rdlen = read(connfd, buf, BUFLEN);
              }
              else {
                rdlen = read(connfd, buf, contentLen);
              }
              contentLen -= rdlen;
              fwrite(buf, sizeof(char), rdlen, stdout);
            }
            printf("\n");
          }

          if (paddingLen > 0) {
            rdlen = read(connfd, buf, paddingLen);
            contentLen -= rdlen;
          }
          break;

        case FCGI_DATA:
          if (contentLen > 0) {
            while (contentLen > 0) {
              if (contentLen > BUFLEN) {
                rdlen = read(connfd, buf, BUFLEN);
              }
              else {
                rdlen = read(connfd, buf, contentLen);
              }
              contentLen -= rdlen;
              fwrite(buf, sizeof(char), rdlen, stdout);
            }
            printf("\n");
          }

          if (paddingLen > 0) {
            rdlen = read(connfd, buf, paddingLen);
            contentLen -= rdlen;
          }
          break;

      }
    }

    printf("Start writing...\n");
    {
      char *content = get_content(params["SCRIPT_FILENAME"], params["QUERY_STRING"]);
      headerBuf.version = FCGI_VERSION_1;
      headerBuf.type = FCGI_STDOUT;

      contentLen = strlen(content);
      headerBuf.contentLengthB1 = (contentLen >> 8) & 0xff;
      headerBuf.contentLengthB0 = contentLen & 0xff;
      headerBuf.paddingLength = (contentLen % 8) > 0 ? 8 - (contentLen % 8) : 0;

    
      write(connfd, &headerBuf, HEAD_LEN);
      write(connfd, content, contentLen);

      printf("%s %d 0x%02x%02x\n", content, contentLen, headerBuf.contentLengthB1, headerBuf.contentLengthB0);
      
      if (headerBuf.paddingLength > 0) {
        write(connfd, buf, headerBuf.paddingLength);
      }
      free(content);
    }
    
    { 
      //回写一个空的 FCGI_STDOUT 表明 该类型消息已发送结束
      headerBuf.type = FCGI_STDOUT;
      headerBuf.contentLengthB1 = 0;
      headerBuf.contentLengthB0 = 0;
      headerBuf.paddingLength = 0;
      write(connfd, &headerBuf, HEAD_LEN);
    }

    {
      // 发送结束请求消息头
      headerBuf.type = FCGI_END_REQUEST;
      headerBuf.contentLengthB1 = 0;
      headerBuf.contentLengthB0 = 8;
      headerBuf.paddingLength = 0;

      bzero(&erBody, sizeof(erBody));
      erBody.protocolStatus = FCGI_REQUEST_COMPLETE;

      write(connfd, &headerBuf, HEAD_LEN);
      write(connfd, &erBody, sizeof(erBody));
    }

    params.clear();
    close(connfd);
    printf("--- end request ---\n");
  }

  close(servfd);
  return 0;
}

