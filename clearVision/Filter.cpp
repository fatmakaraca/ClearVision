#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.

    int padding_height = image.get_height() + 2 * (kernelSize / 2);
    int padding_width = image.get_width() + 2 * (kernelSize / 2);

    //A new GrayscaleImage was created to pad the edges of the image with zeros by `kernelSize/2`.
    GrayscaleImage padding_data(padding_width, padding_height);

    //Copy the data from the image to the area where padding has not been applied.
    for(int i = kernelSize/2; i < padding_height - kernelSize / 2; ++i){
        for(int j = kernelSize/2; j < padding_width - kernelSize / 2; ++j){
            padding_data.get_data()[i][j] = image.get_data()[i - kernelSize / 2][j - kernelSize / 2];
        }
    }
      
    //Traverse the area where padding has not been applied.
    for(int i = kernelSize/2; i < padding_height - kernelSize / 2; ++i){
        for(int j = kernelSize/2; j < padding_width - kernelSize / 2; ++j){
            int kernelTotal = 0;
            int kernelAverage = 0;
            //Traverse the surrounding pixels of the current pixel with a size equal to `kernelSize`.
            for(int k = i - kernelSize / 2; k < i + 1 + kernelSize / 2; ++k){
                for(int l = j - kernelSize / 2; l < j + 1 + kernelSize / 2; ++l){
                    kernelTotal += padding_data.get_data()[k][l]; //Sum the values in the kernel.
                }
            }

            kernelAverage = kernelTotal / (kernelSize * kernelSize); //Calculate the average of the kernel.
            if(kernelAverage < 0){
                kernelAverage = 0;
            }
            else if(kernelAverage > 255){
                kernelAverage = 255;
            }
            image.get_data()[i - kernelSize / 2][j - kernelSize / 2] = kernelAverage; //Assign the average of the kernel to that pixel.

        }
    }
}

double Filter::gaussian_function(int x, int y, double sigma){
    const double pi = M_PI;
    double result = (1 / (2 * pi* sigma * sigma)) * exp(- (double)((x*x + y*y) / (2.0 * sigma * sigma)));
    return result;
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    int padding_height = image.get_height() + 2 * (kernelSize / 2);
    int padding_width = image.get_width() + 2 * (kernelSize / 2);

    //A new GrayscaleImage was created to pad the edges of the image with zeros by `kernelSize/2`.
    GrayscaleImage padding_data(padding_width, padding_height);

    //Copy the data from the image to the area where padding has not been applied.
    for(int i = kernelSize/2; i < padding_height - kernelSize / 2; ++i){
        for(int j = kernelSize/2; j < padding_width - kernelSize / 2; ++j){
            padding_data.get_data()[i][j] = image.get_data()[i - kernelSize / 2][j - kernelSize / 2];
        }
    }

    double sum_of_coefficients = 0; //Keep track of the sum of the coefficients for the normalization process.
    
    //Create the `kernelMatrix`, find the appropriate coefficients, and assign them to the `kernelMatrix`.
    double** kernelMatrix = new double*[kernelSize];
            
    for (int i = 0; i < kernelSize; ++i) {
        kernelMatrix[i] = new double[kernelSize];
        for (int j = 0; j < kernelSize; ++j) {
            double coefficient = gaussian_function((j - (kernelSize/2)), (i - (kernelSize/2)), sigma);
            kernelMatrix[i][j] = coefficient;
            sum_of_coefficients +=  coefficient; //Add the new coefficient.
        }
    }

    //Traverse the area where padding has not been applied.
    for(int i = kernelSize/2; i < padding_height - kernelSize / 2; ++i){
        for(int j = kernelSize/2; j < padding_width - kernelSize / 2; ++j){
            
            double total = 0;
            double new_value = 0;
        
            int kernel_index_x = 0; // Index for the kernel's x-axis
            for(int k = i - kernelSize / 2; k < i + 1 + kernelSize / 2; ++k){
                int kernel_index_y = 0; // Index for the kernel's y-axis
                for(int l = j - kernelSize / 2; l < j + 1 + kernelSize / 2; ++l){
                    // Accumulate the product of the padded image data and the kernel coefficients
                    total += padding_data.get_data()[k][l] * kernelMatrix[kernel_index_x][kernel_index_y];
                    kernel_index_y++; // Move to the next column in the kernel
                }
                kernel_index_x++; // Move to the next row in the kernel
            }

            // Calculate the new pixel value by normalizing the total
            new_value = total / sum_of_coefficients;
            if(new_value < 0){
                new_value = 0;
            }
            else if(new_value > 255){
                new_value = 255;
            }
            // Assign the new pixel value to the corresponding position in the original image
            image.get_data()[i - kernelSize / 2][j - kernelSize / 2] = new_value;
        }
    }    

    for (int i = 0; i < kernelSize; ++i) {
        delete[] kernelMatrix[i]; // Free each row of the kernel
    }
    delete[] kernelMatrix;  // Free the kernel matrix itself
    kernelMatrix = nullptr;  
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].

    GrayscaleImage blurred(image);
    apply_gaussian_smoothing(blurred, kernelSize, 1.0);
    
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).

    for(int i = 0; i < image.get_height(); ++i){
        for(int j = 0; j < image.get_width(); ++j){
            int enhancedValue = image.get_data()[i][j] + amount * (image.get_data()[i][j] - blurred.get_data()[i][j]);
            if (enhancedValue < 0) {
                image.get_data()[i][j] = 0;
            } else if (enhancedValue > 255) {
                image.get_data()[i][j] = 255;
            } else {
                image.get_data()[i][j] = enhancedValue;
            }
        }
    }
}
