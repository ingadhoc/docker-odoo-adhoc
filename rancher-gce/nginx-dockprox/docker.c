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
	sprintf(cURL,"http://localhost/containers/%.99s/json",cId);
	char *js = json_fetch_unixsock(cURL);
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


int main(void)
{

	UpstreamConfTemplate(stdout,"odoo0","123.23.4.5","2345");
	UpstreamConfTemplate(stdout,"odoo0chat","123.23.78.6","9012");
	ServerConfTemplate(stdout,"odoo0.unxs.io","odoo0","odoo0chat");
	exit(0);

	char cURL[] = "http://localhost/containers/json";
	char *js = json_fetch_unixsock(cURL);
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

	char cId[100]={""};
	char cContainerName[256]={""};
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
			if(strstr(str,"odoo"))
			{
				sprintf(cContainerName,"%.128s",str);
				//debug only printf("cEnv=%s\n",cEnv);
				ParseFromJsonArray(cEnv,"VIRTUAL_PORT",cVirtualPort);
				char cVirtualPorts[8][32]={"","","","","","","",""};
				unsigned uNumPorts=0;
				uNumPorts=uSplitPorts(cVirtualPort,cVirtualPorts);
				ParseFromJsonArray(cEnv,"VIRTUAL_HOST",cVirtualHost);
				register int n=0;
				if(cVirtualHost[0])
				{
					FILE *fp;
					if((fp=fopen("/etc/nginx/conf.d/docker.conf","w"))!=NULL)
					{
						for(n=0;n<uNumPorts&&n<8;n++)
						{
							fprintf(fp,"upstream %s {\n"
								"	server %s:%s;\n"
								"}\n"
									,cVirtualHost,
									cContainerIp,cVirtualPorts[n]);
							fprintf(fp,"server {\n"
								"	listen 80;\n"
								"	server_name %s;\n"
								"	location / {\n"
								"		proxy_pass http://%s;\n"
								"	}\n"
								"}\n"
									,cVirtualHost,
									cVirtualHost);
						}
						fclose(fp);
					}

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

	return 0;
}//main()
