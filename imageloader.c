/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"


//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
    //YOUR CODE HERE
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "error: [readData] no file found: %s\n", filename);
        exit(-1);
    }

    // Result image
    Image *img = malloc(sizeof(Image));
    if (img == NULL) {
        fprintf(stderr, "error: [readData] malloc failed\n");
        exit(-1);
    }

    // Read format
    char buffer[10];
    fscanf(fp, "%s", buffer);
    if (strcmp(buffer, "P3") != 0) {
        fprintf(stderr, "error: [readData] not a PPM format file\n");
        exit(-1);
    }

    // Read dimension
    if (fscanf(fp, "%d %d", &img->cols, &img->rows) != 2) {
        fprintf(stderr, "error: [readData] dimension not found\n");
        exit(-1);
    }

    // Read color range
    fscanf(fp, "%d", (int *)buffer);

    // Read pixels color
    img->image = malloc(img->rows * sizeof(Color *));
    for (int i = 0; i < img->rows; ++i) {
        img->image[i] = malloc(img->cols * sizeof(Color));
        for (int j = 0; j < img->cols; ++j) {
            fscanf(fp, "%hhu %hhu %hhu",
                   &img->image[i][j].R,
                   &img->image[i][j].G,
                   &img->image[i][j].B);
        }
    }

    fclose(fp);

    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
    //YOUR CODE HERE

    // Print format
    printf("P3\n");

    // Print dimension
    printf("%d %d\n", image->cols, image->rows);

    // Print color range
    printf("255\n");

    // Print pixels
    Color *p_row;
    for (int i = 0; i < image->rows; ++i) {
        p_row = image->image[i];
        for (int j = 0; j < image->cols; ++j) {
            printf((j == 0 ? "%3"SCNu8 : "%6"SCNu8), p_row->R);
            printf("%4"SCNu8, p_row->G);
            printf("%4"SCNu8, p_row->B);
            ++p_row;
        }
        printf("\n");
    }
}

//Frees an image
void freeImage(Image *image) {
    //YOUR CODE HERE
    if (image == NULL)
        return;
    if (image->image != NULL) {
        for (int i = 0; i < image->rows; ++i)
            free(image->image[i]);
    }
    free(image->image);
    free(image);
}
