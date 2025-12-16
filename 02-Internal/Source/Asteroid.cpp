#include "Asteroid.h"
#include <random>
Asteroid::Asteroid(int screenWidth, int screenHeight)
{
    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;

    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeigth = 0;

    isActive = false;
    
    //Initialize the velocity
    mVelY = 1;

    gAsteroidTexture = new LTexture;
}
void Asteroid::move()
{
    if (isActive)
    {
        //Move the Asteroid down   
        mPosY += mVelY;

        //If the asteroid has passed the screen from the bottom deactivate it
        if (mPosY > SCREEN_HEIGHT)
        {
            isActive = false;
        }
    }
}

void Asteroid::render()
{
    if (isActive)
    {
        gAsteroidTexture->render(mPosX, mPosY);
    }
}

void Asteroid::GenerateAsteroid()
{
    // Generates a random postion from 0 to the border of the screen to spawn the new Asteroid
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, SCREEN_WIDTH - mWidth);

    mPosX = round(distr(gen));
    mPosY = -150;

    isActive = true;
    
}

bool Asteroid::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Asteroid texture
    if (!gAsteroidTexture->loadFromFile("../../03-Media/asteroid.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = gAsteroidTexture->getWidth();
        mHeigth = gAsteroidTexture->getHeight();
    }

    return success;
}
