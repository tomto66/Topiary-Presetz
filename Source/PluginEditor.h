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
#include "PluginProcessor.h"
#include "TopiaryPresetzComponent.h"


class TopiaryPresetzAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TopiaryPresetzAudioProcessorEditor (TopiaryPresetzAudioProcessor&);
    ~TopiaryPresetzAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    TopiaryPresetzAudioProcessor& processor;
	TopiaryPresetzModel* presetzModel;
	TopiaryPresetzComponent topiaryPresetzComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopiaryPresetzAudioProcessorEditor)
};
