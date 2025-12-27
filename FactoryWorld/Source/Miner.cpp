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

    gMinerTexture = new LTexture;
}


void Miner::setPosition(int posX, int posY)
{
    mPosX = posX;
    mPosY = posY;
}

void Miner::render()
{
    if (isActive)
    {
        gMinerTexture->render(mPosX, mPosY);
    }
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
    if (!gMinerTexture->loadFromFile("Media/Miner.png"))
    {
        printf("Failed to load proyectile texture!\n");
        success = false;
    }
    else
    {
        mWidth = gMinerTexture->getWidth();
        mHeigth = gMinerTexture->getHeight();
    }

    return success;
}
