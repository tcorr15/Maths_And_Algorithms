#include <iostream>
#include <time.h>
#include <string.h>
#include <cmath>
#define TWO_PI 6.2831853071795864769252866 //defines 2*pi to be 2*3.14
#define ARRAY_INDEX 100 
#define MEDIAN_STAT 0.5
#define UPPER_STAT 0.75
#define LOWER_STAT 0.25
using namespace std;

//global variables, used throughout the LinearCongurenceMethod
unsigned int increment = 1; 
unsigned int seed = 30; 

double LinearCongruenceMethod(void) //randomly generates a linear number based on the current seed value and the current increment value using bitwise operations
{
	seed = 36969 * (seed & 65535) + (seed >> 16);
	increment = 18000 * (increment & 65535) + (increment >> 16);
	return ((seed << 16) + increment)/(pow(2,32)-1); //converts the 32*bit value into a double value
}

double BoxMullerMethod(const double& variance)//Converts the input vairance into a normal deviated number
{
	static bool haveSpare = false;
	static double rand1, rand2;
	if (haveSpare)
	{
		haveSpare = false;
		time_t t;
		srand(time(&t));
		rand1 = (double)rand() / ((double)RAND_MAX);
		return sqrt(variance * rand1) * sin(rand2);//returns sine of the second random variable multiplied by square root of the first random variable mulitpled by the variance
	}
	haveSpare = true;
	rand1 = (double)rand() / ((double)RAND_MAX);
	if (rand1 < 1e-100) rand1 = 1e-100;
	rand1 = -2 * log(rand1);
	rand2 = (rand() / ((double)RAND_MAX)) * TWO_PI;
	return sqrt(variance * rand1) * cos(rand2); //returns cosine of the second random variable multiplied by square root of the first random variable mulitpled by the variance
}

void errorHandling() {//executes the incorrect error process when the user inputs an incorrect value 
	cin.clear(); //clear bad input flag 
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input 
	cout << "Invalid input; please re-enter." << endl;
}

int main() {
	cout << "This Program will generate 100 random numbers between 0 and 1 using a particular seed.\n"
		"It will then perform several statistical checks to ensure linearity of the random numbers.\n"
		"Finally these number will be converted into a normal distribution.\n" << endl;
	while (true) {//while loop to allow for input of different seed values
		while (cout << "Select a seed number for the linearly random numbers: " << endl && !(cin >> seed))errorHandling();
		double randNoArray[ARRAY_INDEX];
		int med_range = 0, upper_range = 0, lower_range = 0;
		for (int i = 0; i < ARRAY_INDEX; i++) { //generates pseudonumbers using LCG method increments the stat checks
			randNoArray[i] = LinearCongruenceMethod();
			if (randNoArray[i] <= MEDIAN_STAT)
				med_range++;
			if (randNoArray[i] <= LOWER_STAT)
				lower_range++;
			if (randNoArray[i] >= UPPER_STAT)
				upper_range++;
		}
		cout << "Here is the linearly randomised numbers" << endl;
		for (int i = 0; i < 10; i++) {//prints the random numbers
			for (int j = 0; j < 10; j++)
				cout << randNoArray[(i * 10) + j] << ", ";
			cout << endl;
		}
		cout << endl << "---------------" << endl;
		cout << "Statistical Check" << endl;
		cout << "From these numbers the percentage below 50% is: " << med_range <<"% (Note: This number should be 50% for full linearity)"<< endl;
		cout << "From these numbers the percentage below 25% is: " << lower_range << "% (Note: This number should be 25% for full linearity)" << endl;
		cout << "From these numbers the percentage above 75% is: " << upper_range << "% (Note: This number should be 25% for full linearity)" << endl;
		cout << endl << "---------------" << endl;
		cout << "This is the above random numbers between 0 and 1 organised in order" << endl;
		double temp;
		double sum = 0;
		for (int i = 0; i < 100 - 1; i++) { //sorts random numbers from LCG method
			int min = i;
			sum += randNoArray[i];
			for (int j = i + 1; j < 100; j++) {
				if (randNoArray[j] < randNoArray[min])
					min = j;
			}
			temp = randNoArray[i];
			randNoArray[i] = randNoArray[min];
			randNoArray[min] = temp;
		}
		for (int i = 0; i < 10; i++) {//prints the organised numbers
			for (int j = 0; j < 10; j++)
				cout << randNoArray[(i * 10) + j] << ", ";
			cout << endl;
		}
		cout << endl << "---------------" << endl;
		cout << "These numbers will now be converted into a set of normally distributed numbers based\n"
			"on the mean and standard deviation you input (Note: for optimal results make sure to have a standard deviation below 1" << endl;
		double variance, mean;
		while (cout << "Enter mean to use: " << endl && !(cin >> mean))errorHandling();
		while (true) {
			while (cout << "Enter variance to use: " << endl && !(cin >> variance))errorHandling();
			if (variance >= 1) cout << "Error! Make sure the variance is less than 1 (for accurate results" << endl;
			else break;
		}
		cout << "Below is the normally distributed results" << endl;
		double normal_distribute[ARRAY_INDEX];
		for (int i = 0; i < 100; i++)//converts all numbers in from LCG methods to normal distributed numbers
			normal_distribute[i] = mean + abs(BoxMullerMethod(variance*randNoArray[i]));
		for (int i = 0; i < 10; i++) { //prints the normally distributed numbers
			for (int j = 0; j < 10; j++)
				cout << normal_distribute[(i * 10) + j] << ", ";
			cout << endl;
		}
		char response;
		bool breakWhile = false;
		while (breakWhile == false) {
			while (cout << "Would you like to repeat process with a different seed value? (y/n)" << endl && !(cin >> response))errorHandling();
			if (response != 'y' && response != 'n') cout << "Invalid Input" << endl;
			else breakWhile = true;
		}
		if (response == 'n') break;
		cout << endl << "---------------" << endl;
	}
	return 0;
}