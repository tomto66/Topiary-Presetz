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
#include "TopiaryPresetzMasterComponent.h"

TopiaryPresetzMasterComponent::TopiaryPresetzMasterComponent()
{   // size set by TabbedComponent!

	variation = 0;
	getVariationCalledFromChangeInVariationButton = false;
	variationDefinitionComponent.setParent(this);
	addAndMakeVisible(variationDefinitionComponent);

	/////////////////////////////////
	// Preset elements
	/////////////////////////////////

	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		addAndMakeVisible(presetElement[p]); // it sets its own size
	}

	/////////////////////////////////
	// Settings
	/////////////////////////////////

	// WFFNButton

	WFFNButton.setClickingTogglesState(true);
	WFFNButton.setLookAndFeel(&topiaryLookAndFeel);
	WFFNButton.setSize(100, buttonH);
	addAndMakeVisible(WFFNButton);
	WFFNButton.setButtonText("WFFN");
	WFFNButton.onClick = [this]
	{
		setSettings();
	};

	// notePassThoughButton

	notePassThroughButton.setClickingTogglesState(true);
	notePassThroughButton.setLookAndFeel(&topiaryLookAndFeel);
	notePassThroughButton.setSize(100, buttonH);
	addAndMakeVisible(notePassThroughButton);
	notePassThroughButton.setButtonText("Echo notes");
	notePassThroughButton.onClick = [this]
	{
		setSettings();
	};

	// quantizeVariationStartCombo;

	addAndMakeVisible(quantizeVariationStartCombo);
	quantizeVariationStartCombo.setSize(100, buttonH);
	quantizeVariationStartCombo.addItem("Immediate", Topiary::Quantization::Immediate);
	//quantizeVariationStartCombo.addItem("Whole variation", TopiaryPresetzModel::WholePattern);
	quantizeVariationStartCombo.addItem("Measure", Topiary::Quantization::Measure);
	quantizeVariationStartCombo.addItem("1/2", Topiary::Quantization::Half);
	//quantizeVariationStartCombo.addItem("1/3", 4);
	quantizeVariationStartCombo.addItem("1/4", Topiary::Quantization::Quarter);
	//quantizeVariationStartCombo.addItem("1/8", 6);
	//quantizeVariationStartCombo.addItem("1/16", 7);
	//quantizeVariationStartCombo.addItem("1/32", 8);

	quantizeVariationStartCombo.onChange = [this]
	{
		setSettings();
	};

	addAndMakeVisible(saveButton);
	saveButton.setSize(100, buttonH);
	saveButton.setButtonText("Save");
	saveButton.onClick = [this]
	{
		presetzModel->savePreset("Please select Topiay Presetz file to load...", "*.tpr");
	};
	
	addAndMakeVisible(loadButton);
	loadButton.setSize(100, buttonH);
	loadButton.setButtonText("Load");
	loadButton.onClick = [this]
	{
			presetzModel->loadPreset("Please select Topiary Presetz file to load...", "*.tpr");
			setModel(presetzModel);
			getSettings();			
	};

	addAndMakeVisible(nameEditor);
	nameEditor.setSize(210, buttonH);
	
	nameEditor.onReturnKey = [this]
	{
		setSettings();
	};
	nameEditor.onFocusLost = [this]
	{
		setSettings();
	};
	
	getVariationDefinition();

} // TopiaryPresetzMasterComponent() 

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzMasterComponent::~TopiaryPresetzMasterComponent()
{
	presetzModel->removeListener((ActionListener*)this);
} // ~TopiaryPresetzMasterComponent

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::setModel(TopiaryPresetzModel* m)
{
	presetzModel = m;
	
	presetzModel->setListener((ActionListener*)this);

	// trick to call the model and read 
	
	actionListenerCallback(MsgTransport);
	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		presetElement[p].setParent(this, p); // it sets its own size
		getPresetElementData(p); // get the data	
		presetElement[p].inCCEditor.setModel(presetzModel, p + TopiaryLearnMidiId::presetMidiCin);  // for Midi  learning
	}

	variationDefinitionComponent.variationCombo.setSelectedId(1);
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::paint(Graphics& g)
{
	int lineSize = 2;
	int labelOffset = 15;
	
	g.fillAll(TopiaryColour::background);
	g.setColour(TopiaryColour::foreground);
	g.setFont(12.0f);

	variationDefinitionComponent.setBounds(500, 288, variationDefinitionComponent.width, variationDefinitionComponent.heigth);

	//// preset elements

	presetElement[0].setBounds(10, 10, presetElement[0].width, presetElement[0].heigth);
	presetElement[1].setBounds(255, 10, presetElement[1].width, presetElement[1].heigth);
	presetElement[2].setBounds(500, 10, presetElement[2].width, presetElement[1].heigth);
	presetElement[3].setBounds(745, 10, presetElement[3].width, presetElement[1].heigth);

	presetElement[4].setBounds(10, 150, presetElement[4].width, presetElement[0].heigth);
	presetElement[5].setBounds(255, 150, presetElement[5].width, presetElement[1].heigth);
	presetElement[6].setBounds(500, 150, presetElement[6].width, presetElement[1].heigth);
	presetElement[7].setBounds(745, 150, presetElement[7].width, presetElement[1].heigth);



	//// Settings

	g.drawText("Settings", 10, 290, 500, labelOffset, juce::Justification::centredLeft);
	g.drawRoundedRectangle((float)lineSize + 10, (float)305, (float)475, (float)60, (float)4, (float)lineSize);

	WFFNButton.setBounds(20, 310, 100, buttonH);

	notePassThroughButton.setBounds(130, 310, 100, buttonH);

	g.setColour(Colours::white);
	
	g.drawText("Q Variation Start:", 250, 310, 200, labelOffset + 3, juce::Justification::centredLeft);
	quantizeVariationStartCombo.setBounds(350, 310, 110, buttonH);

	nameEditor.setBounds(20, 340, 210, buttonH);
	saveButton.setBounds(250, 340,100,buttonH);   
	loadButton.setBounds(360, 340, 100, buttonH);
	
} // paint

