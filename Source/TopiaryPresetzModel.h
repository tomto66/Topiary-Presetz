/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary Presetz, Copyright Tom Tollenaere 2018-2021.

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
#include "../../Topiary/Source/Model/TopiaryModel.h"
#include "TopiaryPresetzVariation.h"

#include "../../Topiary/Source/Components/TopiaryMidiLearnEditor.h"

class TopiaryPresetzModel : public TopiaryModel 
{
public:
	TopiaryPresetzModel();
	~TopiaryPresetzModel();
		
	void setNumeratorDenominator(int nu, int de);
	void saveStateToMemoryBlock(MemoryBlock& destData) override;
	void restoreStateFromMemoryBlock(const void* data, int sizeInBytes) override;
	bool processVariationSwitch() override;
	bool switchingVariations() override;
	void generateMidi(MidiBuffer* midiBuffer, MidiBuffer* recBuffer) override;
	void initializeVariationsForRunning() override;
	void setRunState(int n) override;
	void addParametersToModel() override;
	void restoreParametersToModel() override;
	void generateTransition();
	void outputVariationEvents();			// potentially generate events when variation button is pressed (outside of running) - certainly needed for presetz
	void setOverrideHostTransport(bool o);
	void swapVariation(int from, int to) override;
	void copyVariation(int from, int to) override;
	void swapPreset(int from, int to) override;
	void copyPreset(int from, int to) override;
	void getVariationNames(String vNames[8]);
	bool walkToTick(TopiaryVariation* parent, int& childIndex, int toTick);
	void nextTick(TopiaryVariation* parent, int& childIndex);

	struct Variation
	{
		bool enabled = false;
		String name;
		int lenInTicks;  // unused for now
		int presetValue[PRESETELEMENTS];
		//int currentPatternChild;
		int type = Topiary::VariationTypeEnd;
		bool ended;
		int timing; // duration of the transition
		TopiaryVariation pattern;
	} override;

	void setVariation(int i);
	void getVariation(int& running, int& selected);
	int getVariationLenInTicks(int v);
	bool getVariationEnabled(int v);
	void getVariationEnables(bool enables[8]);
	void getVariationDefinition(int v, bool &enabled, String &vname, int presetVal[PRESETELEMENTS], int& timing);
	void setVariationDefinition(int v, bool enabled, String vname, int presetVal[PRESETELEMENTS], int timing);

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

	bool midiLearn(MidiMessage m); // called by processor
	void setAllTransitionTimes(int transitionTime);

	bool fakeRun = false;  // used to flag that we are doing a transition even if the plugin is not running

protected:
	Variation variation[8];		
	int rememberVariationStartQ = 0;  // to make plugin do transitions even if not running
	int rememberFakeRunState = -1;    // to make plugin do transitions even if not running

	PresetElementDefinition presetDefinition[PRESETELEMENTS];		// the definitions

	int presetRTValue[PRESETELEMENTS];								// the actual realtime values of where we are in real time (not stored in the presets nor the variations0

	///////////////////////////////////////////////////////////////////////

	void generateTransition(int p, int v, int length)
	{
		// generate the CC msg; depending on the target channel, output it once or 16 times as childs under transition*

		int to, from, channel, cc;
		float timestamp, target;

		auto generateTransitionCCMsgs =[&]()
		{
			auto generateTransitionCCMsg = [&](int channel, int timestamp, int cc, int value)
			{
				int i = variation[v].pattern.numItems;
				variation[v].pattern.add();
				variation[v].pattern.dataList[i].timestamp = timestamp;
				variation[v].pattern.dataList[i].CC = cc;
				variation[v].pattern.dataList[i].value = value;
				variation[v].pattern.dataList[i].midiType = Topiary::MidiType::CC; 
				variation[v].pattern.dataList[i].channel = channel;
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

			float perStepValue = ((float)(to - from) / TRANSITIONSTEPS);
			float perStepTimestamp = ((float)length / TRANSITIONSTEPS);
			target = (float)from + perStepValue;
			timestamp = 0.0;
			for (int step = 0; step < (TRANSITIONSTEPS - 1); step++)
			{
				generateTransitionCCMsgs();
				target = target + perStepValue;
				timestamp = timestamp + perStepTimestamp;
			}

			target = (float) to;
			timestamp = (float) (length - 1);
			// last step is to target value
			generateTransitionCCMsgs();

			/*
			Logger::outputDebugString("------------------------");
			for (int j = 0; j < variation[v].pattern.numItems; j++)
			{
				Logger::outputDebugString("<" + String(j) + "> <ID" + String(variation[v].pattern.dataList[j].ID)+"> CC value: " + String(variation[v].pattern.dataList[j].value) + " timestamp " + String(variation[v].pattern.dataList[j].timestamp) + " midiType " + String(variation[v].pattern.dataList[j].midiType));
			}
			Logger::outputDebugString("------------------------");
			*/

		}
	} // generateTransition

	//////////////////////////////////////////////////////////////////

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopiaryPresetzModel)
};

