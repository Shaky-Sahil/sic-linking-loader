#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *iptr,*outptr;
int progaddr,progaddrcopy;
char record[100];
char starting_address[100];
int recordpointer;
void writetooutputfile();
int modification_locations[10];
int requires_modification();
int main()
{
    char temp[100];
    iptr = fopen("inputprogram.txt","r");
    outptr = fopen("outputfile.txt","w");
    if(iptr==NULL)
    {
        printf("file not open");
    }
    printf("Enter the starting address");
    //scanf("%s",temp);
    //progaddr = strtol(temp,NULL,16);
    scanf("%x",&progaddr);
    printf("the progaddr is:%x",progaddr);
    progaddrcopy = progaddr;
    fgets(record,sizeof(record)/sizeof(char),iptr);
    int i=0;
    while(record[0]!='E')
    {
        if(record[0]=='M')
        {
            strncpy(temp,record+(2),6);
            modification_locations[i] = strtol(temp,NULL,16)+progaddr;
            i++;
        }
        fgets(record,sizeof(record)/sizeof(char),iptr);
    }
    fseek(iptr,0,SEEK_SET);
    fgets(record,sizeof(record)/sizeof(char),iptr);
    while(record[0]!='E')
    {
        if(record[0]=='T')
        {
            strncpy(starting_address,record+(2),6);
            recordpointer=12;
            writetooutputfile();
        }
        fgets(record,sizeof(record)/sizeof(char),iptr);
    }
    fclose(iptr);
    fclose(outptr);
    return 0;
}

void writetooutputfile()
{
    char extendedobjectcode[10];
    char objectcode[7];
    while(record[recordpointer]!='\0'){
    fprintf(outptr,"%lx\t",progaddr+strtol(starting_address,NULL,16));
    if(requires_modification()==1)
    {
        strncpy(extendedobjectcode,record+(recordpointer),8);
        fprintf(outptr,"%lx\n",strtol(extendedobjectcode,NULL,16)+progaddrcopy);
        recordpointer +=9;
        progaddr+=4;
    }
    else{
    strncpy(objectcode,record+(recordpointer),6);
    fprintf(outptr,"%s\n",objectcode);
    recordpointer +=7;
    progaddr+=3;
    }
    if(record[recordpointer]=='^')
    {
        recordpointer++;
    }
    }

}
int requires_modification()
{
    int i=0;
    while(i<sizeof(modification_locations)/sizeof(int))
    {
        if(progaddr+strtol(starting_address,NULL,16)+1==modification_locations[i])
        {
            printf("modification required");
            return 1;
        }
        i++;
    }
    return 0;
}
