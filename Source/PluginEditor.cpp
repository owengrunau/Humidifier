/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AtmosphericAbsorptionAudioProcessorEditor::AtmosphericAbsorptionAudioProcessorEditor (AtmosphericAbsorptionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    temperatureSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    humiditySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distanceSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pressureSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    temperatureSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    humiditySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    distanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    pressureSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);



    temperatureSlider.setTextValueSuffix(" °C");
    humiditySlider.setTextValueSuffix(" %");
    distanceSlider.setTextValueSuffix(" meters");
    pressureSlider.setTextValueSuffix(" atmospheres");

    temperatureSlider.setLookAndFeel(&lookAndFeel);
    humiditySlider.setLookAndFeel(&lookAndFeel);
    distanceSlider.setLookAndFeel(&lookAndFeel);
    pressureSlider.setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(&temperatureSlider);
    addAndMakeVisible(&humiditySlider);
    addAndMakeVisible(&distanceSlider);
    addAndMakeVisible(&pressureSlider);

    setSize(400, 300);

    temperatureSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "temperature",
        temperatureSlider
    );
    humiditySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "humidity",
        humiditySlider
    );
    distanceSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "distance",
        distanceSlider
    );
    pressureSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts,
        "pressure",
        pressureSlider
    );
}

AtmosphericAbsorptionAudioProcessorEditor::~AtmosphericAbsorptionAudioProcessorEditor()
{
    temperatureSlider.setLookAndFeel(nullptr);
    humiditySlider.setLookAndFeel(nullptr);
    distanceSlider.setLookAndFeel(nullptr);
    pressureSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void AtmosphericAbsorptionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText("Humidifier", getLocalBounds(), juce::Justification::top, 1);

}

void AtmosphericAbsorptionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    area.removeFromTop(30);                     

    const int knobWidth = area.getWidth() / 4;

    temperatureSlider.setBounds(area.removeFromLeft(knobWidth).reduced(5));
    humiditySlider.setBounds(area.removeFromLeft(knobWidth).reduced(5));
    pressureSlider.setBounds(area.removeFromLeft(knobWidth).reduced(5));
    distanceSlider.setBounds(area.reduced(5));
}
