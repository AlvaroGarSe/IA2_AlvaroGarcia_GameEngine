#include "Player.h"
Player::Player(int posX, int posY)
{
    setPosition(posX, posY);

    isActive = false;

    texture = new LTexture;
}


void Player::setPosition(int posX, int posY)
{
    transform.x = posX;
    transform.y = posY;
}

bool Player::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Conveyor texture
    if (!texture->loadFromFile("Media/Player.png"))
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
