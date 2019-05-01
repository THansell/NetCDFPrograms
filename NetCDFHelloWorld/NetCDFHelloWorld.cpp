#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.hpp"
#include "netCDFObjects.hpp"

int retval;
char temporaryname[NC_MAX_NAME + 1];

class nc_dimensions {
public:
	vector<nc_dimension> dims;
	nc_dimensions() {} // default constructor to create a new object

	nc_dimensions(int ncid) { // read the dimensions from a netCDF id

	}
	void add_dimension(nc_dimension d) {
		this->dims.push_back(d);
	}
};

int main(int argc, char* argv[])
{
	Argv args(argc, argv);

	int nc_file_id;

	string NetCDFFileName = args[1];

	cout << "Opening file [" << NetCDFFileName << "]" << endl;
	if ((retval = nc_open(NetCDFFileName.c_str(), NC_NOWRITE, &nc_file_id))) {
		nc_error("nc_open", retval);
	}

	int ndim;
	int nvars;
	int natts;
	int unlim;

	if ((retval = nc_inq(nc_file_id, &ndim , &nvars, &natts, &unlim ))) {
		nc_error("nc_inq", retval);
	}

	cout << "Number of Dimensions =[" << ndim << "]" << endl;
	cout << "Number of Variables =[" << nvars << "]" << endl;
	cout << "Number of Attributes =[" << natts << "]" << endl;
	cout << "Unlimited Dimensions =[" << unlim << "]" << endl;

	nc_dimensions myDims;

	for (int i = 0; i < ndim; i++) {
		myDims.add_dimension(nc_dimension(nc_file_id, i));
	}

	for (nc_dimension d: myDims.dims) {
		cout << "Dimension[" << d.getId() << "] name is [" << d.getName() << "]" << " size is [" << d.getSize() << "]" << endl;
	}
	
	char varname[NC_MAX_NAME + 1];
	nc_type vartype;
	int vardims;
	int vardimid[4];
	int varatts;

	if (retval = nc_inq_var(nc_file_id, 0, (char*)& varname, &vartype, &vardims, (int*)&vardimid, &varatts)) {
		nc_error("nc_inc_var", retval);
	}

	cout << "Varname =[" << varname << "]" << endl;
	cout << "Vartype =[" << vartype << "]" << endl;
	cout << "Vardims =[" << vardims << "]" << endl;
	
	for (int i = 0; i < vardims; i++) {
		cout << i << " = " << vardimid[i] << endl;
	}
	
	size_t dims[4] = { 0,0,0,90 };
	short val;

	if (retval = nc_get_var1(nc_file_id, 0, dims, &val)) {
		nc_error("nc_get_var", retval);
	}

	cout << " val = [" << val << "]" << endl;

	vector<short> data;

	cout << " number of elements = [" << 345 * 120 * 121 * 121 << "]" << endl;
	cout << " sizeOf(data) = [" << sizeof(data) << "]" << endl;
	cout << " SIZE_T_MAX = [" << SIZE_MAX << "]" << endl;
	cout << " sizeof(size_t) = [" << sizeof(size_t) << "]" << endl;

	for (int ti = 0; ti < 1; ti++) {
		for (int zi = 0; zi < 120; zi++) {
			for (int yi = 0; yi < 121; yi++) {
				for (int xi = 0; xi < 121; xi++) {
					size_t dims[4] = { ti, zi, yi, xi };
					short bob;
					nc_get_var1(nc_file_id, 0, dims, &bob);
					data.push_back( bob );
				}
			}
			cout << " z [" << zi << "]" << endl;
		}
	}

	cout << "vector size now = [" << data.size() << "]" << endl;
	cout << " from array x = 0 [" << data[0] << "]" << endl ;
	cout << " from array x = 90 [" << data[90] << "]" << endl;

	cout << "Closing file [" << nc_file_id << "]" << endl;
	nc_close(nc_file_id);

}
