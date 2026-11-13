Humidifier is a JUCE plug-in that simulates atmospheric absorption of sound using an FIR filter controlled by four parameters: distance from the source, temperature of the air, atmospheric pressure, and humidity of the air. 

It uses the below formulas (ISO 9613-1:1993) to create a magnitude response from the air parameters, scaled by the distance, and then creates a minimum phase impulse response. The impulse responses are built on a separate thread, and juce::dsp::Convolution is used to convolve the input and crossfade new impulse responses. Minimum phase is used so that the filter has no latency.

Formulas:
https://en.wikibooks.org/wiki/Engineering_Acoustics/Outdoor_Sound_Propagation#Attenuation_by_atmospheric_absorption[5]_[6]_[7] 

Creating a minimum phase response from a magnitude response:
https://www.dsprelated.com/freebooks/filters/Matlab_listing_mps_m_test.html


TODO:
Spectral amplitude response graphic, text scrolling decoration