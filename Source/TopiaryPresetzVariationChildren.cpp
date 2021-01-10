/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary Presetz, Copyright Tom Tollenaere 2018-2021.

Topiary Presetz is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Topiary Presetz is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Topiary Presetz. If not, see <https://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TopiaryPresetzModel.h"
#include "TopiaryPresetzVariationChildren.h"
#include "TopiaryPresetzMasterComponent.h"


/////////////////////////////////////////////////////////////////////////
// VariationDefinitionComponent
/////////////////////////////////////////////////////////////////////////

	VariationDefinitionComponent::VariationDefinitionComponent()
	{
		setSize(width, heigth);
		addAndMakeVisible(variationCombo);
		//variationCombo.addItem("-", 1); // dummy so fillVariationCombo does not crash
		
		variationCombo.onChange = [this]
		{
			parent->getVariationDefinition(); 
		};

		addAndMakeVisible(timingCombo);
		timingCombo.addItem("Instant", Topiary::Quantization::Immediate);
		timingCombo.addItem("1/4", Topiary::Quantization::Quarter);
		timingCombo.addItem("1/2", Topiary::Quantization::Half);
		timingCombo.addItem("Measure", Topiary::Quantization::Measure);

		timingCombo.onChange = [this]
		{
			parent->setVariationDefinition();
		};

		addAndMakeVisible(nameEditor);
		nameEditor.setSelectAllWhenFocused(true);
		nameEditor.onFocusLost = [this]
		{
			parent->setVariationDefinition();
		};

		nameEditor.onReturnKey = [this]
		{
			parent->setVariationDefinition();
			parent->fillVariationCombo();
		};

		addAndMakeVisible(enableButton);
		enableButton.setClickingTogglesState(true);
		enableButton.setButtonText("Enabled");
		enableButton.onClick = [this]
		{
			parent->setVariationDefinition();
		};

		addAndMakeVisible(allTimingButton);
		allTimingButton.setButtonText("ALL");
		allTimingButton.onClick = [this]
		{
			parent->setAllTransitionTimes(timingCombo.getSelectedId());
		};

	} // VariationDefinitionComponent

	///////////////////////////////////////////////////////////////////////////////////

	void VariationDefinitionComponent::setParent(TopiaryPresetzMasterComponent* p)
	{
		parent = p;
		parent->fillVariationCombo();
	} // setparent

	///////////////////////////////////////////////////////////////////////////////////

	VariationDefinitionComponent::~VariationDefinitionComponent()
	{
	} // ~VariationDefinitionComponent

	///////////////////////////////////////////////////////////////////////////////////

	void  VariationDefinitionComponent::resized()
	{

	} // resized

	///////////////////////////////////////////////////////////////////////////////////

	void  VariationDefinitionComponent::paint(Graphics& g)
	{
		int labelOffset = 15;
		int lineWidth = 2;
		auto area = getLocalBounds();

		g.fillAll(TopiaryColour::background);
		g.setColour(TopiaryColour::foreground);
		g.setFont(12.0f);
		g.drawText("Variation Definition", lineWidth, lineWidth, 200, labelOffset, juce::Justification::centredLeft);

		auto recBounds = area.removeFromBottom(area.getHeight() - labelOffset);
		g.drawRoundedRectangle((float) recBounds.getX() + lineWidth, (float) recBounds.getY() + lineWidth, (float) recBounds.getWidth() - 2 * lineWidth, (float) recBounds.getHeight() - 2 * lineWidth, (float) lineWidth, (float) lineWidth);

		// turn it into inner rectangle
		auto inRecBounds = Rectangle<int>::Rectangle(recBounds.getX() + 3 * lineWidth, recBounds.getY() + 3 * lineWidth, recBounds.getWidth() - 6 * lineWidth, recBounds.getHeight() - 6 * lineWidth);

		// area for name and from editors
		auto topBounds = inRecBounds.removeFromTop(eH);
		
		// topBounds is first line of objects
		//auto hBounds = topBounds.removeFromLeft(eW);
		auto leftBounds = topBounds.removeFromLeft(220);
		nameEditor.setBounds(leftBounds);
		nameEditor.setSize(leftBounds.getWidth(), leftBounds.getHeight());

		topBounds.removeFromLeft(25);
		enableButton.setBounds(topBounds);
		enableButton.setSize(topBounds.getWidth(), topBounds.getHeight());

		inRecBounds.removeFromTop(4 * lineWidth);
		topBounds = inRecBounds.removeFromTop(eH);

		// topBounds is second line of objects
		
		leftBounds = topBounds.removeFromLeft(220);
		variationCombo.setBounds(leftBounds);
		variationCombo.setSize(leftBounds.getWidth(), leftBounds.getHeight());
		
		topBounds.removeFromLeft(110);
		leftBounds = topBounds.removeFromLeft(80);
		timingCombo.setBounds(leftBounds);
		timingCombo.setSize(leftBounds.getWidth(), leftBounds.getHeight());
		
		topBounds.removeFromLeft(10);
		allTimingButton.setBounds(topBounds);
		allTimingButton.setSize(topBounds.getWidth(), topBounds.getHeight());

		g.drawText("Transition time", 255, 53, 200, labelOffset, juce::Justification::centredLeft);
	} // paint

