/**
 * Copies a BMP piece by piece, just because.
 */
       
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
    
    //width
    int original_file_line_width = bi.biWidth;
    bi.biWidth = bi.biWidth * n;
    
    //height
    int original_file_line_Height = (bi.biHeight);
    bi.biHeight = (bi.biHeight * n);    
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int original_padding = (4 - (original_file_line_width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //image size, total size of image for pixels and padding
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    
    //file size
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    RGBTRIPLE line[1000];
    int line_incrementer  = 0;
    
    // iterate over infile's scanlines
    for (int i = 0, q = abs(original_file_line_Height); i < q; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < original_file_line_width; j++)
        {   
            // read RGB triple from infile
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for(int k = 0; k < n; k++)
            {
                line[line_incrementer] = triple;
                line_incrementer++;
            }
        }

        for (int j = 0; j < n; j++)
        {
            //write array to outfile
            fwrite(&line , sizeof(RGBTRIPLE) * bi.biWidth, 1, outptr);

            // write out file padding
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
        line_incrementer = 0;
        
        // skip over padding, if any
        fseek(inptr, (original_padding), SEEK_CUR);
    }        
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
