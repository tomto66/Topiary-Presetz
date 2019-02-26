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
#include "TopiaryPresetzHeaderComponent.h"
#include "TopiaryPresetzComponent.h"

TopiaryPresetzHeaderComponent::TopiaryPresetzHeaderComponent()
{
	setSize(TopiaryPresetzComponent::width, TopiaryPresetzComponent::headerHeigth);
	addAndMakeVisible(transportComponent);
	addAndMakeVisible(variationButtonsComponent);
	addAndMakeVisible(warningEditor);
	warningEditor.setReadOnly(true);
	warningEditor.setColour(TextEditor::backgroundColourId, TopiaryColour::orange);
	warningEditor.setColour(TextEditor::textColourId, Colours::lightyellow  );
	warningEditor.setColour(TextEditor::outlineColourId, TopiaryColour::orange);
	
	addAndMakeVisible(timeEditor);
	timeEditor.setVisible(false);
	timeEditor.setEnabled(false);
}

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzHeaderComponent::~TopiaryPresetzHeaderComponent()
{ 	
	presetzModel->removeListener((ActionListener*)this);
}

void TopiaryPresetzHeaderComponent::setModel(TopiaryPresetzModel* m)
{
	presetzModel = m;
	variationButtonsComponent.setModel(m);
	transportComponent.setModel(m);
	presetzModel->setListener((ActionListener*)this);
	variationButtonsComponent.checkModel();
	transportComponent.checkModel();
	warningEditor.setVisible(false);
}
/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzHeaderComponent::resized()
{ // bounds are handled by viewer
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzHeaderComponent::paint(Graphics& g) {
	// load background
	g.fillAll(TopiaryColour::background );
	Image background = ImageCache::getFromMemory(BinaryData::topiary__presetz_logo_7_png, BinaryData::topiary__presetz_logo_7_pngSize);
	g.drawImageAt(background, 0, 0);
	auto area = getLocalBounds();
	
	variationButtonsComponent.setBounds(640,30 ,350,45);
	transportComponent.setBounds(295, 30, 350,45);

	warningEditor.setBounds(645, 5, 330, 18);
	timeEditor.setBounds(402, 17, 70, 18);

}  // paint

/////////////////////////////////////////////////////////////////////////


void TopiaryPresetzHeaderComponent::actionListenerCallback(const String &message)
{
	
	if (message.compare(MsgWarning) == 0)
	{
		warningEditor.setText(presetzModel->getLastWarning());
		warningEditor.setVisible(true);
		startTimer(3000);
	}
	else if (message.compare(MsgTiming) == 0)
	{
		timeEditor.setVisible(true);
		getTime();
		startTimer(5000);
	}
	else
	{
		if (message.compare(MsgVariationSelected) == 0)
			variationButtonsComponent.checkModel();
		if (message.compare(MsgTransport) == 0)
			transportComponent.checkModel();
		if (message.compare(MsgVariationEnables) == 0)
			variationButtonsComponent.getEnabled();
		if (message.compare(MsgLog) == 0)
			warningEditor.setVisible(false); 
	}

} // actionListenerCallback

//////////////////////////////////////////////////////////////

void TopiaryPresetzHeaderComponent::timerCallback()
{
	if (warningEditor.isVisible())
	{
		warningEditor.setVisible(false);
		presetzModel->stopLearningMidi();
	}
	else if (timeEditor.isVisible())
		timeEditor.setVisible(false);
	stopTimer();
}
