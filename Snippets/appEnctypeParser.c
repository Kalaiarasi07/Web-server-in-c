#include <stdio.h>
#include<string.h>

int main()
{
    char request[] = "Accept-Encoding: gzip, deflate, br\r\nAccept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n\r\nfname=kalai&lname=n&fileToUpload=doctest.png&submit=submit";
    const char *delim = "\r\n\r\n";

    char *body = strstr(request, delim);
    body = body+strlen(delim);
    printf("BODY:\n\n%s\n\n", body);
       
    char *token = strtok(body,"&");
    while(token != NULL)
    {
        printf("%s\n",token);
        token = strtok(NULL, "&");
    }
    return 0;
}
