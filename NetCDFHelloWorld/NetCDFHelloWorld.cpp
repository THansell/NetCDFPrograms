#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.h"
#include "netCDFObjects.h"

int retval;
char temporaryname[NC_MAX_NAME + 1];

int main(int argc, char* argv[])
{
	Argv args(argc, argv);

	string NetCDFFileName = args[1];

	NetCDFFile cloud;
	cloud.open(NetCDFFileName);

	cloud.close();
}
