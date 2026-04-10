#pragma once
#include <JuceHeader.h>
#include "AirModel.h"

class AbsorptionFilter {

public:
	AbsorptionFilter();
	~AbsorptionFilter();
	void prepare(const juce::dsp::ProcessSpec& spec);
	void process(const juce::dsp::ProcessContextReplacing<float>& context);
	void setDistance(float newDistance);
	void setTemperature(float temp); //in C
	void setHumidity(float hum);     //realative
	void setPressure(float pres);    //in atm

private:
	AirModel airModel;
	float sampleRate;
	float distance;
	

	juce::AudioBuffer<float> buildImpulseResponse();
	juce::dsp::Convolution convolution;
	

	
};