#pragma once
#include <JuceHeader.h>
#include "BinaryData.h"
class HumidifierLookAndFeel : public juce::LookAndFeel_V4
{
public:
	HumidifierLookAndFeel(const juce::Image& gridImage, int numRows, int numCols);

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
	juce::Label* createSliderTextBox(juce::Slider& slider) override;
	juce::Font getLabelFont(juce::Label& label) override;
private:
	juce::Typeface::Ptr font;
	juce::Image grid;
	int cols, rows;
	int frameHeight, frameWidth;
	int frames;

};
