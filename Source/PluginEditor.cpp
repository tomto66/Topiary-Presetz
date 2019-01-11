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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TopiaryPresetzComponent.h"


//==============================================================================
TopiaryPresetzAudioProcessorEditor::TopiaryPresetzAudioProcessorEditor (TopiaryPresetzAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	setSize(TopiaryPresetzComponent::width, TopiaryPresetzComponent::heigth);
	presetzModel = processor.getModel();
	topiaryPresetzComponent.setModel(presetzModel);
	addAndMakeVisible(topiaryPresetzComponent);
}

TopiaryPresetzAudioProcessorEditor::~TopiaryPresetzAudioProcessorEditor()
{
}

//==============================================================================
void TopiaryPresetzAudioProcessorEditor::paint (Graphics& g)
{
	UNUSED(g)
	topiaryPresetzComponent.setBounds(getLocalBounds());
}

void TopiaryPresetzAudioProcessorEditor::resized()
{
    
}