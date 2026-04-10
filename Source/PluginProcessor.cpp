/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AtmosphericAbsorptionAudioProcessor::AtmosphericAbsorptionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    temperatureLocal = apvts.getRawParameterValue("temperature");
    humidityLocal = apvts.getRawParameterValue("humidity");
    distanceLocal = apvts.getRawParameterValue("distance");
    pressureLocal = apvts.getRawParameterValue("pressure");
}

AtmosphericAbsorptionAudioProcessor::~AtmosphericAbsorptionAudioProcessor()
{
}

//==============================================================================
const juce::String AtmosphericAbsorptionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AtmosphericAbsorptionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AtmosphericAbsorptionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AtmosphericAbsorptionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AtmosphericAbsorptionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AtmosphericAbsorptionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AtmosphericAbsorptionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AtmosphericAbsorptionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AtmosphericAbsorptionAudioProcessor::getProgramName (int index)
{
    return {};
}

void AtmosphericAbsorptionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AtmosphericAbsorptionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AtmosphericAbsorptionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AtmosphericAbsorptionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AtmosphericAbsorptionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
}

//==============================================================================
bool AtmosphericAbsorptionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AtmosphericAbsorptionAudioProcessor::createEditor()
{
    return new AtmosphericAbsorptionAudioProcessorEditor (*this);
}

//==============================================================================
void AtmosphericAbsorptionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void AtmosphericAbsorptionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AtmosphericAbsorptionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AtmosphericAbsorptionAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout pl;
    pl.add(
        std::make_unique<juce::AudioParameterFloat>("temperature", // parameterID                          
            "Temperature", // parameter name
            juce::NormalisableRange<float>(-50.0f, 50.0f, 0.01f, 1.0f),
            0.0f,
            "Celcius"),
        std::make_unique<juce::AudioParameterFloat>("humidity", // parameterID                          
            "Humidity", // parameter name
            juce::NormalisableRange<float>(-50.0f, 50.0f, 0.01f, 1.0f),
            0.0f,
            "Wetness"),
        std::make_unique<juce::AudioParameterFloat>("distance", // parameterID                          
            "Distance", // parameter name
            juce::NormalisableRange<float>(0.0f, 1000.0f, 0.1f, 1.0f),
            0.0f,
            "Meters"),
        std::make_unique<juce::AudioParameterFloat>("pressure", // parameterID                          
            "Pressure", // parameter name
            juce::NormalisableRange<float>(-50.0f, 50.0f, 0.01f, 1.0f),
            0.0f,
            "Atmospheres")
    );
    return pl;
}