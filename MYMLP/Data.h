#ifndef DATA_H
#define DATA_H

#define INFINITE 0

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

class Data
{
public:
	Data(int, char *);
	~Data();

	void read();
	void printMatrixDist();
	inline int getDimension() { return dimension; };
	inline double getDistance(int vi, int vj) { return distMatrix[(vi != -1 ? vi : 1) - 1][(vj != -1 ? vj : 1) - 1]; };
	inline double **getMatrixCost() { return distMatrix; }
	inline double getXCoord(int vi) { return xCoord[vi - 1]; }
	inline double getYCoord(int vi) { return yCoord[vi - 1]; }
	inline bool getExplicitCoord() { return explicitCoord; };

	string getInstanceName(); // Get instance's name

private:
	string instaceName;
	int nbOfPar;

	int dimension;

	double **distMatrix;
	double *xCoord, *yCoord;

	// Computing Distances
	static double CalcDistEuc(double *, double *, int, int);
	static double CalcDistAtt(double *, double *, int, int);
	static double CalcDistGeo(double *, double *, int, int);
	static void CalcLatLong(double *, double *, int, double *, double *);

	bool explicitCoord;
};

#endif
