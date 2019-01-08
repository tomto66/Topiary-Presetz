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
#include "TopiaryPresetzLogComponent.h"
#include "TopiaryPresetzRealTimeComponent.h"
#include "TopiaryPresetzComponent.h"
#include "TopiaryPresetzAutomationComponent.h"

class TopiaryPresetzTabbedComponent:  public Component, ActionListener
{
public:
	TopiaryPresetzTabbedComponent();
    ~TopiaryPresetzTabbedComponent();
	void setModel(TopiaryPresetzModel* model);
    void paint (Graphics&) override;
    void resized() override;  
	void actionListenerCallback(const String &message) override;

private:
	TopiaryPresetzModel* beatsModel;
	TabbedComponent beatsTabs;
	TopiaryPresetzMasterComponent masterComponent;
	TopiaryPresetzLogComponent logComponent;
	TopiaryPresetzRealTimeComponent realtimeComponent;
	TopiaryPresetzAutomationComponent automationComponent;

	int tabHeight;
	int height;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopiaryPresetzTabbedComponent)
};
