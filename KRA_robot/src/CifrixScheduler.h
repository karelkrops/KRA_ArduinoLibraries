#ifndef CIFRIX_SCHEDULER
#define CIFRIX_SCHEDULER

/**
   Autor: Karel Kropš
   pro CIFIRX CZ s.r.o.
   verze: 1.1.2
   datum verze: 22.8.2020

   verze bez arduina s knihovnou std::string zatim neni pouzivana
   implementovana je knihovna ve verzi pro Arduino
*/

/* definice prekladu */
#define CIFRIX_FOR_ARDUINO

/* vlastni deklarace */
#ifdef CIFRIX_FOR_ARDUINO
#include <Arduino.h>
#else
#include <string>
#endif

#include "CifrixSchedulerConnector.h"

//typedef std::map<std::string,int> SchedulerMap;

class CifrixSchedulerData
{
    friend class CifrixScheduler;

private:
    CifrixSchedulerData(){pointerNextItem = 0; startTime=0; sumaTime=0; maxTime=0; minTime=0; numberLoop=0;};
    CifrixSchedulerConnector *cifrixScheduler;
    void (*functionInit)();
    void (*functionLoop)();
    bool statusLoop;
    bool statusInit;
    int priority;
    unsigned long millisCallLoop;
    bool statusStatic;
    CifrixSchedulerData *pointerNextItem;
#ifdef CIFRIX_FOR_ARDUINO
    String index;
    unsigned long startTime;
    unsigned long sumaTime;
    unsigned long maxTime;
    unsigned long minTime;
    int numberLoop;
#else
    std::string index;
#endif
};

class CifrixScheduler
{
public:
    /*
    typedef struct Scheduler_s
    {
        CifrixSchedulerConnector *cifrixScheduler;
        void (*functionInit)();
        void (*functionLoop)();
        bool statusLoop;
        bool statusInit;
        int priority;
        int minimumMilisCallLoop;
        bool statusStatic;
    } Scheduler_t;

    typedef std::map<std::string, Scheduler_t> SchedulerMap;
*/
private:
    //    static SchedulerMap schedulerMap;
    //static std::map<std::string,CifrixSchedulerConnector> schedulerMap;
    /* data */
public:
    CifrixScheduler(/* args */);
    virtual ~CifrixScheduler();
    /**
     * 
     */
    static void onSheduler(const char nameIndex[], CifrixSchedulerConnector &classSheduler, bool statusInit = true, bool statusLoop = true, unsigned long millisCallLoop = 0, int priority = 10);
    static void onSheduler(const char nameIndex[], void (&functionInit)(), void (&functionLoop)(), bool statusInit = true, bool statusLoop = true, unsigned long millisCallLoop = 0, int priority = 10);
    static void init();
    static void loop();
    static void nulptr(){};

private:
    static bool goToFirst();
    static bool goToNext();
    static bool isNotNull() { return actuelItem != 0; };
    static CifrixSchedulerData *actuelItem;
    static CifrixSchedulerData *firstItem;
#ifdef CIFRIX_FOR_ARDUINO
    static unsigned long startTime;
    static unsigned long oldTime;
    static float averageTime;
    static unsigned long maxTime;
    static unsigned long minTime;
    static int numberLoop;
    static void dubugTaskTimeStart();
    static void debugTaskTimeStop();

public:
    static void debugTime();
#endif
};

#endif // CIFRIX_SCHEDULER