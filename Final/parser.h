int parse_status = 1;

 /*function to get a line from a string */
char *get_nextline(char *bufp,char line[])
{
    /* Desc: takes a pointer to a string and a line array to store nextline and returns pointer to remaining string */

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
/* function to parse key-value items from form data */
void data_parser(char item[])
{
    
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
    printf("\nvalue:%s\n",value);
}
/* function to parse file data */
void file_parser(char item[])
{
    
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

    /* writing to file */

    FILE *fptr;
    fptr = fopen(fname, "w");
    if (fptr == NULL) {
        printf("Error!");
        parse_status = 0;
        return;
    }

    while(*keyvalue != '\0')
    {
        fprintf(fptr, "%c", *keyvalue);
        keyvalue++;
    }
    
    printf("\nFile successfully written\n");


}
void parse_data(char item[])
{
    // static int i = 1;
    // printf("item%d:\n%s\n",i++,item);

    char item1[strlen(item)]; 
    strcpy(item1,item);

    char *firstLine;
    firstLine = strtok(item1,"\r\n");
    // printf("firstline: %s\n",firstLine);

     /* find if it is a file item */

    if (strstr(firstLine,"filename") != NULL) 
    {
        file_parser(item);
       
    }
    else if(strstr(firstLine,"name") != NULL)
    {
        data_parser(item);
    }

}

/*function to parse request of content-type : multipart/form-data*/
int multiparttype_parser(char *bufp)
{
    
     /* body of request to be parsed */
    char boundary[100];
    int i=0;
    while(*bufp != '\r')
        boundary[i++] = *bufp++;
    boundary[i++] = '\r';
    boundary[i++] = '\n';
    boundary[i] = '\0';

    bufp += 2;

 printf("bound:\n%s",boundary);
//  printf("body to parse:\n%s",body);

    char item[1000];  /* one data item between boundaries */
    memset(item, 0, strlen(item));
    
   
    char line[100];
    while( bufp = get_nextline(bufp,line))
    {
        // printf("%s",line);
        if(strcmp(line,boundary) != 0) /* if not a boundary line, append to item */
        {
           
            strcat(item,line);            
        }
        else  /* if boundary line, send item for parsing */
        {
        
            parse_data(item);
            memset(item, 0, strlen(item));
            
        }
            
    }    

 return parse_status;
    
}
/*function to parse request of content-type : application/x-www-form-urlencoded*/
int applicationtype_parser(char *body)
{
    
    char *token = strtok(body,"&");
    while(token != NULL)
    {
        printf("%s\n",token);
        token = strtok(NULL, "&");
    }
    return parse_status;
}
