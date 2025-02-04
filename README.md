# ClearVision

This project contains a series of command-line tools that demonstrate image processing techniques and how secret messages can be embedded (and extracted) from an image. Below are the basic operations and commands with descriptions of what each one does.

Commands

mean <img> <kernel_size>

Description: Applies a Mean filter to the input image using the given kernel size.

gauss <img> <kernel_size> <sigma>

Description: Applies Gaussian smoothing to the input image with the specified kernel size and sigma value.

unsharp <img> <kernel_size> <amount>

Description: Applies an Unsharp Mask filter to the input image using the given kernel size and amount of sharpening.

add <img1> <img2>

Description: Adds the pixel values of two images together and generates a new image.

sub <img1> <img2>

Description: Subtracts the pixel values of img2 from img1 and generates a new image.

equals <img1> <img2>

Description: Compares two images to check if they are identical and prints the result to the console.

disguise <img>

Description: Generates a secret image from the given grayscale image and saves it to a .dat file.

reveal <dat>

Description: Reads the given .dat file, reconstructs the secret image, and saves it as a grayscale image.

enc <img> <message>

Description: Embeds the provided secret message into the image by modifying the least significant bits (LSBs) of the pixel values.

dec <img> <msg_len>

Description: Extracts and decrypts the hidden message from the image, assuming the message length is specified by msg_len.

Usage

Each command can be run via the terminal as follows:

$ <command> <parameters>

Examples:

Apply Mean Filter:

$ mean image.png 3

Apply Gaussian Smoothing:

$ gauss image.png 5 1.5

Apply Unsharp Mask:

$ unsharp image.png 3 2.0

Embed a Secret Message:

$ enc image.png "This is a secret message"

Extract a Secret Message:

$ dec image.png 20
