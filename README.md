# JS Universal C Header
This is a C header and source file containing constants and functions I find myself copy-and-pasting from previous programs I wrote.

This is intended for baremetal programs, where importing `math.h` may be undesirable.


<details>
<summary> Functions </summary>
  
## int js_Eng_Not(char returnStr[15], double inputNum, int sigFigs)<BR />
This function formats a double number in engineering notation, from yocto- (10<sup>-24</sup>) to yotta- (10<sup>24</sup>).<BR />
String is 9 chars long, and is always right-justified so the prefix is always the 9th char.<BR />

### Inputs:
- `returnStr` is the pointer to the string to pass the formatted string back to.
- `inputNum` is the double that is going to be converted to a string in engineering notation.
- `sigFigs` is the integer number of significant figures the number will be displayed with. Number of significant figures must be between or including 1 to 7.

### Returns:<BR />
- `0` if a success
- `1` if error
- `"SigFigErr"` in `returnStr` if significant figure not in range. 
- `"OutOfRng"` in `returnStr` if value not in range.

