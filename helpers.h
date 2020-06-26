#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Black and White image
void black_white(int height, int width, RGBTRIPLE image[height][width]);

// adjust luminosity image
void adjsut_luminosity(int height, int width, RGBTRIPLE image[height][width],int lum);

// adjust contrast image
void adjsut_contrast(int height, int width, RGBTRIPLE image[height][width],int cont);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]);
