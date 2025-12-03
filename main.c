/*
Name: Jyotshna Rowlajanni
Date: 05-11-2025
Description: To write a code for LSB Image Steganography(Hiding the messages under a medium)
*/
#include <stdio.h>
#include<string.h> 
#include "encode.h"
#include "decode.h"
#include "types.h"
OperationType check_operation_type(char *); // Function prototype

int main(int argc, char *argv[])
{

    // Step 1 : Check the argc >= 4 true - > step 2
    // Step 2 : Call the check_operation_type(argv[1]) == e_encode )) true - > step 3
    // Step 3 : Declare structure variable EncodeInfo enc_info
    // Step 4 : Call the read_and_validate_encode_args(argv,&enc_info)== e_success)
    //  true -> Step 5 , false - > terminate the program
    // Step 5 : Call the do_encoding (&encInfo);
    // Check the argc >= 4 
    if(argc >= 3)
    {
        // Call the check_operation_type(argv[1]) == e_encode )) 
        if(check_operation_type(argv[1]) == e_encode)
        {
            EncodeInfo enc_info; // Allocate memory for structure by declare structure variable EncodeInfo enc_info
            // Call the read_and_validate_encode_args(argv,&enc_info)== e_success)
            if(read_and_validate_encode_args(argv, &enc_info) == e_success)
            {
                //Call the do_encoding (&encInfo);
                if(do_encoding(&enc_info) == e_success)
                {
                    printf("Encoding has done successfully \n"); // Print message as encoding done
                }
                else
                {
                    printf("Encoding failed \n"); // Print error message
                    return 0; // Terminates here
                }
            }
            else
            {
                printf("Error, encoding not possible \n"); // Print error message
                return 0; // Terminates here
            }
        }
        // Call the check_operation_type(argv[1]) == e_decode ))
        else if(check_operation_type(argv[1]) == e_decode)
        {
            DecodeInfo dec_info; // Allocate memory for structure by declare structure variable DecodeInfo dec_info
            // Call the read_and_validate_decode_args(argv,&decc_info)== e_success)
            if(read_and_validate_decode_args(argv, &dec_info) == e_success)
            {
                //Call the do_decoding (&decInfo);
                if(do_decoding(&dec_info) == e_success)
                {
                    printf("Decoding has done successfully \n"); // Print message as decoding successfully
                }
                else
                {
                    printf("Decoding failed \n"); // Print error messsage
                    return 0; // Terminates here
                }
            }
            else
            {
                printf("Error, decoding not possible \n"); // Print error message
                return 0; // Terminates here
            }
        }
        else
        {
            printf("Error, argv[1] must be '-e' or '-d' \n"); // Print error message
            return 0; // Terminates here
        }
    }
    else
    {
        printf("Error, argc is must greater than or equals to 4\n"); // Print error message
        return 0; // Terminates here
    }
}

OperationType check_operation_type(char *symbol)
{
    // Step 1 : Check whether the symbol is -e or not true - > return e_encode false -> Step 2
    // Step 2 : Check whether the symbol is -d or not true - > return e_decode false -> return e_unsupported
    //Check whether the symbol is -e or not true 
    if(strcmp(symbol, "-e") == 0)
    {
        return e_encode; // return e_encode means as 0
    }
    //Check whether the symbol is -d or not true
    else if(strcmp(symbol, "-d") == 0)
    {
        return e_decode; // return e_encode means as 0
    }
    // false
    else
    {
        return e_unsupported; // return e_unsupported means as 2
    }
}
