#include "SecretImage.h"
#include <fstream>
#include <string>
#include <iostream>


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.

    int length_of_upper = (image.get_width() * (image.get_width() + 1)) / 2 ; // Calculate the number of elements in the upper triangular matrix
    int length_of_lower = (image.get_width() * (image.get_width() - 1)) / 2 ; // Calculate the number of elements in the lower triangular matrix

    width = image.get_width();
    height = image.get_height();

    upper_triangular = new int[length_of_upper];
    lower_triangular = new int[length_of_lower];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.

    int upper_index = 0;  // Initialize the index for the upper triangular matrix
    int lower_index = 0;  // Initialize the index for the lower triangular matrix
    for(int i = 0; i < image.get_height(); ++i){
        for(int j = 0; j < image.get_width(); ++j){
            if(j >= i){  // Check if in the upper triangular region
                upper_triangular[upper_index] = image.get_data()[i][j];  // Add pixel to upper triangular matrix
                upper_index ++;
            }
            else{  // Lower triangular region
                lower_triangular[lower_index] = image.get_data()[i][j];  // Add pixel to lower triangular matrix
                lower_index ++;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    this->width = w;               
    this->height = h;              
    this->upper_triangular = upper;     
    this->lower_triangular = lower;
    

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.

    delete[] upper_triangular;  
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.

    int upper_index = 0;
    int lower_index = 0;

    for(int i = 0; i < image.get_height(); ++i){
        for(int j = 0; j < image.get_width(); ++j){
            if(j >= i){
                image.get_data()[i][j] = upper_triangular[upper_index];
                upper_index ++;
            }
            else{
                image.get_data()[i][j] = lower_triangular[lower_index];
                lower_index ++;
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.

    int upper_index = 0;
    int lower_index = 0;

    for(int i = 0; i < image.get_height(); ++i){
        for(int j = 0; j < image.get_width(); ++j){
            if(i >= j){
                upper_triangular[upper_index] = image.get_data()[i][j];
                upper_index++;
            }
            else{
                lower_triangular[lower_index] = image.get_data()[i][j];
                lower_index++;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.
     
    

    // Open the file with the given filename
    std::ofstream outfile(filename);

    // Check if the file was opened successfully
    if (!outfile.is_open()) {
        std::cerr << "File cannot open!" << std::endl;
        return;
    }

    // 1. Write width and height on the first line, separated by a single space.
    outfile << width << " " << height << std::endl;
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write them as: "element1 element2 ... element15"

    int length_of_upper = (width * (width + 1)) / 2 ;
    int length_of_lower = (width * (width - 1)) / 2 ;

    for (int i = 0; i < length_of_upper; ++i) {
        outfile << upper_triangular[i];
        if (i < length_of_upper - 1) {  // Add space except after the last element
            outfile << " "; 
        }
    }
    outfile << std::endl; 

    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.

    
    for (int i = 0; i < length_of_lower; ++i) {
        outfile << lower_triangular[i];
        if (i < length_of_lower - 1) {  // Add space except after the last element
            outfile << " "; 
        }
    }
    outfile << std::endl; 
  
    // Close the file after writing
    outfile.close();  
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        //std::cerr << "File cannot open" << std::endl;
        //return SecretImage(0, 0, nullptr, nullptr);  
        throw std::runtime_error("File cannot open!");
    }
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int width, height;
    
    infile >> width >> height;
    // 3. Allocate memory for both arrays.

    int length_of_upper = (width * (width + 1)) / 2;
    int length_of_lower = (width * (width - 1)) / 2;

    int* upper_triangular = new int[length_of_upper];
    int* lower_triangular = new int[length_of_lower];

    // 4. Read the upper_triangular array from the second line, space-separated.

    for (int i = 0; i < length_of_upper; ++i) {
        if (!(infile >> upper_triangular[i])) {
            delete[] upper_triangular;  // Cleanup
            delete[] lower_triangular;   // Cleanup
            throw std::runtime_error("Error reading upper triangular array");
        }
    }

    // 5. Read the lower_triangular array from the third line, space-separated.
    
    for (int i = 0; i < length_of_lower; ++i) {
        if (!(infile >> lower_triangular[i])) {
            delete[] upper_triangular;  // Cleanup
            delete[] lower_triangular;   // Cleanup
            throw std::runtime_error("Error reading lower triangular array");
        }
    }

    
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    infile.close();
    //SecretImage secret_image(width, height, upper_triangular, lower_triangular);
    //return secret_image;
    return SecretImage(width, height, upper_triangular, lower_triangular);
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
