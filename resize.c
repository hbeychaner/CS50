/**
 * copy.c
 *
 * CS50 AP
 * Whodunit
 *
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
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

    // Initialize variable for the rezize factor. Default = 1.
    int scale = 1;
    // Verify input is a valid integer for resize.
    for (int i = 0; i < strlen(argv[3]); i++)
    {
        if (!isdigit(argv[3][i]))
        {
            printf("Please provide a valid integer to resize the image.\n");
            return 6;
        }
        else scale = atoi(argv[3]);
    }

    // Determine padding prior to scaling.
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Initialize separate info/headers for use in scaled image.
    BITMAPINFOHEADER scaled_bi = bi;
    BITMAPFILEHEADER scaled_bf = bf;

    // Update file size, image size, width, and height.
    scaled_bi.biWidth  = bi.biWidth * scale;
    scaled_bi.biHeight = bi.biHeight * scale;

    // Determine padding to add post-scaling
    int scaled_padding = (4 - bi.biWidth * sizeof(RGBTRIPLE) % 4) % 4;

    // Now that we have padding and new image size, we can calculate actual new filesize
    // for an updated BITMAPFILEHEADER.
    scaled_bi.biSizeImage = ((sizeof(RGBTRIPLE) * scaled_bi.biWidth) + scaled_padding) * abs(scaled_bi.biHeight);
    scaled_bf.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + (scaled_bi.biSizeImage);

    // write outfile's BITMAPFILEHEADER
    fwrite(&scaled_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&scaled_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Nest all the copylines in another for loop to iterate over scale.
    // iterate over infile's scanlines

    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int l = 0; l < scale; l++)
        {
            fseek(inptr, bf.bfOffBits + i * (padding + (bi.biWidth * sizeof(RGBTRIPLE))), SEEK_SET);

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for(int m = 0; m < scale; m++)
                {
                // write RGB triple to outfile. Scale replaces 1 to write pixels a certain number of times.
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

                // then add it back (to demonstrate how)
                for (int k = 0; k < scaled_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            }

        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
        fseek(inptr, -bi.biWidth, SEEK_CUR);
    }

        // close infile
        fclose(inptr);

        // close outfile
        fclose(outptr);

        // that's all folks
        return 0;
}