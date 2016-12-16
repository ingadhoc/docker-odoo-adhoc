#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curl/curl.h>
#include "jsmn.h"

#include "json.h"
#include "log.h"
#include "buf.h"

char URL[] = "http://localhost/containers/json";

int main(void)
{
    char *js = json_fetch_unixsock(URL);

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

	char cId[256]={""};
	char cContainerName[256]={""};
	char cContainerIp[256]={""};
    for (size_t i = 0, j = 1; j > 0; i++, j--)
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
        	char *str = json_token_tostr(js, t2);
		sprintf(cId,"Id=%.128s\n",str);
		cContainerName[0]=0;
		cContainerIp[0]=0;
	}
	if (t->type == JSMN_STRING && json_token_streq(js, t, "io.rancher.container.name"))
	{
       		jsmntok_t *t2 = &tokens[i+1];
       		char *str = json_token_tostr(js, t2);
		if(strstr(str,"odoo"))
		{
			sprintf(cContainerName,"io.rancher.container.name=%.128s\n",str);
			printf("%s",cId);
			printf("%s",cContainerName);
			printf("%s",cContainerIp);
		}
	}
	if (t->type == JSMN_STRING && json_token_streq(js, t, "io.rancher.container.ip"))
	{
       		jsmntok_t *t2 = &tokens[i+1];
       		char *str = json_token_tostr(js, t2);
		sprintf(cContainerIp,"io.rancher.container.ip=%s\n",str);
		//printf("%s",cId);
		//printf("%s",cContainerName);
		//printf("%s",cContainerIp);
	}
    }

    return 0;
}
