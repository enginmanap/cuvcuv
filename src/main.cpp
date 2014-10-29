#include <iostream>
#include <SDL2/SDL.h>

#define HEIGHT  640
#define WIDTH  480

void saveToFile(Uint32 pixels[], int pixelSize){
	SDL_Surface* surface = SDL_CreateRGBSurface(0,640,480,32,0,0,0,0);
	Uint32 *surfacePixels = (Uint32 *)surface->pixels;
	for (int pixel = 0; pixel < pixelSize; ++pixel) {
		surfacePixels[pixel] = pixels[pixel];
	}

	SDL_SaveBMP(surface, "screen.bmp");

}

int main(int argc, char **argv){
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);

    Uint32 * pixels = new Uint32[HEIGHT * WIDTH];
    memset(pixels, 255, HEIGHT * WIDTH * sizeof(Uint32));

    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, 640 * sizeof(Uint32));
        SDL_WaitEvent(&event);

        switch (event.type)
        {

        case SDL_QUIT:
        	saveToFile(pixels, HEIGHT * WIDTH);
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
