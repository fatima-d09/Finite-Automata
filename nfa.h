#ifndef NFA_H
#define NFA_H

#include <stdbool.h>
#include "IntHashSet.h"

// // Forward declaration for IntHashSet
// typedef struct IntHashSet IntHashSet;


typedef struct NFA {
    int states; // Number of states
    IntHashSet currentState; // Set of current states
    int inputAlphabetSize;
    int initialState;
    IntHashSet acceptStates; // Set of accepting states (using an array this time)
    IntHashSet** transitionTable; // Transitions
    int numTransitions; // Number of transitions
} NFA;

// Initializing NFA
NFA* newNFA(int states, int inputAlphabetSize);

// Setters
void NFA_setTransition(NFA* nfa, int state, int symbol, int nextState);
void NFA_setAcceptState(NFA* nfa, int state);

// Getters
int NFA_getNumStates(NFA* nfa);

// Processing Functions
void NFA_processSymbol(NFA* nfa, int symbol, IntHashSet* currentState);
void NFA_processString(NFA* nfa, char* input);

// Freeing Memory
void freeNFA(NFA* nfa);

// Check if the NFA is in an accepting state
bool NFA_isInAcceptState(NFA* nfa);

// REPL Loop
void NFA_replLoop(NFA* nfa);

// Test Cases
NFA* endsInAt(NFA* nfa);
NFA* containsGot(NFA* nfa);
NFA* happeningAnagram(NFA* nfa);

#endif /* NFA_H */