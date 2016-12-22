/*
 *FILE
 *PURPOSE
 *	Very initial and simplistic hardcoded AdHoc Odoo docker data parser
 *	for creating Nginx conf.d file
 *AUTHOR/LEGAL
 *	(C) Gary Wallis for AdHoc Ing. S.A. 2016-2017
 *LICENSE
 *	MIT License
*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include "jsmn.h"

#include "json.h"
#include "log.h"
#include "buf.h"
#include "template.h"
#define MAXBUFLEN 102400

void GetJson(char const *cResource,char *cJson);

void AppFunctions(FILE *fp,char *cFunction)
{
}//void AppFunctions(FILE *fp,char *cFunction)

void UpstreamConfTemplate(FILE *fpOut,
		char const *cUpstreamServerName,
		char const *cUpstreamIp,
		char const *cUpStreamPort)
{
	struct t_template template;

	template.cpName[0]="cUpstreamServerName";
	template.cpValue[0]=cUpstreamServerName;

	template.cpName[1]="cUpstreamIp";
	template.cpValue[1]=cUpstreamIp;

	template.cpName[2]="cUpStreamPort";
	template.cpValue[2]=cUpStreamPort;

	template.cpName[3]="";//close template!


	char cTemplate[MAXBUFLEN + 1];
	FILE *fp = fopen("/var/local/dockprox/upstream.conf.tpl", "r");
	if(fp!=NULL)
	{
		size_t newLen=fread(cTemplate,sizeof(char),MAXBUFLEN,fp);
		if(ferror(fp)!=0)
		{
			fputs("Error reading file upstream.conf.tpl\n", stderr);
			exit(1);
		}
		else
		{
			cTemplate[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}
	else
	{
		fputs("Error opening file /var/local/dockprox/upstream.conf.tpl\n", stderr);
	}
	Template(cTemplate,&template,fpOut);

}//void UpstreamConfTemplate(FILE *fpOut,...)


void ServerConfTemplate(FILE *fpOut,
	char const *cPublicServerName,
	char const *cUpstreamServerName,
	char const *cUpstreamServerNameChat)
{
	struct t_template template;

	template.cpName[0]="cPublicServerName";
	template.cpValue[0]=cPublicServerName;

	template.cpName[1]="cUpstreamServerName";
	template.cpValue[1]=cUpstreamServerName;

	template.cpName[2]="cUpstreamServerNameChat";
	template.cpValue[2]=cUpstreamServerNameChat;

	template.cpName[3]="";//close template!


	char cTemplate[MAXBUFLEN + 1];
	FILE *fp = fopen("/var/local/dockprox/server.conf.tpl", "r");
	if(fp!=NULL)
	{
		size_t newLen=fread(cTemplate,sizeof(char),MAXBUFLEN,fp);
		if(ferror(fp)!=0)
		{
			fputs("Error reading file server.conf.tpl\n", stderr);
			exit(1);
		}
		else
		{
			cTemplate[newLen++] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}
	else
	{
		fputs("Error opening file /var/local/dockprox/server.conf.tpl\n", stderr);
	}
	Template(cTemplate,&template,fpOut);

}//void ServerConfTemplate(FILE *fpOut,...)


unsigned uSplitPorts(char const *cVirtualPort,char cVirtualPorts[8][32])
{
	char str[512]={""};
	sprintf(str,"%.511s",cVirtualPort);
	char ** res  = NULL;
	char *  p    = strtok (str, ",");
	int n_spaces = 0, i;

	while (p)
	{
		res = realloc (res, sizeof (char*) * ++n_spaces);
		if (res == NULL)
			exit (-1); /* memory allocation failed */
		res[n_spaces-1] = p;
		p = strtok (NULL, ",");
	}

	/* realloc one extra element for the last NULL */
	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	/* save the result */
	for (i = 0; i < (n_spaces) && i<7; ++i)
 		strncpy(cVirtualPorts[i],res[i],31);
	free (res);

	return(n_spaces);

}//unsigned uSplitPorts(char const *cVirtualPort,char cVirtualPorts[8][32])

//Case sensitive
void ParseFromJsonArray(char const *cEnv, char const *cName, char *cValue)
{
	cValue[0]=0;
	char cNamePattern[100]={""};
	sprintf(cNamePattern,"%.64s=",cName);
	unsigned uNamePatternStrLen=strlen(cNamePattern);
	char *cp=strstr(cEnv,cNamePattern);
	if(cp!=NULL)
	{
		char *cp2=strchr(cp+uNamePatternStrLen,'\"');
		if(cp!=NULL)
		{
			*cp2=0;
			sprintf(cValue,"%.127s",cp+uNamePatternStrLen);
		}
	}
}//void ParseFromJsonArray(char const *cEnv, char const *cName, char *cValue)


