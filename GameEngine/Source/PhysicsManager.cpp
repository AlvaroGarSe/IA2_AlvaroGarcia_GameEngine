#include "PhysicsManager.h"
#include "iostream"
using namespace std;

bool PhysicsManager::checkCollision(GameObject a, GameObject b)
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
    leftA = a.transform.x;
    rightA = a.transform.x + a.mWidth;
    topA = a.transform.y;
    bottomA = a.transform.y + a.mHeigth;

    //Calculate the sides of rect B
    leftB = b.transform.x;
    rightB = b.transform.x + b.mWidth;
    topB = b.transform.y;
    bottomB = b.transform.y + b.mHeigth;
	
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
