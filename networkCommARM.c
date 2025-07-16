#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <curl/curl.h>

#define OK 0
#define INIT_ERR 1
#define REQ_ERR 2
#define USE_ERR 3

int print_usage()
{
    printf("usage: test [options] \n");
	printf("\t-o -u <address> <text>, --POST \n");
	printf("\t-g -u <address>, --GET\n");
	printf("\t-p -u <address>, --POST\n");
	printf("\t-d -u <address>, --DELETE\n");
	printf("\t-h, for help\n");

	return USE_ERR;
}

int main(int argc, char *argv[])
{
    int option;
    uint32_t verbSelected = 0;

	char urlBuffer[128];
	char txtBuffer[256];

	CURL	*curl;
	CURLcode  res;
	struct curl_slist *headers = NULL;

	curl = curl_easy_init();
	if (curl)
		printf("pass\n");
	else
		printf("fail\n");

    while((option = getopt(argc,  argv, "ogpdu:h")) !=-1)
    {
        switch(option)
        {
			case 'h': // --HELP
				print_usage();
				break;
            case 'o': // --POST
				verbSelected == 0x0u ? verbSelected = 0x1111u: print_usage();
				break;
            case 'g': // -- GET
				verbSelected == 0x0u ? verbSelected = 0x3333u : print_usage();
				printf("so far so good\n");
				break;
            case 'p': // -- PUT
				verbSelected == 0x0u ? verbSelected = 0x5555u : print_usage();
				break;
            case 'd': // -- DELETE
				verbSelected == 0x0u ? verbSelected = 0x7777u : print_usage();
				break;
			case 'u': // --url
			 	if (verbSelected == 0u)
				{
					printf("ERROR: Please select HTTP verb first.\n");
					print_usage();
				}
				strcpy(urlBuffer, optarg);
				printf("buffer is: %s\n", urlBuffer);
				switch (verbSelected)
				{
					case 0x1111u: // --POST
						if (curl)
						{
							strcpy(txtBuffer, optarg);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, txtBuffer);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(txtBuffer));
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_cleanup(curl);
						}
						else
						{
							return INIT_ERR;
						}
						break;
					case 0x3333u: // -- GET
						if (curl) 
						{
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_cleanup(curl);
							return OK;
						} 
						else 
						{
							return INIT_ERR;
						}
						break;
					case 0x5555u: // -- PUT
						if (curl)
						{
							strcpy(txtBuffer, optarg);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
							curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, txtBuffer);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(txtBuffer));
							headers = curl_slist_append(headers, "Content-Type: text/plain");
							curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_cleanup(curl);
						}
						else
						{
							return INIT_ERR;
						}
						break;
					case 0x7777u: // -- DELETE
						if (curl)
						{
							strcpy(txtBuffer, optarg);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_cleanup(curl);
						}
						else
						{
							return INIT_ERR;
						}
						break;
					default:
				}
        }
    }

	return -1;
}
