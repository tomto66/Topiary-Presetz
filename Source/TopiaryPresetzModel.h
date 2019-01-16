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
#include "../../Topiary/Source/TopiaryModel.h"
#define PRESETZ

class TopiaryPresetzModel : public TopiaryModel 
{
public:
	TopiaryPresetzModel();
	~TopiaryPresetzModel();
		
	//void savePreset(File f) override;
	//void loadPreset(File f) override;
	void saveStateToMemoryBlock(MemoryBlock& destData) override;
	void restoreStateFromMemoryBlock(const void* data, int sizeInBytes) override;
	bool processVariationSwitch() override;
	bool switchingVariations() override;
	void getVariationDetailForGenerateMidi(XmlElement** parent, XmlElement** noteChild, int& parentLength, bool& ending, bool& ended) override;
	void initializeVariationsForRunning() override;
	void addParametersToModel() override;
	void restoreParametersToModel() override;
	void generateTransition();
	void threadRunner() override;
	void outputVariationEvents();			// potentially generate events when variation button is pressed (outside of running) - certainly needed for presetz
	

#define PRESETELEMENTS 8
#define PRESETTRANSITIONSTEPS 10

	struct Variation
	{
		bool enabled = false;
		String name;
		int lenInTicks;  // unused for now
		int presetValue[PRESETELEMENTS];
		XmlElement* pattern; // otherwise audioprocessor won't run
		XmlElement* currentPatternChild;
		bool ending;
		bool ended;
		int timing; // duration of the transition
	} override;

	void setVariation(int i);
	void getVariation(int& running, int& selected);
	int getVariationLenInTicks(int v);
	bool getVariationEnabled(int v);
	void getVariationEnables(bool enables[8]);
	void getVariationDefinition(int v, bool &enabled, String &vname, int presetVal[PRESETELEMENTS], int& timing);
	void setVariationDefinition(int v, bool enabled, String vname, int presetVal[PRESETELEMENTS], int timing);
	void setEnded() override;

	struct PresetElementDefinition
	{
		// these are fixed; they cannot be changed by variation (but can be changed per presetElement of course)
		String name;
		int fromValue = 0;
		int toValue = 127;
		int inCC = 0;
		int inChannel = 0;
		int outCC = 0;
		int outChannel;
		bool enabled;
	};

	void getPresetDefinition(int p, String &pname, int &fromValue, int &toValue, int &inCC, int &inChannel, int &outCC, int & outChannel, bool &enabled);
	void setPresetDefinition(int p, String pname, int fromValue, int toValue, int inCC, int inChannel, int outCC, int outChannel, bool enabled);

	void setRTValue(int p, int value, int channel, int CC);  
	int getRTValue(int p);
	void processCC(MidiMessage& msg, MidiBuffer* buffer) override; 
	void processCC(MidiMessage& msg) override;

protected:
	Variation variation[8];		
	PresetElementDefinition presetDefinition[PRESETELEMENTS];		// the definitions

	int presetRTValue[PRESETELEMENTS];								// the actual realtime values of where we are in real time (not stored in the presets nor the variations0
			
	///////////////////////////////////////////////////////////////////////

	void generateTransition(int p, int v, XmlElement *transition, int length)
	{
		// generate the CC msg; depending on the target channel, output it once or 16 times as childs under transition*

		int to, from, channel, cc;
		float timestamp, target;

		auto generateTransitionCCMsgs =[&]()
		{
			auto generateTransitionCCMsg = [&](int channel, int timestamp, int cc, int value)
			{
				// generate one of them for channel
				XmlElement *newChild = new XmlElement("CC");
				transition->prependChildElement(newChild);
				newChild->setAttribute("Timestamp", timestamp);
				newChild->setAttribute("CC", cc);
				newChild->setAttribute("Value", value);
				newChild->setAttribute("Channel", channel);
				newChild->setAttribute("midiType", Topiary::MidiType::CC);
			};

			/////////////////////////////

			generateTransitionCCMsg(presetDefinition[p].outChannel, (int) timestamp, cc, (int)target);

		}; // generateTransitionCCMsgs

		//////////////////////////////

		// generates 1 transion for presetElement p and variation v over the length

		if (presetDefinition[p].enabled == true)
		{
			// we are looping from the realtime value to the target value in the variation
			from = presetRTValue[p];
			to = variation[v].presetValue[p];
			channel = presetDefinition[p].outChannel;
			cc = presetDefinition[p].outCC;
			if (from == to) return; // nothing to do

			float perStepValue = ((float)(to - from) / PRESETTRANSITIONSTEPS);
			float perStepTimestamp = ((float)length / PRESETTRANSITIONSTEPS);
			target = (float)from + perStepValue;
			timestamp = 0.0;
			for (int step = 0; step < (PRESETTRANSITIONSTEPS - 1); step++)
			{
				generateTransitionCCMsgs();
				target = target + perStepValue;
				timestamp = timestamp + perStepTimestamp;
			}

			target = (float) to;
			timestamp = (float) (length - 1);
			// last step is to target value
			generateTransitionCCMsgs();
		}
	} // generateTransition

	//////////////////////////////////////////////////////////////////

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryPresetzModel)
};

