#pragma once
#ifndef _STATEMANAGER_
#define _STATEMANAGER_

#include "States.h"
#include <vector>

using namespace std;

class StateManager
{
private: //Properties
	static vector<State*>s_states;

public: //Methods
	static void Update();
	static void Render();
	static void PushState(State* pState);	//Used when going to pause state
	static void PopState(); //Used when leaving pause state
	static void ChangeState(State* pState);
	static void Quit(); //Clean up
	static vector<State*>& GetStates(); //Return a reference to our State* vector
	

private: //Methods
	StateManager(){}	
};

typedef StateManager STMA;//Creating an alias

#endif

