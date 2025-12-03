#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h" 
// Function definitions
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{ 
    if(argv[2][0] != '.')//check whether the file name is there or not before .
    {
        if(strstr(argv[2],".bmp") != NULL)//check the source file extension is matched or not(.bmp)
        {
            decInfo->output_image_fname = argv[2]; // assign the source image file name to structure member
        }
        // if the source file is not .bmp file 
        else
        {
            printf("Error : source file should be .bmp file\n"); // Print error message
            return e_failure; // return e_failure as 0
        }
    }
    else
    {
        printf("Error : source file name should not start with '.' \n"); // Print error message
        return e_failure; // return e_failure as 0
    }

    // Check whether secret file name is there or not
    if(argv[3] != NULL)
    {
        int len = strlen(argv[3]); // calculate length of the secret file name
        // Check whether the last character is . or not
        if(argv[3][len-1] != '.')
        {
            char *ptr = strstr(argv[3],"."); // get the pointer to . in the secret file name
            *(ptr+1) = '\0'; // terminate the string after . adding null character
        }
        decInfo->secret_fname = argv[3]; // assign the secret file name to structure member
    }
    else
    {
        decInfo->secret_fname = malloc(20);// allocate memory dynamically to secret file name
        strcpy(decInfo -> secret_fname, "output."); // assign default secret file name as output.
    }
    return e_success; // return e_success as 1    
}
// Function Defintion to open files for decoding
Status open_files_decode(DecodeInfo *decInfo)
{
    //Src Image file to decode
    decInfo->fptr_output_image = fopen(decInfo->output_image_fname,"r");
    //Do error handling
    if (decInfo->fptr_output_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_image_fname);

        return e_failure;
    }

    //Move 54 bytes to decode original data
    fseek(decInfo->fptr_output_image,54,SEEK_SET);

    //No failure return e_success
    return e_success;
}
// Function definition to decode magic string
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8], ch, str[5];
    size_t i;
    for(i = 0;i<strlen(magic_string);i++)
    {
        ch = 0;
        fread(buffer,8,1,decInfo->fptr_output_image); // Read 8 bytes from image file
        decode_byte_to_lsb(buffer,&ch); // Call Decode the byte from LSBs
        str[i] = ch; // Store the decoded character in str array
    }
    str[i] = '\0'; // Null terminate the string in str array
    printf("Decoded magic string is %s\n",str); // Print the decoded magic string
    // Compare the decoded magic string with original magic string
    if(strcmp(magic_string,str) == 0)
    {
        printf("Magic string is decoded succesfully\n");
    }
    // If not matched
    else
    {
        printf("Magic string is not decoded successfully\n");
        return e_failure; // return e_failure as 0
    }
    return e_success; // return e_success as 1

}
// Function definition to decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size = 0;
    fread(buffer,32,1,decInfo->fptr_output_image); // Read 32 bytes from image file
    decode_size_to_lsb(buffer,&size); // Call Decode the size from LSBs
    decInfo->size_secret_file_extn = size; // Store the decoded size in structure member
    printf("Length of secret file extension is : %d\n",size); // Print the size of secret file extension
    return e_success; // return e_success as 1
}
// Function definition to decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8], ch, str[5];
    size_t i;
    for(i=0;i<decInfo->size_secret_file_extn;i++)
    {
        ch = 0;
        fread(buffer,8,1,decInfo->fptr_output_image); // Read 8 bytes from image file
        decode_byte_to_lsb(buffer,&ch);// Call Decode the byte from LSBs
        str[i] = ch; // Store the decoded character in str array
    }
    str[i] = '\0'; // Null terminate the string in str array
    printf("Extension of secret file is %s\n",str);
    strcat(decInfo->secret_fname,str); // Concatenate the decoded extension to secret file name
    printf("Secret file name is %s\n",decInfo->secret_fname);
    return e_success; // return e_success as 1

}
// Function definition to decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int secret_file_size = 0;
    fread(buffer,32,1,decInfo->fptr_output_image); // Read 32 bytes from image file
    decode_size_to_lsb(buffer,&secret_file_size); // Call Decode the size from LSBs
    decInfo-> size_secret_data = secret_file_size; // Store the decoded size in structure member
    printf("secret file size is %ld\n",decInfo-> size_secret_data);
    return e_success; // return e_success as 1
}
// Function definition to decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w"); // Open the secret file in write mode
    //Do error handling
    if (decInfo->fptr_secret == NULL)
    {
        printf("Error in opening the file");
        return e_failure; // return e_failure as 0
    }
    char buffer[8], ch;
    size_t i;
    for(i=0;i<decInfo->size_secret_data;i++)
    {
        ch = 0;
        fread(buffer,8,1,decInfo->fptr_output_image); // Read 8 bytes from image file
        decode_byte_to_lsb(buffer,&ch); // Call Decode the byte from LSBs
        fwrite(&ch,1,1,decInfo->fptr_secret); // Write the decoded character to secret file
    }
    printf("Secret message is decoded successfully\n");
    return e_success; // return e_success as 1
}
// Function definition to decode byte from LSBs
Status decode_byte_to_lsb(char *image_buffer,char *data)
{
    for(size_t i = 0; i < 8; i++)
    {
        *data = (image_buffer[i] & 1) << i | *data; // Extract LSB and store in data
    }
    return e_success; // return e_success as 1

}
// Function definition to decode size from LSBs
Status decode_size_to_lsb(char *imageBuffer,int *data)
{
    for(size_t i = 0; i < 32; i++)
    {
        *data = (imageBuffer[i] & 1) << i | *data; // Extract LSB and store in data
    }
    return e_success; // return e_success as 1
}
// Function definition to perform decoding
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo) != e_success) return e_failure; //Check open files for decoding 
    if(decode_magic_string(MAGIC_STRING,decInfo) != e_success) return e_failure; // Check decode magic string
    if(decode_secret_file_extn_size(decInfo) != e_success) return e_failure; // Check decode secret file extension size
    if(decode_secret_file_extn(decInfo) != e_success) return e_failure; // Check decode secret file extension
    if(decode_secret_file_size(decInfo) != e_success) return e_failure; // Check decode secret file size
    if(decode_secret_file_data(decInfo) != e_success) return e_failure; // Check decode secret file data
    return e_success; // return e_success as 1
    
}
