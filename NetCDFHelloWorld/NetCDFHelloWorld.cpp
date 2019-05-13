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
	NetCDFFile existing;
	NetCDFFile created;

	existing.open(argv[1]);
	
	nc_variable timeslice = existing.getVariableNamed("time");
	size_t dims[1];
	dims[0] = 0;
	short value = timeslice.getShortValueAt(dims);
	cout << "[" << value << "]" << endl;
	exit(0);

	created.create(argv[2]);

	nc_attribute time;
	time.setName("timestamp");
	time.setType(NC_CHAR);
	time.setValue("BOGUS TIMESTAMP");
	created.addAttribute(time);

	existing.copyAttributes(created.getId());

	for (int i = 0; i < existing.getNumberOfDimensions(); i++) {
		nc_dimension copy = existing.getDimension(i);
		created.addDimension(copy);
	}

	for (int i = 0; i < existing.getNumberOfVariables(); i++) {
	nc_variable copy = existing.getVariable(i);
		created.addVariable(copy);
	}

	existing.close();
	created.close();
	created.open(args[2]);
	created.DumpTo(cout);
	created.close();
}
