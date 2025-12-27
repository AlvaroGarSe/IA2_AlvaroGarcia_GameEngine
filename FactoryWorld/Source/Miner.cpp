#include "Miner.h"
#include "TimeManager.h"

Miner::Miner(int posX, int posY)
{
    setPosition(posX, posY);

    maxCapacity = 10;
    amountResources = 0;

    minningMaxTime = 2;
    minningCurrentTime = 0;

    isActive = false;

    isOn = false;

    texture = new LTexture;
}


void Miner::setPosition(int posX, int posY)
{
    transform.x = posX;
    transform.y = posY;
}

void Miner::startMiner()
{
    isOn = true;
    minningCurrentTime = TimeManager::GetInstance().getTicks() + minningMaxTime;
}

void Miner::stopMiner()
{
    isOn = false;
}

void Miner::mine()
{
    if (amountResources >= maxCapacity)
    {
        printf("MaxCapac");
    }
    else
    {
        printf("Mined 1");
        amountResources++;
        minningCurrentTime = TimeManager::GetInstance().getTicks() + minningMaxTime;
    }
}

void Miner::transferMaterial()
{
    printf("Transf 1");
    amountResources--;
}

void Miner::Update()
{
    if (!isActive || !isOn) { return; }
    if (TimeManager::GetInstance().getTicks() >= minningCurrentTime)
    {
        mine();
    }
    if (amountResources > 0)
    {
        transferMaterial();
    }

}

bool Miner::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Conveyor texture
    if (!texture->loadFromFile("Media/Miner.png"))
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
