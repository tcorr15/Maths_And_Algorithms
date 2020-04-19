#include <cmath> 
#include <string> 
#include <iostream> 

using namespace std;

bool OddEven(int i) {//checks to see if the input is even or odd 
	int i_out = i % 2;
	if (i_out == 0)
		return false;
	return true;
}

float errorMaker(int power, float coeff, float x,float delta_x, int a, int b,int randInt){//finds the error value for each x term in function.
	float coeffVal = coeff;
	int powerVal = power;
	for (int i = 0; i < 4; i++) {//differentiates the x term 4 times unless the value reaches zero
		coeffVal = coeffVal * powerVal;
		powerVal--;
		if (powerVal < 0) return 0.; //breaks the function if differentiation process results in zero
	}
	return (pow(delta_x,4) / 180)*(b-a)*abs(coeffVal*pow(randInt, (powerVal - 4)));
}

float SimpsonsRule(int powerArray[], float coeff[], int a, int b, int n) {//performs simpsons rule upon the given function (defined by 2 arrays powerArray and coeff) within the range a and b and with n intervals
	float delta_x = (((double)b - (double)a) / (double)n);
	int randInt = rand() % (b - a + 1);
	double sum_out = 0, x = a;
	for (int i = 1; i <= n + 1; i++) {//defines the Simpson's Rule sum by substituting x into the function and multiplying that sum by the appropriate scalar (1,2 or 4)
		float function_Sum = 0.;
		for (int j = 0; j <= powerArray[0]; j++) {//sums the given function depending on the value of x
			if(coeff[j] != 0)
				function_Sum += (coeff[j] * pow(x, powerArray[j]));
		}
		if (i != 1 && i != (n + 1)) { //applies the appropriate scalar factor
			bool isOdd = OddEven(i);
			if (isOdd == false)
				function_Sum = function_Sum * 4;
			else
				function_Sum = function_Sum * 2;
		}
		sum_out += function_Sum;
		x += delta_x; //increments x by delta x((b - a) /n)
	}
	float error = 0;
	for (int i = 0; i <= powerArray[0]; i++) {//defines the error from the function
		error += errorMaker(powerArray[i],coeff[i],x,delta_x,a,b,randInt);
	}
	cout<<"Error of Function is "<<error<<endl;
	return ((delta_x / 3) * sum_out);
}

void errorHandling() {//executes the incorrect error process when the user inputs an incorrect value 
	cin.clear(); //clear bad input flag 
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input 
	cout << "Invalid input; please re-enter." << endl;
}

int main() {
	int min_bound, max_bound, no_intervals, power;
	while (cout << "Please enter the maximum power in the linear function: " && !(cin >> power)) errorHandling();//Produces a while loop which is only broken when a valid input is given
	int *powerArray = new int[power]; //an array which holds all the powers of the linear fucntion (in descending order)
	float *coefficientsArray = new float[power]; //an array holding all coefficients of the linear function
	for (int i = 0; i <= power; i++) {//asks the user for the coefficients of each power term and populates the arrays above with these values
		powerArray[i] = power - i;
		while (cout << "Please enter coeefficient for x^" << power - i << ": " && !(cin >> coefficientsArray[i])) errorHandling();
	}
	while (cout << "Enter minimum bound of the function: " << endl && !(cin >> min_bound))errorHandling();
	while (cout << "Enter maximum bound of the function: " << endl && !(cin >> max_bound)) errorHandling();
	while (cout << "Finally enter the number of intervals: " << endl && !(cin >> no_intervals)) errorHandling();
	cout << "The result of integrationg is " << SimpsonsRule(powerArray, coefficientsArray, min_bound, max_bound, no_intervals) << endl;
	return 0;
}