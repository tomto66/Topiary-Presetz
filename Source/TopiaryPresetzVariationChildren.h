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
#include"TopiaryPresetzModel.h"

class TopiaryPresetzMasterComponent;

////////////////////////////////////////////////////////////////////////////////
// VariationDefinitionComponent
////////////////////////////////////////////////////////////////////////////////

class VariationDefinitionComponent : public Component
{
public:

	ComboBox variationCombo;
	TextEditor nameEditor;
	TopiaryButton enableButton;
	ComboBox timingCombo;
	TextButton allTimingButton;

	int width = 480;
	int heigth = 78;

	VariationDefinitionComponent();
	~VariationDefinitionComponent();
	void setParent(TopiaryPresetzMasterComponent* p);

	void resized();
	void paint(Graphics& g) override;

private:

	TopiaryPresetzMasterComponent* parent;

	static const int eW = 120; // editor & combo & enable button
	static const int eH = 20; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VariationDefinitionComponent)

}; // VariationDefinitionComponent

////////////////////////////////////////////////////////////////////////////////
// PresetElementComponent
////////////////////////////////////////////////////////////////////////////////

class PresetElementComponent : public Component
{
public:

	ToggleButton enableButton;
	TextEditor nameEditor;

	TextEditor valueFromEditor, valueToEditor, inChannelEditor, outCCEditor, outChannelEditor;
	TopiaryMidiLearnEditor inCCEditor;

	TopiaryLookAndFeel presetLookAndFeel;

	Slider presetSlider, RTSlider;

	int width = 234;
	int heigth = 130;
	int index; // number of the preset in the parent

	PresetElementComponent();
	~PresetElementComponent();
	
	void resized();
	void paint(Graphics& g) override;

	void setParent(TopiaryPresetzMasterComponent* p, int i);  // i is index in parent
	void setEnables();

private:

	TopiaryPresetzMasterComponent* parent;
	
	static const int eW = 150; // editor & combo & enable button
	static const int eH = 20;

	static const int seW = 30; // size for small editors (CC, from, to channel)

	static const int sliderTW = 30;
	static const int sliderTH = 20;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetElementComponent)

}; // VariationDefinitionComponent