#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size 
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
// Function definition to get image size for bmp
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3u;
}
// Function definition to get file size
uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
// Function definition to read and validate encode arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Validate src image file name
    if(argv[2][0] != '.')
    {
        // Check whether the source file extension is .bmp
        if(strstr(argv[2] , ".bmp") != NULL)
        {
            encInfo -> src_image_fname = argv[2]; // assign the source image file name to structure member
        }
        else
        {
            printf(" Error, image file name should end with '.bmp' \n");
            return e_failure; // return e_failure as 0
        }
    }
    //  If the source file name starts with .
    else
    {
        printf("Error, image file name should starts with '.' \n");
        return e_failure; // return e_failure as 0
    }
    // Validate secret file name
    if(argv[3][0] != '.')
    {
        // Check whether the secret file name extension is .txt or .bmp or .h or .sh
        if((strstr(argv[3] , ".txt") != NULL))
        {
            encInfo -> secret_fname = argv[3]; // assign the secret file name to structure member
            strcpy(encInfo -> extn_secret_file, "txt"); // copy the secret file extension to structure member
        }
        //  If the secret file name ends with .bmp
        else if((strstr(argv[3] , ".bmp") != NULL))
        {
            encInfo -> secret_fname = argv[3]; // assign the secret file name to structure member
            strcpy(encInfo -> extn_secret_file, "bmp"); // copy the secret file extension to structure member
            
        }
        //  If the secret file name ends with .h
        else if((strstr(argv[3] , ".h") != NULL))
        {
            encInfo -> secret_fname = argv[3]; // assign the secret file name to structure member
            strcpy(encInfo -> extn_secret_file, "h"); // copy the secret file extension to structure member

        }
        //  If the secret file name ends with .sh
        else if((strstr(argv[3] , ".sh") != NULL))
        {
            encInfo -> secret_fname = argv[3]; // assign the secret file name to structure member
            strcpy(encInfo -> extn_secret_file, "sh"); // copy the secret file extension to structure member

        }
        // If the secret file name extension is not matched
        else
        {
            printf(" Error, secret file name should end with '.txt' or '.bmp' or '.h' or '.sh' \n");
            return e_failure; // return e_failure as 0
        }
    }
    // If the secret file name starts with .
    else
    {
        printf("Error, secret file name should starts with '.' \n");
        return e_failure; // return e_failure as 0
    }
    // Validate stego image file name
    if(argv[4] != NULL)
    {
        // Check whether the stego file name starts with .
        if(argv[4][0] != '.')
        {
            // Check whether the stego file extension is .bmp
            if(strstr(argv[4] , ".bmp") != NULL)
            {
                encInfo -> stego_image_fname = argv[4]; // assign the stego image file name to structure member
            }
            else
            {
                printf(" Error, output file name should end with '.bmp' \n");
                return e_failure; // return e_failure as 0
            }
        }
        else
        {
            printf("Error, output file name should starts with '.' \n");
            return e_failure; // return e_failure as 0
        }
    }
    else
    {
        encInfo -> stego_image_fname = "default.bmp"; // assign default stego image file name to structure member
    }
    return e_success; // return e_success as 1
}
// Function definition to open files for encoding
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}
// Function definition to check capacity
Status check_capacity(EncodeInfo *encInfo)
{
   encInfo -> image_capacity =  get_image_size_for_bmp(encInfo -> fptr_src_image); // Get the image capacity
   encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret); // Get the secret file size
   char *exten = strstr(encInfo -> secret_fname, ".txt"); // Get the secret file extension
   int exten_size = strlen(exten); // Calculate the length of secret file extension
    // Calculate total bytes to store
   int total_bytes = 54 + (strlen(MAGIC_STRING) * 8) + 32 + (exten_size * 8) + 32 + ((encInfo -> size_secret_file) * 8);
    // Check whether the image capacity is greater than total bytes to store
   if( encInfo -> image_capacity > total_bytes)
   {
        //printf("Image capacity is greater than total bytes \n");
        return e_success; // return e_success as 1
   }
   else
   {
        printf("Error, total bytes should be lesser than image capacity \n");
        return e_failure; // return e_failure as 0
   }

}
// Function definition to copy bmp header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer, 54, 1, fptr_src_image); // Read 54 bytes from source image file
    fwrite(buffer, 54, 1, fptr_dest_image); // Write 54 bytes to dest image file
    // Check whether the header is copied or not
    if(ftell(fptr_src_image) == ftell(fptr_dest_image)) 
    {
        //printf("Copied header successfully \n");
        return e_success; // return e_success as 1
    }
    else
    {
        printf("Header not copied \n");
        return e_failure; // return e_failure as 0

    }
}
// Function definition to encode magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    // Encode each character of magic string
    while(*magic_string)
    {
        fread(buffer, 8, 1, encInfo -> fptr_src_image); // Read 8 bytes from source image file
        encode_byte_to_lsb(*magic_string, buffer); // Call Encode the byte to LSBs
        fwrite(buffer, 8, 1, encInfo -> fptr_stego_image); // Write 8 bytes to stego image file
        magic_string++; // Move to next character
    }
    // Check whether the magic string is encoded or not
    if(ftell(encInfo -> fptr_src_image) == ftell(encInfo -> fptr_stego_image))
    {
        return e_success; // return e_success as 1
    }
    else
    {
        printf("Error , magic string not encoded \n");
        return e_failure; // return e_failure as 0
    }
}
//  Function definition to encode secret file extension size
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo -> fptr_src_image); // Read 32 bytes from source image file
    encode_size_to_lsb(size, buffer); // Call Encode the size to LSBs
    fwrite(buffer, 32, 1, encInfo -> fptr_stego_image); // Write 32 bytes to stego image file
    // Check whether the secret file extension size is encoded or not
    if(ftell(encInfo -> fptr_src_image) == ftell(encInfo -> fptr_stego_image))
    {
        return e_success; // return e_success as 1
    }
    else
    {
        printf("Error , secret file extension size is not encoded \n");
        return e_failure; // return e_failure as 0
    }
    
}
// Function definition to encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    for (int i = 0; i < strlen(file_extn); i++)
    {
        fread(buffer, 8, 1, encInfo -> fptr_src_image); // Read 8 bytes from source image file
        encode_byte_to_lsb(file_extn[i], buffer); // Call Encode the byte to LSBs
        fwrite(buffer, 8, 1, encInfo -> fptr_stego_image); // Write 8 bytes to stego image file
    }
    //  Check whether the secret file extension is encoded or not
    if(ftell(encInfo -> fptr_src_image) == ftell(encInfo -> fptr_stego_image))
    {
        return e_success; // return e_success as 1
    }
    else
    {
        printf("Error , secret file extension is not encoded \n");
        return e_failure; // return e_failure as 0
    }
}
// Function definition to encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo -> fptr_src_image); // Read 32 bytes from source image file
    encode_size_to_lsb(file_size, buffer); // Call Encode the size to LSBs
    fwrite(buffer, 32, 1, encInfo -> fptr_stego_image); // Write 32 bytes to stego image file
    // Check whether the secret file size is encoded or not
    if(ftell(encInfo -> fptr_src_image) == ftell(encInfo -> fptr_stego_image))
    {
        return e_success; // return e_success as 1
    }
    else
    {
        printf("Error , secret file size is not encoded \n");
        return e_failure; // return e_failure as 0
    }
}
//  Function definition to encode secret file data 
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_secret); // Move the file pointer to the beginning of secret file
    fread(encInfo -> secret_data, encInfo -> size_secret_file, 1, encInfo -> fptr_secret); // Read the secret file data
    char buffer[8];
    for (int i = 0; i < encInfo -> size_secret_file; i++)
    {
        fread(buffer, 8, 1, encInfo -> fptr_src_image); // Read 8 bytes from source image file
        encode_byte_to_lsb(encInfo -> secret_data[i], buffer); // Call Encode the byte to LSBs
        fwrite(buffer, 8, 1, encInfo -> fptr_stego_image); // Write 8 bytes to stego image file
    }
    // Check whether the secret file data is encoded or not
    if(ftell(encInfo -> fptr_src_image) == ftell(encInfo -> fptr_stego_image))
    {
        return e_success; //    return e_success as 1
    }
    else
    {
        printf("Error , secret file data is not encoded \n");
        return e_failure; // return e_failure as 0
    }
}
// Function definition to copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    // Copy remaining data from source image to dest image
    while (fread(&ch, 1, 1, fptr_src) == 1) {
        fwrite(&ch, 1, 1, fptr_dest); // Write the byte to dest image file
    }
    return e_success; // return e_success as 1

}
// Function definition to encode byte to LSBs
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i] = image_buffer[i] & (~1) | (data >> i) & 1; // Set the LSB with data bit
    }
}
// Function definition to encode size to LSBs
Status encode_size_to_lsb(int size, char *imageBuffer)
{
    for(int i=0;i<32;i++)
    {
        imageBuffer[i] = imageBuffer[i] & (~1) | (size >> i) & 1; // Set the LSB with size bit
    }
}
// Function definition to perform encoding
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) != e_success) return e_failure; //Check open files for encoding
    if(check_capacity(encInfo) != e_success) return e_failure; //Check capacity
    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) != e_success) return e_failure; //  Check Copy bmp header
    if(encode_magic_string(MAGIC_STRING , encInfo) != e_success) return e_failure; // Check Encode magic string
    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) != e_success) return e_failure; // Check Encode secret file extension size
    if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) != e_success) return e_failure; // Check Encode secret file extension
    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) != e_success) return e_failure; // Check Encode secret file size
    if(encode_secret_file_data(encInfo) != e_success) return e_failure; // Check Encode secret file data
    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) != e_success) return e_failure; // Check Copy remaining image data
    return e_success; // return e_success as 1
}
