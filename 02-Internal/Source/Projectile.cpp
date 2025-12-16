#include "Projectile.h"
Projectile::Projectile(int SHIP_WIDHT, int shipPosX, int shipPosY)
{
    mPosX = shipPosX + SHIP_WIDHT * 0.29;
    mPosY = shipPosY - 30;

    mHeigth = 0;
    mWidth = 0;
    
    isActive = false;

    //Initialize the velocity
    mVelY = -1;

    gProjectileTexture = new LTexture;
}
void Projectile::move()
{
    if (isActive)
    {
        //Move the projectile up   
        mPosY += mVelY;

        //If the projectile passed the screen from the top deactivate it
        if (mPosY < -mHeigth)
        {
            //delete this;
            isActive = false;
        }
    }
}

void Projectile::render()
{
    if (isActive)
    {
        gProjectileTexture->render(mPosX, mPosY);
    }
}

void Projectile::GenerateProjectile(int SHIP_WIDHT, int shipPosX, int shipPosY) 
{
    // Activates the projectile and move it in the head of the ship
    
    mPosX = shipPosX + SHIP_WIDHT * 0.29;
    mPosY = shipPosY - 30;

    isActive = true;
    
}

bool Projectile::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load projectile texture
    if (!gProjectileTexture->loadFromFile("../../03-Media/projectile.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = gProjectileTexture->getWidth();
        mHeigth = gProjectileTexture->getHeight();
    }

    return success;
}
