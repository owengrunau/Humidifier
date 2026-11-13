#include "HumidifierLookAndFeel.h"

HumidifierLookAndFeel::HumidifierLookAndFeel(const juce::Image& gridImage, int numRows, int numCols):
    grid(gridImage),
    rows(numRows), cols(numCols)
{
    frameHeight = grid.getHeight() / numRows;
    frameWidth = grid.getWidth() / numCols;
    frames = numRows * numCols;

    font = juce::Typeface::createSystemTypefaceFor(BinaryData::SkModernistRegular_otf, BinaryData::SkModernistRegular_otfSize);

}

void HumidifierLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
    {

    const int frameIndex = juce::jlimit(0, frames - 1, (int)std::round(sliderPos * (frames - 1)));

    //using a rectangle keeps knob centered if the slider is not square
    auto dest = juce::Rectangle<int>(x, y, width, height).withSizeKeepingCentre(juce::jmin(width, height), juce::jmin(width, height));

    //top left px of frame
    int sourceX = frameIndex % cols * frameWidth;
    int sourceY = frameIndex / cols * frameHeight;




    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);



    g.drawImage(grid, dest.getX(), dest.getY(), dest.getWidth(), dest.getHeight(), 
        sourceX, sourceY, frameWidth, frameHeight);
}

juce::Label* HumidifierLookAndFeel::createSliderTextBox(juce::Slider& slider){
    auto label = LookAndFeel_V4::createSliderTextBox(slider);

    label->setJustificationType(juce::Justification::centredBottom);
    label->setKeyboardType(juce::TextInputTarget::decimalKeyboard);

    //forum.juce.com/t/text-position-when-editing-labels-and-text-editor/5362/12 
    label->onEditorShow = [label]
        {
            auto editor = label->getCurrentTextEditor();
            editor->setJustification(Justification::centredBottom);
            editor->setIndents(0, 0);

            auto editorBounds = editor->getBounds();
            editorBounds.setY(editorBounds.getY() - 2);
            editorBounds.translate(2, 0);
            editorBounds.expand(0, 2);
            editor->setBounds(editorBounds);
        };

    label->setColour(juce::Label::textColourId, juce::Colour(79, 216, 255));
    label->setColour(juce::Label::textColourId, juce::Colour(0, 0, 0));
    label->setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    label->setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

    label->setColour(juce::Label::textWhenEditingColourId, juce::Colours::white);
    label->setColour(juce::Label::backgroundWhenEditingColourId, juce::Colour(juce::Colours::transparentBlack));
    label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colour(juce::Colours::transparentBlack));
    label->setColour(juce::TextEditor::highlightColourId, juce::Colour(255, 0, 255).withAlpha(0.4f)); 
    label->setColour(juce::CaretComponent::caretColourId, juce::Colours::white);                     \
    return label;
}


juce::Font HumidifierLookAndFeel::getLabelFont(juce::Label& label)
{
    if (dynamic_cast<juce::Slider*> (label.getParentComponent()) != nullptr)
        return juce::FontOptions(font).withHeight(16.0f);

    return LookAndFeel_V4::getLabelFont(label);
}

