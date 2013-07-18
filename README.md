# Bin2PNG

Convert a binary file to a PNG image and then decrypt it back to binary.

## Binary to PNG :
Each bytes of the binary file are converted to numbers (0-255), which will then define a pixel color.
For each bytes will be a grayscale pixel in the final PNG image. 
The PNG image can have more pixels than the number of bytes of the binary file. 
The excess pixels will have a RGB(255, 0, 0) color (red) and will be ignored when decrypting the file.

## PNG to Binary :
When decrypting the PNG file to create the corresponding binary file, we take each pixels of the image and get their color.
If the R & G color are different, we skip this pixel. It is not at grayscale, and so is an excess pixel.
It it is grayscale, we take the color number for R and then convert it to it's binary representation.
We construct an unsigned char array with all decrypted binary data of pixels and then save it back to a binary file.

## Usage :
You must build it