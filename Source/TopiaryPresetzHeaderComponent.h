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
#include "TopiaryPresetzModel.h"
#include "TopiaryPresetzTransportComponent.h"
#include "TopiaryPresetzVariationButtonsComponent.h"

class TopiaryPresetzHeaderComponent : public Component,  ActionListener, Timer
{
public:

	TopiaryPresetzHeaderComponent();
	~TopiaryPresetzHeaderComponent();
	void setModel(TopiaryPresetzModel* m);
	void paint(Graphics& g) override;
	void resized() override;
	void actionListenerCallback(const String &message) override;
	void timerCallback();

private:
	TopiaryPresetzTransportComponent transportComponent;
	TopiaryPresetzVariationButtonsComponent variationButtonsComponent;
	TopiaryPresetzModel* presetzModel;
	TextEditor warningEditor;
	TextEditor timeEditor; 

	void getTime()
	{
		int m, b;
		String timing;

		presetzModel->getTime(m, b);
		timing = String(m) + " : " + String(b);

		if (timeEditor.getText().compare(timing) != 0)
			timeEditor.setText(timing);

	} // getTime

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryPresetzHeaderComponent)
};



