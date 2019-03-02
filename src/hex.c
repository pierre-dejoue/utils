/*
 * This small program converts any binary file into a readable ASCII file.
 * Usage: hex file_name
 * - July 2001 Pierre Dejoue -
 */

#include <stdio.h>
#include <stdlib.h>

#define LINE_SZ 16
#define TRUE 1
#define FALSE 0

int main(int argc, char** argv)
{
    FILE * input_file;
    unsigned char line[LINE_SZ+1];
    unsigned int line_counter;
    int line_sz, ind, end_of_file;
    char * filename = argv[1];

    if(!(input_file = fopen(filename,"r")))
    {
        fprintf(stderr, "Usage: hex <filename>\nRead a binary file. Pierre Dejoue 2001.\n");
        exit(1);
    }

    line_counter=0;
    line[16]='\0';    /* string ending */
    end_of_file=FALSE;
    while(end_of_file==FALSE)
    {
        /* read a line, check end of file */
        line_sz=fread(line, 1, LINE_SZ, input_file);
        if(line_sz != LINE_SZ)
            end_of_file=TRUE;

        /* print line counter */
	    printf("%08x ", line_counter * LINE_SZ);

        /* print line in hex */
        for(ind=0; ind<16; ind++)
        {
            if(ind%4==0)
               printf(" ");
            if(ind<line_sz)
               printf("%02x", line[ind]);
            else
               printf("  ");
        }

        /* print line in ASCII. replace non-ascii by a dot */
        for(ind=0; ind<16; ind++)
          if(ind<line_sz)
          {
            if(line[ind]<32 || line[ind]>126)
              line[ind]='.';
          } else {
            line[ind]='\0';
          }
        printf("    %s\n", line);

        line_counter++;
    }

    fclose(input_file);
    exit(0);
}