void GetLabelsByContainerId(char const *cId, char *cEnv)
{
	char cURL[256] = {""};
	//sprintf(cURL,"http://localhost/containers/%.99s/json",cId);
	//char *js = json_fetch_unixsock(cURL);
	char cJson[102400];
	sprintf(cURL,"/containers/%.99s/json",cId);
	GetJson(cURL,cJson);
	char *js=cJson;
	jsmntok_t *tokens = json_tokenise(js);

	/*
	* Format:
	*
	*	"Env": [
	*		"VIRTUAL_HOST=odoo0.localhost",
	*		"VIRTUAL_PORT=8069",
	*		...,
	*	],
	*	...,
	*
	*/

	char *str;
	for(size_t i = 0, j = 1; j > 0; i++, j--)
	{
		jsmntok_t *t = &tokens[i];

		// Should never reach uninitialized tokens
		log_assert(t->start != -1 && t->end != -1);

		//Adjust j for size
		if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
		j += t->size;

		//print token strings
		if (t->type == JSMN_STRING && json_token_streq(js, t, "Env"))
		{
			jsmntok_t *t2 = &tokens[i+1];
			str = json_token_tostr(js, t2);
			sprintf(cEnv,"%.255s",str);
		}
	}
}//void GetLabelsByContainerId(char *cId)


void GetJson(char const *cResource,char *cJson)
{
	struct sockaddr_un address;
	int  socket_fd, nbytes;
	char buffer[102400]={""};

	socket_fd=socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd<0)
	{
		printf("socket() failed\n");
		exit(3);
	}

 	memset(&address,0,sizeof(struct sockaddr_un));
 	address.sun_family=AF_UNIX;
#define UNIX_PATH_MAX 256
 	snprintf(address.sun_path,UNIX_PATH_MAX,"/var/run/docker.sock");

	if(connect(socket_fd,(struct sockaddr *)&address,sizeof(struct sockaddr_un))!= 0)
	{
		printf("connect() failed\n");
		exit(4);
 	}

	nbytes=snprintf(buffer,102400,"GET %.127s HTTP/1.1\nHost: localhost\n\n",cResource);
	write(socket_fd,buffer,nbytes);

	nbytes=read(socket_fd,buffer,102400);
	buffer[nbytes] = 0;
	//printf("nbytes=%u\n%s",nbytes,buffer);

	char *cp=NULL;
	register int i,j;
	if((cp=strstr(buffer,"Content-Length: "))!=NULL)
	{

		//skip 3 lines
		j=0;
		for(i=27;j<3&&i<100;i++)
			if(*(cp+i)=='\n' || *(cp+i)=='\r')
				j++;
		sprintf(cJson,"%.102399s",cp+i+1);
	}
	else if((cp=strstr(buffer,"Transfer-Encoding: chunked"))!=NULL)
	{
		//skip 4 lines
		j=0;
		for(i=27;j<4&&i<100;i++)
			if(*(cp+i)=='\n' || *(cp+i)=='\r')
				j++;
		//debug only printf("%d %d\n",i,j);
		sprintf(cJson,"%.102399s",cp+i+1);
	}

	close(socket_fd);

}//void GetJson()


