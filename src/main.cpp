#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "FileReader.h"
#include "FreeImage.h"

#define HEIGHT  640
#define WIDTH  480

void saveToFile(Uint32 pixels[], int height, int width, std::string filename) {
	std::cout << "dumping to file " << std::endl;
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0,
			0);

	Uint32 *surfacePixels = (Uint32 *) surface->pixels;
	for (int pixel = 0; pixel < (height * width); ++pixel) {
		surfacePixels[pixel] = pixels[pixel];

	}
	//SDL_SaveBMP(surface, filename.c_str());

	FIBITMAP *img = FreeImage_ConvertFromRawBits((unsigned char *)pixels, width, height, width * 4, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, true);

	if(FreeImage_Save(FIF_PNG, img, filename.c_str(), 	PNG_DEFAULT)){
		std::cout << "true" << std::endl;
	} else
		std::cout << "false" << std::endl;


	std::cout << ", done" << std::endl;
}

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
printf("\n*** ");
if(fif != FIF_UNKNOWN) {
printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
}
printf(message);
printf(" ***\n");
}
// In your main program …


int main(int argc, char **argv) {
	FreeImage_Initialise();
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
	bool leftMouseButtonDown = false;
	bool quit = false;

	SDL_Event event;

	int height = HEIGHT, width = WIDTH;

	FileReader* reader;
	Scene* scene = NULL;
	//read the file
	try {
		reader = new FileReader("scene1.test");

		scene = reader->readFile();
		scene->getSamplingSize(height, width);
		std::cout << "sample size is " << height << ", " << width << std::endl;
	} catch (int i) {
		std::cout << "file read failed" << "exiting" << std::endl;
		exit(1);
	}

	Uint32* pixels = scene->getPixels(height, width);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture * texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width,height);
	scene->renderScene();
	while (!quit) {
		SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
		SDL_WaitEvent(&event);

		switch (event.type) {
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				leftMouseButtonDown = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				leftMouseButtonDown = true;
			/* no break */
		case SDL_MOUSEMOTION:
			if (leftMouseButtonDown) {
				int mouseX = event.motion.x;
				int mouseY = event.motion.y;
				pixels[mouseY * width + mouseX] = 0;
			}
			break;
		case SDL_QUIT:

			quit = true;
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	saveToFile(pixels, height, width, scene->getSaveFilename());
	delete reader;
	//delete scene; not needed since it is managed by reader;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	FreeImage_DeInitialise();
	return 0;
}
