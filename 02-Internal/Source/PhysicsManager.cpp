#include "PhysicsManager.h"
#include "iostream"
using namespace std;

bool PhysicsManager::checkCollision(Object a, Object b)
{
    if (!a.isActive || !b.isActive)
    {
        return false;
    }
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    

    //Calculate the sides of rect A
    leftA = a.mPosX;
    rightA = a.mPosX + a.mWidth;
    topA = a.mPosY;
    bottomA = a.mPosY + a.mHeigth;

    //Calculate the sides of rect B
    leftB = b.mPosX;
    rightB = b.mPosX + b.mWidth;
    topB = b.mPosY;
    bottomB = b.mPosY + b.mHeigth;
	
    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}
