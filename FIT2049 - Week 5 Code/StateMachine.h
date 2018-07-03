#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>

// E - a user defined enum/index for a unique state
// T - the class that owns the state machine.

template<class E, class T>
class StateMachine
{
public:
	typedef void (T::*CallbackOnEnter)();
	typedef void (T::*CallbackOnUpdate)(float);
	typedef void (T::*CallbackOnDraw)();
	typedef void (T::*CallbackOnExit)();

private:
	
	T* m_pOwner;
	E m_currentState;

	// Maps to store function pointers to state enter/update/draw/exit functions.
	std::map<E, CallbackOnUpdate> m_statesOnUpdate;
	std::map<E, CallbackOnDraw> m_statesOnDraw;
	std::map<E, CallbackOnEnter> m_statesOnEnter;
	std::map<E, CallbackOnExit> m_statesOnExit;


public:
	StateMachine(T* pOwner, E emptyState)
	{
		m_currentState = emptyState;
		m_pOwner = pOwner;
	}

	~StateMachine()
	{		
	}

	void RegisterState(E state, CallbackOnEnter callbackEntry, CallbackOnUpdate callbackUpdate, CallbackOnDraw callbackDraw, CallbackOnExit callbackExit)
	{
		m_statesOnEnter[state] = callbackEntry;
		m_statesOnUpdate[state] = callbackUpdate;
		m_statesOnDraw[state] = callbackDraw;
		m_statesOnExit[state] = callbackExit;
	}

	E GetCurrentState(void)
	{
		return m_currentState;
	}

	void ChangeState(E statenext)
	{
		if (m_pOwner)
		{
			CallbackOnExit callbackExit = m_statesOnExit[m_currentState];
			if (callbackExit)
			{
				// Exit old state
				(m_pOwner->*callbackExit)();
			}
		}
		
		m_currentState = statenext;

		if (m_pOwner)
		{
			CallbackOnEnter callbackEnter = m_statesOnEnter[m_currentState];
			if (callbackEnter)
			{
				// Enter new state
				(m_pOwner->*callbackEnter)();
			}
		}
	}

	//Update Current State
	void Update(float fTimeStep)
	{	
		if (m_pOwner)
		{
			CallbackOnUpdate callback = m_statesOnUpdate[m_currentState];
			if (callback)
			{
				(m_pOwner->*callback)(fTimeStep);
			}
		}
	}

	//Draw Current State
	void Draw()
	{
		if (m_pOwner)
		{
			CallbackOnDraw callback = m_statesOnDraw[m_currentState];
			if (callback)
			{
				(m_pOwner->*callback)();
			}
		}
	}
};

#endif
