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

// prints various command line arguments formats for selection
int print_usage()
{
    printf("usage: test [options] \n");
	printf("\t-o -u <address> <text>    , --POST \n");
	printf("\t-g -u <address>           , --GET\n");
	printf("\t-p -u <address> <text>    , --POST\n");
	printf("\t-d -u <address><directory>, --DELETE\n");
	printf("\t-h, for help\n");

	return USE_ERR;
}

// returns concatnated string of arg values from main()
char* getStr(int count, char* dest, char* src[])
{
	for (int i = 4; i < count; i++)
	{
		if (i != 4)
		{
			strcat(dest, " ");
		}
		strcat(dest, src[i]);
	}
	return dest;
}

int main(int argc, char *argv[])
{
    int option;
    uint32_t verbSelected = 0;

	char urlBuffer[128];
	char txtBuffer[256];

	CURL	*curl;
	CURLcode  res;
	long http_code = 0L;
	struct curl_slist *headers = NULL;

	curl = curl_easy_init();
/*	if (curl)
		printf("pass\n");
	else
		printf("fail\n");
*/
    while((option = getopt(argc,  argv, "ogpdu:h")) !=-1)
    {
        switch(option)
        {
			case 'h': // --HELP
				print_usage();
				break;
            case 'o':
				verbSelected == 0x0u ? verbSelected = 0x1111u: print_usage();
				break;
            case 'g':
				verbSelected == 0x0u ? verbSelected = 0x3333u : print_usage();
//				printf("so far so good\n");
				break;
            case 'p':
				verbSelected == 0x0u ? verbSelected = 0x5555u : print_usage();
				break;
            case 'd':
				verbSelected == 0x0u ? verbSelected = 0x7777u : print_usage();
				break;
			case 'u': // --url
			 	if (verbSelected == 0u)
				{
					printf("ERROR: Please select HTTP verb first.\n");
					print_usage();
				}
				
				strcpy(urlBuffer, optarg);
				// printf("url buffer: %s\n", urlBuffer);
				
				switch (verbSelected)
				{
					case 0x1111u: // --POST
						if (curl)
						{
							getStr(argc, txtBuffer, argv);
							// printf("text buffer: %s\n", txtBuffer);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, txtBuffer);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(txtBuffer));
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
							printf("HTTP Status Code: %ld\n", http_code);
							curl_easy_cleanup(curl);
							return OK;
						}
						else
						{
							return INIT_ERR;
						}
						break;
					case 0x3333u: // --GET
						if (curl) 
						{
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
                            printf("HTTP Status Code: %ld\n", http_code);
							curl_easy_cleanup(curl);
							return OK;
						} 
						else 
						{
							return INIT_ERR;
						}
						break;
					case 0x5555u: // --PUT
						if (curl)
						{
							strcpy(txtBuffer, optarg);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
                            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, txtBuffer);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(txtBuffer));
							headers = curl_slist_append(headers, "Content-Type: text/plain");
							curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
                            printf("HTTP Status Code: %ld\n", http_code);
							curl_easy_cleanup(curl);
						}
						else
						{
							return INIT_ERR;
						}
						break;
					case 0x7777u: // --DELETE
						if (curl)
						{
							strcpy(txtBuffer, optarg);
							curl_easy_setopt(curl, CURLOPT_URL, urlBuffer);
							curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
							curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
							res = curl_easy_perform(curl);
							if (res != CURLE_OK) 
							{
								return REQ_ERR;
							}
							curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &http_code);
	                        printf("HTTP Status Code: %ld\n", http_code);
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
