#include <iostream>
#include <SDL2/SDL.h>
#include "FileReader.h"

#define HEIGHT  640
#define WIDTH  480

void saveToFile(Uint32 pixels[], int height, int width) {
	SDL_Surface* surface = SDL_CreateRGBSurface(0, height, width, 32, 0, 0, 0, 0);
	Uint32 *surfacePixels = (Uint32 *) surface->pixels;
	for (int pixel = 0; pixel < (height * width); ++pixel) {
		surfacePixels[pixel] = pixels[pixel];
	}

	SDL_SaveBMP(surface, "screen.bmp");

}

int main(int argc, char **argv) {
	bool leftMouseButtonDown = false;
	bool quit = false;

	SDL_Event event;

	int height = HEIGHT, width = WIDTH;

	//read the file
	try {
		FileReader reader("scene1.test");
		Scene* scene;
		scene = reader.readFile();
		scene->getSamplingSize(height,width);
		std::cout << "sample size is " << height << ", " << width << std::endl;
	} catch (int i) {
		std::cout << "file read failed" << "exiting" << std::endl;
		exit(1);
	}

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, height, width, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture * texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, height, width);

	Uint32 * pixels = new Uint32[height * width];
	memset(pixels, 255, height * width * sizeof(Uint32));

	while (!quit) {
		SDL_UpdateTexture(texture, NULL, pixels, height * sizeof(Uint32));
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
				pixels[mouseY * height + mouseX] = 0;
			}
			break;
		case SDL_QUIT:
			saveToFile(pixels, height, width);
			quit = true;
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
