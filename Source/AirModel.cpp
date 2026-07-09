#include "AirModel.h"
#include <cmath>
void AirModel::setTemperature(float temp){
	temperature = temp;
} 
void AirModel::setHumidity(float hum) {
	humidity = hum;
}
void AirModel::setPressure(float pres) {
	pressure = pres;
}
float AirModel::getAttenuationCoeffAtFreqDb(float freq) {
    float bigF = freq/pressure;
    float bigFrO = FrO/pressure;
    float bigFrN = FrN/pressure;
    float t = getTempK() / T0;
	return pressure * (20.0f / log(10.0f)) * (pow(bigF, 2.0f) / PS0) * (1.84 * pow(10.0f, -11.0f) * sqrt(t) + pow(t, -5.0f / 2.0f) * (0.01275f * exp(-2239.1f / getTempK()) / (bigFrO + (pow(bigF, 2.0f)/bigFrO)) + 0.1068f * exp(-3352.0f / getTempK()) / (bigFrN + pow(bigF, 2.0f)/bigFrN)));
} 
float AirModel::getAbsHumidity(float relH) {
    float pSat = PS0 * pow(10.0f, (-6.8346f * pow((T01 / getTempK()), 1.261f) + 4.6151f));
    return (relH / pressure) * (pSat);
}
float AirModel::getTempK() {
    return temperature + 273.15f;
}
void AirModel::calcFrO() {
	float absH = getAbsHumidity(humidity);
    FrO = (24.0f + (4.04f * pow(10.0f, 4.0f) * absH * ((0.02f + absH) / (0.391f + absH))))/PS0;
}
void AirModel::calcFrN() {
    float absH = getAbsHumidity(humidity);
	FrN = (1 / PS0) * sqrt((T0 / getTempK())) * (9.0f + 280.0f * absH * exp(-4.170f * (pow((T0 / getTempK()), (1.0f / 3.0f)) - 1.0f)));
}
