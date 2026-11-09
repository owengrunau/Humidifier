#include "AbsorptionFilter.h"
#include <cmath>

AbsorptionFilter::AbsorptionFilter(int kernelSize)
	:
	juce::Thread("IR Generator"),
	kernelSize{ kernelSize },
	impulseResponse{ 1, kernelSize },
	fft{ static_cast<int>(std::log2(kernelSize)) },
	complexMag(kernelSize, { 0.0f, 0.0f }),
	output(kernelSize, { 0.0f, 0.0f }),
	window(kernelSize, 0.0f)

{	
	for (int i = 0; i < kernelSize; i++){
		window[i] = 0.5f + 0.5f * std::cos(juce::MathConstants<float>::pi * i / kernelSize);
	}

};

AbsorptionFilter::~AbsorptionFilter() {
	stopThread(2000);
}

int AbsorptionFilter::getKernelSize() {
	return kernelSize;
}

void AbsorptionFilter::setParams(float temp, float hum, float pres, float dist) {
	pendingTemp.store(temp);
	pendingHum.store(hum);
	pendingPres.store(pres);
	pendingDist.store(dist);
}

void AbsorptionFilter::prepare(const juce::dsp::ProcessSpec& spec) {

	sampleRate.store(static_cast<float>(spec.sampleRate));
	convolution.prepare(spec);
	forceRebuild.store(true);

	if (!isThreadRunning()){
		startThread(juce::Thread::Priority::normal);
	}
}

void AbsorptionFilter::process(const juce::dsp::ProcessContextReplacing<float>& context) {
	convolution.process(context);
}

void AbsorptionFilter::calcMagnitudeResponse() {
	magnitudeResponse.clear();
	airModel.calcFrO();
	airModel.calcFrN();
	float resolution = sampleRate.load() / kernelSize;
	for (int i = 0; i <= kernelSize / 2; i++) {
		float db;
		db = airModel.getAttenuationCoeffAtFreqDb(i * resolution);
		float m = std::pow(10.0f, -db / 20.0f);
		magnitudeResponse.push_back(m);
	}
}

void AbsorptionFilter::buildImpulseResponse() {
	//this constructs a minimum phase IR using the magnitude response given by the air model formulas
	//essentially this: https://www.dsprelated.com/freebooks/filters/Matlab_listing_mps_m_test.html


	// https://ccrma.stanford.edu/~jos/sasp/Minimum_Phase_Filter_Design.html#13817
	//The time domain signal corresponding to a log spectrum is called the cepstrum, 
	// a frequency response is minimum phase if and only if the corresponding cepstrum is causal

	// casual means impulse response h(t) = 0 for time t < 0

	//load ln magnitude response into complex vector 
	complexMag[0] = {std::pow(magnitudeResponse[0], distance), 0.0f};
	const float floorMag = 1.0e-5f;
	for (int i = 1; i <= kernelSize / 2; i++) {
		
		//implement distance at this stage

		float m = std::max(std::pow(magnitudeResponse[i], distance), floorMag);

		complexMag[i] = juce::dsp::Complex<float>(std::log(m), 0.0f); //log the real to get the cepstrum

		if (i != 0 && i < kernelSize) {
			complexMag[kernelSize - i] = complexMag[i]; //mirror
		}
	} 

	fft.perform(complexMag.data(), output.data(), true); //inverse fft, this is now the cepstrum

	// folding: https://www.dsprelated.com/freebooks/filters/Matlab_listing_fold_m.html#sec:fold
	//since our cepstrum is symmetric, adding the negative to the positive is just multiplying by 2

	for (int i = 1; i < kernelSize / 2; i++) {
		output[i] *= 2.0f;
	}
	for (int i = (kernelSize / 2) + 1; i < kernelSize; i++) {
		output[i] = juce::dsp::Complex<float>(0.0f, 0.0f); 
	}
	//after folding, go back into the freq domain to reverse the ln() we did 
	fft.perform(output.data(), complexMag.data(), false); 
	for (int i = 0; i < kernelSize; i++) {
		complexMag[i] = std::exp(complexMag[i]);
	}
	//inverse fft to get IR 
	fft.perform(complexMag.data(), output.data(), true);

	//set IR samples and apply window
	for (int i = 0; i < kernelSize; i++) {
		impulseResponse.setSample(0, i, output[i].real()*window[i]);
	}

	convolution.loadImpulseResponse(std::move(impulseResponse), static_cast<double>(sampleRate.load()),
		juce::dsp::Convolution::Stereo::no, 
		juce::dsp::Convolution::Trim::no,
		juce::dsp::Convolution::Normalise::no);

	impulseResponse = juce::AudioBuffer<float>(1, kernelSize); //reallocate after std::move
}

void AbsorptionFilter::run() {
	//thread loop that starts at prepare and lasts until thread is stopped,
	//process block is constantly updating pending parameters
	float lastTemp = NAN, lastHum = NAN, lastPres = NAN, lastDist = NAN;

	while (!threadShouldExit())
	{
		//get parameters from audio thread
		const float temp = pendingTemp.load();
		const float hum = pendingHum.load();
		const float pres = pendingPres.load();
		const float dist = pendingDist.load();

		const bool force = forceRebuild.exchange(false);
		const bool airChanged = force || temp != lastTemp || hum != lastHum || pres != lastPres;
		const bool distChanged = force || dist != lastDist;

		//if only distance changes
		if (airChanged || distChanged)
		{
			if (airChanged)
			{
				airModel.setTemperature(temp);
				airModel.setHumidity(hum);
				airModel.setPressure(pres);
				calcMagnitudeResponse();
			
					
			}

			distance = dist;
			buildImpulseResponse();

			lastTemp = temp; lastHum = hum; lastPres = pres; lastDist = dist;
		}

		wait(30);
	}
}