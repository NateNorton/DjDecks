/*
  ==============================================================================

    Style.cpp
    Created: 22 Feb 2022 3:28:31pm
    Author:  Nathan Norton

  ==============================================================================
*/

#include "Style.h"

void CustomLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);

    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin (8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (juce::Colour{ 162, 213, 198 });
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        juce::Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (juce::Colour{ 7, 123, 138 });
        g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    //auto thumbWidth = lineW * 2.0f;
    juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

    g.setColour (juce::Colour{ 84, 15, 19 });
    // draw line from centre to outer edge
    g.drawLine(backgroundArc.getBounds().getCentreX(),
               backgroundArc.getBounds().getCentreY(),
               thumbPoint.getX(),
               thumbPoint.getY(),
               lineW);
}

//juce::Label* CustomLookAndFeel::createSliderTextBox (juce::Slider& slider)
//{
//    auto l = new juce::LookAndFeel_V2::SliderLabelComp();
//
//    l->setJustificationType (juce::Justification::centred);
//    l->setKeyboardType (juce::TextInputTarget::decimalKeyboard);
//
//    l->setColour (juce::Label::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
//    l->setColour (juce::Label::backgroundColourId,
//                  (slider.getSliderStyle() == juce::Slider::LinearBar || slider.getSliderStyle() == juce::Slider::LinearBarVertical)
//                  ? juce::Colours::transparentBlack
//                  : slider.findColour (juce::Slider::textBoxBackgroundColourId));
//    l->setColour (juce::Label::outlineColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));
//    l->setColour (juce::TextEditor::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
//    l->setColour (juce::TextEditor::backgroundColourId,
//                  slider.findColour (juce::Slider::textBoxBackgroundColourId)
//                  .withAlpha ((slider.getSliderStyle() == juce::Slider::LinearBar || slider.getSliderStyle() == juce::Slider::LinearBarVertical)
//                                        ? 0.7f : 1.0f));
//    l->setColour (juce::TextEditor::outlineColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));
//    l->setColour (juce::TextEditor::highlightColourId, slider.findColour (juce::Slider::textBoxHighlightColourId));
//
//    return l;
//}

void CustomLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                           juce::Button& button,
                                           const juce::Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto cornerSize = 6.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (4.0f, 4.0f);
    
    auto backCol = juce::Colour{ 162, 213, 198 };

    auto baseColour = backCol.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                      .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour (baseColour);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        juce::Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.strokePath (path, juce::PathStrokeType (1.0f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (juce::Colour{ 7, 123, 138 });
        g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    }
}

void CustomLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button,
                                     bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
    juce::Font font (getTextButtonFont (button, button.getHeight()));
    g.setFont (font);
    g.setColour (juce::Colour{ 84, 15, 19 });

    const int yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                          juce::Justification::centred, 2);
}
