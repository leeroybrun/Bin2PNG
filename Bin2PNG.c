//
// Convert a binary file to a PNG image and then decrypt it back to binary.
//
// Binary to PNG :
//     Each bytes of the binary file are converted to numbers (0-255), which will then define a pixel color.
//     For each bytes will be a grayscale pixel in the final PNG image. 
//     The PNG image can have more pixels than the number of bytes of the binary file. 
//     The excess pixels will have a RGB(255, 0, 0) color (red) and will be ignored when decrypting the file.
// 
// PNG to Binary :
//     When decrypting the PNG file to create the corresponding binary file, we take each pixels of the image and get their color.
//     If the R & G color are different, we skip this pixel. It is not at grayscale, and so is an excess pixel.
//     It it is grayscale, we take the color number for R and then convert it to it's binary representation.
//     We construct an unsigned char array with all decrypted binary data of pixels and then save it back to a binary file.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lodepng.h"

#define PNG_FILE "image.png"
#define BIN_IN_FILE "in.exe"
#define BIN_OUT_FILE "out.exe"

int binaryToPng();
int pngToBinary();

// Main
void main(int argc, char *argv[]) {
	if(argc > 1) {
		if(strcmp(argv[1], "encrypt") == 0) {
			binaryToPng();
		} else if(strcmp(argv[1], "decrypt") == 0) {
			pngToBinary();
		} else {
			printf("Usage : %s (encrypt|decrypt)", argv[0]);
			puts("");
		}
	} else {
		printf("Usage : %s (encrypt|decrypt)", argv[0]);
		puts("");
	}
}

// Convert a binary file to PNG
int binaryToPng() {
	FILE *binaryFile;
	unsigned long fileSize;
	unsigned char *fileBuff;
	unsigned char *pngData;
	unsigned int imageSize;
	char pixelColorStr[4];
	int pixelColor;
	unsigned int i, x, y, error;

	// Open binary file
	binaryFile = fopen(BIN_IN_FILE, "rb");
	if(binaryFile == NULL) {
		printf("Error reading '%s' file.", BIN_IN_FILE);
		return 0;
	} else {
		// Get file length
		fseek(binaryFile, 0, SEEK_END);
		fileSize = ftell(binaryFile);
		fseek(binaryFile, 0, SEEK_SET);

		// Allocate memory for the file buffer
		fileBuff = (unsigned char *)malloc(fileSize);

		// Get final image size (fileSize
		imageSize = (int)ceil(sqrt((double) fileSize));

		// Print various informations
		printf("Size of file : %d bytes\n", fileSize);
		printf("Size of final image : %d x %d px\n", imageSize, imageSize);
		puts("\n");

		// Allocate memory for the PNG data array
		pngData = (unsigned char *) malloc(imageSize * imageSize * 4);

		// Read binary file to buffer
		fread(fileBuff, fileSize, 1, binaryFile);

		puts("Starting conversion to PNG file...");
		
		x = 0;
		y = 0;
		// Process each bytes, add pixel to pngData array
		for(i = 0; i < fileSize; i++) {
			// Get decimal value for this byte, will be the pixel color (convert byte to int)
			sprintf(pixelColorStr, "%d", fileBuff[i]);
			sscanf(pixelColorStr, "%d", &pixelColor);

			// Set pixel data
			pngData[4 * imageSize * y + 4 * x + 0] = pixelColor; // R
			pngData[4 * imageSize * y + 4 * x + 1] = pixelColor; // G
			pngData[4 * imageSize * y + 4 * x + 2] = pixelColor; // B
			pngData[4 * imageSize * y + 4 * x + 3] = 255;		 // A

			x += 1;

			// When reached end of pixels line, go to next one
			if(x == imageSize) {
				x = 0;
				y += 1;
			}
		}

		// Complete the image with red pixels
		while(x < imageSize && y < imageSize || y < imageSize) {
			pngData[4 * imageSize * y + 4 * x + 0] = 255; // R
			pngData[4 * imageSize * y + 4 * x + 1] = 0;   // G
			pngData[4 * imageSize * y + 4 * x + 2] = 0;   // B
			pngData[4 * imageSize * y + 4 * x + 3] = 255; // A

			x += 1;

			// When reached end of pixels line, go to next one
			if(x == imageSize) {
				x = 0;
				y += 1;
			}
		}

		printf("Writing PNG file to : %s\n", PNG_FILE);

		// Write PNG file
		error = lodepng_encode32_file(PNG_FILE, pngData, imageSize, imageSize);

		// Free memory
		free(pngData);
		free(fileBuff);
		fclose(binaryFile);

		if(error) {
			printf("error %u: %s\n", error, lodepng_error_text(error));
			return 0;
		} else {
			puts("Success !");
			return 1;
		}
	}
}

// Convert a PNG file to binary
int pngToBinary() {
	FILE *binaryFile;
	unsigned char *binaryBuff;
	unsigned char *pngData;
	int imageSize;
	int i, x, y;
	unsigned int error;

	// Decode PNG file to pngData array
	error = lodepng_decode32_file(&pngData, &imageSize, &imageSize, PNG_FILE);
	if(error) {
		printf("error %u: %s\n", error, lodepng_error_text(error));
		return 0;
	}

	printf("Image size : %d", imageSize);

	// Allocate memory for the binary file buffer
	binaryBuff = (unsigned char *)malloc(imageSize*imageSize*sizeof(unsigned char));

	puts("Starting conversion to binary file...");

	// Process each pixels, get color number, convert to byte and add to binary array
	i = 0;
	for(y = 0; y < imageSize; y++) {
		// Process pixel only if it is grayscale (R & B have same color)
		for(x = 0; x < imageSize && pngData[4 * imageSize * y + 4 * x + 0] == pngData[4 * imageSize * y + 4 * x + 1]; x++) {
			//printf("RGBA(%d, %d, %d, %d)\n", pngData[4 * imageSize * y + 4 * x + 0], pngData[4 * imageSize * y + 4 * x + 1], pngData[4 * imageSize * y + 4 * x + 2], pngData[4 * imageSize * y + 4 * x + 3]);
			binaryBuff[i] = (int)((pngData[4 * imageSize * y + 4 * x + 0] & 0XFF)); // Convert to binary

			i += 1;
		}
	}
	
	printf("Writing binary file to : %s\n", BIN_OUT_FILE);

	// Write data to binary file
	binaryFile = fopen(BIN_OUT_FILE, "wb");
	if(binaryFile == NULL) {
		printf("Error reading '%s' file.", BIN_IN_FILE);
		return 0;
	} else {
		fwrite(binaryBuff, (imageSize*imageSize*sizeof(unsigned char)), 1, binaryFile);

		puts("Success !");

		free(pngData);
		free(binaryBuff);
		fclose(binaryFile);

		return 1;
	}
}

