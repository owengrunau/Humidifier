#include "AbsorptionFilter.h"
#include <cmath>

AbsorptionFilter::AbsorptionFilter(int kernelSize)
	:
	juce::Thread("IR Generator"),
	kernelSize{ kernelSize },
	impulseResponse{ 1, kernelSize },
	fft{ static_cast<int>(std::log2(kernelSize)) },
	complexMag(kernelSize, { 0.0f, 0.0f }),
	output(kernelSize, { 0.0f, 0.0f })
{
};

AbsorptionFilter::~AbsorptionFilter() {
	stopThread(2000);
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

	if (!isThreadRunning())
		startThread(juce::Thread::Priority::normal);
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
		float m = pow(10.0f, -db / 20.0f);
		magnitudeResponse.push_back(m);
	}
}

void AbsorptionFilter::buildImpulseResponse() {
	

	impulseResponse.clear();

	//load mag response into complex vector 
	complexMag[0] = {pow(magnitudeResponse[0], distance), 0.0f};
	for (int i = 1; i <= kernelSize / 2; i++) {
		//implement distance at this stage
		complexMag[i] = juce::dsp::Complex<float>(pow(magnitudeResponse[i], distance), 0.0f); //imaginary is 0 for linear phase

		complexMag[kernelSize - i] = complexMag[i]; //complex congugate is the same since imaginary part is 0 
	} 

	fft.perform(complexMag.data(), output.data(), true);

	for (int i = 0; i < kernelSize; i++) {
		impulseResponse.setSample(0, i, output[i].real()); 
	}
	////////////////
	float peak = impulseResponse.getMagnitude(0, 0, kernelSize);
	juce::File f("C:\\temp\\ir_debug.log");
	f.appendText("dist=" + juce::String(distance)
		+ " mag0=" + juce::String(magnitudeResponse[0])
		+ " mag10=" + juce::String(magnitudeResponse.size() > 10 ? magnitudeResponse[1000] : -1.0f)
		+ " peak=" + juce::String(peak) + "\n");
	///////////////

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
		//get parameters from process block (audio thread)
		const float temp = pendingTemp.load();
		const float hum = pendingHum.load();
		const float pres = pendingPres.load();
		const float dist = pendingDist.load();

		const bool force = forceRebuild.exchange(false);
		const bool airChanged = force || temp != lastTemp || hum != lastHum || pres != lastPres;
		const bool distChanged = force || dist != lastDist;

		//if only distance changes, we can skip the air model calculations 
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