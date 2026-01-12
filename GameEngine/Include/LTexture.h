#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>


class LTexture
{
public:
	//Initializes variables
	LTexture(int Widht = 0, int Height = 0);

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, float scaleX = 1, float scaleY = 1, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }

	SDL_Texture* getSDLTexture() { return mTexture; }

	bool LoadFromSurface(SDL_Surface* loadedSurface);

	// File path to the image of the texture
	std::string mTexturePath;

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

};

