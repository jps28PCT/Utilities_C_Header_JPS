/******************************************************************************
 * Copyright (C) 2025 Jacob Smithmyer.  All rights reserved.
 * SPDX-License-Identifier: MIT
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * 
*******************************************************************************
 * 
 * This is the c source file to compliment the js_univ.c header file.
 * This defines the functions declared there.
 * 
******************************************************************************/

/*******************************   INCLUDES   *******************************/
#include <stdio.h>
#include "js_univ.h"


/*************************   FUNCTION DEFINITIONS   *************************/

/*****************************************************************************
 * This function formats a double number in engineering notation, from yocto- 
 * (10^-24) to yotta- (10^24).
 * String is 9 chars long, and is always right-justified so the prefix is 
 * always the 9th char
 *
 * @param	returnStr is the pointer to the string to pass the formatted 
 *			string back to.
 * @param	inputNum is the double that is going to be converted to a string
 *			in engineering notation
 * @param	sigFigs is the integer number of significant figures the number
 * 			will be displayed with. Number of significant figures must be 
 * 			between or including 1 to 7.
 * 
 * @return	0 if a success, 1 if error (Significant figure not in range: 
 * 			"SigFigErr", value not in range: "OutOfRng")
****************************************************************************/
int js_Eng_Not(char returnStr[15], double inputNum, int sigFigs){
	
	/***** VARIABLE INITIALIZATION *****/
	int exp = 0;							//Value of exponent (multiple of 3)
	double val = inputNum;					//Value that is checked to be within (1000, 1]
	char prefix;							//Engineering notation prefix symbol
	int numsAfterDecimal;					//Used for sprintf() %f formatting.
	/*** END VARIABLE INITIALIZATION ***/

	if(sigFigs > 7 || sigFigs < 1){			//Test to see if sigFigs is within range
		sprintf(returnStr, "SigFigErr");	//Significant figure range error
		return 1;							//Return 1 for error
	}

	while(js_abs(val) >= 1000){	//when magnitude of value is greater than or equal to 1000
		exp += 3;								//Increment exponent by 3
		val = inputNum / js_Power(10.0, exp);	//Find the new value of inputNum * 10^exp
	}

	while(js_abs(val) < 1){ //when value is less than one
		exp -= 3;								//Decrement exponent by 3
		val = inputNum / js_Power(10.0, exp);	//Find the new value of inputNum * 10^exp
	}

	//Select prefix based on exponent value:
	switch (exp) {
		case -24: 	prefix = 'y';	//yocto-
			break;
		case -21: 	prefix = 'z';	//zepto-
			break;
		case -18: 	prefix = 'a';	//atto-
			break;
		case -15: 	prefix = 'f';	//femto-
			break;
		case -12: 	prefix = 'p';	//pico-
			break;
		case -9: 	prefix = 'n';	//nano-
			break;	
		case -6: 	prefix = 'u';	//micro-
			break;
		case -3: 	prefix = 'm';	//mili-
			break;
		case 0: 	prefix = ' ';	//NO PREFIX
			break;
		case 3: 	prefix = 'k';	//kilo-
			break;
		case 6: 	prefix = 'M';	//mega-
			break;
		case 9: 	prefix = 'G';	//giga-
			break;
		case 12: 	prefix = 'T';	//tera-
			break;
		case 15: 	prefix = 'P';	//peta-
			break;
		case 18: 	prefix = 'E';	//exa-
			break;
		case 21: 	prefix = 'Z';	//zetta-
			break;
		case 24: 	prefix = 'Y';	//yotta-
			break;
		default: 	sprintf(returnStr, "OutOfRng");	//Out of range error
				 	return 1;						//Return 1 for error
			break;
	}//END switch(exp)

	//Calculate how many digits go after the decimal point due to significant figures
	if(js_abs(val) < 10){
		numsAfterDecimal = sigFigs - 1;
	}
	else if (js_abs(val) < 100){
		numsAfterDecimal = sigFigs - 2;
	}
	else{
		numsAfterDecimal = sigFigs - 3;
	}

	if(numsAfterDecimal < 0) numsAfterDecimal = 0;
	
	sprintf(returnStr, "%*.*f %c", sigFigs - numsAfterDecimal + 1, numsAfterDecimal, val, prefix);	//Formats number and prefix and writes it to returnStr
	return 0;												//Return 0 for success

}//END js_Eng_Not()




