#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Only argument is name of card ie card.raw\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[1];
    char filename[10];
    int counter = 0;
    bool already_found_jpeg = false;
    bool start_of_new_JPEG = true;
    unsigned char buffer[512];
    FILE *global_pointer = NULL;
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    //repeat until end of card
    while(fread(buffer, 512, 1, inptr) == 1)//if true run the do loop again //read 512 bytes into a buffer
    {
        //Start of new jpeg?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            if (buffer[3] == 0xe0 || buffer[3] == 0xe1 || buffer[3] == 0xe4 || 
                buffer[3] == 0xe5 || buffer[3] == 0xe6 || buffer[3] == 0xe7 || 
                buffer[3] == 0xe8 || buffer[3] == 0xe9 || buffer[3] == 0xea || 
                buffer[3] == 0xeb || buffer[3] == 0xec || buffer[3] == 0xed || 
                buffer[3] == 0xee || buffer[3] == 0xef                        )
                {
                    if (start_of_new_JPEG && !already_found_jpeg)
                    {

                        //open a new file
                        sprintf(filename, "%03i.jpg", counter);
                        FILE *img = fopen(filename, "w");
                        global_pointer = img;
                        if (img == NULL)
                        {
                            fclose(img);
                            fprintf(stderr, "Could not create %s.\n", filename);
                            return 3;
                        }                        
                        //write the chunk of data
                        fwrite(buffer, 512, 1, img);
                        already_found_jpeg = !already_found_jpeg;
                    }
                    else if (start_of_new_JPEG && already_found_jpeg )
                    {
                        fclose(global_pointer);

                        sprintf(filename, "%03i.jpg", counter);
                        FILE *img = fopen(filename, "w");
                        global_pointer = img;
                        if (img == NULL)
                        {
                            fclose(img);
                            fprintf(stderr, "Could not create %s.\n", filename);
                            return 3;
                        }
                        fwrite(buffer, 512, 1, img);
                    }
                    counter++;
                }
        }   else if (already_found_jpeg) //already found a jpeg
            {
                //code runs when program has already found a jpeg file
                fwrite(buffer, sizeof(buffer), 1, global_pointer);

            }
            else
            {

            }
    }
    // close infile
    fclose(inptr);
}