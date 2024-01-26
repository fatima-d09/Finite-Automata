#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "IntHashSet.h"

typedef struct nfa{
    int states; // num of states
    IntHashSet currentState; // set of current states
    int inputAlphabetSize;
    int initialState; 
    IntHashSet acceptStates; // set of accepting states 
    IntHashSet** transitionTable; // transitions
    int numTransitions; // num of transitions
} NFA; 


//initializing nfa
NFA* newNFA(int states, int inputAlphabetSize) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->states = states;
    nfa->inputAlphabetSize = inputAlphabetSize;
    nfa->currentState = new_IntHashSet(states);
    nfa->initialState = 0;
    nfa->acceptStates = new_IntHashSet(states);
    nfa->numTransitions = 0;
    nfa->transitionTable = (IntHashSet**)malloc(states * sizeof(IntHashSet*));
    
    // Allocating memory for the transition table & initializing with empty sets
    for (int i = 0; i < states; i++) {
        nfa->transitionTable[i] = (IntHashSet*)malloc(inputAlphabetSize * sizeof(IntHashSet));
        for (int j = 0; j < inputAlphabetSize; j++) {
            nfa->transitionTable[i][j] = new_IntHashSet(16); // Assuming a reasonable initial size
        }
    }


    return nfa;
}
// SETTERS -------------------------------------------------------------------------------------------------------------------

void NFA_setTransition(NFA* nfa, int state, int symbol, int nextState) {
    // Create a new IntHashSet if it doesn't exist for this state and symbol
    if (nfa->transitionTable[state][symbol] == NULL) {
        nfa->transitionTable[state][symbol] = new_IntHashSet(16); // Assuming a reasonable initial size
    }
    // Add the next state to the set
    IntHashSet_insert(nfa->transitionTable[state][symbol], nextState);

    nfa->numTransitions++;
}

void NFA_setAcceptState(NFA* nfa, int state){
    IntHashSet_insert(nfa->acceptStates, state);
}

// GETTERS------------------------------------------------------------------------------------------------------------------

int NFA_getNumStates(NFA* nfa){
    return nfa->states; 
}

// PROCESSING FUNCTIONS -------------------------------------------------------------------------------------------------------

void NFA_processSymbol(NFA* nfa, int symbol, IntHashSet* currentState) {
    // creating a hashset to house the "post transformed" states
    IntHashSet newStates = new_IntHashSet(nfa->states);

    // Iterator to go through the currentStates
    IntHashSetIterator currentStateIterator = IntHashSet_iterator(*currentState);

    while (IntHashSetIterator_hasNext(currentStateIterator)) {
        int state = IntHashSetIterator_next(currentStateIterator);
        IntHashSetIterator transitionStatesIterator = IntHashSet_iterator(nfa->transitionTable[state][symbol]);

        while (IntHashSetIterator_hasNext(transitionStatesIterator)) {
            int nextState = IntHashSetIterator_next(transitionStatesIterator);
            IntHashSet_insert(newStates, nextState);
        }

        free(transitionStatesIterator);
    }

    free(currentStateIterator);

    // Update the currentState passed by reference
    free(*currentState);
    *currentState = newStates;
}


// pretty much the same as my DFA. this is my "run automata" function 
void NFA_processString(NFA* nfa, char* input) {
    int inputSize = strlen(input);

    // Initialize the currentState only once before processing input
    IntHashSet currentState = new_IntHashSet(nfa->states);
    IntHashSet_insert(currentState, nfa->initialState);

    for (int i = 0; i < inputSize; i++) {
        char symbol = input[i];
        int index;

        if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')) {
            if (symbol >= 'A' && symbol <= 'Z') {
                index = symbol - 'A'; // uppercase letters are mapped to 0 to 25
            } else {
                index = symbol - 'a' + 26; // lowercase letters are mapped to 26 to 51
            }
        } 

        // process the symbol using the NFA_processSymbol function
        if (index >= 0 && index < nfa->inputAlphabetSize) {
            NFA_processSymbol(nfa, index, &currentState); // pass currentState by reference
            
         //   IntHashSet_print(currentState); // debug 
        }
    }

    // Finally, update the nfa's currentState with the accumulated states
    free(nfa->currentState);
    nfa->currentState = currentState;
}

//-----------------------------------------------------------------------------------------------------------------------------

//freeing all memory for the NFA (like the clean state protocol if you've seen iron man 3)
void freeNFA(NFA* nfa){
    for(int i = 0; i < nfa->states; i++){
        for(int j = 0; j < nfa->inputAlphabetSize; j++){
            IntHashSet_free(nfa->transitionTable[i][j]); // free each colum 
        }
        free(nfa->transitionTable[i]); // free each row 
    }
    free(nfa->transitionTable); 
    IntHashSet_free(nfa->currentState); 
    IntHashSet_free(nfa->acceptStates);
    free(nfa);
}

