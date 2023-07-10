/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
    static Color new_color;

    uint8_t B = image->image[row][col].B;

    if (B & 1) {
        new_color.B = 255;
        new_color.G = 255;
        new_color.R = 255;
    } else {
        new_color.B = 0;
        new_color.G = 0;
        new_color.R = 0;
    }

    return &new_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
    Color *new_color;
    for (int i = 0; i < image->rows; ++i) {
        for (int j = 0; j < image->cols; ++j) {
            new_color = evaluateOnePixel(image, i, j);
            image->image[i][j] = *new_color;
        }
    }

    return image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    Image *image;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <PPM-Image-Path>\n", argv[0]);
        exit(-1);
    }

    image = readData(argv[1]);
    writeData(image);
    fflush(stdout);

    image = steganography(image);
    writeData(image);
    fflush(stdout);

    freeImage(image);

    return 0;
}
