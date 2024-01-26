#ifndef DFA_H
#define DFA_H

#include <stdbool.h>

typedef struct dfa {
    int states;                 // Number of states
    int currentState;           // Current state
    int inputAlphabetSize;      // Size of the input alphabet
    int** transitionTable;      // Transition function
    int initialState;           // Initial state
    bool* acceptStates;         // Array of accepting states
} DFA;

// Function to create a new DFA
DFA* newDFA(int states, int inputAlphabetSize);

// Setter functions
void DFA_setTransition(DFA* dfa, int state, int symbol, int nextState);
void DFA_setAcceptState(DFA* dfa, int state);

// Processing functions
void DFA_processSymbol(DFA* dfa, int symbol);
void DFA_processString(DFA* dfa, char* input);
bool DFA_isInAcceptState(DFA* dfa);
void resetDFA(DFA* dfa);

// Miscellaneous functions
void printAcceptStates(DFA* dfa);
void freeDFA(DFA* dfa);

// REPL loop
void DFA_replLoop(DFA* dfa);

// Test case methods
DFA* exactlyCSC(DFA* dfa);
DFA* startsWithVowel(DFA* dfa);
DFA* containsEnd(DFA* dfa);
DFA* evenOneAndZero(DFA* dfa);

#endif /* DFA_H */