//checks if the nfa is in an accepting state
bool NFA_isInAcceptState(NFA* nfa){
    // initialize an iterator that goes thru currentState HashSet 
    IntHashSetIterator iterator = IntHashSet_iterator(nfa->currentState); 

    // while the currentState hashset has a next value...
    while(IntHashSetIterator_hasNext(iterator)){ 
        int state = IntHashSetIterator_next(iterator); // state = the next value
        if(IntHashSet_lookup(nfa->acceptStates, state)){ // if that state exists in the accept states set...
            free(iterator); // free the iterator mem...
            return true; // and return true ! 
        }
    }
    free(iterator); // else, free the mem
    return false;  // and return false! 
}

// copy pasted from DFA (citation: myself)
void NFA_replLoop(NFA* nfa){
    char input[100];

    //start the input loop
    while (1) {
        printf("\n");

        printf("Enter an input ('quit' to quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // exit the loop if input is empty
        }
        input[strlen(input) - 1] = '\0'; // removes the newline character

        //exit the loop if the input is "quit"
        if (strcmp(input, "quit") == 0) { // I had to search the string compare method: https://www.programiz.com/c-programming/library-function/string.h/strcmp
            break; 
        }

        //actually run the nfa with the given input
        NFA_processString(nfa, input);

        //checks the shit 
        if (NFA_isInAcceptState(nfa)) {
            printf("Input accepted\n");
        
        } else {
            printf("Input rejected\n");
        }
    }
}


// TEST CASES ---------------------------------------------------------------------------------------------------------------------
NFA* endsInAt(NFA* nfa){
    NFA_setTransition(nfa, 0, 0, 1); // state 0 -> state 1 if A
    NFA_setTransition(nfa, 0, 26, 1); // state 0 -> state 1 if a
    NFA_setTransition(nfa, 1, 19, 2); // state 1 -> state 2 if T 
    NFA_setTransition(nfa, 1, 45, 2); // state 1 -> state 2 if t

    // all other letters get sent back to state 0
    for (int i = 0; i < nfa->inputAlphabetSize; i++) {
            NFA_setTransition(nfa, 0, i, 0);
    }

    // set accepting state 
    NFA_setAcceptState(nfa, 2);

    return nfa;
}
//-------------------------------------------------------------------------------------------------------------------------------

NFA* containsGot(NFA* nfa){
    NFA_setTransition(nfa, 0, 6, 1); // state 0 -> state 1 if G
    NFA_setTransition(nfa, 1, 6, 1);

    NFA_setTransition(nfa, 0, 32, 1); // state 0 -> state 1 if g
    NFA_setTransition(nfa, 1, 32, 1);


    NFA_setTransition(nfa, 1, 14, 2); // state 1 -> state 2 if O
    NFA_setTransition(nfa, 1, 40, 2); // state 1 -> state 2 if o

    NFA_setTransition(nfa, 2, 19, 3); // state 2 -> state 3 if T
    NFA_setTransition(nfa, 2, 45, 3); // state 1 -> state 2 if t

    // set every other letter back to state 0
    for (int i = 0; i < nfa->inputAlphabetSize; i++) {
        NFA_setTransition(nfa, 0, i, 0);
    }

    // but if you are already in state 3, stay there! 
    for (int i = 0; i < nfa->inputAlphabetSize; i++) {
        NFA_setTransition(nfa, 3,i,3);
    }

    NFA_setAcceptState(nfa, 3);
    return nfa;
}

