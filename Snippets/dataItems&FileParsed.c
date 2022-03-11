#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// takes a pointer to a string and a line array to store nextline and returns pointer to remaining string
char *get_nextline(char *bufp,char line[])
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
//function to parse key-value items from form data
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
//function to parse file data 
void file_parser(char item[])
{
    // printf("file parser: %s",item);
    char find[] = "filename=\"";
    char *keyvalue = strstr(item,find);
    keyvalue += strlen(find);

    char fname[100];int i=0;
    while(*keyvalue != '"')
    {
        fname[i++] = *keyvalue++;
    }
    fname[i] = '\0';
    printf("fname:%s",fname);

    keyvalue++;

    //writing to file

    FILE *fptr;
    fptr = fopen(fname, "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    while(*keyvalue != '\0')
    {
        fprintf(fptr, "%c", *keyvalue);
        keyvalue++;
    }
    
    printf("\nfile successfully written");


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
    "Content-Disposition: form-data; name=\"fileToUpload\"; filename=\"temp.txt\"\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "This is the first line\r\n"
    "ddfffffffff \r\n"
    "sample text  goes here\r\n"
    "sample text  goes here\r\n"
    "the end:)\r\n"
    "\r\n"
    "-----------------------------16596797181037153033630983533\r\n";

    // printf("%s",buf);
    char boundary[] = "-----------------------------16596797181037153033630983533\r\n";

    char item[1000];
    memset(item, 0, strlen(item));
    
    char *bufp = buf;
    char line[100];
    while( bufp = get_nextline(bufp,line))
    {
        // printf("%s",line);
        if(strcmp(line,boundary) != 0)
        {
            strcat(item,line);
            
        }
        else
        {

            extract_data(item);
            memset(item, 0, strlen(item));
            
        }
            
    }
    


    return 0;
}
