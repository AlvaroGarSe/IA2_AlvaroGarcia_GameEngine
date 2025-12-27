#include "Background.h"
#include <iostream>
using namespace std;

Background::Background(int ScreenWidht, int ScreenHeight, string path)
{
    transform.x = 0;
    transform.y = 0;

    // Receive the path of the backgound that we want to use, this is to use the same class for each game
    PATH = path;

    gBackgroundTexture = new LTexture(ScreenWidht,ScreenHeight);
}

void Background::render()
{
    //Show the background
    gBackgroundTexture->render(transform.x, transform.y);
}

bool Background::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load background texture
    if (!gBackgroundTexture->loadFromFile(PATH))
    {
        printf("Failed to load background texture!\n");
        success = false;
    }

    return success;
}