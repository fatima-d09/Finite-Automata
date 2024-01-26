#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct dfa{
    int states; //num of States 
    int currentState; 
    int inputAlphabetSize; // input alphabet
    int** transitionTable; //transition function    
    int initialState; //the initial state
    bool* acceptStates; //set of accepting states (i am using an array)
} DFA;

DFA* newDFA(int states, int inputAlphabetSize){
// initializing the dfa
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->states = states;
    dfa->inputAlphabetSize = inputAlphabetSize;
    dfa->currentState = 0;

// allocating the memory for the transition table
    dfa->transitionTable = (int**)malloc(states * sizeof(int*));
    for (int i = 0; i < states; i++) {
        dfa->transitionTable[i] = (int*)malloc(inputAlphabetSize* sizeof(int));
    }

//populating the transition table w/default (placeholde`r) values
    for(int i =0; i < states; i++){
        for(int j = 0; j < inputAlphabetSize; j++){
            dfa -> transitionTable[i][j] = -1; 
        }
    }

// allocating memory for the array of acceptStates
dfa ->acceptStates = (bool*)malloc(states*sizeof(bool)); 

// clearing the acceptStates array, for some reason it populates as soon as it is initialized????????????
for (int i = 0; i < dfa->states; i++) {
        dfa->acceptStates[i] = false;
    }

return dfa; 
}

// SETTERS -------------------------------------------------------------------------------------------------------- 

    // set transition 
    void DFA_setTransition(DFA* dfa, int state, int symbol, int nextState){
        dfa->transitionTable[state][symbol] = nextState; 
    }

    // sets a state from the array to be an accepted one
    void DFA_setAcceptState(DFA* dfa, int state){
        dfa -> acceptStates[state] = true;
    }

// -------------------------------------------------------------------------------------------------------------------


// PROCESSING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------

// process the input symbol 
    void DFA_processSymbol(DFA* dfa, int symbol) {
        if (symbol >= 0 && symbol < dfa->inputAlphabetSize) {
            if (dfa->transitionTable[dfa->currentState][symbol] != -1) {
                dfa->currentState = dfa->transitionTable[dfa->currentState][symbol];
            }
        // printf("Current state: %d\n", dfa->currentState); debug line
        }
    }


// This is my DFA_run function (takes an input and runs the DFA). I named it DFA_processString so that it made sense to me.
    void DFA_processString(DFA* dfa, char* input) {
        int inputSize = strlen(input);
        for (int i = 0; i < inputSize; i++) {
            char symbol = input[i];
            int index;

            if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')) {
                if (symbol >= 'A' && symbol <= 'Z') {
                    index = symbol - 'A'; // uppercase letters are mapped to 0 to 25
                } else {
                    index = symbol - 'a' + 26; // lowercase letters are mapped to 26 to 51
                }
            } else if (symbol >= '0' && symbol <= '1') {
                index = symbol - '0' + 52; // 0 is mapped to 52, 1 is mapped to 53
            } 

            // process the symbol using the DFA_processSymbol function
            if (index >= 0 && index < dfa->inputAlphabetSize) {
                DFA_processSymbol(dfa, index);
            }
        }
    }


