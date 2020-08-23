#include "KRA_ControllerPID.h"

KRA_ControllerPID::KRA_ControllerPID()
{
  valueMin = -32767;
  valueMax = 32768;
  konstantP = 1;
  konstantI = 0;
  konstantD = 1;
  konstant2D = 1;
  lastDeltaTimeD=0;
  lastDeltaTime2D=0;
  lastTime = micros();
  err=0;
  errD=0;
  regP=0;
  regD=0;
  reg2D=0;
  regI=0;
  lastValue=(valueMax+valueMin)/2;
}
void KRA_ControllerPID::initPID(float kP, float kI, float kD, float k2D) {
  konstantP = kP;
  konstantI = kI;
  konstantD = kD;
  konstant2D = k2D;
}
void KRA_ControllerPID::setMinMax(float valMin, float valMax)
{
  valueMin = -valMin;
  valueMax = valMax;
  lastValue=(valMax+valMin)/2;
}
float KRA_ControllerPID::calcPID(float measuredValue,float requiredValue)
{
  float o;
  lastDeltaTime2D = lastDeltaTimeD;
  if (micros() - lastTime < 2000000) // ochrana pred dlouhou dobou nepouzivani. Pokud 2 s mimo provoz, pak se nepouzije D a I složka.
  lastDeltaTimeD = micros() - lastTime;
  else lastDeltaTimeD=0;
  lastTime = micros();
  err = measuredValue - requiredValue;
  errD = err - errD;
  err2D = errD - err2D;
  regP = err*konstantP;
  regD = errD*konstantP*konstantD*(lastDeltaTimeD/1000);
  reg2D = err2D*konstantP*konstant2D*((lastDeltaTimeD+lastDeltaTime2D)/2000);
  regI += konstantP*konstantI*(lastDeltaTimeD/1000);
  o = regP+regD+reg2D+regI;

  lastValue = measuredValue;
  if (o>valueMax)o=valueMax;
  if (o<valueMin)o=valueMin;
  return o;
}
