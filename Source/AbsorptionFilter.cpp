#include "AbsorptionFilter.h"


void AbsorptionFilter::prepare(const juce::dsp::ProcessSpec& spec) {
	sampleRate = static_cast<float>(spec.sampleRate);
}
void AbsorptionFilter::process(const juce::dsp::ProcessContextReplacing<float>& context) {

}

juce::AudioBuffer<float> AbsorptionFilter::buildImpulseResponse() {
	juce::AudioBuffer<float> impulseResponse(1, 4096);
	float binSize = sampleRate / 4096.0f;
	for (int i = 0; i < 2048; i++) {
		float ampDb = 
	}

}


void AbsorptionFilter::setDistance(float newDistance) {
	distance = newDistance;
}
void AbsorptionFilter::setTemperature(float temp) {
	airModel.setTemperature(temp);
}
void AbsorptionFilter::setHumidity(float hum) {
	airModel.setHumidity(hum);
}
void AbsorptionFilter::setPressure(float pres) {
	airModel.setPressure(pres);
}


