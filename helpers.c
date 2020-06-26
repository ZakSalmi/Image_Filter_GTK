#include "helpers.h"
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            if (image[i][j].rgbtBlue == image[i][j].rgbtRed&& image[i][j].rgbtBlue==image[i][j].rgbtGreen/3)
        {
            continue;
        }
        else{
            double moyenne =  image[i][j].rgbtBlue + image[i][j].rgbtRed+image[i][j].rgbtGreen;
            moyenne=round(moyenne/3);
            image[i][j].rgbtBlue= moyenne;
            image[i][j].rgbtRed= moyenne;
            image[i][j].rgbtGreen= moyenne;

            }
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            BYTE rgb[3]={image[i][j].rgbtRed,image[i][j].rgbtGreen,image[i][j].rgbtBlue};
            double red=.393 * rgb[0]+ .769 *rgb[1] + .189 * rgb[2];
            if (red>255)
            {
                image[i][j].rgbtRed=255;
            }
            else
            {
                image[i][j].rgbtRed=round(red);
            }
            double green= .349 * rgb[0] + .686 * rgb[1] + .168 *rgb[2];
            if (green>255)
            {
                image[i][j].rgbtGreen=255;
            }
            else
            {
                image[i][j].rgbtGreen=round(green);
            }

            double blue=.272 * rgb[0] + .534 * rgb[1] + .131 * rgb[2];
            if (blue>255)
            {
                image[i][j].rgbtBlue=255;
            }
            else
            {
                image[i][j].rgbtBlue=round(blue);
            }

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2 ; j++)
        {
            RGBTRIPLE temp=image[i][j];
            image[i][j]=image[i][width-1-j];
            image[i][width-1-j]=temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{int sumBlue;
    int sumGreen;
    int sumRed;
    float counter;
    //create a temporary table of colors to not alter the calculations
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            sumBlue = 0;
            sumGreen = 0;
            sumRed = 0;
            counter = 0.00;

            // sums values of the pixel and 8 neighboring ones, skips iteration if it goes outside the pic
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > height - 1)
                {
                    continue;
                }

                for (int h = -1; h < 2; h++)
                {
                    if (i + h < 0 || i + h > width - 1)
                    {
                        continue;
                    }

                    sumBlue += image[j + k][i + h].rgbtBlue;
                    sumGreen += image[j + k][i + h].rgbtGreen;
                    sumRed += image[j + k][i + h].rgbtRed;
                    counter++;
                }
            }

            // averages the sum to make picture look blurrier
            temp[j][i].rgbtBlue = round(sumBlue / counter);
            temp[j][i].rgbtGreen = round(sumGreen / counter);
            temp[j][i].rgbtRed = round(sumRed / counter);
        }
    }

    //copies values from temporary table
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
}

