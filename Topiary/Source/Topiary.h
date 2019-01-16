/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary, Copyright Tom Tollenaere 2018-2019.

Topiary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Topiary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Topiary. If not, see <https://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
/////////////////////////////////////////////////////////////////////////////

#define UNUSED(x) x;
#define xstr(s) str(s)
#define str(s) #s

/////////////////////////////////////////////////////////////////////////////

String validateNote(String newNote);
int validNoteNumber(String note);

/////////////////////////////////////////////////////////////////////////////

class TopiaryButton : public TextButton
{
public:
	TopiaryButton();
	~TopiaryButton();
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
	int specialState = 0;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryButton)

};  // TopiaryButton

/////////////////////////////////////////////////////////////////////////////

class Topiary 
{  public:
	
	enum VariationButtonIds
	{
		VariationButtons = 1001,
		VariationSwitchButtons = 1002
	};

	enum TransportButtonIds
	{
		TransportButtons = 1007
	};
	enum TopiaryButtonSpecialState
	{
		waitToApply = 1,
		waitToEnd = 2
	};

	enum TransportRunState : int
	{
		Stopped = 0,     // stopped (transport stopped if not overriden!)
		Running = 1,     // really running, producing output
		Recording = 2,   // running & recording (never in performance, only in setup)
		Armed = 3,  	 // ready to start running  @first note in
		Ended = 4,       // transport still running, but has produced last note (after an ending pattern; will not produce more notes)
		Ending = 5		 // stop command given, but finishing the last notes; goes to Ended when totally done
	};

	enum MidiType : int
	{
		NoteOn = 1,
		NoteOff = 2,
		CC = 3
	};

	enum Quantization : int
	{
		Immediate = 100,
		WholePattern = 20,
		Measure = 1,
		Half = 3,
		//Third = 4,
		Quarter = 5,
		//Eight = 6,
		//Sixteenth = 7,
		//Thirty2nd = 8
	};

	enum VariationSwitch : int
	{
		SwitchFromStart = 1,		// when we switch variation, the pattern starts at the beginning
		SwitchWithinBeat = 2,		// when we switch variation, the pattern starts in same tick as current beat in the ongonig variation, but in first beat of new one
		SwitchWithinMeasure = 3,	// when we switch variation, the pattern starts in same tick of same beat in the ongoing variation, but in the frist measure of the new one
		SwitchWithinPattern = 4		// when we switch variation, the pattern starts intelligently - if number of measures are equal it will be in same tick, beat, measure as the ongoing one
									// if #measures are different, we start intelligently, e.g. in measure 8 of 8 (ongoing) --> measure 2 of 2 (do patterncursors (ongoing) MODULO the ticklen of the new variation!
	};

	enum LogType : int
	{
		Debug = 0,		// low lever info for debugging
		Warning = 1,	// things that go wrong, user error
		MidiIn = 2,		// monitor incoming midi
		MidiOut = 3,	// monitor outgoing midi
		Transport = 4,	// msgs for transport events/buttons
		Variations = 5,	// msgs vor variations events/buttons
		Info = 6,		// general information e.g. "New pattern loaded with x notes"
		License = 7		// License info
	};

	enum ThreadRunnerState : int
	{
		NothingToDo = 2604,
		Generating = 2605,
		DoneGenerating = 2607 // meaning it needs to go back to NothingToDo
	};

	static const int TICKS_PER_QUARTER = 240;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Topiary)
}; // Topiary

/////////////////////////////////////////////////////////////////////////////

class TopiaryLookAndFeel : public LookAndFeel_V4
{ 
public:
	TopiaryLookAndFeel();
	~TopiaryLookAndFeel();
	
	static void drawTopiaryButtonBackground(Graphics& g, TopiaryButton& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown);

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
									const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
private:
		
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryLookAndFeel)
}; // TopiaryLookAndFeel


/////////////////////////////////////////////////////////////////////////////

class TopiaryColour
{
public:
	static const Colour background;
	static const Colour foreground;
	static const Colour warning;
	static const Colour brightBlue;
	static const Colour orange;
	static const Colour green;
	static const Colour yellow;
	static const Colour lightBlue;
	static const Colour darkBlue;
	static const Colour paleBlue;
	static const Colour sliderTrace;
	static const Colour sliderThumb;
private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryColour)

}; // TopiaryColour

/////////////////////////////////////////////////////////////////////////////


