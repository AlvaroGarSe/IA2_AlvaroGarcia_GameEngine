#include "Conveyor.h"
Conveyor::Conveyor(int posX, int posY)
{
    setPosition(posX, posY);

    isActive = false;

    gConveyorTexture = new LTexture;
}


void Conveyor::setPosition(int posX, int posY)
{
    mPosX = posX;
    mPosY = posY;
}

void Conveyor::render()
{
    if (isActive)
    {
        gConveyorTexture->render(mPosX, mPosY, 0.3, 0.2);
    }
}

bool Conveyor::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Conveyor texture
    if (!gConveyorTexture->loadFromFile("Media/Conveyor.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = gConveyorTexture->getWidth();
        mHeigth = gConveyorTexture->getHeight();
    }

    return success;
}
