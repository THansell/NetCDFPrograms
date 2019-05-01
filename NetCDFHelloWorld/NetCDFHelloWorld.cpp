#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.hpp"

int main(int argc, char* argv[])
{
	Argv args(argc, argv);
	for (int i = 0; i < args.size(); i++) {
		cout << "args[" << i << "] = [" << args[i] << "]" << endl;
	}
}