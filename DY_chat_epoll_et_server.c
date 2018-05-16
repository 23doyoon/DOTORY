#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 5
#define EPOLL_SIZE 50
#define MAX_CLNT 100

void error_handling(char *buf);
void setNonBlockingMod(int fd);
void sendMsg(char *msg, int len);

static int clntNumber[MAX_CLNT];
static int clntCnt = 0;

int main(int argc, char *argv[]){
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len;
	int i, j;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while(1){
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if(event_cnt == -1){
			printf("epoll_wait() error");
			break;
		}
		puts("return epoll_wait(by Level Trigger Method)");

		for(i=0; i<event_cnt; ++i){
			if(ep_events[i].data.fd == serv_sock){
				adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				setNonBlockingMod(clnt_sock);
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				clntNumber[clntCnt++] = clnt_sock;
				printf("connected clien : %d\n", clnt_sock);
			}
			else{
				while(1){
					str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
					if(str_len == 0){
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						for(j=0; j<clntCnt; ++j){
							if(clntNumber[j] == ep_events[i].data.fd){
								memcpy(&clntNumber[j], &clntNumber[j+1], clntCnt-(j+1));
								clntCnt--;
								break;
							}
						}
						printf("closed client : %d\n", ep_events[i].data.fd);
						break;
					}
					else if(str_len < 0){
						if(errno = EAGAIN)
							break;
					}
					else
						sendMsg(buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char *buf){
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}

void setNonBlockingMod(int fd){
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void sendMsg(char *msg, int len){
	int i = 0;
	for(i=0; i<clntCnt; ++i)
		write(clntNumber[i], msg, len);
}
