#include "TextManager.h"

bool TextManager::Init()
{
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}

TTF_Font* TextManager::LoadFont(string fontPath, int size)
{
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return nullptr;
	}
	
	return font;
}

bool TextManager::DrawText(string text, SDL_Color textColor, TTF_Font* font, int quadX, int quadY, SDL_Renderer* gRenderer)
{
	if (!font || !gRenderer) return false;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);

	if (!textSurface) {
		std::cerr << "Unable to render text surface: " << TTF_GetError() << std::endl;
		return false;
	}

	int w = textSurface->w;
	int h = textSurface->h;

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	SDL_FreeSurface(textSurface);

	if (!textTexture) return false;

	SDL_Rect renderQuad = { quadX, quadY, w, h};
	SDL_RenderCopy(gRenderer, textTexture, nullptr, &renderQuad);
	SDL_DestroyTexture(textTexture);

	return true;
}

void TextManager::CloseFont(TTF_Font* font)
{
	TTF_CloseFont(font);
}