#include "TextManager.h"

bool TextManager::init()
{

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return -1;
	}
	return true;
}

TTF_Font* TextManager::selectFont(string fontPath, int size)
{
	
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return NULL;
	}
	
	return font;
}

bool TextManager::renderFont(SDL_Color textColor, TTF_Font* font, int quadX, int quadY, int value, SDL_Renderer* gRenderer)
{
	std::string valueText = "SCORE: " + std::to_string(value);
	
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, valueText.c_str(), textColor);
	if (!textSurface) {
		std::cerr << "Unable to render text surface: " << TTF_GetError() << std::endl;
	}
	else {
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		SDL_FreeSurface(textSurface);

		if (textTexture) {
			SDL_Rect renderQuad = { quadX, quadY, textSurface->w, textSurface->h };
			SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);
			SDL_DestroyTexture(textTexture);
		}
	}
	return false;
}

void TextManager::closeFont(TTF_Font* font)
{
	TTF_CloseFont(font);
}

