/* This small program converts an ASCII file with hexadecimal codes
 * into a binary file.
 * Usage: ascii2bin ascii_file
 * - July 2001. Pierre Dejoue -
 */
#include <stdio.h>
#include <stdlib.h>

unsigned int get_hex(FILE * in_flow);

#define INVALID_CHARACTER (0xFFFFFFFF)

int main(int argc, char** argv)
{
    FILE * input_file;
    unsigned int  hex;
    unsigned int c;
    char data_string[2];

    char * filename = argv[1];

    if(!(input_file = fopen(filename,"r")))
    {
        fprintf(stderr, "Usage: ascii2bin <filename>\nConvert hex code into binary. Pierre Dejoue 2001.\n");
        exit(1);
    }

    while(1)
    {
	    /* Get two hexadecimal characters from the input file */
        if((hex=get_hex(input_file))==EOF)
        	break;
        data_string[0]=hex;
        if((hex=get_hex(input_file))==EOF)
        	break;
        data_string[1]=hex;
	    /* convert the hex value into a byte */
        sscanf(data_string, "%02x", &c);
        /* print the result */
        printf("%c", c);
    }

    fclose(input_file);
    exit(0);
}


/*
 * get_hex
 *
 * Description: This function reads the input flow in_flow
 * until EOF or an hexadecimal character.
 * 0, 1 .. 9, a .. f, A .. F  are interpreted as hex
 * Space, comma and newline characters are skipped
 * Other characters entails EOF and a warning to the user.
 * Output: an hex character or EOF.
 */
unsigned int get_hex(FILE * in_flow)
{
	unsigned int buf = INVALID_CHARACTER;
    static unsigned int back_up = INVALID_CHARACTER;

    if(back_up != INVALID_CHARACTER)
    {
        buf = back_up;
        back_up = INVALID_CHARACTER;
    }
	while(buf != INVALID_CHARACTER || (buf=getc(in_flow))!=EOF)
    {
		if(buf == '0')
        {
            if((back_up=getc(in_flow))!=EOF)
            {
                if(back_up == 'x')
                {
                    buf = ' ';
                    back_up = INVALID_CHARACTER;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        if((buf >= '1' && buf <= '9') ||
		   (buf >= 'A' && buf <= 'F') ||
		   (buf >= 'a' && buf <= 'f') )
        {
			break;
	    }
		if(buf != ' ' && buf != ',' && buf != '\n')
        {
			fprintf(stderr,"Warning: non hexadecimal character found. Conversion interrupted.\n");
			buf=EOF;
            break;
        }
        buf = INVALID_CHARACTER;
   }
   return buf;
}
