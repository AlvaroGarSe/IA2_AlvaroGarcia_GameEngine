#include "Player.h"
Player::Player(int posX, int posY)
{
    setPosition(posX, posY);

    isActive = false;

    gPlayerTexture = new LTexture;
}


void Player::setPosition(int posX, int posY)
{
    mPosX = posX;
    mPosY = posY;
}

void Player::render()
{
    if (isActive)
    {
        gPlayerTexture->render(mPosX, mPosY);
    }
}

bool Player::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Conveyor texture
    if (!gPlayerTexture->loadFromFile("Media/Player.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = gPlayerTexture->getWidth();
        mHeigth = gPlayerTexture->getHeight();
    }

    return success;
}
