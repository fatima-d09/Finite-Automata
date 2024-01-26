#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"
#include "nfa.h"
#include "dfa.h"
// #include "convertor.c"


int main(){
    printf("\n");

//RUNNING TEST CASES ----------------------------------------------------------------------------------------------------------------------

    // EXACTLY CSC -------------------------------------------------------------------------------------------------------------
        DFA* dfa_exactlyCSC = newDFA(5, 52); 
        printf("Testing DFA that recognizes a string that is exactly 'CSC' (all caps no quotes)" );
        exactlyCSC(dfa_exactlyCSC);
        DFA_replLoop(dfa_exactlyCSC);

        printf("\n");

    // STARTS WITH VOWEL--------------------------------------------------------------------------------------------------------
    
        DFA* dfa_startsWithVowel = newDFA(3, 52);

        printf("Testing DFA that recognizes strings that starts with a vowel"); 
        startsWithVowel(dfa_startsWithVowel);
        DFA_replLoop(dfa_startsWithVowel);

        printf("\n");

    // CONTAINS END ------------------------------------------------------------------------------------------------------------
        DFA* dfa_containsEnd = newDFA(4, 52); 

        printf("Testing DFA that recognizes strings that contain the sequence 'end' ");
        containsEnd(dfa_containsEnd);
        DFA_replLoop(dfa_containsEnd);

        printf("\n");

    // EVEN ONES AND ZEROS -----------------------------------------------------------------------------------------------------

        DFA* dfa_evenOneAndZero = newDFA(4, 54);

        printf("Testing DFA that recognizes a string with an even number of 1s and 0s:");
        evenOneAndZero(dfa_evenOneAndZero);
        DFA_replLoop(dfa_evenOneAndZero);

        printf("\n");

// --------------------------------------------------------------------------------------------------------------------------

// free the lost souls trapped within the eternal prison of this godforsaken data structure

    freeDFA(dfa_exactlyCSC);
    freeDFA(dfa_startsWithVowel);
    freeDFA(dfa_evenOneAndZero);
    freeDFA(dfa_containsEnd);


// NFA Test Cases 


printf("NOW TESTING THE NFA CASES!! \n\n");

// RUNNING TEST CASES ---------------------------------------------------------------------------------------------------------
    //ENDING IN "AT" (or "at")
    printf("Testing NFA that recognizes a string ends in 'at': ");
    NFA* NFA_endsInAt = newNFA(3, 52);
    endsInAt(NFA_endsInAt);
    NFA_replLoop(NFA_endsInAt);

    printf("\n");

    //CONTAINS "GOT" (or "got")
    printf("Testing NFA that recognizes a string that contains 'GOT': ");
    NFA* NFA_containsGot = newNFA(4, 52);
    containsGot(NFA_containsGot);
    NFA_replLoop(NFA_containsGot);

    printf("\n");

    //HAPPENING ANAGRAM
    printf("Testing NFA that recognizes a strings with more than one a,e,h,i,g OR two n's and p's");
    NFA* NFA_happeningAnagram = newNFA(13, 52);
    happeningAnagram(NFA_happeningAnagram);
    NFA_replLoop(NFA_happeningAnagram);

    printf("\n");



    freeNFA(NFA_endsInAt);
    freeNFA(NFA_containsGot);
    freeNFA(NFA_happeningAnagram);


    // printAcceptStates(convertNFAtoDFA(endsInAt));





}
