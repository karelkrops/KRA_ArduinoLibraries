#ifndef CIFRIX_SCHEDULER_CONNECTOR_H
#define CIFRIX_SCHEDULER_CONNECTOR_H

class CifrixSchedulerConnector
{
private:
    /* data */

public:
    CifrixSchedulerConnector(/* args */){};
    virtual ~CifrixSchedulerConnector(){};
    virtual void schedulerInit(){};
    virtual void schedulerLoop(){};
};
  

#endif // CIFRIX_SCHEDULER_CONNECTOR_H