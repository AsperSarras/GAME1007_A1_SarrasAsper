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
	static bool s_stateChange;

public: //Methods
	static void Update();
	static void Render();
	static void PushState(State* pState);	
	static void PopState(); 
	static void ChangeState(State* pState);
	static void Quit(); 
	static vector<State*>& GetStates(); 
	static const bool StateChanging();
	

private: //Methods
	StateManager(){}	
};

typedef StateManager STMA;//Creating an alias

#endif