int main(void)
{

	char cJson[102400];
	GetJson("/containers/json",cJson);

	//char cURL[] = "http://localhost/containers/json";
	//char *js = json_fetch_unixsock(cURL);
	char *js=cJson;
	printf("%.99s\n",js);
	jsmntok_t *tokens = json_tokenise(js);

	/* The Docker containers API response is in this format:
	*
	* [
	*   {
	*      ...,
	*      "Names": [
	*              "/r-base-nginx-proxy-1-5c11f051"
	*      ],
	*      ...,
	*      "Labels": {
	*        ...,
	*        "io.rancher.container.ip": "10.42.19.155/16",
	*        ...,
	*        "io.rancher.container.name": "base-nginx-proxy-1",
	*      },
	*      ...,
	*   },
	*   {
	*      ...,
	*      "Names": [
	*              "/r-base-nginx-proxy-2-5caaf051"
	*      ],
	*      ...,
	*      "Labels": {
	*        ...,
	*        "io.rancher.container.ip": "10.42.19.156/16",
	*        ...,
	*        "io.rancher.container.name": "base-nginx-proxy-2",
	*      },
	*      ...,
	*   },
	*   ...,
	* ]
	*
	*/

	if(system("if [  -f /etc/nginx/conf.d/docker.conf ];then cp /etc/nginx/conf.d/docker.conf /etc/nginx/conf.d/docker.conf.0;fi"))
	{
		fprintf(stderr,"Could not copy /etc/nginx/conf.d/docker.conf\n");
		exit(2);
	}

	FILE *fp;
	if((fp=fopen("/etc/nginx/conf.d/docker.conf","w"))==NULL)
	{
		fprintf(stderr,"Could not open /etc/nginx/conf.d/docker.conf\n");
		exit(2);
	}
	printf("Opened /etc/nginx/conf.d/docker.conf for write\n");

	char cId[100]={""};
	char cContainerName[256]={""};
	char cContainerNameChat[256]={""};
	char cContainerIp[256]={""};
	char cEnv[512]={""};
	char cVirtualHost[128]={""};
	char cVirtualPort[128]={""};
	char *str;
	for(size_t i = 0, j = 1; j > 0; i++, j--)
	{
		jsmntok_t *t = &tokens[i];

		// Should never reach uninitialized tokens
		log_assert(t->start != -1 && t->end != -1);

		//Adjust j for size
		if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
		j += t->size;

		//print token strings
		if (t->type == JSMN_STRING && json_token_streq(js, t, "Id"))
		{
			jsmntok_t *t2 = &tokens[i+1];
			str = json_token_tostr(js, t2);
			sprintf(cId,"%.99s",str);
			cContainerName[0]=0;
			cContainerIp[0]=0;
			cVirtualHost[0]=0;
			GetLabelsByContainerId(cId,cEnv);
		}
		if (t->type == JSMN_STRING && json_token_streq(js, t, "io.rancher.container.name"))
		{
			jsmntok_t *t2 = &tokens[i+1];
			str = json_token_tostr(js, t2);
			if(strstr(str,"odoo") && !strstr(str,"-db"))
			{
				sprintf(cContainerName,"%.128s",str);
				sprintf(cContainerNameChat,"%.128s-chat",str);
				fprintf(fp,"#cId=%s\n",cId);
				//debug only printf("cEnv=%s\n",cEnv);
				ParseFromJsonArray(cEnv,"VIRTUAL_PORT",cVirtualPort);
				char cVirtualPorts[8][32]={"","","","","","","",""};
				unsigned uNumPorts=0;
				uNumPorts=uSplitPorts(cVirtualPort,cVirtualPorts);
				ParseFromJsonArray(cEnv,"VIRTUAL_HOST",cVirtualHost);
				register int n=0;
				if(cVirtualHost[0])
				{
					for(n=0;n<uNumPorts&&n<8;n++)
					{
						if(n==0)
							UpstreamConfTemplate(fp,cContainerName,cContainerIp,cVirtualPorts[n]);
						else
							UpstreamConfTemplate(fp,cContainerNameChat,cContainerIp,cVirtualPorts[n]);
					}
					ServerConfTemplate(fp,cVirtualHost,cContainerName,cContainerNameChat);
					fprintf(fp,"#cId=%s\n\n",cId);

					printf("cVirtualHost=%s\n",cVirtualHost);
					printf("cVirtualPort=%s\n",cVirtualPort);
					printf("cId=%s\n",cId);
					printf("io.rancher.container.name=%s\n",cContainerName);
					printf("io.rancher.container.ip=%s\n",cContainerIp);
					//debug only uNumPorts=uSplitPorts("8069,8073,8080",cVirtualPorts);
					printf("uNumPorts=%u\n",uNumPorts);
					for(n=0;n<uNumPorts&&n<8;n++)
					{
						printf("cVirtualPorts[%d]=%s\n",n,cVirtualPorts[n]);
					}
				}
			}
		}
		if (t->type == JSMN_STRING && json_token_streq(js, t, "io.rancher.container.ip"))
		{
       			jsmntok_t *t2 = &tokens[i+1];
			str = json_token_tostr(js, t2);
			sprintf(cContainerIp,"%s",str);
			char *cp;
			//chop cidr
			if((cp=strchr(cContainerIp,'/'))!=NULL)
				*cp=0;
		}
	}

	fclose(fp);
	if(system("diff /etc/nginx/conf.d/docker.conf /etc/nginx/conf.d/docker.conf.0 > /dev/null 2>&1"))
	{
		if(!system("kill -HUP `pidof nginx | cut -f 2 -d ' '`"))
			printf("Reload nginx\n");
		else
			fprintf(stderr,"Reload nginx failed\n");
	}
	printf("Normal exit\n");
	return 0;
}//main()
