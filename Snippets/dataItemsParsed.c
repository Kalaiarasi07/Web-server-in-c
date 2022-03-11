#include<stdio.h>
#include<string.h>

// takes a pointer to a string and a line array to store nextline and returns pointer to remaining string
char *ggetline(char *bufp,char line[])
{
    memset(line, 0, strlen(line));
    int i=0;
    while(*bufp != '\n' && *bufp!='\0')
    {
        line[i++] = *bufp++;
    }
    
    line[i++] = '\n';
    line[i] = '\0';
    
   if(*bufp == '\0')
   {
       return NULL;
   }
    return bufp+1;
    
    
}

void data_parser(char item[])
{
    // printf("data parser: \n%s",item);
    char find[] = "name=\"";
    char *keyvalue = strstr(item,find);
    keyvalue += strlen(find);

    char key[100];int i=0;
    while(*keyvalue != '"')
    {
        key[i++] = *keyvalue++;
    }
    key[i] = '\0';
    printf("key:%s",key);

    keyvalue++;
    i=0;
    char value[100];
    while(*keyvalue != '\0')
    {
        if(*keyvalue !=' ' && *keyvalue!='\r' && *keyvalue!='\n')
            value[i++] = *keyvalue;
        keyvalue++;
    }
    value[i] = '\0';
    printf("\nvalue:%s",value);
}
void file_parser(char item[])
{
    // printf("file parser: %s",item);

}
void extract_data(char item[])
{
    // static int i = 1;
    // printf("item%d:\n%s\n",i++,item);
    char item1[strlen(item)];
    strcpy(item1,item);

    char *firstLine;
    firstLine = strtok(item1,"\r\n");
    // printf("firstline: %s\n",firstLine);

    if (strstr(firstLine,"filename") != NULL)
    {
        file_parser(item);
       
    }
    else if(strstr(firstLine,"name") != NULL)
    {
        data_parser(item);
    }

}
int main()
{
    char buf[] = 
    // "-----------------------------16596797181037153033630983533\r\n"
    "Content-Disposition: form-data; name=\"fname\"\r\n"
    "\r\n"
    "Kaz\r\n"
    "-----------------------------16596797181037153033630983533\r\n"
    "Content-Disposition: form-data; name=\"lname\"\r\n"
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
    char boundary[] = "-----------------------------16596797181037153033630983533\r\n";

    char item[1000];
    memset(item, 0, strlen(item));
    // sends item between boundaries to extract data
    
    char *bufp = buf;
    char line[100];
    while( bufp = ggetline(bufp,line))
    {
        // printf("%s",line);
        if(strcmp(line,boundary) != 0)
        {
        //    char *p = line;
            strcat(item,line);
            // strcat(item,"\r\n");
        }
        else
        {

            extract_data(item);
            memset(item, 0, strlen(item));
            
        }
            
    }
    


    return 0;
}
