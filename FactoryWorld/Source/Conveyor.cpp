#include "Conveyor.h"
Conveyor::Conveyor(int posX, int posY, Orientation orientation)
{
    setPosition(posX, posY);

    isActive = false;

    texture = new LTexture;

    orientation = orientation;
}


void Conveyor::setPosition(int posX, int posY)
{
    transform.x = posX;
    transform.y = posY;
}

bool Conveyor::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Conveyor texture
    if (!texture->loadFromFile("Media/Conveyor.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = texture->getWidth();
        mHeigth = texture->getHeight();    
    }

    return success;
}
