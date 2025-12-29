#include "Player.h"
Player::Player(int posX, int posY)
{
    SetPosition(posX, posY);

    isActive = false;

    texture = new LTexture;
}
