#include "Pipe.h"
#include <random>
#include <iostream>
Pipe::Pipe(int screenWidth, int screenHeight)
{
    mPosX = 0;
    mPosY = 0;
    mWidth = 0;
    mHeigth = 0;

    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGTH = screenHeight;

    isActive = false;
    hasGivenScore = false;
    isTop = false;
    
    //Initialize the velocity
    mVelX = PIPE_VEL;

    gPipeTexture = new LTexture;
}
void Pipe::move()
{
    //Move the pipe left
    mPosX -= mVelX;

    //If the pipe pass throug the left side of the screen deactivate it
    if ((mPosX < -mWidth))
    {
        isActive = false;
    }
}

bool Pipe::checkScore(int posXBird)
{
    if (posXBird > mPosX + mWidth + 30 && !hasGivenScore && isTop)
    {
        hasGivenScore = true;
        return true;
    }
    return false;
}

void Pipe::render()
{
    if (isActive)
    {
        gPipeTexture->render(mPosX, mPosY);
    }
}

bool Pipe::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load pipe texture
    if (!gPipeTexture->loadFromFile("../../03-Media/Pipe.png"))
    {
        printf("Failed to load ship texture!\n");
        success = false;
    }
    else
    {
        mWidth = gPipeTexture->getWidth();
        mHeigth = gPipeTexture->getHeight();
    }

    return success;
}

void Pipe::GeneratePipe(bool up, int shift)
{
    isActive = true;
    hasGivenScore = false;
    // If the pipe is generated in the top it will not give score because if not it will receive 2, from the top pipe and the bottom one
    isTop = up;
    mPosX = SCREEN_WIDTH;
    if (up)
    {
        mPosY = -mHeigth * 3 / 4 + shift;
    }
    else
    {
        mPosY = SCREEN_HEIGTH - mHeigth / 4  + shift;
    }
}
