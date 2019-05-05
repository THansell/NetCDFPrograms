#include <iostream>
#include <netcdf.h>

#include "netCDFObjects.hpp"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2

namespace nc_local {
	char temp_name[1025];
	int temp_int[50];
}

int nc_super::getId() {
	return this->id;
}

std::string nc_super::getName() {
	return this->name;
}

void nc_error(std::string funname, const char e) {
	std::cerr << "Error: function[" << funname << "] returned [" << nc_strerror(e) << "]" << std::endl;
	exit(ERRCODE);
}

nc_variable::nc_variable(int ncid, int varid, nc_dimensions * dims) {
	int retval;
	if (retval = nc_inq_var(ncid, varid, nc_local::temp_name, &this->type, &this->number_of_dimensions, nc_local::temp_int, &this->number_of_attributes)) {
		nc_error("nc_variable constructor calling nc_inq_var", retval);
	}
	this->id = varid;
	this->name = nc_local::temp_name;
	this->dims = dims;
	this->dimension_ids = (int*)calloc(this->number_of_dimensions, sizeof(int));
	for (int i = 0; i < this->number_of_dimensions; i++) {
		this->dimension_ids[i] = nc_local::temp_int[i];
	}
}

void nc_variable::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << "Variable {" << std::endl
		<< indent << "  ID = [" << this->id << "]" << std::endl
		<< indent << "  NAME = [" << this->name << "]" << std::endl
		<< indent << "  NUMBER OF DIMENSIONS = [" << this->number_of_dimensions << "]" << std::endl
		<< indent << "  NUMBER OF ATTRIBUTES = [" << this->number_of_attributes << "]" << std::endl;
	stream << indent << "  DIMENSION IDS = ( ";
	std::string sep = "";
	for (int i = 0; i < this->number_of_dimensions; i++) {
		stream << sep << this->dims->getNameOf(this->dimension_ids[i]);
		sep = ", ";
	}
	<< indent << "  NUMBER OF ATTRIBUTES = [" << this->number_of_attributes << "]" << std::endl;
	stream << " )";
	stream << indent << "}" << std::endl;
}

nc_dimension::nc_dimension() {
}

nc_dimension::nc_dimension(int ncid, int dimid) {
	int retval;
	if (retval = nc_inq_dim(ncid, dimid, nc_local::temp_name, &this->size)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimname]", retval);
	}
	this->id = dimid;
	this->name = nc_local::temp_name;
}


size_t nc_dimension::getSize() {
	return this->size;
}


void nc_dimension::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent<< "Dimension { " << std::endl
		   << indent << "  ID = [" << this->id << "]" << std::endl
		   << indent << "  NAME = [" << this->name << "]" << std::endl
		   << indent << "  SIZE = [" << this->size << "]" << std::endl
		   << indent << "}" << std::endl;
}

nc_dimensions::nc_dimensions() {
}

void nc_dimensions::readDimensions(int ncid, int dimcount) {
	for (int i = 0; i < dimcount; i++) {
		dims[i] = nc_dimension(ncid, i);
	}
}

std::string nc_dimensions::getNameOf(int id) {
	return dims[id].getName();
}
 
size_t nc_dimensions::getSize() {
	return this->dims.size();
}

nc_dimension& nc_dimensions::operator[] (int id) {
	return this->dims[id];
}
NetCDFFile::NetCDFFile() {
	this->number_of_dimensions = 0;
}

bool NetCDFFile::isOpen() {
	return (this->id != -1);
}

void NetCDFFile::open(std::string file) {
	int retval;
	std::string funname = "NetCDFFile::open calling";
	// Open the file
	if (retval = nc_open(file.c_str(), NC_NOWRITE, &this->id)) {
		nc_error(funname + " nc_open", retval);
	}
	
	if (retval = nc_inq_path(this->id, NULL , nc_local::temp_name)) {
		nc_error(funname + " nc_inq_path", retval);
	}
	this->name = nc_local::temp_name;

	if (retval = nc_inq_ndims(this->id, &this->number_of_dimensions)) {
		nc_error(funname + " nc_inq_ndims", retval);
	}


	this->dimensions.readDimensions(this->id,this->number_of_dimensions );

	if (retval = nc_inq_nvars(this->id, &this->number_of_variables)) {
		nc_error(funname + " nc_inq_vars", retval);
	}
	for (int i = 0; i < this->number_of_variables; i++) {
		variables.push_back(nc_variable(this->id, i, &this->dimensions));
	}
}


int NetCDFFile::getNumberOfDimension() {
	return this->number_of_dimensions;
}

void NetCDFFile::DumpTo(std::ostream& stream, std::string indent) {
	stream << "NetCDF { " << std::endl
		   << "  ID = [" << this->id << "]" << std::endl
		   << "  Path = [" << this->name << "]" << std::endl
		   << "  Number of Dimensions = [" << this->number_of_dimensions << "]" << std::endl;
	for (int i = 0; i < this->number_of_dimensions; i++) {
		this->dimensions[i].DumpTo(stream, "  ");
	}
	stream << "  Number Of Variables = [" << this->number_of_variables << "]" << std::endl;
	for (nc_variable var : this->variables) {
		var.DumpTo(stream, "  ");
	}
	stream << "}" << std::endl;
} 

