#pragma once
#include <JuceHeader.h>
#include "AirModel.h"
#include <vector>

class AbsorptionFilter : public juce::Thread{

public:
	AbsorptionFilter(int kernelSize);
	~AbsorptionFilter() override;

	void prepare(const juce::dsp::ProcessSpec& spec);
	void process(const juce::dsp::ProcessContextReplacing<float>& context);

	void setParams(float temp, float hum, float pres, float dist); // in celcius, relative humidity, atm, meters

	void calcMagnitudeResponse();
	void buildImpulseResponse();

	void run() override;

	int getKernelSize();


private:
	int kernelSize;
	std::vector<float> window;

	std::atomic<float> pendingTemp{20.0f};
	std::atomic<float> pendingHum{ 50.0f };
	std::atomic<float> pendingDist{ 1.0f };
	std::atomic<float> pendingPres{ 1.0f };

	float distance;

	AirModel airModel;

	std::vector<float> magnitudeResponse;


	juce::AudioBuffer<float> impulseResponse;
	juce::dsp::FFT fft;
	juce::dsp::Convolution convolution;

	std::vector<juce::dsp::Complex<float>> complexMag;
	std::vector<juce::dsp::Complex<float>> output;
	

	std::atomic<float> sampleRate{ 0.0f };
	std::atomic<bool>  forceRebuild{ false }; //force rebuild is for sample rate changes 
	
	
};