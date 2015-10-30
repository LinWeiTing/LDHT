//Written by Lin Wei Ting in National Taipei University of Technology Applied Network Laboratory. Taipei, Taiwan.

#ifndef __DHT_H__
#define __DHT_H__

#include "Arduino.h"

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85
#define DHT11 11
#define DHT21 21
#define DHT22 22


class LDHT
{
private:
  uint8_t data[6];
  uint8_t _pin, _type, _count;
  unsigned long _lastreadtime;
  boolean firstreading;
	float temp, humi;
public:

  LDHT(uint8_t pin, uint8_t type, uint8_t count=6);
  void begin(void);
  int read(void);
	float readHumidity(void);
	float readTemperature(bool Select=true);
	float convertCtoK(float);
	float convertCtoF(float);
	float convertFtoC(float);
	float readHeatIndex(float,float);
	float readDewPoint(float,float);
};

#endif