// MISC. FUNCTIONS ------------------------------------------------------------------------------------------------------------------------

    // This is a debug function 
    void printAcceptStates(DFA* dfa) {
        printf("Accept States: ");
        for (int i = 0; i < dfa->states; i++) {
            if (dfa->acceptStates[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    // this method is a fix to a bug that I encountered: The states would not reset after each input. This was the solution
    void resetDFA(DFA* dfa){
        dfa -> currentState = 0;
    }

    // to check if the automata "dfa" is in an accepting State
    bool DFA_isInAcceptState(DFA* dfa){
        return dfa -> acceptStates[dfa -> currentState]; 
    }

    // frees all the memory allocated to the dfa
    void freeDFA(DFA* dfa) {
        for (int i = 0; i < dfa->states; i++) {
            free(dfa->transitionTable[i]);
        }
        free(dfa->transitionTable);
        free(dfa->acceptStates);
        free(dfa);
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------



// repl loop that asks for input and checks it 
void DFA_replLoop(DFA* dfa){
    char input[100];

    //start the input loop
    while (1) {
        resetDFA(dfa);//reset DFA on rejected input to avoid previous inputs advancing the state
                    //  and also allowing you to test cases after a succsessful one is enput
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

        //actually run the dfa with the given input
        DFA_processString(dfa, input);

        //checks the shit 
        if (DFA_isInAcceptState(dfa)) {
            printf("Input accepted\n");
        
        } else {
            printf("Input rejected\n");
        }
    }
}


// TEST CASE METHODS ---------------------------------------------------------------------------------------------------------------------------
DFA* exactlyCSC(DFA* dfa){

//makes non-c and non-s letters lead to a not accepting state
    for (int i = 0; i < 52; i++) {
        if (i != 2 && i != 18) {
            DFA_setTransition(dfa, 0, i, 4); 
            DFA_setTransition(dfa, 1, i, 4);
            DFA_setTransition(dfa, 2, i, 4); 
        }
    }

//  DFA_setTransition(dfa, initial state, symbol, destination set)
    DFA_setTransition(dfa, 0, 2, 1); // if input is "C", state 0 -> state 1
    DFA_setTransition(dfa, 1, 18, 2); // if input is "S", state 1 -> state 2
    DFA_setTransition(dfa, 2, 2, 3); // if input is "C", state 2 -> state 3
    

// set accepting states
    DFA_setAcceptState(dfa, 3); 

    return dfa; 
}

//-----------------------------------------------------------------------------------------------------------------------------
DFA* startsWithVowel(DFA* dfa){

//set the states for all the characters that are NOT valid
    for (int i = 0; i < 52; i++) {
        if (i != 26 && i != 30 && i != 34 && i != 40 && i != 46 &&
            i != 0  && i != 4 && i !=8 && i != 14 && i !=20) {
            DFA_setTransition(dfa, 0, i, 2); // 2 is a non accepting state
        }
    }


    // i could make this a bit shorter with a for loop like the code bit above, but this is much more readable imo
    // and i am too lazy to change it
    // but i am aware this is not DRY code. 
    //set uppercase
    DFA_setTransition(dfa, 0, 0, 1); // if input starts with "A", state 0 -> state 1 
    DFA_setTransition(dfa, 0, 4, 1); // if input starts with "E", state 0 -> state 1
    DFA_setTransition(dfa, 0, 8, 1); // if input starts with "I", state 0 -> state 1
    DFA_setTransition(dfa, 0, 14, 1); // if input starts with "O", state 0 -> state 1
    DFA_setTransition(dfa, 0, 20, 1); // if input starts with "U", state 0 -> state 1

    //set lowercase 
    DFA_setTransition(dfa, 0, 26, 1); // if input starts with "a", state 0 -> state 1 
    DFA_setTransition(dfa, 0, 30, 1); // if input starts with "e", state 0 -> state 1
    DFA_setTransition(dfa, 0, 34, 1); // if input starts with "i", state 0 -> state 1
    DFA_setTransition(dfa, 0, 40, 1); // if input starts with "o", state 0 -> state 1
    DFA_setTransition(dfa, 0, 46, 1); // if input starts with "u", state 0 -> state 1

    
    // define accept state
    DFA_setAcceptState(dfa, 1);

    return dfa; 
}

// ----------------------------------------------------------------------------------------------------------------------------

DFA* containsEnd(DFA* dfa){

    DFA_setTransition(dfa, 0, 4, 1); //if e, 0 -> 1 
    DFA_setTransition(dfa, 1, 13, 2); // if n, 1 -> 2
    DFA_setTransition(dfa, 2, 3, 3); // if d, 2 -> 3

    DFA_setTransition(dfa, 0, 4 + 26, 1); //if E, 0 -> 1 
    DFA_setTransition(dfa, 1, 13 + 26, 2); // if N, 1 -> 2
    DFA_setTransition(dfa, 2, 3 + 26 , 3); // if D, 2 -> 3


// all other letters lead back to state 0 
for (int i = 0; i < dfa->inputAlphabetSize; i++) {
        if (i != (4) && i != (13) && i != (3) &&
            i != (4 + 26) && i != (13 + 26) && i != (3 + 26)){

            DFA_setTransition(dfa, 0, i, 0);
            DFA_setTransition(dfa, 1, i, 0);
            DFA_setTransition(dfa, 2, i, 0);
            }
        }

    DFA_setAcceptState(dfa, 3);
    return dfa; 
}

// ----------------------------------------------------------------------------------------------------------------------------

DFA* evenOneAndZero(DFA* dfa){
    //state 0 transitions 
    DFA_setTransition(dfa, 0, 52, 2); // reminder: 52 = 0, 53 = 1
    DFA_setTransition(dfa, 0, 53, 1); 

    //state 1 transitions
    DFA_setTransition(dfa, 1, 52, 3);
    DFA_setTransition(dfa, 1, 53, 0);

    //state 2 transitions 
    DFA_setTransition(dfa, 2, 52, 0); 
    DFA_setTransition(dfa, 2, 53, 3);

    //state 3 transtitions 
    DFA_setTransition(dfa, 3, 52, 1);
    DFA_setTransition(dfa, 3, 53, 2);


    //set accept state 
    DFA_setAcceptState(dfa, 0);

    return dfa;
}



// int main(){

//     printf("\n");

// //RUNNING TEST CASES ----------------------------------------------------------------------------------------------------------------------

//     // EXACTLY CSC -------------------------------------------------------------------------------------------------------------
//         DFA* dfa_exactlyCSC = newDFA(5, 52); 
//         printf("Testing DFA that recognizes a string that is exactly 'CSC' (all caps no quotes)" );
//         exactlyCSC(dfa_exactlyCSC);
//         DFA_replLoop(dfa_exactlyCSC);

//         printf("\n");

//     // STARTS WITH VOWEL--------------------------------------------------------------------------------------------------------
    
//         DFA* dfa_startsWithVowel = newDFA(3, 52);

//         printf("Testing DFA that recognizes strings that starts with a vowel"); 
//         startsWithVowel(dfa_startsWithVowel);
//         DFA_replLoop(dfa_startsWithVowel);

//         printf("\n");

//     // CONTAINS END ------------------------------------------------------------------------------------------------------------
//         DFA* dfa_containsEnd = newDFA(4, 52); 

//         printf("Testing DFA that recognizes strings that contain the sequence 'end' ");
//         containsEnd(dfa_containsEnd);
//         DFA_replLoop(dfa_containsEnd);

//         printf("\n");

//     // EVEN ONES AND ZEROS -----------------------------------------------------------------------------------------------------

//         DFA* dfa_evenOneAndZero = newDFA(4, 54);

//         printf("Testing DFA that recognizes a string with an even number of 1s and 0s:");
//         evenOneAndZero(dfa_evenOneAndZero);
//         DFA_replLoop(dfa_evenOneAndZero);

//         printf("\n");

// // --------------------------------------------------------------------------------------------------------------------------

// // free the lost souls trapped within the eternal prison of this godforsaken data structure

//     freeDFA(dfa_exactlyCSC);
//     freeDFA(dfa_startsWithVowel);
//     freeDFA(dfa_evenOneAndZero);
//     freeDFA(dfa_containsEnd);

//     return 0;
// }