#include "Bird.h"
Bird::Bird(int screenWidth, int screenHeight)
{
    mPosX = screenWidth * 0.3;
    mPosY = screenHeight/2;
    mWidth = 0;
    mHeigth = 0;

    SCREEN_HEIGHT = screenHeight;

    isActive = true;
    
    //Initialize the velocity
    mVelY = GRAVITY;

    gBirdTexture = new LTexture;
}
void Bird::move()
{
    //Move the bird up or down
    mPosY += mVelY;

    //If the bird goes to the top or the bottom it will remain there and not pass the border
    if ((mPosY < 0))
    {
        mPosY = 0;
    }
    if (mPosY > SCREEN_HEIGHT - mHeigth)
    {
        mPosY = SCREEN_HEIGHT - mHeigth;
    }
}

void Bird::render()
{
    //Show the bird
    gBirdTexture->render(mPosX, mPosY);
}

bool Bird::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load bird texture
    if (!gBirdTexture->loadFromFile("../../03-Media/bird.png"))
    {
        printf("Failed to load ship texture!\n");
        success = false;
    }
    else
    {
        mWidth = gBirdTexture->getWidth();
        mHeigth = gBirdTexture->getHeight();
    }

    return success;
}

void Bird::handleEvent(InputManager& inpManager)
{
    mVelY = 0.0f;
    // When the user press Space the bird goes up
    if (inpManager.GetKey(SDL_SCANCODE_SPACE))
    {
        mVelY = BIRD_VELY;
    }
    else {
        mVelY = GRAVITY;
    }
}
