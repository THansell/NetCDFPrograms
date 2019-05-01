#include <iostream>
#include <netcdf.h>

#include "netCDFObjects.hpp"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2

namespace nc_local {
	char temp_name[NC_MAX_NAME + 1];
}

void nc_error(std::string funname, const char e) {
	std::cerr << "Error: function[" << funname << "] returned [" << nc_strerror(e) << "]" << std::endl;
	exit(ERRCODE);
}

nc_dimension::nc_dimension(int ncid, int dimid) {
	int retval;
	if (retval = nc_inq_dim(ncid, dimid, nc_local::temp_name, &this->size)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimname]", retval);
	}
	this->id = dimid;
	this->name = nc_local::temp_name;
}

int nc_dimension::getId() {
	return this->id;
}

size_t nc_dimension::getSize() {
	return this->size;
}

std::string nc_dimension::getName() {
	return this->name;
}