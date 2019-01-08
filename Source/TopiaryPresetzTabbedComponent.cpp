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
#include "TopiaryPresetzComponent.h"
#include "TopiaryPresetzTabbedComponent.h"

TopiaryPresetzTabbedComponent::TopiaryPresetzTabbedComponent() : beatsTabs(TabbedButtonBar::Orientation::TabsAtTop)   
// above needed otherwise the header declares TabbedButtonBar beatsTabs (without orientation!!!)
{
	tabHeight = TopiaryPresetzComponent::heigth - TopiaryPresetzComponent::headerHeigth - 30;
	height = TopiaryPresetzComponent::heigth - TopiaryPresetzComponent::headerHeigth;

	addAndMakeVisible(beatsTabs);
	// do NOT EVER make the tab components themselves visible - it'll screw up royally!!!!

	setSize(TopiaryPresetzComponent::width, height);
	
	masterComponent.setSize(TopiaryPresetzComponent::width, tabHeight);
	beatsTabs.addTab("Master", TopiaryColour::background, &masterComponent, true);

	realtimeComponent.setSize(TopiaryPresetzComponent::width, tabHeight);
	beatsTabs.addTab("Realtime", TopiaryColour::background, &realtimeComponent, true);

	automationComponent.setSize(TopiaryPresetzComponent::width, tabHeight);
	beatsTabs.addTab("Automation", TopiaryColour::background, &automationComponent, true);

	logComponent.setSize(TopiaryPresetzComponent::width, tabHeight);
	beatsTabs.addTab("Log", TopiaryColour::background, &logComponent, true);
	
	setSize(TopiaryPresetzComponent::width, TopiaryPresetzComponent::heigth - TopiaryPresetzComponent::headerHeigth);

}

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzTabbedComponent::~TopiaryPresetzTabbedComponent()
{
	beatsModel->removeListener((ActionListener*)this);
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzTabbedComponent::setModel(TopiaryPresetzModel* model) 
{
	beatsModel = model;
	masterComponent.setModel(beatsModel);
	logComponent.setModel(beatsModel);
	realtimeComponent.setModel(beatsModel);
	automationComponent.setModel(beatsModel);
	beatsModel->setListener((ActionListener*)this);
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzTabbedComponent::paint(Graphics& g)
{
	UNUSED(g)
	auto area = getLocalBounds();
	beatsTabs.setBounds(area);
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzTabbedComponent::resized()
{
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzTabbedComponent::actionListenerCallback(const String &message)
{
	if (message.compare(MsgTransport) == 0)
	{
		int b, n, d, runState;
		bool o, w;

		beatsModel->getTransportState(b, n, d, runState, o, w);
		if ((runState == Topiary::Running) || (runState == Topiary::Armed) || (runState == Topiary::Ending))
		{
			masterComponent.setEnabled(false);
			automationComponent.setEnabled(false);
		}
		else
		{
			masterComponent.setEnabled(true);
			automationComponent.setEnabled(true);
		}
	}
} // actionListenerCallback

/////////////////////////////////////////////////////////////////////////