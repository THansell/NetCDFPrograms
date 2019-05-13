#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.h"
#include "netCDFObjects.h"

void usage(std::string prog_name) {
	cout << "Usage: " << prog_name << " (--dump|--create) <file>" << endl;
	cout << "     -d, --dump        dump the NetCDF file <file> to standard output" << endl;
	cout << "     -c. --create      create a NetCDF file <file> (If a file of that name exists nothing will happen)" << endl;
}

int main(int argc, char* argv[])
{
	Argv args(argc, argv);

	CommandLineFlags clflags;
	clflags.addFlag("dump", { "--dump", "-d" });
	clflags.addFlag("create", { "--create", "-c" });
	clflags.process(args);

	NetCDFFile cloud;

	if (clflags.testFlag("dump")) {
		cloud.open(args[1]);
		cloud.DumpTo(cout);
		cloud.close();
	} else if (clflags.testFlag("create")) {
		cloud.create(args[1]);
		cloud.close();
	}
	else {
		usage(BaseName(args[0]));
	}
}
