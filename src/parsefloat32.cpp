/*
 * Display detailed information about 32-bit float numbers (IEEE 754).
 * Input can be the decimal or hexadecimal representation of a float.
 *
 * Copyright (c) 2012 Pierre DEJOUE
 */
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>


using INT32U = unsigned;
using FP32 = float;
using FP64 = double;


static_assert(4 == sizeof(INT32U), "Wrong size of INT32U");
static_assert(4 == sizeof(FP32),   "Wrong size of FP32");
static_assert(8 == sizeof(FP64),   "Wrong size of FP64");


const unsigned MANTISSA_SIZE = 23u;
const unsigned EXPONENT_SIZE = 8u;


union fp32
{
    INT32U hex;
    FP32   fp;

    struct fp32_format      // Bitfields
    {
        INT32U mantissa : MANTISSA_SIZE;
        INT32U exponent : EXPONENT_SIZE;
        INT32U sign     : 1;
    } parts;
};


enum fp_type
{
    NORMAL,
    SUBNORMAL,
    POS_ZERO,
    NEG_ZERO,
    POS_INFINITY,
    NEG_INFINITY,
    NOT_A_NUMBER
};


void print_usage()
{
    std::cerr << "Usage: parsefloat32 FLOAT [FLOAT ...]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Display detailed information about 32-bit float numbers (IEEE 754)." << std::endl;
    std::cerr << "Input can be the decimal or hexadecimal representation of a float." << std::endl;
}


void sprint_binary(char * str, INT32U val, size_t n)
{
    str[n] = '\0';    // String termination character

    for(int idx = n-1, mask = 1; idx >= 0; idx--, mask <<= 1)
    {
        str[idx] = ((val & mask) ? '1' : '0');
    }
}


int main(int argc, char *argv[])
{
    enum  fp_type fp_type;
    union fp32    input_fp32;
    int           ulp_power;
    double        ulp;
    char          mantissa_binary[MANTISSA_SIZE+1];

    if (argc <= 1)
    {
        print_usage();
        exit(1);
    }

    // For each argument in the command line
    for(int arg_idx = 1; argc-- >= 2; arg_idx++)
    {
        // Read input string

        char * input_fp32_str = argv[arg_idx];

        printf("-----------------------------------------------------------------------------------\n");
        printf("Input: %s\n", input_fp32_str);

        std::string input_fp32_string(input_fp32_str);
        if(input_fp32_string.substr(0, 2) == "0x")
        {
            int nb_parsed = sscanf(input_fp32_str + 2, "%08x", &input_fp32.hex);

            if(nb_parsed != 1)  // Skip '0x' at the beginning of the string
            {
                input_fp32.hex = 0x7FFFFFFF;
                printf("Error parsing the input data: %s. np_parsed = %d.\n\n",  input_fp32_str, nb_parsed);
                continue;
            }
        }
        else
        {
            input_fp32.fp  = static_cast<FP32>(atof(input_fp32_str));
        }

        // Analysis of the floating-point value

        fp_type = NORMAL;

        if(input_fp32.parts.exponent == 0)
        {
            fp_type   = SUBNORMAL;
            ulp_power = -126 - static_cast<int>(MANTISSA_SIZE);

            if(input_fp32.fp == 0.0)
            {
                fp_type = (input_fp32.parts.sign == 0 ? POS_ZERO : NEG_ZERO);
            }
        }
        else if(input_fp32.parts.exponent == 0xFF)
        {
            if(input_fp32.parts.mantissa != 0)
            {
                fp_type = NOT_A_NUMBER;
            }
            else
            {
                fp_type = (input_fp32.parts.sign == 0 ? POS_INFINITY : NEG_INFINITY);
            }

            ulp_power = 0;  // Not relevant
        }
        else
        {
            ulp_power = input_fp32.parts.exponent - 127 - MANTISSA_SIZE;
        }

        ulp = pow(2.0, ulp_power);

        sprint_binary(mantissa_binary, input_fp32.parts.mantissa, MANTISSA_SIZE);

        // Display information about the floating-point value

        switch(fp_type)
        {
            case NEG_ZERO:
                printf("    = -0.0 (Negative Zero)\n");
                break;

            case POS_ZERO:
                printf("    = +0.0 (Positive Zero)\n");
                break;

            case NEG_INFINITY:
                printf("    = Negative Infinity\n");
                break;

            case POS_INFINITY:
                printf("    = Positive Infinity\n");
                break;

            case NOT_A_NUMBER:
                printf("    = NAN (Not A Number)\n");
                break;

            case NORMAL:
                printf("    Normal floating-point:\n");
                printf("    Exponent              = 2^%d\n",  input_fp32.parts.exponent - 127);
                printf("    Significand           = 1.%s (1.[mantissa], in binary)\n",  mantissa_binary);
                printf("                          = %.15E     (in decimal)\n", 1.0 + input_fp32.parts.mantissa * ulp);
                break;

            case SUBNORMAL:
                printf("    Subnormal floating-point:\n");
                printf("    Exponent              = 2^-126\n");
                printf("    Significand           = 0.%s (0.[mantissa], in binary)\n", mantissa_binary);
                printf("                          = %.15E     (in decimal)\n", 0.0 + input_fp32.parts.mantissa * ulp);
                break;
        }

        printf("    Value64               = %.15E\n",             input_fp32.fp);
        printf("    Hex32                 = 0x%08X\n",            input_fp32.hex);
        printf("     [sign]     (1  bit)  = 0x%d\n",              input_fp32.parts.sign);
        printf("     [exponent] (8  bits) = 0x%02X     = %d\n",   input_fp32.parts.exponent, input_fp32.parts.exponent);
        printf("     [mantissa] (23 bits) = 0x%06X = %d\n",       input_fp32.parts.mantissa, input_fp32.parts.mantissa);

        if(fp_type == NORMAL || fp_type == SUBNORMAL)
        {
            // The IEEE float and double formats were designed so that the numbers are “lexicographically ordered”.
            // See: http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm

            union fp32 fp32_prev_value, fp32_next_value;
            if(input_fp32.parts.sign == 0)      // Positive float
            {
                fp32_prev_value.hex = input_fp32.hex - 1;
                fp32_next_value.hex = input_fp32.hex + 1;
            }
            else                                // Negative float
            {
                fp32_prev_value.hex = input_fp32.hex + 1;
                fp32_next_value.hex = input_fp32.hex - 1;
            }

            printf("\n");
            printf("    ULP (Unit in the Last Place) = 2^%d = %.15E\n", ulp_power, ulp);
            printf("\n");
            printf("    %.15E   0x%08X   (prev)\n", fp32_prev_value.fp, fp32_prev_value.hex);
            printf("    %.15E   0x%08X   (this)\n", input_fp32.fp,      input_fp32.hex);
            printf("    %.15E   0x%08X   (next)\n", fp32_next_value.fp, fp32_next_value.hex);
        }
        printf("\n");
    }

    exit(0);
}
