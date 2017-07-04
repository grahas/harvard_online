#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
// ensure proper usage
if (argc != 4)
{
    fprintf(stderr, "Usage: ./resize # infile outfile\n");
    return 1;
}
// amount to resize the program
int n = atoi(argv[1]);

// remember filenames
char *infile = argv[2];
char *outfile = argv[3];

// open input file 
FILE *inptr = fopen(infile, "r");
if (inptr == NULL)
{
    fprintf(stderr, "Could not open %s.\n", infile);
    return 2;
}

// open output file
FILE *outptr = fopen(outfile, "w");
if (outptr == NULL)
{
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile);
    return 3;
}

// read infile's BITMAPFILEHEADER
BITMAPFILEHEADER bf;
fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

// read infile's BITMAPINFOHEADER
BITMAPINFOHEADER bi;
fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

// ensure infile is (likely) a 24-bit uncompressed BMP 4.0
if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
    bi.biBitCount != 24 || bi.biCompression != 0)
{
    fclose(outptr);
    fclose(inptr);
    fprintf(stderr, "Unsupported file format.\n");
    return 4;
}
// update header
//width
int original_file_line_width = bi.biWidth;
int a = bi.biWidth;
bi.biWidth = a * n;

//height
int original_file_line_Height = bi.biHeight;
int b = bi.biHeight;
bi.biHeight = b * n;    

// determine padding for scanlines
int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
//int original_padding = (4 - (original_file_line_width * sizeof(RGBTRIPLE)) % 4) % 4;

//image size, total size of image for pixels and padding
bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);

//file size
bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

// write outfile's BITMAPFILEHEADER
fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

// write outfile's BITMAPINFOHEADER
fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

//dynamic memory space for storing a single read line of the pic 
RGBTRIPLE line[bi.biWidth + padding/3 ];
//printf("%lu",sizeof(line));

// iterate over infile's scanlines
for (int i = 0, o = abs(original_file_line_Height); i < o; i++)
{

    // iterate over pixels in scanline
    for (int j = 0; j < original_file_line_width; j++)
    {
        // temporary storage
        RGBTRIPLE triple;

        // read RGB triple from infile
        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

        for (int k = 0; k < n; k ++)
        {
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
    }

    // skip over padding, if any
    fseek(inptr, padding, SEEK_CUR);

    // then add it back (to demonstrate how)
    for (int k = 0; k < padding; k++)
    {
        fputc(0x00, outptr);
    }
    //printf("%lu",sizeof(line));
    for(int r = 0; r < n - 1; r++)
    {
        fseek(outptr, -(int)(sizeof(line)), SEEK_CUR);//move file pointer to the begining of the line
        fread(line, (int)sizeof(line), 1, outptr);//read the entire row of pixels into line
        fwrite(line, (int)sizeof(line), 1, outptr);//write the contense of line

    }
}

// close infile
fclose(inptr);

// close outfile
fclose(outptr);
// success
return 0;
    
}