
#include <iostream>
#include <string>
#include "SceneReader.h"
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
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {

    	std::cout << "file name is not specified," << std::endl;
        std::cout << "usage: "<< argv[0] << " filename" << std::endl;
        exit(1);
    }
    std::string fileToread = argv[1];
    std::string masterPath;

    unsigned int startPos = 0;
    unsigned int endPos = fileToread.find_last_of('/');
    if(endPos == std::string::npos){
        std::cout << "file is in the current directory" << std::endl;
        masterPath = "";
    } else {
    	masterPath = fileToread.substr(startPos, endPos+1);//+1 for including seperator in the end
    	fileToread = fileToread.substr(endPos+1);
    }

	unsigned int height = HEIGHT, width = WIDTH;

	SceneReader* reader;
	Scene* scene = NULL;
	//read the file
	try {
		reader = new SceneReader(masterPath,fileToread);

		scene = reader->readFile();
		scene->getSamplingSize(height, width);
		std::cout << "sample size is " << height << ", " << width << std::endl;
	} catch (int i) {
		std::cout << "file read failed" << "exiting" << std::endl;
		exit(1);
	}

	unsigned char* pixels = scene->getPixels(height, width);


	scene->buildOctree();

	scene->renderScene();
	saveToFile(pixels, height, width, scene->getSaveFilename());
	delete reader;
	//delete scene; not needed since it is managed by reader;
	return 0;
}
