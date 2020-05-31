#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

double distance(double x1, double x2, double y) {
	return sqrt((x2 - x1)*(x2 - x1) + y*y);
}

int main() {
	double c = 3.00E8;
	double PI = 4 * atan(1);

	string filename;
	cout << "Please enter the name of the file: ";
	cin >> filename;

	ofstream file;
	file.open(filename);

	double y; // in meters
	double d;
	double sigma;
	int slits;
	cout << "How many slits? ";
	cin >> slits;
	slits = abs(slits);
	cout << "Please enter y, d, and sigma in meters: "; //Check for spacings = 0;
	cin >> y >> d >> sigma;
	sigma = abs(sigma);

	double screenWidth;

	cout << "Please enter the desired screen width: ";
	cin >> screenWidth;

	double a = -screenWidth/2.; //Endpoints on screen
	double b = screenWidth/2.;

	double startAt = -(d * slits + sigma * (slits - 1))/2;
	//double ax1 = -(d + sigma/2);
	//double ax = -d / 2.; //ax and bx are x coors of endpoints of slit
	//double bx = d / 2.;
	double lambda;
	cout << "Please insert the wavelength in nm: ";
	cin >> lambda;
	lambda *= 1E-9;
	double f = c / lambda;
	double omega = 2 * PI * f;
	double k = 2 * PI / lambda;
	int n = 100; //Number of intervals for slit- loops go n times
	int m = 1000; //Intervals along screen, although it's really m + 1 so I get the endpoints

	double dx = (b - a) / m;
	double screenPower = 0;

	double I0 = 1.; //Normallize intensity to 1
	double E0 = sqrt(I0) / n;

	double sourcePower = I0 * (slits * d);

	for(int i = 0; i < m + 1; i++) {
		double x = a + (b - a) / m * i;
		double Exsin = 0;
		double Excos = 0;

		for (int j = 0; j < slits; j++) { //Which slit
			for(int l = 0; l < n; l++) { //How far along slit ***K IS THE WAVE NUMBER***			
				double slitX = startAt + (d + sigma) * j + d / n * (l + 0.5);
				double dist = distance(x, slitX, y);
				Excos += cos(k * dist) * E0;
				Exsin += sin(k * dist) * E0;
			}
		}

		double Ix = Exsin * Exsin + Excos * Excos;
		screenPower += Ix * dx;
	
		//cout << x << "\t" << Ix << "\n";
		file << x << "\t" << Ix << "\n";
	}

	file.close();

	//My "energy conservation" attempts were unsuccessful. I don't think this means my program is wrong, since my graphs match others I've found;
	//I think something is wrong my my power integration.
	cout << "The source emits a power of " << sourcePower << " m * arbitrary intensity units.\n";
	cout << "The screen recieves a power of " << screenPower << " m * arbitrary intensity units.\n";
	double error = abs(screenPower - sourcePower) / abs (sourcePower);
	cout << "This gives a relative error of " << error << ".\n";

	//I initially experimented with having the program give the gnuplot command itself, but this lagged considerably and didn't always work
	//I had it output the command so I could copy and paste and save myself a little trouble when graphing
	//This also serves to confirm that the program executed correctly
	string command = "gnuplot -p -e \"p \'" + filename + "\' u 1:2 w l\"\n";
	cout << command;
	//system(command.c_str());

	return 0;
}
