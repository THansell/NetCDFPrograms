#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.h"
#include "NetCDFFile.h"
#include "filename_functions.h"

int main(int argc, char* argv[])
{
	Argv args(argc, argv);
	NetCDFFile existing;
	NetCDFFile created;

	existing.open(argv[1]);
	nc_dimension time = existing.getDimensionNamed("time");
	for (size_t i = 0; i < time.getSize(); i++) {
		size_t dims[1]; 
		dims[0] = i;
		int value = existing.getVariableNamed("time").getIntValueAt(dims);
		nc_attribute timestamp;
		timestamp.setName("timestamp");
		timestamp.setType(NC_CHAR);
		timestamp.setValue(to_string(value));

		string createdFileName = BaseName(argv[1], true) + "-" + to_string(value) + ".nc";
		cout << "Creating [" << createdFileName << "] at timeslice [" << value << "]" << endl;
		created.create(createdFileName);
		created.addAttribute(timestamp);

		existing.copyAttributes(created.getId());

		for (int i = 0; i < existing.getNumberOfDimensions(); i++) {
			nc_dimension copy = existing.getDimension(i);
			if (copy.getName().compare("time") == 0) {
				copy.setSize(1);
			}
			created.addDimension(copy);
		}

		for (int i = 0; i < existing.getNumberOfVariables(); i++) {
			nc_variable copy = existing.getVariable(i);
			created.addVariable(copy);
		}

		for (int i = 0; i < existing.getNumberOfVariables(); i++) {
			nc_variable copy = existing.getVariable(i);
			if (copy.getName().compare("time") == 0) {
				dims[0] = 0;
				created.getVariableNamed("time").putIntValueAt(value, dims);
			} else if (copy.getName().compare("cloud") == 0) {
				size_t start[4], count[4];
				start[0] = i; count[0] = 1;
				start[1] = 0; count[1] = existing.getDimensionNamed("z").getSize();
				start[2] = 0; count[2] = existing.getDimensionNamed("y").getSize();
				start[3] = 0; count[3] = existing.getDimensionNamed("x").getSize();
				short* data; data = new short[count[1] * count[2] * count[3]];
				copy.getHyperSlabShort(start, count, data);
				created.getVariableNamed("cloud").putHyperSlabShort(start, count, data);
			} else {
				int* data = new int[copy.getDataSize()];
				copy.getDataInt(data);
				created.getVariableNamed(copy.getName()).putDataInt(data);
				delete[] data;
			}
		}

		created.close();
	}
	existing.close();
}
