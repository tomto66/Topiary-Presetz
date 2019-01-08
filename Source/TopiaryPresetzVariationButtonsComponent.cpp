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
#include "TopiaryPresetzVariationButtonsComponent.h"
#include "../../Topiary/Source/TopiaryVariationButtonsComponent.cpp"

/*
TopiaryPresetzVariationButtonsComponent::TopiaryPresetzVariationButtonsComponent()
{
}

/////////////////////////////////////////////////////////////////////////////

TopiaryPresetzVariationButtonsComponent::~TopiaryPresetzVariationButtonsComponent()
{
}

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzVariationButtonsComponent::setModel(TopiaryPresetzModel *m)
{
	presetzModel = m;
	getEnabled();  // see if there are disabled variationbuttons
} // setModel

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzVariationButtonsComponent::updateToggleState(TextButton* button)
{
	String buttonText = button->getButtonText();
	presetzModel->setVariation(buttonText.getIntValue()-1);
} // updateToggleState

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzVariationButtonsComponent::checkModel()
{  // Called by the HeaderComponent timer to check whether the model has changed; if so adapt accordingly
	
		int running;
		int selected;
		presetzModel->getVariation(running, selected);
		if (selected == running)
			selectButton(running,running);
		else
			selectButton(selected,running);
	

} // checkModel

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzVariationButtonsComponent::getEnabled()
{
	bool enables[8];
	presetzModel->getVariationEnables(enables);
		for (int i = 0; i < 8; i++)
			enableButton(i, enables[i]);

} // getEnabled

/////////////////////////////////////////////////////////////////////////////
*/