void TopiaryPresetzMasterComponent::resized()
{
} // resized

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::actionListenerCallback(const String &message)
{
	if (message.compare(MsgTransport) == 0)
		getSettings();
	if (message.compare(MsgMaster) == 0) 
	{
		getVariationDefinition();  // will get the variation in the selected combo; if invalid it will set it to 1; then it will get the preset definitions
	}
	else if (message.compare(MsgRealTimeParameter) == 0)
	{
		// something changed in realtime; update the RT values
		for (int i = 0; i < PRESETELEMENTS; i++)
		{
			presetElement[i].RTSlider.setValue(presetzModel->getRTValue(i), dontSendNotification);
		}
	}
	else if (message.compare(MsgVariationSelected) == 0)
	{
		// set the combo to the selected variation
		int unused, newVariation;
		presetzModel->getVariation(newVariation, unused);
		if (variation != newVariation)
		{
			variationDefinitionComponent.variationCombo.setSelectedId(newVariation + 1, dontSendNotification);
			getVariationCalledFromChangeInVariationButton = true;
			getVariationDefinition();
			getVariationCalledFromChangeInVariationButton = false;
			UNUSED(unused)
		}
	}
	
} // actionListenerCallback

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::getPresetElementData(int p)
{
	// get the preset definition data for preset P, as well as the variation data for the variation selected

	jassert((p >= 0) && (p < PRESETELEMENTS));
	String pname;
	int from, to, inCC, inChannel, outCC, outChannel;
	bool enabled = true;  // avoid warning for uninitialized variable

	presetzModel->getPresetDefinition(p, pname, from, to, inCC, inChannel, outCC, outChannel, enabled);

	presetElement[p].RTSlider.setValue(presetzModel->getRTValue(p), dontSendNotification);
	presetElement[p].inCCEditor.setText(String(inCC), dontSendNotification);
	presetElement[p].inChannelEditor.setText(String(inChannel), dontSendNotification);
	presetElement[p].outCCEditor.setText(String(outCC), dontSendNotification);
	presetElement[p].outChannelEditor.setText(String(outChannel), dontSendNotification);
	presetElement[p].valueFromEditor.setText(String(from), dontSendNotification);
	presetElement[p].valueToEditor.setText(String(to), dontSendNotification);
	presetElement[p].enableButton.setToggleState(enabled, dontSendNotification);
	presetElement[p].nameEditor.setText(pname, dontSendNotification);

	presetElement[p].RTSlider.setRange(from, to, 1);
	presetElement[p].presetSlider.setRange(from, to, 1);

} // getPresetElementData

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::setPresetElementData(int p)
{
	// write the preset definition data for preset P, as well as the variation data for the variation selected
	jassert((p >= 0) && (p < PRESETELEMENTS));

	bool warning = false;

	// do some validations:
	int from = presetElement[p].valueFromEditor.getText().getIntValue();
	int to = presetElement[p].valueToEditor.getText().getIntValue();
	int inCC = presetElement[p].inCCEditor.getText().getIntValue();
	int outCC = presetElement[p].outCCEditor.getText().getIntValue();
	int inChannel = presetElement[p].inChannelEditor.getText().getIntValue();
	int outChannel = presetElement[p].outChannelEditor.getText().getIntValue();

	if ((from < 0) || (from > 127))
	{
		presetzModel->Log("Preset "+String(p+1)+": From value needs to be beween 0 and 127.", Topiary::Warning);
		warning = true;
		from = 0;
	}

	if ((to < 0) || (to > 127))
	{
		presetzModel->Log("Preset " + String(p+1) + ": To value needs to be beween 0 and 127.", Topiary::Warning);
		to = 127;
		warning = true;
	}

	if (from >= to)
	{
		presetzModel->Log("Preset " + String(p+1) + ": From should be < To.", Topiary::Warning);
		from = 0;
		warning = true;
	}

	if ((inCC < 0) || (inCC > 127))
	{
		presetzModel->Log("Preset " + String(p+1) + ": CC input value needs to be beween 0 and 127.", Topiary::Warning);
		inCC = 20;
		warning = true;
	}

	if ((outCC < 0) || (outCC > 127))
	{
		presetzModel->Log("Preset " + String(p + 1) + ": CC output value needs to be beween 0 and 127.", Topiary::Warning);
		outCC = 20;
		warning = true;
	}
	
	if ((inChannel < 0) || (inChannel > 16))
	{
		presetzModel->Log("Preset " + String(p+1) + ": input channel value needs to be beween 0 and 16 (0 meaning omni).", Topiary::Warning);
		inChannel = 1;
		warning = true;
	}

	if ((outChannel < 1) || (outChannel > 16))
	{
		presetzModel->Log("Preset " + String(p + 1) + ": output channel value needs to be beween 1 and 16.", Topiary::Warning);
		outChannel = 1;
		warning = true;
	}

	bool enabled = presetElement[p].enableButton.getToggleState();

	if (!warning)
	{
		presetzModel->setPresetDefinition(p,
			presetElement[p].nameEditor.getText(), from, to, inCC, inChannel, outCC, outChannel, enabled);
	}
	presetElement[p].RTSlider.setRange(from, to, 1);
	presetElement[p].presetSlider.setRange(from, to, 1);
	
	// check if cc messages on enabled variations and same midi channel are used and warn but still save
	// do this here otherwise the msg that the preset was saved overwrites the warning

	for (int pp = 0; pp < PRESETELEMENTS; pp++)
	{
		if (pp != p)
		{
			if ((presetElement[p].inChannelEditor.getText().getIntValue() == presetElement[pp].inChannelEditor.getText().getIntValue()) &&
				(presetElement[p].inCCEditor.getText().getIntValue() == presetElement[pp].inCCEditor.getText().getIntValue()) &&
				(presetElement[p].enableButton.getToggleState() && presetElement[pp].enableButton.getToggleState()))
				presetzModel->Log("Presets " + String(p+1) + " & " + String(pp+1) + " use same input CC on same channel. ", Topiary::LogType::Warning);

			if ((presetElement[p].outChannelEditor.getText().getIntValue() == presetElement[pp].outChannelEditor.getText().getIntValue()) &&
				(presetElement[p].outCCEditor.getText().getIntValue() == presetElement[pp].outCCEditor.getText().getIntValue()) &&
				(presetElement[p].enableButton.getToggleState() && presetElement[pp].enableButton.getToggleState()))
				presetzModel->Log("Presets " + String(p + 1) + " & " + String(pp + 1) + " use same output CC on same channel. ", Topiary::LogType::Warning);

		}
	}

}  // setPresetElementData

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::getVariationDefinition()
{
	// get from model

	if (variation != (variationDefinitionComponent.variationCombo.getSelectedId() - 1))
	{
		variation = variationDefinitionComponent.variationCombo.getSelectedId() - 1;
		if (variation != -1  && !getVariationCalledFromChangeInVariationButton)  // because otherwise the variation is already set in the editor
			presetzModel->setVariation(variation); // so that the variation buttons follow
	}

	if (variation < 0) return;  // this should never happen, except when initializing

	bool enabled;
	String vname;
	int presetVal[PRESETELEMENTS];
	int timing;

	presetzModel->getVariationDefinition(variation, enabled, vname, presetVal, timing);

	variationDefinitionComponent.enableButton.setToggleState(enabled, dontSendNotification);
	variationDefinitionComponent.nameEditor.setText(vname, dontSendNotification);
	variationDefinitionComponent.timingCombo.setSelectedId(timing, dontSendNotification);

	///////////// set the presetValues in the preset elements
	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		// set the variation presetVals and get the presetelements
		presetElement[p].presetSlider.setValue(presetVal[p], dontSendNotification);

		getPresetElementData(p); // should not be needed
	}
	
} // getVariationDefinition

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::setVariationDefinition()
{
	// this does NOT save the preset definitions!!!

	int presetVal[PRESETELEMENTS];
	
	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		presetVal[p] = (int) presetElement[p].presetSlider.getValue();
	}

	if (variation != (variationDefinitionComponent.variationCombo.getSelectedId() - 1))
	{
		variation = variationDefinitionComponent.variationCombo.getSelectedId() - 1;
	}

	jassert(variation > -1);
	
	presetzModel->setVariationDefinition(variation, variationDefinitionComponent.enableButton.getToggleState(), variationDefinitionComponent.nameEditor.getText(), 
																									presetVal, variationDefinitionComponent.timingCombo.getSelectedId()); 
		
}  // setVariationDefinition

///////////////////////////////////////////////////////////////////////////

void TopiaryPresetzMasterComponent::setRTValue(int p, int v)
{
	presetzModel->setRTValue(p, v, presetElement[p].outChannelEditor.getText().getIntValue(), presetElement[p].outCCEditor.getText().getIntValue());

}  // setRTValue

///////////////////////////////////////////////////////////////////////////
