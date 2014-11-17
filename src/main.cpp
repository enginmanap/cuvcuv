
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "FileReader.h"
#ifdef USE_FREEIMAGE_PNG
#include "FreeImage.h"
#else
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

#define HEIGHT  640
#define WIDTH  480




void saveToFile(unsigned char pixels[], int height, int width, std::string filename) {
	std::cout << "exporting to file " << std::endl;
#ifdef USE_FREEIMAGE_PNG
    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);
    if(FreeImage_Save(FIF_PNG, img, filename.c_str(), PNG_DEFAULT) == 0 ){
#else
	if(stbi_write_png(filename.c_str(), width, height, 4, pixels, width * 4) == 0){
#endif
		std::cerr << "png export failed." << std::endl;
	} else
		std::cout << "png export finished." << std::endl;

}


int main(int argc, char *argv[]) {
	bool isTimesPrinted = false;
	bool quit = false;

	SDL_Event event;

	int height = HEIGHT, width = WIDTH;

	FileReader* reader;
	Scene* scene = NULL;
	//read the file
	Uint32 start,readFile,buildOctree,renderAll;
	start = SDL_GetTicks();
	try {
		reader = new FileReader("scene1.test");

		scene = reader->readFile();
		scene->getSamplingSize(height, width);
		readFile = SDL_GetTicks() - start;
		std::cout << "sample size is " << height << ", " << width << std::endl;
	} catch (int i) {
		std::cout << "file read failed" << "exiting" << std::endl;
		exit(1);
	}

	unsigned char* pixels = scene->getPixels(height, width);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("Ray Tracer",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Texture * texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width,height);
#else
	SDL_Texture * texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width,height);
#endif

	scene->buildOctree();
	 buildOctree = SDL_GetTicks() - readFile;

	while (!quit) {
		if (!isTimesPrinted && scene->renderScene()){
			renderAll = SDL_GetTicks() - buildOctree;
			std::cout << "start: " << start/1000 << std::endl;
			std::cout << "file read: " << readFile/1000<< std::endl;
			std::cout << "building octree: " << buildOctree/1000 << std::endl;
			std::cout << "render: " << renderAll/1000 << std::endl;
			isTimesPrinted = true;
		}
		SDL_UpdateTexture(texture, NULL, pixels, width * 4 ); // for 4 channels
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:

			quit = true;
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	if (!isTimesPrinted){
		renderAll = SDL_GetTicks() - buildOctree;
		std::cout << "start: " << start/1000 << std::endl;
		std::cout << "file read: " << readFile/1000<< std::endl;
		std::cout << "building octree: " << buildOctree/1000 << std::endl;
		std::cout << "render: " << renderAll/1000 << std::endl;
		isTimesPrinted = true;
	}
	saveToFile(pixels, height, width, scene->getSaveFilename());
	delete reader;
	//delete scene; not needed since it is managed by reader;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
