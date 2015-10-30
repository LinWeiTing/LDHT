/* DHT library
MIT license
Written by Lin Wei Ting in National Taipei University of Technology Applied Network Laboratory. Taipei, Taiwan.
*/

#include "LDHT.h"

LDHT::LDHT(uint8_t pin, uint8_t type, uint8_t count) {
    _pin = pin;
    _type = type;
    _count = count;
    firstreading = true;
}

void LDHT::begin(void) {
    // set up the pins!
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH);
	temp = 25;
    _lastreadtime = 0;
}


int readPin(int pin)
{
    return digitalRead(pin);
}
//read the Humidity value.
float LDHT::readHumidity(void)
{
	return humi;
}
//read the Temperature value.
//Select = true,Celcius ; Select = false,Fahrenheit ; Select default = true.
float LDHT::readTemperature(bool Select)
{
	float f;

	if(Select)
		return temp;
	else
	{
      	f = convertCtoF(temp);
		return f;
	}
}
//convert Celcius to Kelvin.
float LDHT::convertCtoK(float tempCelcius)
{
	return tempCelcius + 273.15;
}
//convert Celcius to Fahrenheit.
float LDHT::convertCtoF(float tempCelcius)
{
	return tempCelcius * 9 / 5 + 32;
}
//convert Fahrenheit to Celcius.
float LDHT::convertFtoC(float tempFahrenheit)
{
  return (tempFahrenheit - 32) * 5 / 9;
}
// read the Heat Index value.
//https://en.wikipedia.org/wiki/Heat_index
float LDHT::readHeatIndex(float tempCelcius, float percentHumidity)
{

  return convertFtoC(-42.379 +
           2.04901523 * convertCtoF(tempCelcius) +
          10.14333127 * percentHumidity +
          -0.22475541 * convertCtoF(tempCelcius)*percentHumidity +
          -0.00683783 * pow(convertCtoF(tempCelcius), 2) +
          -0.05481717 * pow(percentHumidity, 2) +
           0.00122874 * pow(convertCtoF(tempCelcius), 2) * percentHumidity +
           0.00085282 * convertCtoF(tempCelcius)*pow(percentHumidity, 2) +
          -0.00000199 * pow(convertCtoF(tempCelcius), 2) * pow(percentHumidity, 2));
}
// read the Dew Point value.
//https://en.wikipedia.org/wiki/Dew_point
float LDHT::readDewPoint(float tempCelcius, float percentHumidity)
{
	double A0= 373.15/(273.15 + tempCelcius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * percentHumidity;
	double T = log(VP/0.61078);
	return (241.88 * T) / (17.558-T);
}

int LDHT::read()
{
    uint8_t laststate = HIGH;
    unsigned long counter = 0;
    uint8_t j = 0, i;
    unsigned long currenttime;

    // pull the pin high and wait 250 milliseconds
    digitalWrite(_pin, HIGH);
    delay(250);

    currenttime = millis();
    if (currenttime < _lastreadtime) {
        // ie there was a rollover
        _lastreadtime = 0;
    }

    if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
        return true; // return last correct measurement
    }

    firstreading = false;

    _lastreadtime = millis();

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    // now pull it low for ~20 milliseconds
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delay(20);
    digitalWrite(_pin, HIGH);
    delayMicroseconds(40);
    pinMode(_pin, INPUT);

    unsigned char count_buf[MAXTIMINGS];
    int count_len = 0;
    // read in timings
    for ( i=0; i < MAXTIMINGS; i++)
    {
        counter = 0;
        while (1)
        {
            if(readPin(_pin) != laststate)
            {
                //if(counter > 9)
                break;
            }
            counter++;
            // delayMicroseconds(1);
            if (counter == 100)
            {
                break;
            }
        }

        count_buf[count_len++] = counter;
        laststate = readPin(_pin);

        if (counter == 100) break;

        if ((i >= 4) && (i%2 == 0))
        {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 25)
            data[j/8] |= 1;

            j++;
        }
    }

        float f = 0.0;
		switch(_type)
		{
			case DHT11:
				f = data[2];
				if(f>=(temp-15) && f<=(temp+15))
				{
				temp = f;
			
				f = data[0];
				humi = f;
			
				return true;
				}
				else
					return false;
				break;
			case DHT21:
			case DHT22:
				f = data[2] & 0x7F;
				f *= 256;
				f += data[3];
				f /= 10;
				if (data[2] & 0x80)
				f *= -1;

				if(f>=(temp-15) && f<=(temp+15))
				{
					temp = f;


					f = data[0];
					f *= 256.0;
					f += data[1];
					f /= 10.0;

					humi = f;

					return true;
				}
				else
					return false;
				break;
		}
}
