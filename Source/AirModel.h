#pragma once
#include <JuceHeader.h>

class AirModel {

public:
	float getAttenuationCoeffAtFreqDb(float freq);

	void setTemperature(float temp); //in C
	void setHumidity(float hum);     //realative
	void setPressure(float pres);    //in atm
	
	void calcFrO();
	void calcFrN();
private:
	static constexpr float PS0 = 1.0f;
	static constexpr float T0 = 293.15f;  //in K
	static constexpr float T01 = 273.16f; //in K
	float temperature; //in C
	float humidity;    //relative humidity in percentage
	float pressure;    //in atm

	float FrO;
	float FrN;
	



	float getAbsHumidity(float relH);
	float getTempK();
	
	
};