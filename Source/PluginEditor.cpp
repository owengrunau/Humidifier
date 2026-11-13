/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AtmosphericAbsorptionAudioProcessorEditor::AtmosphericAbsorptionAudioProcessorEditor (AtmosphericAbsorptionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
    lookAndFeel(juce::ImageCache::getFromMemory(BinaryData::atomknob_grid_16x8_png, BinaryData::atomknob_grid_16x8_pngSize), 16, 8)
{
    background = juce::ImageCache::getFromMemory(BinaryData::backgroundWithTitle_png, BinaryData::backgroundWithTitle_pngSize);

    temperatureSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    humiditySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    distanceSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pressureSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);


    temperatureSlider.setTextValueSuffix(juce::String(juce::CharPointer_UTF8("\xc2\xb0" "C")));    humiditySlider.setTextValueSuffix("%");
    distanceSlider.setTextValueSuffix(" meters");
    pressureSlider.setTextValueSuffix(" atmospheres");

    temperatureSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 20);
    humiditySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 20);
    distanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 20);
    pressureSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 20);
   

    addAndMakeVisible(&temperatureSlider);
    addAndMakeVisible(&humiditySlider);
    addAndMakeVisible(&distanceSlider);
    addAndMakeVisible(&pressureSlider);

    setLookAndFeel(&lookAndFeel);
    setResizable(true, false);                           
    setResizeLimits(400, 225, 1600, 900);             
    getConstrainer()->setFixedAspectRatio(800.0 / 450.0);
    setSize(640, 360);


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
    setLookAndFeel(nullptr);
}

//==============================================================================
void AtmosphericAbsorptionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.drawImage(background, getLocalBounds().toFloat());    



}

void AtmosphericAbsorptionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    const float scale = getWidth() / 800.0f;

    const int slotWidth = 800 / 5;
    const int knobWidth = 150, knobHeight = 110, knobY = 300;

    

    auto placeInSlot = [&](juce::Slider& s, int slot)
        {
            const int centreX = slotWidth * slot + slotWidth / 2;
            s.setBounds(centreX - knobWidth / 2, knobY, knobWidth, knobHeight);
            s.setTransform(juce::AffineTransform::scale(scale));
        };

    placeInSlot(temperatureSlider, 0);
    placeInSlot(humiditySlider, 1);
    placeInSlot(pressureSlider, 2);
    placeInSlot(distanceSlider, 3);
}
