#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.hpp"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2

void nc_error(string funname, const char e) {
	cerr << "Error: function[" << funname << "] returned [" << nc_strerror(e) << "]" << endl;
	exit(ERRCODE);
}

int retval;
char temporaryname[NC_MAX_NAME + 1];

class nc_dimension {
public:
	int id;
	string name;
	size_t size;
	
	nc_dimension(int ncid, int dimid) {
		if (retval = nc_inq_dim(ncid, dimid, (char *)&temporaryname, &this->size)) {
			nc_error("nc_inq_dimname", retval);
		}
		this->id = dimid;
		this->name = temporaryname;
	}
};

class nc_dimensions {
public:
	vector<nc_dimension> dims;
	nc_dimensions() {}
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
		cout << "Dimension[" << d.id << "] name is [" << d.name << "]" << " size is [" << d.size << "]" << endl;
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

//	short data[345][120][121][121];
	short data[345*120*121*121];

	cout.flush();


	if (retval = nc_get_var_short(nc_file_id, 0, data)) {
		nc_error("nc_get_var", retval);
	}

	cout << " from array x = 0 [" << data[0] << "]" << endl;
//	cout << " from array x = 0 [" << data[0][0][0][0] << "]" << endl ;
//	cout << " from array x = 90 [" << data[0][0][0][90] << "]" << endl;

	cout << "Closing file [" << nc_file_id << "]" << endl;
	nc_close(nc_file_id);

}
