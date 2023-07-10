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

#define USE_VERSION 3

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "error: file not exits\n");
        exit(-1);
    }

    Image *img = malloc(sizeof(Image));

    // Read format
    char buffer[12];
    fscanf(fp, "%s", buffer);
    if (strcmp(buffer, "P3") != 0) {
        fprintf(stderr, "error: image format is not PPM\n");
        exit(-1);
    }

    // Read dimension
    if (fscanf(fp, "%d %d", &img->cols, &img->rows) != 2) {
        fprintf(stderr, "error: read PPM dimension failed\n");
        exit(-1);
    }

    // Read color range
    fscanf(fp, "%d", (int*) buffer);

    // Read pixels
#if (USE_VERSION == 1 || USE_VERSION == 3)
    Color (*data)[img->cols] = malloc(img->rows * img->cols * sizeof(Color));
    for (int i = 0; i < img->rows; ++i) {
        Color *p_row = data[i];
        for (int j = 0; j < img->cols; ++j) {
            fscanf(fp, "%"SCNu8, &p_row[j].R);
            fscanf(fp, "%"SCNu8, &p_row[j].G);
            fscanf(fp, "%"SCNu8, &p_row[j].B);
        }
    }
    img->image = (Color **)data;
#elif (USE_VERSION == 2)
    img->image = malloc(img->rows * sizeof(Color*));
    for (int i = 0; i < img->rows; ++i) {
        img->image[i] = malloc(img->cols * sizeof(Color));
        for (int j = 0; j < img->cols; ++j) {
            fscanf(fp, "%"SCNu8, &img->image[i][j].R);
            fscanf(fp, "%"SCNu8, &img->image[i][j].G);
            fscanf(fp, "%"SCNu8, &img->image[i][j].B);
        }
    }
#else
    fprintf(stderr, "error: [readData] not implemented\n");
    exit(-1);
#endif
    fclose(fp);

    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE

    // print format
    fprintf(stdout, "P3\n");

    // print dimension
    fprintf(stdout, "%d %d\n", image->cols, image->rows);

    // print color range
    fprintf(stdout, "255\n");

    // print values
#if (USE_VERSION == 1)
    Color (*data)[image->cols] = (Color (*)[image->cols]) image->image;
    for (int i = 0; i < image->rows; ++i) {
        Color *p_row = data[i];
        for (int j = 0; j < image->cols; ++j) {
            Color pixel = p_row[j];
            fprintf(stdout, (j == 0 ? "%3"SCNu8: "%6"SCNu8) , pixel.R);
            fprintf(stdout, "%4"SCNu8, pixel.G);
            fprintf(stdout, "%4"SCNu8, pixel.B);
        }
        fprintf(stdout, "\n");
    }
#elif (USE_VERSION == 2)
    for (int i = 0; i < image->rows; ++i) {
        for (int j = 0; j < image->cols; ++j) {
            fprintf(stdout, (j == 0 ? "%3"SCNu8: "%6"SCNu8) , image->image[i][j].R);
            fprintf(stdout, "%4"SCNu8, image->image[i][j].G);
            fprintf(stdout, "%4"SCNu8, image->image[i][j].B);
        }
        fprintf(stdout, "\n");
    }
#elif (USE_VERSION == 3)
    Color *p_data = (Color *) image->image;
    for (int i = 0; i < image->rows * image->cols; ++i) {
        fprintf(stdout, (i % image->cols == 0 ? "%3"SCNu8 : "%6"SCNu8), p_data->R);
        fprintf(stdout, "%4"SCNu8, p_data->G);
        fprintf(stdout, "%4"SCNu8, p_data->B);
        fprintf(stdout, ((i + 1) % image->cols == 0 ? "\n" : ""));
        ++p_data;
    }
#else
    fprintf(stderr, "error: [writeData] not implemented\n");
    exit(-1);
#endif

}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
#if (USE_VERSION == 2)
    for (int i = 0; i < image->rows; ++i)
        free(image->image[i]);
#endif
    free(image->image);
    free(image);
}