/////////////////////////////////////////////////////////////////////////
// PresetElementComponent
/////////////////////////////////////////////////////////////////////////

	PresetElementComponent::PresetElementComponent()
	{
		setSize(width, heigth);

		addAndMakeVisible(nameEditor);
		nameEditor.setSelectAllWhenFocused(true);
		nameEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
		};

		nameEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
		};

		addAndMakeVisible(inChannelEditor);
		inChannelEditor.setSelectAllWhenFocused(true);
		inChannelEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
		};

		inChannelEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
		};

		addAndMakeVisible(outChannelEditor);
		outChannelEditor.setSelectAllWhenFocused(true);
		outChannelEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
		};

		outChannelEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
		};



		addAndMakeVisible(inCCEditor);
		inCCEditor.setSelectAllWhenFocused(true);
		inCCEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
		};

		inCCEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
		};

		addAndMakeVisible(outCCEditor);
		outCCEditor.setSelectAllWhenFocused(true);
		outCCEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
		};

		outCCEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
		};

		addAndMakeVisible(valueFromEditor);
		valueFromEditor.setSelectAllWhenFocused(true);
		valueFromEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
			//presetSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
			//RTSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
		};

		valueFromEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
			//presetSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
			//RTSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
		};

		addAndMakeVisible(valueToEditor);
		valueToEditor.setSelectAllWhenFocused(true);
		valueToEditor.onFocusLost = [this]
		{
			parent->setPresetElementData(index);
			//presetSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
			//RTSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
		};

		valueToEditor.onReturnKey = [this]
		{
			parent->setPresetElementData(index);
			//presetSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
			//RTSlider.setRange(valueFromEditor.getText().getIntValue(), valueToEditor.getText().getIntValue(),1);
		};

		addAndMakeVisible(enableButton);
		enableButton.setClickingTogglesState(true);
		enableButton.setButtonText("");
		enableButton.onClick = [this]
		{
			parent->setPresetElementData(index);
		};

		addAndMakeVisible(presetSlider);
		presetSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		presetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sliderTW, sliderTH);
		presetSlider.setTextBoxIsEditable(true);
		presetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, sliderTW, sliderTH);
		presetSlider.setRange((double)0, (double)127, (double)1);
		presetSlider.setTextValueSuffix("");
		presetSlider.onValueChange = [this]
		{
			parent->setVariationDefinition(); // the preset value is part of the variation definition, not the preset definition!
		};

		addAndMakeVisible(RTSlider);
		RTSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		RTSlider.setTextBoxStyle(Slider::TextBoxBelow, false, sliderTW, sliderTH);
		RTSlider.setTextBoxIsEditable(true);
		RTSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, sliderTW, sliderTH);
		RTSlider.setRange((double)0, (double)127, (double)1);
		RTSlider.setTextValueSuffix("");
		RTSlider.onValueChange = [this]
		{
			parent->setRTValue(index, (int) RTSlider.getValue());
		};


	} // VariationDefinitionComponent

	///////////////////////////////////////////////////////////////////////////////////

	void PresetElementComponent::setParent(TopiaryPresetzMasterComponent* p, int i)
	{
		parent = p;
		index = i;
	} // setparent

	///////////////////////////////////////////////////////////////////////////////////

	PresetElementComponent::~PresetElementComponent()
	{
	} // ~VariationDefinitionComponent

	///////////////////////////////////////////////////////////////////////////////////

	void  PresetElementComponent::resized()
	{

	} // resized

	///////////////////////////////////////////////////////////////////////////////////

	void  PresetElementComponent::paint(Graphics& g)
	{
		int labelOffset = 15;
		int lineWidth = 2;
		auto area = getLocalBounds();

		g.fillAll(TopiaryColour::background);
		g.setColour(TopiaryColour::foreground);
		g.setFont(12.0f);
		g.drawText("Preset Definition "+String(index+1), lineWidth, lineWidth, 200, labelOffset, juce::Justification::centredLeft);

		auto recBounds = area.removeFromBottom(area.getHeight() - labelOffset);
		g.drawRoundedRectangle((float)recBounds.getX() + lineWidth, (float)recBounds.getY() + lineWidth, (float)recBounds.getWidth() - 2 * lineWidth, (float)recBounds.getHeight() - 2 * lineWidth, (float)lineWidth, (float)lineWidth);

		// turn it into inner rectangle
		auto inRecBounds = Rectangle<int>::Rectangle(recBounds.getX() + 3 * lineWidth, recBounds.getY() + 3 * lineWidth, recBounds.getWidth() - 6 * lineWidth, recBounds.getHeight() - 6 * lineWidth);

		// area for name enable tickbox
		auto topBounds = inRecBounds.removeFromTop(eH);

		// topBounds is first line of objects
		
		auto sideBounds = topBounds.removeFromLeft(25);
		sideBounds.removeFromTop(5);
		sideBounds.removeFromRight(5);
		enableButton.setBounds(sideBounds);
		enableButton.setSize(sideBounds.getWidth(), sideBounds.getHeight());

		topBounds.removeFromLeft(2 * lineWidth);
		nameEditor.setBounds(topBounds);

		inRecBounds.removeFromTop(4 * lineWidth);

		sideBounds = inRecBounds.removeFromLeft(4* seW + lineWidth); // space for the 4 editors

		g.drawText("CC             Ch           in          P / RT", 10, 52, 200, labelOffset, juce::Justification::centredLeft);
		auto lineBounds = sideBounds.removeFromTop(eH); // line for CC 7 Channel editor
		lineBounds.removeFromLeft((int)seW);
		inCCEditor.setBounds(lineBounds.removeFromLeft(seW));

		lineBounds.removeFromLeft((int)seW-10);
		inChannelEditor.setBounds(lineBounds.removeFromLeft(seW));

		inCCEditor.setSize(inCCEditor.getBounds().getWidth(), inCCEditor.getBounds().getHeight());
		inChannelEditor.setSize(inChannelEditor.getBounds().getWidth(), inChannelEditor.getBounds().getHeight());

		g.drawText("CC             Ch           out             ", 10, 80, 200, labelOffset, juce::Justification::centredLeft);
		lineBounds = sideBounds.removeFromTop(4 * lineWidth);
		lineBounds = sideBounds.removeFromTop(eH); // line for CC 7 Channel editor
		lineBounds.removeFromLeft((int)seW);
		outCCEditor.setBounds(lineBounds.removeFromLeft(seW));

		lineBounds.removeFromLeft((int)seW - 10);
		outChannelEditor.setBounds(lineBounds.removeFromLeft(seW));

		outCCEditor.setSize(outCCEditor.getBounds().getWidth(), outCCEditor.getBounds().getHeight());
		outChannelEditor.setSize(outChannelEditor.getBounds().getWidth(), outChannelEditor.getBounds().getHeight());

			   
		g.drawText("Frm            To", 10, 108, 200, labelOffset, juce::Justification::centredLeft);

		lineBounds = sideBounds.removeFromTop(4*lineWidth);		
		lineBounds = sideBounds.removeFromTop(eH);
		lineBounds.removeFromLeft((int)seW);
		valueFromEditor.setBounds(lineBounds.removeFromLeft(seW));
		
		lineBounds.removeFromLeft((int)seW-10);
		valueToEditor.setBounds(lineBounds.removeFromLeft(seW));

		valueFromEditor.setSize(valueFromEditor.getBounds().getWidth(), valueFromEditor.getBounds().getHeight());
		valueToEditor.setSize(valueToEditor.getBounds().getWidth(), valueToEditor.getBounds().getHeight());
		
		//////////////////
		// sliders; inRecBounds is space for the sliders

		lineBounds = inRecBounds.removeFromLeft(50);

		//g.fillRect(lineBounds);
		presetSlider.setBounds(lineBounds);
		presetSlider.setSize(lineBounds.getWidth(), lineBounds.getHeight());
		presetSlider.setLookAndFeel(&presetLookAndFeel);

		lineBounds = inRecBounds.removeFromLeft(50);
		RTSlider.setBounds(lineBounds);
		RTSlider.setSize(lineBounds.getWidth(), lineBounds.getHeight());
		RTSlider.setLookAndFeel(&presetLookAndFeel);
		
	} // paint

	///////////////////////////////////////////////////////////////////////////////////

	void PresetElementComponent::setEnables()
	{
		// enables/disables stuff
		if (enableButton.getToggleState()) {
			nameEditor.setEnabled(true);
			valueFromEditor.setEnabled(true);
			valueToEditor.setEnabled(true);
			inCCEditor.setEnabled(true);
			inChannelEditor.setEnabled(true);
			outCCEditor.setEnabled(true);
			outChannelEditor.setEnabled(true);
			presetSlider.setEnabled(true);
			RTSlider.setEnabled(true);
		}
		else
		{
			nameEditor.setEnabled(false);
			valueFromEditor.setEnabled(false);
			valueToEditor.setEnabled(false);
			inCCEditor.setEnabled(false);
			inChannelEditor.setEnabled(false);
			outCCEditor.setEnabled(false);
			outChannelEditor.setEnabled(false);
			presetSlider.setEnabled(false);
			RTSlider.setEnabled(false);
		}
	}

