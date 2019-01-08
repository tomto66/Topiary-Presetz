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
#include "TopiaryPresetzHeaderComponent.h"
#include "TopiaryPresetzTabbedComponent.h"

class TopiaryPresetzComponent : public Component
{
public:
	TopiaryPresetzComponent();
	~TopiaryPresetzComponent();
	void setModel(TopiaryPresetzModel* presetzModel);
	void paint(Graphics& g) override;
	void resized() override;
	static const int headerHeigth;
	static const int width;
	static const int heigth;

private:
	TopiaryPresetzModel* presetzModel;  
	
	TopiaryPresetzHeaderComponent topiaryPresetzHeaderComponent;
	TopiaryPresetzTabbedComponent topiaryPresetzTabbedComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryPresetzComponent)

};



