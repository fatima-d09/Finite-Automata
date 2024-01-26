# Finite-Automata
Finite automata are simple computing “devices” that recognize patterns.

- They are 2 types of Finite automata:
  1. Deterministic Finite automata (DFA): At any given point, there is only one possible next state for a given input symbol.
  2. Non-deterministic Finite automata (NFA):  There can be multiple paths the automaton may take for a given input. The acceptance criteria are based on the existence of at least one computation path leading to an accepting state.



#  ┬┴┤´❛ᴥ❛`ʔฅ HOW TO RUN PROJECT ┬┴┤´❛ᴥ❛`ʔฅ 

THIS PROJECT WAS MADE ON VSCODE, COMPILED AND TESTED THE CODE IN CMD, ON A COMPUTER 🏃 ON WINDOWS 10. 

1. You have to change your terminal's directory to the project file 
	- NOTE: This will be different depending on your system [ TAKE HEED (=✖ᆽ✖=) !!!]

2. Enter the following prompt: "gcc -std=c99 -pedantic -Wall -Werror -o a.exe *.c" 
	- This will compile ✨ my code ✨ to the a.exe executable

3. Then, type "a.exe" into the terminal to run the executable

4. 🪄! It should work; (hopefully)! 



# PROJECT NOTES:

- For my input alphabet, instead of using the 128 ASCII characters, I mapped it to 53 integers (0-25 = uppercase (ABC...XYZ), 26-51 = lowercase (abc..xyz), 52 & 53 = binary 1 & 0) 

- My code was ran through valgrind, and I received no errors. A couple of memory leaks are left, and they continue to ANNOY me to this day ┗(｀O ´)┛. 
	- It also said that memory leaks were the exception in regards to errors through valgrind.

- I could not figure out how to translate an NFA into a DFA for the life of me; it was so hard (⩺_⩹)! All my efforts were futile, but alas I shall be successful next time (In sha Allah).



Made By: Fatima Dembele







