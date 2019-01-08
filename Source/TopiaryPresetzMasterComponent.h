/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary Presetz, Copyright Tom Tollenaere 2018-2019.

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
#include"../../Topiary/Source/TopiaryTable.h"  
#include "TopiaryPresetzVariationChildren.h"

class TopiaryPresetzMasterComponent;


class TopiaryPresetzMasterComponent : public Component, ActionListener
{
public:
	TopiaryPresetzMasterComponent();
	~TopiaryPresetzMasterComponent();

	void paint(Graphics&) override;
	void resized() override;
	void setModel(TopiaryPresetzModel* m);

	void getPresetElementData(int p);
	void setPresetElementData(int p);
	void getVariationDefinition();
	void setVariationDefinition();
	void setRTValue(int p, int v);   // does NOT output in real time !!!


private:
	TopiaryPresetzModel* presetzModel;
	TopiaryLookAndFeel topiaryLookAndFeel;
	void actionListenerCallback(const String &message);

	VariationDefinitionComponent variationDefinitionComponent;
	
	int variation = -1; // the one we are working on; 0-7

	static const int buttonH = 20;
	static const int sliderH = 80;
	static const int sliderV = 80;
	static const int sliderTW = 60;
	static const int sliderTH = 20;

	int buttonW = 100;
	int patternButtonOffsetX = 280;

	// the preset components

	PresetElementComponent presetElement[PRESETELEMENTS];


	// settings stuff

	TopiaryButton WFFNButton;
	TopiaryButton notePassThroughButton;
	//ComboBox switchVariationCombo;
	ComboBox quantizeVariationStartCombo;
	//ComboBox quantizeRunStopCombo;
	TextButton saveButton;
	TextButton loadButton;
	TextEditor nameEditor;


	//////////////////////////////////////////////////////

	void setSettings()
	{  
		// called when settings changes
		
		presetzModel->setWFFN(WFFNButton.getToggleState());
		presetzModel->setNotePassThrough(notePassThroughButton.getToggleState());
		presetzModel->setVariationStartQ(quantizeVariationStartCombo.getSelectedId());
		
		//presetzModel->setRunStopQ(quantizeRunStopCombo.getSelectedId());
		presetzModel->setName(nameEditor.getText());
		//presetzModel->setSwitchVariation(switchVariationCombo.getSelectedId());

	} // setSettings

	//////////////////////////////////////////////////////


	void getSettings()
	{  
		// called when settings change
		
		WFFNButton.setToggleState(presetzModel->getWFFN(), dontSendNotification);
		notePassThroughButton.setToggleState(presetzModel->getNotePassThrough(), dontSendNotification);
		quantizeVariationStartCombo.setSelectedId(presetzModel->getVariationStartQ(), dontSendNotification);
		//quantizeRunStartCombo.setSelectedId(presetzModel->getRunStartQ(), dontSendNotification);
		//quantizeRunStopCombo.setSelectedId(presetzModel->getRunStopQ(), dontSendNotification);
		//switchVariationCombo.setSelectedId(presetzModel->getSwitchVariation(), dontSendNotification);
		auto name = presetzModel->getName();
		nameEditor.setText(name, dontSendNotification);
		
		// disable when host overridden
		int b, n, d, bs;
		bool override, wf;
		presetzModel->getTransportState(b, n, d, bs, override, wf);
		
	} // getSettings
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryPresetzMasterComponent)
};

/////////////////////////////////////////////////////////////////////