// convience function that takes makes a loop on a state with a specific letter
void letterLoop(NFA* nfa, int letter, int state){
    for (int i = 0; i < nfa-> inputAlphabetSize; i++) { 
        if(i != letter){
        NFA_setTransition(nfa, state, i, state); 
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
NFA* happeningAnagram(NFA* nfa){

for (int i = 0; i < nfa->inputAlphabetSize; i++) {
        NFA_setTransition(nfa, 0,i,0);
        NFA_setTransition(nfa, 11, i, 11);
    }

    NFA_setTransition(nfa, 0, 0, 1); //if A, state 0 -> state 1 
    NFA_setTransition(nfa, 1, 0, 11); // if A, state 1 -> state 11
    letterLoop(nfa, 0, 1);

    NFA_setTransition(nfa, 0, 4, 2); // if E, state 0 -> state 2
    NFA_setTransition(nfa, 2, 4, 11); // if E, state 2 -> state 11
    letterLoop(nfa, 4, 2); 
    
    NFA_setTransition(nfa, 0, 7, 3); // if H state 0 -> state 3
    NFA_setTransition(nfa, 3, 7, 11); // if H state 3 -> state 11
    letterLoop(nfa, 7, 3);

    NFA_setTransition(nfa, 0, 8, 4); // if I state 0 -> state 4
    NFA_setTransition(nfa, 4, 8, 11); // if I state 4 -> state 11
    letterLoop(nfa, 8, 4);
    
    NFA_setTransition(nfa, 0, 6, 5); // if G state 0 -> state 4
    NFA_setTransition(nfa, 5, 6, 11); // if G state 5 -> state 11
    letterLoop(nfa, 6, 5);

    NFA_setTransition(nfa, 0, 13, 6); // if N state 0 -> state 4
    letterLoop(nfa, 13, 6);
    NFA_setTransition(nfa, 6, 13, 7); // if N state 6 -> state 7
    letterLoop(nfa, 13, 7);
    NFA_setTransition(nfa, 7, 13, 11); // if N state 7 -> state 11 (more than twice) 

    NFA_setTransition(nfa, 0, 15, 8); // if P state 0 -> state 8
    letterLoop(nfa, 15, 8);
    NFA_setTransition(nfa, 8, 15, 9); // if P state 6 -> state 7
    letterLoop(nfa, 15, 10);
    NFA_setTransition(nfa, 9, 15, 11); // if P state 7 -> state 11 (more than twice) 


    //lower case letter transitions
    NFA_setTransition(nfa, 0, 0+26, 1); //if A, state 0 -> state 1 
    NFA_setTransition(nfa, 1, 0+26, 11); // if A, state 1 -> state 11
    letterLoop(nfa, 0+26, 1);

    NFA_setTransition(nfa, 0, 4+26, 2); // if E, state 0 -> state 2
    NFA_setTransition(nfa, 2, 4+26, 11); // if E, state 2 -> state 11
    letterLoop(nfa, 4+26, 2); 
    
    NFA_setTransition(nfa, 0, 7+26, 3); // if H state 0 -> state 3
    NFA_setTransition(nfa, 3, 7+26, 11); // if H state 3 -> state 11
    letterLoop(nfa, 7+26, 3);

    NFA_setTransition(nfa, 0, 8+26, 4); // if I state 0 -> state 4
    NFA_setTransition(nfa, 4, 8+26, 11); // if I state 4 -> state 11
    letterLoop(nfa, 8+26, 4);
    
    NFA_setTransition(nfa, 0, 6+26, 5); // if G state 0 -> state 4
    NFA_setTransition(nfa, 5, +26, 11); // if G state 5 -> state 11
    letterLoop(nfa, 6+26, 5);

    NFA_setTransition(nfa, 0, 13+26, 6); // if N state 0 -> state 4
    letterLoop(nfa, 13+26, 6);
    NFA_setTransition(nfa, 6, 13+26, 7); // if N state 6 -> state 7
    letterLoop(nfa, 13+26, 7);
    NFA_setTransition(nfa, 7, 13+26, 11); // if N state 7 -> state 11 (more than twice) 
    
    NFA_setTransition(nfa, 0, 15+26, 8); // if P state 0 -> state 8
    letterLoop(nfa, 15, 8);
    NFA_setTransition(nfa, 8, 15+26, 9); // if P state 8 -> state 9
    letterLoop(nfa, 15, 10);
    NFA_setTransition(nfa, 9, 15+26, 11); // if p state 9 -> state 11
    NFA_setAcceptState(nfa, 11);

    return nfa;
}



// int NFA_main(){
//     printf("\n");

// // RUNNING TEST CASES ---------------------------------------------------------------------------------------------------------
//     //ENDING IN "AT" (or "at")
//     printf("Testing NFA that recognizes a string ends in 'at': ");
//     NFA* NFA_endsInAt = newNFA(3, 52);
//     endsInAt(NFA_endsInAt);
//     NFA_replLoop(NFA_endsInAt);

//     printf("\n");

//     //CONTAINS "GOT" (or "got")
//     printf("Testing NFA that recognizes a string that contains 'GOT': ");
//     NFA* NFA_containsGot = newNFA(4, 52);
//     containsGot(NFA_containsGot);
//     NFA_replLoop(NFA_containsGot);

//     printf("\n");

//     //HAPPENING ANAGRAM
//     printf("Testing NFA that recognizes a strings with more than one a,e,h,i,g OR two n's and p's");
//     NFA* NFA_happeningAnagram = newNFA(13, 52);
//     happeningAnagram(NFA_happeningAnagram);
//     NFA_replLoop(NFA_happeningAnagram);

//     printf("\n");



//     freeNFA(NFA_endsInAt);
//     freeNFA(NFA_containsGot);
//     freeNFA(NFA_happeningAnagram);
//     return 0; 
// }