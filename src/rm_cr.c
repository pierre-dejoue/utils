/*  Remove carriage return character (<cr> , \r, '0d' ...) in dos text files.
 *  Pierre Dejoue 2001
 */

#include <stdio.h>
#include <stdlib.h>

void usage(void);

int main(int argc, char** argv)
{
    FILE * input_file;
    FILE * tmp_file;
    char * filename;
    unsigned int c, file_idx, retval;

    retval=0;

    if(argc <= 1) { usage(); }

    for(file_idx=1; file_idx < (unsigned)argc; file_idx++)
    {
      filename = argv[file_idx];

      printf("%s: ", filename);
      /* open input file and tmp flow */
      if(!(input_file = fopen(filename,"r")))
      {
        printf("Cannot open file\n");
        retval=1;
      }
      else if(!(tmp_file = tmpfile()))
      {
        printf("Cannot open temporary file\n");
        fclose(input_file);
        retval=1;
      }
      else
      {
        /* parse the input file */
        while((c=getc(input_file))!=EOF)
        {
          if( c != 0x0d ) {
        	fprintf(tmp_file, "%c", c);
          }
        }
        /* rewind tmp flow */
        fflush(tmp_file);
        rewind(tmp_file);
        /* close then reopen the file in write mode */
        fclose(input_file);
        if(!(input_file = fopen(filename,"w")))
        {
            printf("No write access to %s\n", filename);
            fclose(tmp_file);
            retval=1;
        }
        else
        {
          /* copy the tmp flow in the original file */
          while((c=getc(tmp_file))!=EOF)
          {
            if( c != 0x0d )
            {
	          fprintf(input_file, "%c", c);
            }
          }
          /* close files and exit */
          fclose(input_file);
          fclose(tmp_file);
          printf("OK\n");
        }
      }
    }
    exit(retval);
}


void usage() {
    fprintf(stderr, "Usage: rm_cr FILE [FILE ...]\n\nRemove carriage return character from DOS text files.\n");
}
