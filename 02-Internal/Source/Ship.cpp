#include "Ship.h"
Ship::Ship(int screenWidth, int screenHeight)
{
    mPosX = screenWidth /2;
    mPosY = screenHeight *0.85;
    mWidth = 0;
    mHeigth = 0;

    SCREEN_WIDTH = screenWidth;

    isActive = true;
    
    //Initialize the velocity
    mVelX = 0;


    gShipTexture = new LTexture;
}
void Ship::move()
{
    //Move the ship left or right
    mPosX += mVelX;

    //If the ship went too far to the left or right
    if ((mPosX < 0))
    {
        //Move back
        mPosX = 0;
    }
    if (mPosX + mWidth > SCREEN_WIDTH)
    {
        //Move back
        mPosX = SCREEN_WIDTH - mWidth;
    }
}

void Ship::render()
{
    //Show the ship
    gShipTexture->render(mPosX, mPosY);
}

bool Ship::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load ship texture
    if (!gShipTexture->loadFromFile("../../03-Media/ship.png"))
    {
        printf("Failed to load ship texture!\n");
        success = false;
    }
    else
    {
        mWidth = gShipTexture->getWidth();
        mHeigth = gShipTexture->getHeight();
    }

    return success;
}

void Ship::handleEvent(InputManager& inpManager)
{
    mVelX = 0.0f;

    // The ship moves left when the player presses the left arrow or the A
    if (inpManager.GetKey(SDL_SCANCODE_LEFT) || inpManager.GetKey(SDL_SCANCODE_A))
    {
        mVelX = -SHIP_VEL;
    }

    // The ship moves right when the player presses the right arrow or the D
    if (inpManager.GetKey(SDL_SCANCODE_RIGHT) || inpManager.GetKey(SDL_SCANCODE_D))
    {
        mVelX = +SHIP_VEL;
    }
}