/*****************************************************************************
 * This function finds the power of 10 for the engineering notation formatting
 * of doubles. 
 * The purpose of this function is to allow for specific formatting by only 
 * returning the exponent and allowing the formatting to be handled by the 
 * parent function.
 *
 * @param	inputNum is the double that will be used to find the exponent for
 *			engineering notation
 *
 * @return	integer exponent for engineering notation formatting
****************************************************************************/
int js_Eng_Power(double inputNum){
	
	/***** VARIABLE INITIALIZATION *****/
	int exp = 0;
	double val = inputNum;
	char prefix;
	/*** END VARIABLE INITIALIZATION ***/

	while(val >= 1000){	//when value is greater than or equal to 1000
		exp += 3;								//Increments exponent by 3
		val = inputNum / js_Power(10.0, exp);	//Find the new value of inputNum * 10^exp
	}

	while(val < 1){		//when value is less than one
		exp -= 3;								//Decrements exponent by 3
		val = inputNum * js_Power(10.0, exp);	//Find the new value of inputNum * 10^exp
	}

	return exp;
}//END js_Eng_Power()




/*****************************************************************************
 * This function raises a double to an integer power and returns the product
 * as a double
 *
 * @param	base is the base of the operation.
 * @param	power is the exponent, and can be any whole integer.
 *
 * @return	the value found by exponentiation (base^power). 
 *
****************************************************************************/
double js_Power(double base, int power){
	
	/***** VARIABLE INITIALIZATION *****/
	double product = 1.0;
	/*** END VARIABLE INITIALIZATION ***/

	//Positive exponent: multiply by itself
	if (power > 0){
		for( int i=0; i<power; i++){	//Loops for 'power' amount of times
			product = product * base;	//Multiply by base
		}
	}

	//Negative exponent: divide
	else if (power < 0){
		for( int i=0; i>power; i--){	//Loops for 'power' amount of times
			product = product / base;	//Divide by base
		}
	}

	//Exponent is zero
	else {
		product = 1.0;	//n^0=1 by definition, added for redundancy
	}

	return product;		//Return calculated value of base^power

}//END js_Power()




/*****************************************************************************
 * This function returns the absolute value of a float.
 *
 * @param	value_in is the number to find the absolute value of.
 *
 * @return	the absolute value of value_in, | value_in |
 *
****************************************************************************/
double js_abs(double value){
	if(value < 0){
		return -1 * value;
	}
	else{
		return value;
	}
}//END js_abs()



/*****************************************************************************
 * This function converts an integer between and including 0 and 9 to its 
 * ASCII char representation.
 *
 * @param	IntToConv is the integer to convert between and including 0 and 9
 *
 * @return	The ASCII representation of the integer.
 * 			Returns 'E' if greater than 9 or less than 0.
 *
****************************************************************************/
char js_Int_to_Char(int IntToConv){
	
	/***** VARIABLE INITIALIZATION *****/
	char CharToRet;							//ASCII char representing the int
	/*** END VARIABLE INITIALIZATION ***/

	if(IntToConv > 9  || IntToConv < 0){	//If IntToConv is less than 0 or greater than 9
		CharToRet = 'E';					//'E' for ERROR - out of range
	}
	else{									//Else if within range
		CharToRet = 0x30 + IntToConv;		//Find the ASCII char value of the int
	}

	return CharToRet;						//Returns ASCII char representation of int

}//END js_Int_to_Char()



/*****************************************************************************
 * This function converts an ASCII char between and including '0' and '9' to 
 * its integer equivalent.
 * @param	CharToConv is the char to convert
 *
 * @return	The converted integer value of the char
 * 			Returns 0xEEEE if not a number.
 *
****************************************************************************/
char js_Char_to_Int(char CharToConv){
	
	/***** VARIABLE INITIALIZATION *****/
	int IntToRet;							//ASCII char representing the int
	/*** END VARIABLE INITIALIZATION ***/

	if(CharToConv > '9'  || CharToConv < '0'){	//If IntToConv is less than 0 or greater than 9
		IntToRet = 0xEEEE;					//0xEEEE for ERROR - out of range
	}
	else{									//Else if within range
		IntToRet = CharToConv - 0x30;		//Find the ASCII char value of the int
	}

	return IntToRet;						//Returns ASCII char representation of int

}//END js_Int_to_Char()

