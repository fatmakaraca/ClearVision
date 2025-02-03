#include "Crypto.h"
#include "GrayscaleImage.h"



// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();
    // 2. Calculate the image dimensions.
    int image_length = image.get_height() * image.get_width();
    // 3. Determine the total bits required based on message length.
    int total_bits_required = message_length * 7;
    // 4. Ensure the image has enough pixels; if not, throw an error.
    if(image_length < total_bits_required){
        throw std::runtime_error("Error: Image does not have enough pixels to extract the required LSBs.");
    }
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int starting_pixel = image_length - total_bits_required;
    // 6. Extract LSBs from the image pixels and return the result.
    for(int i = starting_pixel; i < image_length; ++i){
        int row_index = i / image.get_width();
        int column_index = i % image.get_width();
        int lsb = image.get_data()[row_index][column_index] & 1;
        LSB_array.push_back(lsb);
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if(LSB_array.size() % 7 != 0){
        throw std::runtime_error("Error: LSB array size is not a multiple of 7.");
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    
    std::ostringstream oss;  // Create an output string stream to store the resulting characters

    // Iterate over each group of 7 bits in the LSB_array
    for(int i = 0; i < LSB_array.size() / 7; ++i){
        std::string binaryString = "";  // Initialize an empty string to store each 7-bit binary sequence
        int first_index = i * 7; // Calculate the starting index of the current 7-bit sequence in LSB_array

        // Concatenate 7 bits from LSB_array into the binaryString
        binaryString += std::to_string(LSB_array[first_index]);
        binaryString += std::to_string(LSB_array[first_index + 1]);
        binaryString += std::to_string(LSB_array[first_index + 2]);
        binaryString += std::to_string(LSB_array[first_index + 3]);
        binaryString += std::to_string(LSB_array[first_index + 4]);
        binaryString += std::to_string(LSB_array[first_index + 5]);
        binaryString += std::to_string(LSB_array[first_index + 6]);
        
        // Convert the 7-bit binary string to a bitset for easy binary-to-decimal conversion
        std::bitset<7> binaryValue(binaryString);
        int decimalValue = static_cast<int>(binaryValue.to_ulong()); // Convert binary to decimal integer

        // Convert the decimal value to a character and add it to the output stream
        char c = static_cast<char>(decimalValue);
        oss << c; // Append the character to the output string stream

    }
    
    // 3. Collect the characters to form the decrypted message.
    message = oss.str();
    // 4. Return the resulting message.

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.
    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    for(char c : message){
        std::bitset<7> binaryValue(c);
        for (int i = 6; i >= 0; --i) { 
            LSB_array.push_back(binaryValue[i]);
        }
       
    }
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    
    // TODO: Your code goes here.
    int image_length = image.get_height() * image.get_width();

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    if(LSB_array.size() > image_length){
        throw std::runtime_error("Error: Image has not enough pixels.");
    }
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.

    int starting_pixel = image_length - LSB_array.size();
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for(int i = starting_pixel; i < image_length; ++i){
        int row_index = i / image.get_width();
        int column_index = i % image.get_width();
    
        int number = image.get_data()[row_index][column_index];
        
        std::bitset<8> binary(number);

        if(binary[0] != LSB_array[i - starting_pixel]){
            binary.flip(0);
        }

        int new_value = static_cast<int>(binary.to_ulong());
        
        if (new_value < 0 || new_value > 255) {
            new_value = std::max(0, std::min(255, new_value)); 
        }

        image.get_data()[row_index][column_index] = new_value;
        bool deger = image.get_data()[row_index][column_index] == new_value;
    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    SecretImage secret_image(image);
    return secret_image;
}
