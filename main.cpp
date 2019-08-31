#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

int progressBar(int,int,int,int);

int main(int argc, char**argv){
	int bar = -1;
	if(argc < 3){
		std::cout << "Usage: " << argv[0] << " ImageFilepath HeightmapFilepath" << std::endl;
		return 1;
	}
	
	int x1, y1, n1, x2, y2, n2;
	unsigned char* img = stbi_load(argv[1], &x1, &y1, &n1, 3);
	unsigned char* hm = stbi_load(argv[2], &x2, &y2, &n2, 1);
	
	if(img == NULL){
		std::cout << "Error loading '" << argv[1] << "'" << std::endl;
		return 1;
	}
	if(hm == NULL){
		std::cout << "Error loading '" << argv[2] << "'" << std::endl;
		return 1;
	}
	
	size_t imgsize = x1*y1*3;
	unsigned char* output = (unsigned char*)malloc(imgsize);
	
	for(unsigned char* im3 = hm; im3 != hm + imgsize/3; im3++){
		*im3 = (*im3)/4;
	}
	
	int i = 0;
	for(unsigned char* im1 = img, *im2 = hm, *o = output; im1 != img + imgsize; im1 += 3, im2 += 1, o += 3, i+=3){
		int offset = 0;
		for(int j = 0; j < 256; j++){
			if((i/n1)/x1 == (((i+(j*n1))/n1)/x1) && (int)(*(im2 + j)) == j){
				offset = j*3;
			}
		}
		*(o+1) = (uint8_t)(*(im1+offset+1));
		*(o+2) = (uint8_t)(*(im1+offset+2));
		*o = (uint8_t)(0);
		offset = 0;
		bool isJ;
		for(int j = 1; j < 256; j++){
			if((i/3) - j <= 0){
				isJ = false;
			}else{
				isJ = (*(im2 - j) == j);
			}
			if((i/n1)/x1 == (((i-(j*n1))/n1)/x1) && isJ){
				offset = j*n1;
			}
		}
		*o = (uint8_t)(*(im1-offset));
		bar = progressBar(i, (int)imgsize, 60, bar);
	}

	
	progressBar(1,1,60,0);

	std::cout << std::endl << "Saving image..." << std::endl;

	stbi_write_png("Anaglyph.png", x1, y1, n1, output, x1*n1);

	stbi_image_free(img);
	stbi_image_free(hm);

	return 0;
}

int progressBar(int currentVal, int maxVal, int width, int oldBars){
	int bars = (float)currentVal/(float)maxVal * (float)width;
	if(bars != oldBars){
		std::cout << " Progress - ";
		for(int i = 0; i <= width; i++){
			if(i <= bars){
				std::cout << "█";
			}else{
				std::cout << "░";
			}
		}
		std::cout << "\r";
		std::cout.flush();
	}
	return bars;
}