void black_white(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            double moyenne =  image[i][j].rgbtBlue + image[i][j].rgbtRed+image[i][j].rgbtGreen;
            moyenne=round(moyenne/3);
            if (moyenne>127)
            {
            image[i][j].rgbtBlue= 255;
            image[i][j].rgbtRed= 255;
            image[i][j].rgbtGreen= 255;
            }
            else if (moyenne <= 127)
            {
            image[i][j].rgbtBlue= 0;
            image[i][j].rgbtRed= 0;
            image[i][j].rgbtGreen= 0;
            }
        }
    }
    return;
}
void adjsut_luminosity(int height, int width, RGBTRIPLE image[height][width],int lum)
{
if (lum<-255 || lum>255)
{
return ;
}
if (lum>0)
{
 for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
                {
           int gb=image[i][j].rgbtBlue+lum;
           int  gr=image[i][j].rgbtRed+lum;
           int gg=image[i][j].rgbtGreen+lum;
        ///////////
        if (gb>255)
        {
            image[i][j].rgbtBlue= 255;
        }
        else
        {
             image[i][j].rgbtBlue= gb;
        }
        ///////////
        if (gr>255)
        {
            image[i][j].rgbtRed= 255;
        }
        else
        {
             image[i][j].rgbtRed= gr;
        ///////////
        }if (gg>255)
        {
            image[i][j].rgbtGreen= 255;
        }
        else
        {
             image[i][j].rgbtGreen= gg;
        }

        }
    }

}
else{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
                {
           int gb=image[i][j].rgbtBlue+lum;
           int  gr=image[i][j].rgbtRed+lum;
           int gg=image[i][j].rgbtGreen+lum;
        ///////////
        if (gb<0)
        {
            image[i][j].rgbtBlue= 0;
        }
        else
        {
             image[i][j].rgbtBlue= gb;
        }
        ///////////
        if (gr<0)
        {
            image[i][j].rgbtRed= 0;
        }
        else
        {
             image[i][j].rgbtRed= gr;
        ///////////
        }if (gg<0)
        {
            image[i][j].rgbtGreen= 0;
        }
        else
        {
             image[i][j].rgbtGreen= gg;
        }

        }
    }

}
return ;
}
void adjsut_contrast(int height, int width, RGBTRIPLE image[height][width],int contrast)
{
if (contrast<-255 || contrast>255)
{
return ;
}

float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
 for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
                {
           int gb=factor * (image[i][j].rgbtBlue - 128) + 128;
           int gr=factor * (image[i][j].rgbtRed - 128) + 128;
           int gg=factor * (image[i][j].rgbtGreen - 128) + 128;
        ///////////
        if (gb>255)
        {
            image[i][j].rgbtBlue= 255;
        }
        else if (gb<0)
        {
            image[i][j].rgbtBlue= 0;
        }
        else
        {
             image[i][j].rgbtBlue= gb;
        }
        ///////////
        if (gr>255)
        {
            image[i][j].rgbtRed= 255;
        }
        else if (gr<0)
        {
            image[i][j].rgbtRed= 0;
        }
        else
        {
             image[i][j].rgbtRed= gr;
        ///////////
        }if (gg>255)
        {
            image[i][j].rgbtGreen= 255;
        }
        else if (gg<0)
        {
            image[i][j].rgbtGreen= 0;
        }
        else
        {
             image[i][j].rgbtGreen= gg;
        }

        }
    }

return ;
}

int function(int val)
{
	return val<255? val:255;
}



RGBTRIPLE pixel(int i, int j, int height, int width,RGBTRIPLE image[height][width])
{
	int gx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
	int redValueX,greenValueX,blueValueX,redValueY,greenValueY,blueValueY;redValueX=greenValueX=blueValueX=redValueY=greenValueY=blueValueY=0;

	for (int k=-1; k<=1;k++)
	{
		for (int l=-1; l<=1; l++)
		{

		 if( i+k>=0 && i+k<height && i + l>=0 && i+l < width)
			 {
			 	int wx=gx[k+1][l+1];
			 	redValueX+=wx*image[i+k][j+l].rgbtRed;
			 	greenValueX=wx*image[i+k][j+l].rgbtGreen;
		 		blueValueX=wx*image[i+k][j+l].rgbtBlue;
			 	int wy=gx[l+1][k+1];
		 		redValueY+=wy*image[i+k][j+l].rgbtRed;
			 	greenValueY=wy*image[i+k][j+l].rgbtGreen;
		 		blueValueY=wy*image[i+k][j+l].rgbtBlue;
			 }
		}
	}
	RGBTRIPLE pixel;
	pixel.rgbtRed=function(round(sqrt(redValueX*redValueX+redValueY*redValueY)));

	pixel.rgbtGreen=function(round(sqrt(greenValueX*greenValueX+greenValueY*greenValueY)));

	pixel.rgbtBlue=function(round(sqrt(blueValueX*blueValueX+blueValueY*blueValueY)));
	return pixel;
}



void edges (int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE new_image[height][width];
	for (int i=0; i<height ;i++)
	{
		for (int j=0; j<width ; j++)
		{
			new_image[i][j] = pixel(i,j,height,width,image);
		}
	}
	for (int i=0; i<height;i++)
	{
		for (int j=0;j<width;j++)
		{
			image[i][j]=new_image[i][j];
		}
	}

}
