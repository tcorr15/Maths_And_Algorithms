#include <iostream>
#include <cmath>
#include <vector>
#define PI 3.14159265359 //to be used to calculate embedded charges

using namespace std;

void errorHandling() {//executes the incorrect error process when the user inputs an incorrect value
	cin.clear(); //clear bad input flag
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
	cout << "Invalid input; please re-enter." << endl;
}

void Gauss_Seidel(double b_Matrix[],int iterations) {//Performs the Gauss-Seidel Method to find electric potential across dielector
	int **A_Matrix = new int *[iterations];
	for (int i = 0; i < iterations; i++)//creates a dynamic A-matrix
		A_Matrix[i] = new int[iterations];
	double* x_Matrix = new double[iterations];
	for (int j = 0; j < iterations; j++) //declares all values in the x matrix (all electric potential values at each FDM node
		x_Matrix[j] = 0;
	double delta_y = pow(1, -6) / iterations;
	for (int i = 0; i < iterations; i++) {//Assigns each value in matrix A to match Gauss-Seidel method, looping through each row of matrix
		for (int j = 0; j < iterations; j++) {//Loops through each column in matrix declaring all entities on main diagonal to be -2 and the two either side to be 1
			if (j == i) A_Matrix[i][j] = -2;
			else if (j < (i - 1) || j > (i + 1))
				A_Matrix[i][j] = 0.0;
			else {
				try {
					A_Matrix[i][j] = 1.0;
				}
				catch (...) {}
			}
		}
		b_Matrix[i] = b_Matrix[i] * (delta_y * delta_y); // applies scalar (delta y squared) to element i in b-matrix 
	}
	b_Matrix[0] = b_Matrix[0] - 3.3; //since the first parallel plate is 3.3V then subtract 3.3 from b-matrix
	for (int i = 0; i < (iterations + 20); i++) {//applies gauss seidel model for 20 iterations after number of nodes filled (to ensure convergence is minimal) 
		for (int j = 0; j < iterations; j++) {//goes through all rows in and set the X matrix equal to the sum of the matching b row - all balues in A matrix (except for main diagonal) divided by entity on main diagonal
			double summation = b_Matrix[j];
			for (int k = 0; k< iterations; k++) {//subtracts all values from the sum value that are not on main diagonal as well as the entity in A-Matrix not being 0
				if (j != k && A_Matrix[j][k] != 0 && i != 0)
					summation = summation - abs(A_Matrix[j][k] * x_Matrix[k]);
			}
			x_Matrix[j] = abs(summation / A_Matrix[j][j]);
		}
	}
	cout << "After "<<iterations + 20<<" iterations of Gauss-Seidel, these are the final solutions to each nodes: " << endl << endl;
	for (int i = 0; i < iterations; i++) {//prints out final values 
		cout << "Node[" << i << "]: " << x_Matrix[i] << " V" << endl;
	}
	cout << endl << endl;
	for (int i = 0; i < iterations; i++)//deletes all dynamic arrays
		delete[] A_Matrix[i];
	delete[] A_Matrix;
	delete[] x_Matrix;
}

double ElectricField(int iteration, double field_Strength_Array[][10]) { //calculates the electric field strength across a dielector and returns the maximum electric field
	double max = 0;//holds the maximum electric field strength in dielector
	for (int i = 1; i < iteration; i++) {
		if (field_Strength_Array[1][i] > max || i == 1) max = field_Strength_Array[1][i];
	}
	return max;
}

int main() {
	cout << "This program is designed to find the electric potential across a dielector of length 100 micrometers and of width 1 micrometer within a capacitor where one plate is 3.3V and the other 0V."
		"\n The program will also calculate the Electric field strength at all these uniform nodes and display the maximum value."
		"\n-----------------------------------------" << endl << endl;
	int no_nodes;
	double max = 0;//holds the maximum electric field strength in dielector
	while (cout << "Enter the number of nodes along capacitor plate: " << endl && !(cin >> no_nodes))errorHandling();
	double* field_Strength_Array = new double[no_nodes]; //holds all the electric field strength values across dielector
	for (int i = 1; i <= no_nodes; i++) {
		double q = 50 * sin((PI * (i * (pow(1,-6) / no_nodes))) / pow(1,-6));
		field_Strength_Array[i-1] = q / 3.0; //charge divided by permitivity
		if (field_Strength_Array[i-1] > max || i == 1) max = field_Strength_Array[i-1];//calculates all charges
	}
	Gauss_Seidel(field_Strength_Array, no_nodes); //runs gauss-seidel on Ax = b to find all the values in x (electric potentials at each node)
	cout<<"-----------------------------------------" << endl << endl;
	cout << "Maximum Field Strength is " << max << endl;
	delete[] field_Strength_Array;
	return 0;
}