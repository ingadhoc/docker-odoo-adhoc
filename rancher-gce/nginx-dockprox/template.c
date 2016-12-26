/*
FILE
	template.c
PURPOSE
NOTES
AUTHOR/LEGAL
	(C) 2006-2017 Gary Wallis for Unixservice, LLC.
	GPLv2 license applies. See LICENSE file included.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "template.h"

//Pass the template contents in one buffer: cTemplate
//Variables {{cLikeThis}} in the cTemplate are replaced if t_template
//setup correctly.
void Template(char *cTemplate, struct t_template *template, FILE *fp)
{
	register int i,uState=0,j=0,k=0;
	char cVarName[256]={""};
	char cVarNameS1[1024]={""};

	for(i=0;cTemplate[i];i++)
	{
		if(cTemplate[i]=='{' && uState==0 && cTemplate[i+1]=='{')
		{
			uState=1;
			continue;
		}
		else if(cTemplate[i]=='{' && uState==1 )
		{
			uState=2;
			continue;
		}
		else if(cTemplate[i]=='}' && uState==2 )
		{
			uState=3;
			continue;
		}
		else if(cTemplate[i]=='}' && uState==1 )
		{
			uState=0;
			cVarNameS1[k]=0;
			fprintf(fp,"{%s}",cVarNameS1);
			k=0;
			continue;
		}
		else if(cTemplate[i]=='}' && uState==3 )
		{
			register int n=0,uMatch=0;

			uState=0;
			cVarName[j]=0;
			while(template->cpName[n][0])
			{
				if(!strcmp(template->cpName[n],cVarName))
				{
					fprintf(fp,"%s",template->cpValue[n]);
					uMatch=1;
				}
				n++;
				if(n>99) break;
			}
			if(!uMatch)
			{
				if(cVarName[0]=='f')
				{
					//Every application that links to this lib
					//needs to supply this function
					AppFunctions(fp,cVarName);
				}
			}
			j=0;
			continue;
		}
		if(uState==1)
		{
			if(k>1023) return;
			cVarNameS1[k++]=cTemplate[i];
			continue;
		}
		else if(uState==2)
		{
			if(j>254) return;
			cVarName[j++]=cTemplate[i];
			continue;
		}
		fputc((int)cTemplate[i],fp);
	}

}//int Template()
