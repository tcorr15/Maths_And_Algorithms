#include <cmath>
#include <string.h>
#include <cstring>
#include <iostream>

using namespace std;

float functionValue(float xValue, float coeff, int power, bool isDifferential) { //Returns a float result of the multiplication of coeff, power and x value (depending on whether its a differential or not)
	if (isDifferential == false) return coeff * pow(xValue, power); //normal
	else return (power * coeff) * pow(xValue, (power - 1));//differentiated function
}

float NewtonMethod(int powerArray[], float coeff[], int tolerance, float guess, int power) {//performs newtons method upon a linear function which has been 
	int count = 0;
	bool isConverging = true;
	float curConvergence = guess, prevConvergence = 0;
	float toleranceDecimal = pow(10, -float(tolerance)), prevXValue = 0, currentXValue = guess, functionResult, differentialFunctionResult;
	while (abs(prevXValue - currentXValue) > toleranceDecimal && isConverging == true) { //while loop to continue to increase the count value for until a consistent value has been met or the series begins to diverge
		prevXValue = currentXValue;
		count++;
		functionResult = 0;
		differentialFunctionResult = 0;
		for (int i = 0; i <= power; i++) {
			functionResult += functionValue(prevXValue, coeff[i], powerArray[i], false);
			differentialFunctionResult += functionValue(prevXValue, coeff[i], powerArray[i], true);
		}
		currentXValue = prevXValue - (functionResult / differentialFunctionResult);
		prevConvergence = curConvergence;
		curConvergence = currentXValue - prevXValue;
		if (abs(curConvergence) > abs(prevConvergence)) {
			isConverging = false;
		}
	}
	if (isConverging == false) {
		cout << "Series does not converge. Process halted." << endl;
		return 0;
	}
	else {
		cout << "Count Value = " << count << endl;
		return floor(currentXValue / toleranceDecimal) * toleranceDecimal;
	}
}

void errorHandling() {//executes the incorrect error process when the user inputs an incorrect value
	cin.clear(); //clear bad input flag
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
	cout << "Invalid input; please re-enter." << endl;
}

int main() { //The beginning of the program. Will request inputs from the user for linear function values, tolerance and initial guess before it begins performing Newtons Method.
	int initialGuess, power, tolerance;
	cout << "This function will enable you to identify the root of a linear function. \n\n" << endl;//declaring the inputs from the user for use, requests the number of power terms in linear equations
	while (cout << "Please enter the maximum power in the linear fucntion: " && !(cin >> power)) errorHandling();//Produces a while loop which is only broken when a valid input is given
	int *powerArray = new int[power]; //an array which holds all the powers of the linear fucntion (in descending order)
	float *coefficientsArray = new float[power]; //an array holding all coefficients of the linear function
	for (int i = 0; i <= power; i++) {//asks the user for the coefficients of each power term and populates the arrays above with these values
		powerArray[i] = power - i;
		while (cout << "Please enter coeefficient for x^" << power - i << ": " && !(cin >> coefficientsArray[i])) errorHandling();
	}
	bool breakloop = false;
	while (breakloop == false) { //creates a loop that will allow the user to retry the process with a new guess and tolerance value
		while (cout << "Please enter tolerance value (number of decimal places): " && !(cin >> tolerance)) errorHandling();
		while (cout << "Enter initial Guess (Make it reasonable): " && !(cin >> initialGuess)) errorHandling();
		float output = NewtonMethod(powerArray, coefficientsArray, tolerance, float(initialGuess), power); //runs newtons method root function
		cout << "The final value from Newtons Method (the nearest root to initial guess) is: " << output << endl;
		int repeat = 2;
		while (repeat != 0 && repeat != 1) { //loop error check to identify if the user wants to repeat the process
			cout << "Would you like to repeat the process with a new guess and tolerance (1 for yes, 0 for no): " << endl;
			cin >> repeat;
			if (repeat == 0) breakloop = true;
			else if (repeat != 1) {
				cout << "Error! Incorrect format" << endl;
				cin.clear();
			}
		}
	}
	return 0;
}
