#include<stdio.h>
#include<string.h>
void data_parser(char item[])
{
    printf("data parser: \n%s",item);
}
void file_parser(char item[])
{
    printf("file parser: %s",item);
}
void extract_data(char item[])
{
    static int i = 1;
    printf("item%d: %s\n",i++,item);
    char item1[100];
    strcpy(item1,item);

//     char *firstLine;
//     firstLine = strtok(item1,"\r\n");
    // printf("firstline: %s\n",firstLine);
    // if(strstr(firstLine,"name") != NULL)
    // {
    //     // data_parser(item);
    //     printf("data parser: ");

    // }
    // else if (strstr(firstLine,"filename") != NULL)
    // {
    //     // file_parser(item);
    //     printf("file parser: ");
    // }
    

}
int main()
{
    char buf[] = 
    // "-----------------------------16596797181037153033630983533\r\n"
    "Content-Disposition: form-data; name=\"fname\"\r\n"
    "\r\n"
    "Kaz\r\n"
    "-----------------------------16596797181037153033630983533\r\n"
    "Content-Disposition: form-data; name=\"fname\"\r\n"
    "\r\n"
    "Inej\r\n"
    "-----------------------------16596797181037153033630983533\r\n"
    "Content-Disposition: form-data; name=\"fileToUpload\"; filename=\"hello.txt\"\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "Hello this is Kalai\r\n"
    "ddfffffffff \r\n"
    "Hello this is Kalai\r\n"
    "Hello this is Kalai\r\n"
    "the end\r\n"
    "\r\n"
    "-----------------------------16596797181037153033630983533\r\n";

    // printf("%s",buf);
    char boundary[] = "-----------------------------16596797181037153033630983533";

    char item[100],*line;
    // sends item between boundaries to extract data
    line = strtok(buf,"\r\n");
    printf("line: %s\n",line);
    while(line != NULL)
    {
        // printf("line: %s\n",line);
        if(strcmp(line,boundary) != 0)
        {
            strcat(item,line);
            strcat(item,"\r\n");
        }
        else
        {
            printf("calling extract data\n");
            extract_data(item);
            memset(item, 0, strlen(item));
            
        }
        line = strtok(NULL,"\r\n");
        printf("line:  %s\n",line);
        
        
    }
    


    return 0;
}
