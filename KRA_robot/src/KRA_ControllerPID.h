#ifndef KRA_CONTROLLERPID_H
#define KRA_CONTROLLERPID_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 0.0.1
 * datum verze: 23.8.2020
 * 
 * Třída je určena k výpočtu PID regulace
 * Narozdil od jednoducheho trivialniho vypoctu,
 * tato trida konstanty a vlivy pocita vzhledem k casove jednotce 1ms
 * To se hodi, hlavne pri nerovnomernem casovani
 * k výpočtu se dále používá složka Proporcionalni, Integracni, Derivacni (1. i 2. derivace)
 * 
 */

/**
 * popis digitální PID regulace (CZ): https://www.dixell.cz/teorie-regulace/
 * zdroj vzorečků: https://en.wikipedia.org/wiki/PID_controller
 * (CZ) https://cs.wikipedia.org/wiki/PID_regul%C3%A1tor
 */

#include <Arduino.h>

class KRA_ControllerPID{
  public:
  KRA_ControllerPID();
  void initPID(float kP,float kI,float kD,float k2D=0);
  void setMinMax(float valMin,float valMax);
  float calcPID(float measuredValue,float requiredValue=0);
  protected:
  float valueMax;
  float valueMin;
  float konstantP;
  float konstantI;
  float konstantD;
  float konstant2D;
  unsigned long lastDeltaTimeD;
  unsigned long lastDeltaTime2D;
  unsigned long lastTime;
  float err;
  float errD;
  float err2D;
  float regP;
  float regI;
  float regD;
  float reg2D;
  float lastValue;
};

#endif // KRA_CONTROLLERPID_H
