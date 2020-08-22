
#include "CifrixScheduler.h"

//#include "FileSystem.h"
#include <Arduino.h>

//CifrixScheduler::SchedulerMap CifrixScheduler::schedulerMap = {};
CifrixSchedulerData *CifrixScheduler::actuelItem = 0;
CifrixSchedulerData *CifrixScheduler::firstItem = 0;
unsigned long CifrixScheduler::startTime = 0;
unsigned long CifrixScheduler::oldTime = 0;
float CifrixScheduler::averageTime = 0;
unsigned long CifrixScheduler::maxTime = 0;
unsigned long CifrixScheduler::minTime = 0;
int CifrixScheduler::numberLoop = 0;

CifrixScheduler::CifrixScheduler(/* args */)
{
}

CifrixScheduler::~CifrixScheduler()
{
}

void CifrixScheduler::init()
{

#ifdef FILE_SYSTEM_H
//    FileSystem fs = FileSystem();
//    fs.schedulerInit();
#endif //FILE_SYSTEM_H

    //    SPIFFS.begin();
    //   delay(1000);
    //       Serial.println("START CifrixScheduler::init()");
    // seradi ulohy podle priority
    // spousti vsechny inicializace aktivovaných úloh

    //    SchedulerMap pokus;

    for (goToFirst(); isNotNull(); goToNext())
    {
        if (actuelItem == 0)
            Serial.println("ERROR actuelItem");
        if (actuelItem->statusInit)
        {
            //                        Serial.println("inicializuji " + String(actuelItem->index.c_str()));
            if (actuelItem->statusStatic)
            {
                if (actuelItem->functionInit != 0)
                {
                    //                                        Serial.println("spostim");
                    (actuelItem->functionInit)();
                    //                                        Serial.println("dokonceno");
                }
            }
            else
            {
                if (actuelItem->cifrixScheduler != 0)
                {
                    //                                        Serial.println("spostim");
                    actuelItem->cifrixScheduler->schedulerInit();
                    //                                        Serial.println("dokonceno");
                }
            }
        }
    }
}

#ifdef CIFRIX_FOR_ARDUINO
#endif

void CifrixScheduler::loop()
{
    unsigned long deltaTime = 0;
    for (goToFirst(); isNotNull(); goToNext())
    {
        if (actuelItem == 0)
            Serial.println("ERROR actuelItem");
        if (actuelItem->statusLoop
#ifdef CIFRIX_FOR_ARDUINO
            && (micros() - actuelItem->startTime) > (actuelItem->millisCallLoop * 1000)
#endif
        )
        {
            //                        Serial.println("loop " + String(actuelItem->index.c_str())); delay(50);

#ifdef CIFRIX_FOR_ARDUINO
            actuelItem->startTime = micros();
            actuelItem->numberLoop++;
#endif
            if (actuelItem->statusStatic)
            {
                if (actuelItem->functionLoop != 0)
                {
                    (actuelItem->functionLoop)();
                }
            }
            else
            {
                if (actuelItem->cifrixScheduler != 0)
                {
                    actuelItem->cifrixScheduler->schedulerLoop();
                }
            }
#ifdef CIFRIX_FOR_ARDUINO
            deltaTime = micros() - actuelItem->startTime;
            if (deltaTime > actuelItem->maxTime)
                actuelItem->maxTime = deltaTime;
            if (deltaTime < actuelItem->minTime)
                actuelItem->minTime = deltaTime;
            actuelItem->sumaTime += deltaTime;
            actuelItem->startTime = micros();
#endif
        }
    }
#ifdef CIFRIX_FOR_ARDUINO
    // if (startTime==0)loopTimeStart=micros();
    deltaTime = micros() - startTime;
    if (deltaTime > maxTime)
        maxTime = deltaTime;
    if (deltaTime < minTime)
        minTime = deltaTime;
    //averageTime=averageTime - (averageTime/numberLoop) + ((micros()-float(loopTimeStart))/loopNumberLast);
    startTime = micros();
    numberLoop++;
#endif
#ifdef CIFRIX_FOR_ARDUINO
#endif
}

#ifdef CIFRIX_FOR_ARDUINO
void CifrixScheduler::debugTime()
{
    CifrixSchedulerData *pointerTmp = actuelItem;
    if (numberLoop > 0)
        averageTime = (micros() - oldTime) / numberLoop;
    Serial.println("averageTime: " + String(averageTime / 1000, 3) + "ms, min: " + String(float(minTime) / 1000, 3) + "ms, max: " + String(float(maxTime) / 1000, 3) + "ms, numberLoop: " + String(numberLoop) + " <== All loop");

    for (goToFirst(); isNotNull(); goToNext())
    {
        if (actuelItem->statusLoop)
        {
            Serial.println("averageTime: " + String((actuelItem->numberLoop == 0 ? 0 : float(actuelItem->sumaTime) / actuelItem->numberLoop) / 1000, 3) + "ms, min: " + String(float(actuelItem->minTime) / 1000, 3) + "ms, max: " + String(float(actuelItem->maxTime) / 1000, 3) + "ms, numberLoop: " + String(actuelItem->numberLoop) + " <== " + actuelItem->index);
        }
        actuelItem->minTime = actuelItem->maxTime;
        actuelItem->numberLoop = 0;
        actuelItem->maxTime = 0;
        actuelItem->sumaTime = 0;
    }
    minTime = maxTime;
    maxTime = 0;
    numberLoop = 0;
    oldTime = micros();
    startTime = micros();
    actuelItem = pointerTmp;
}
#endif

void CifrixScheduler::onSheduler(const char nameIndex[], CifrixSchedulerConnector &classSheduler, bool statusInit, bool statusLoop, unsigned long millisCallLoop, int priority)
{
    CifrixSchedulerData *px;
    if (firstItem == 0)
    {
        firstItem = new CifrixSchedulerData();
        px = firstItem;
    }
    else
    {
        for (goToFirst(); isNotNull(); goToNext())
        {
            px = actuelItem;
        }
        px->pointerNextItem = new CifrixSchedulerData();
        px = px->pointerNextItem;
    }
    px->index = nameIndex;
    px->cifrixScheduler = &classSheduler;
    px->functionInit = 0;
    px->functionLoop = 0;
    px->statusLoop = statusLoop;
    px->statusInit = statusInit;
    px->priority = priority;
    px->millisCallLoop = millisCallLoop;
    px->statusStatic = false;
}

void CifrixScheduler::onSheduler(const char nameIndex[], void (&functionInit)(), void (&functionLoop)(), bool statusInit, bool statusLoop, unsigned long millisCallLoop, int priority)
{
    CifrixSchedulerData *px;
    if (firstItem == 0)
    {
        firstItem = new CifrixSchedulerData();
        px = firstItem;
    }
    else
    {
        for (goToFirst(); isNotNull(); goToNext())
        {
            px = actuelItem;
        }
        px->pointerNextItem = new CifrixSchedulerData();
        px = px->pointerNextItem;
    }
    px->index = nameIndex;
    px->cifrixScheduler = 0;
    px->functionInit = &functionInit;
    px->functionLoop = &functionLoop;
    px->statusLoop = statusLoop;
    px->statusInit = statusInit;
    px->priority = priority;
    px->millisCallLoop = millisCallLoop;
    px->statusStatic = true;
}

bool CifrixScheduler::goToNext()
{
    if (actuelItem != 0)
        actuelItem = actuelItem->pointerNextItem;
    else
        actuelItem = 0;
    return isNotNull();
}

bool CifrixScheduler::goToFirst()
{
    actuelItem = firstItem;
    return isNotNull();
}