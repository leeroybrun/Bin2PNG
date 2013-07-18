#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lodepng.h"

#define PNG_FILE "image.png"
#define BIN_IN_FILE "in.exe"
#define BIN_OUT_FILE "out.exe"

int binaryToPng();
int pngToBinary();
char *DecToBin(int number);

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

	//system("pause");
}

int binaryToPng() {
	FILE *binaryFile;
	size_t bytesRead = 0;
	unsigned long fileSize;
	unsigned char *fileBuff;
	unsigned char* img;
	int imageSize;
	char pixelColorStr[3];
	int pixelColor;
	unsigned int i, x, y;
	unsigned int error;

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

		fileBuff = (unsigned char *)malloc(fileSize);

		// Get final image size
		imageSize = ceil(sqrt((double) fileSize));

		// Print various informations
		printf("Size of file : %d bytes\n", fileSize);
		printf("Size of final image : %d x %d px\n", imageSize, imageSize);
		puts("\n");

		img = (unsigned char *) malloc(imageSize * imageSize * 4);

		// Read file
		bytesRead = fread(fileBuff, fileSize, 1, binaryFile);
		
		x = 0;
		y = 0;
		// Process each bytes, write pixel color to image
		for(i = 0; i < fileSize; i++) {
			// Get decimal value for this byte, will be the pixel color
			sprintf(pixelColorStr, "%d", fileBuff[i]);
			sscanf(pixelColorStr, "%d", &pixelColor);
			printf("%d ", pixelColor);

			img[4 * imageSize * y + 4 * x + 0] = pixelColor;
			img[4 * imageSize * y + 4 * x + 1] = pixelColor;
			img[4 * imageSize * y + 4 * x + 2] = pixelColor;
			img[4 * imageSize * y + 4 * x + 3] = 255;

			x += 1;

			if(x == imageSize) {
				puts("");
				x = 0;
				y += 1;
			}
		}

		error = lodepng_encode32_file(PNG_FILE, img, imageSize, imageSize);

		free(img);
		free(fileBuff);
		fclose(binaryFile);

		if(error) {
			printf("error %u: %s\n", error, lodepng_error_text(error));
			return 0;
		} else {
			printf("Success !");
			return 1;
		}
	}
}

int pngToBinary() {
	FILE *binaryFile;
	size_t bytesRead = 0;
	unsigned long fileSize;
	unsigned char *fileBuff;
	unsigned char* img;
	unsigned int imageSize;
	char pixelColorStr[3];
	int pixelColor;
	unsigned int i, x, y;
	unsigned int error;
	char* byte;

	error = lodepng_decode32_file(&img, &imageSize, &imageSize, PNG_FILE);
	if(error) {
		printf("error %u: %s\n", error, lodepng_error_text(error));
		return 0;
	}

	for(x = 0; x < imageSize; x++) {
		for(y = 0; y < imageSize; y++) {
			byte = (char*)img[4 * imageSize * y + 4 * x + 0];
			printf("%s ", byte);
		}
		puts("");
	}
}

void DecToBin(int number, char *bin)
{
    if (number == 0) {
		sprintf(bin, "%s0", bin);
	} else if (number == 1) {
		sprintf(bin, "%s1", bin);
	} else if (number % 2 == 0) {
		sprintf(bin, "%s0", bin);
		strcpy(bin, DecToBin(number / 2));
	} else {
        return DecToBin(number / 2) + "1";
	}
}