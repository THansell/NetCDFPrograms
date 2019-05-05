#include <iostream>

using namespace std;

#include <netcdf.h>

#include "Argv.hpp"
#include "netCDFObjects.hpp"

int retval;
char temporaryname[NC_MAX_NAME + 1];

int main(int argc, char* argv[])
{
	Argv args(argc, argv);

	int nc_file_id;

	string NetCDFFileName = args[1];

	NetCDFFile fred;
	fred.open(NetCDFFileName);

	fred.DumpTo(cout);
	exit(0);

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


	cout << "vector size now = [" << data.size() << "]" << endl;
	cout << " from array x = 0 [" << data[0] << "]" << endl ;
	cout << " from array x = 90 [" << data[90] << "]" << endl;

	cout << "Closing file [" << nc_file_id << "]" << endl;
	nc_close(nc_file_id);

}
