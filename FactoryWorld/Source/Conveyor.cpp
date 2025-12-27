#include "Conveyor.h"
Conveyor::Conveyor(int posX, int posY, Orientation orientation)
{
    SetPosition(posX, posY);

    isActive = false;

    texture = new LTexture;
    transform.scaleX = 0.5;
    transform.scaleY = 0.5;

    this->orientation = orientation;
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
