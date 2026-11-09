#pragma once
#include <JuceHeader.h>
class HumidifierLookAndFeel : public juce::LookAndFeel_V4
{
public:
	HumidifierLookAndFeel();
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
};

