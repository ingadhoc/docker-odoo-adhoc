/*
FILE
	main.c
PURPOSE
	Create nginx proxy configuration file from docker.socket data
AUTHOR
	(C) 2016-17 Gary Wallis for AdHoc Ingenieria S.A.
	See cJSON.h for more info con cJSON Copyright (c) 2009 Dave Gamble
*/


#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define UNIX_PATH_MAX 256


#include "cJSON.h"
void parse_object(cJSON *item)
{
    printf("1 %s=%s,%d\n",item->string,item->valuestring,item->valueint);
    cJSON *subitem = item->child;
    while (subitem)
    {
        // handle subitem
        printf("2 %s=%s,%d\n",subitem->string,subitem->valuestring,subitem->valueint);
        if (subitem->child)
        {
            parse_object(subitem->child);
        }

        subitem = subitem->next;
    }
}

void ParseJson(char *cJson)
{
	//strip first and last [] it breaks cJSON !
	if(cJson[0]=='[')
		cJson++;
	if(cJson[strlen(cJson)-1]==']')
		cJson[strlen(cJson)-1]=0;
		
	cJSON *root = cJSON_Parse(cJson);
	char *rendered = cJSON_Print(root);
	printf("%s\n",rendered);

	cJSON *item= cJSON_GetObjectItem(root, "NetworkSettings");

	parse_object(item);

}//void ParseJson(char *cJson)


int main(void)
{
	struct sockaddr_un address;
	int  socket_fd, nbytes;
	char buffer[102400]={""};
	char cJson[102400]={""};

	socket_fd=socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd<0)
	{
		printf("socket() failed\n");
		return 1;
	}

 	memset(&address,0,sizeof(struct sockaddr_un));
 	address.sun_family=AF_UNIX;
 	snprintf(address.sun_path,UNIX_PATH_MAX,"/var/run/docker.sock");

	if(connect(socket_fd,(struct sockaddr *)&address,sizeof(struct sockaddr_un))!= 0)
	{
		printf("connect() failed\n");
		return 1;
 	}

	nbytes=snprintf(buffer,102400,"GET /containers/json HTTP/1.1\nHost: localhost\n\n\n");
	write(socket_fd,buffer,nbytes);

	nbytes=read(socket_fd,buffer,102400);
	buffer[nbytes] = 0;

	char *cp=NULL;
	unsigned uConLen=0;
	if((cp=strstr(buffer,"Content-Length: "))!=NULL)
	{
		sscanf(cp+16,"%u",&uConLen);
		//printf("uConLen=%u\n",uConLen);
		char *cp2=NULL;
		if((cp2=strchr(cp+17,'\n'))!=NULL)
		{
			*(cp2+1)=0;
			unsigned uHeadLen=strlen(buffer);
			//printf("uHeadLen=%u\n",uHeadLen);
			buffer[uConLen+uHeadLen+1]=0;
			sprintf(cJson,"%.102399s",cp2+3);
		}
	}

	close(socket_fd);

	printf("%s\n",cJson);
	ParseJson(cJson);

	return 0;
}//main()
