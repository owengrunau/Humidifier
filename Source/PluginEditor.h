/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HumidifierLookAndFeel.h"

//==============================================================================
/**
*/
class AtmosphericAbsorptionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AtmosphericAbsorptionAudioProcessorEditor (AtmosphericAbsorptionAudioProcessor&);
    ~AtmosphericAbsorptionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AtmosphericAbsorptionAudioProcessor& audioProcessor;

    HumidifierLookAndFeel lookAndFeel;

    juce::Slider temperatureSlider, humiditySlider, distanceSlider, pressureSlider;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> temperatureSliderAttachment, humiditySliderAttachment, distanceSliderAttachment, pressureSliderAttachment;
  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AtmosphericAbsorptionAudioProcessorEditor)
};
