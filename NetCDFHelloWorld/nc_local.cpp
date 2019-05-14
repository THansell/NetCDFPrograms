#include <string>
#include <iostream>
#include <netcdf.h>
#include "nc_local.h"

std::string netcdf_types[] = {
	"NC_NAT",
	"NC_BYTE",
	"NC_CHAR",
	"NC_SHORT",
	"NC_INT",
	"NC_FLOAT",
	"NC_DOUBLE",
	"NC_UBYTE",
	"NC_USHORT",
	"NC_UINT",
	"NC_INT64",
	"NC_UINT64",
	"NC_STRING"
};

namespace nc_local {
	char temp_name[1025];
	int temp_int[50];
}


void nc_error(std::string funname, const char e) {
	std::cerr << "Error: function[" << funname << "] returned [" << nc_strerror(e) << "]" << std::endl;
	exit(ERRCODE);
}