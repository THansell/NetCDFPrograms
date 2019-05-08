#include <iostream>
#include <netcdf.h>
#include "netCDFObjects.h"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2

namespace nc_local {
	char temp_name[1025];
	int temp_int[50];
}

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

void nc_error(std::string funname, const char e) {
	std::cerr << "Error: function[" << funname << "] returned [" << nc_strerror(e) << "]" << std::endl;
	exit(ERRCODE);
}

int nc_super::getId() {
	return this->id;
}

std::string nc_super::getName() {
	if (this->name.empty()) {
		this->populateName();
	}
	return this->name;
}

nc_attribute::nc_attribute() {
}

nc_attribute::nc_attribute(int ncid, int varid, int id) {
	this->id = id;
	this->varid = varid;
	this->ncid = ncid;
}

size_t nc_attribute::getSize() {
	if (!this->size) {
		if (int retval = nc_inq_attlen(this->ncid, this->varid, this->getName().c_str(), &this->size)) {
			nc_error("nc_attribute::getSize calling nc_inq_attlen", retval);
		}
	}
	return this->size;
}

nc_type nc_attribute::getType() {
	nc_type type;
	if (int retval = nc_inq_atttype(this->ncid, this->varid, this->getName().c_str(), &type)) {
		nc_error("nc_attribute::getSize calling nc_inq_atttype", retval);
	}
	return type;
}

std::string nc_attribute::getValue() {
	char* stuff = (char*)calloc(this->getSize()+1, sizeof(char));
	if (int retval = nc_get_att(this->ncid, this->varid, this->getName().c_str(), (void *)stuff)) {
		nc_error("nc_attribute::getValue calling nc_get_att", retval);
	}
	stuff[this->getSize()] = '\0';
	std::string val = stuff;
	return val;
}

void nc_attribute::populateName() {
	if (int retval = nc_inq_attname(this->ncid, this->varid, this->id, nc_local::temp_name)) {
		nc_error("nc_attribute constructor calling nc_inq_attname", retval);
	}
	this->name = nc_local::temp_name;
}

void nc_attribute::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << ((this->varid == NC_GLOBAL)?"Global ": "" ) << "Attribute["
		<< this->getId() << "] : " 
		<< this->getName() << " = \"" << this->getValue() << "\""
		<< std::endl;
} 

nc_dimension::nc_dimension() {
}

nc_dimension::nc_dimension(int ncid, int dimid) {
	if (int retval = nc_inq_dim(ncid, dimid, nc_local::temp_name, &this->size)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimname]", retval);
	}
	this->id = dimid;
	this->name = nc_local::temp_name;
}

size_t nc_dimension::getSize() {
	return this->size;
}

void nc_dimension::populateName() {
	this->name = "BogusDimension";
}

void nc_dimension::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << "Dimension[" << this->id << "] : "
		   << this->name << ":size = [" << this->size << "]" << std::endl;
}
nc_variable::nc_variable(int ncid, int varid) {
	if (int retval = nc_inq_var(ncid, varid, nc_local::temp_name, &this->type, &this->number_of_dimensions, nc_local::temp_int, &this->number_of_attributes)) {
		nc_error("nc_variable constructor calling nc_inq_var", retval);
	}

	this->ncid = ncid;
	this->id = varid;
	this->name = nc_local::temp_name;

	for (int i = 0; i < this->number_of_dimensions; i++) {
		this->dimids.push_back(nc_local::temp_int[i]);
	}
}

nc_dimension nc_variable::getDimension(int dimid) {
	nc_dimension dim(this->ncid, dimid);
	return dim;
}

nc_attribute nc_variable::getAttribute(int attid) {
	nc_attribute att(this->ncid, this->getId(), attid);
	return att;
}

void nc_variable::populateName() {
	this->name = "BogusVariable";
}

void nc_variable::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << "Variable [" << this->id << "] : " << this->name << "(";
	std::string sep = "";
	for (int i = 0; i < this->number_of_dimensions; i++) {
		nc_dimension dim = this->getDimension(this->dimids[i]);
		stream << sep << dim.getName();
		sep = ", ";
	}
	stream << ")" << std::endl;
	for (int i = 0; i < this->number_of_attributes; i++) {
		nc_attribute att = this->getAttribute(i);
		att.DumpTo(stream, "  ");
	}
	stream << indent << "}" << std::endl;
}

NetCDFFile::NetCDFFile() {
}

bool NetCDFFile::isOpen() {
	return (this->id != -1);
}

void NetCDFFile::open(std::string file) {
	// Open the file
	if (int retval = nc_open(file.c_str(), NC_NOWRITE, &this->id)) {
		nc_error("NetCDFFile::open calling nc_open", retval);
	}	
}

void NetCDFFile::close() {
	if (int retval = nc_close(this->id)) {
		nc_error("NetCDFFile::close calling nc_close", retval);
	}
}

int NetCDFFile::getNumberOfDimensions() {
	if (this->number_of_dimensions == -1) {
		if (int retval = nc_inq_ndims(this->getId(), &this->number_of_dimensions)) {
			nc_error("NetCDFFile::getNumberOfDimensions calling nc_inq_ndims", retval);
		}
	}
	return this->number_of_dimensions;
}

int NetCDFFile::getNumberOfVariables() {
	if (this->number_of_variables == -1) {
		if (int retval = nc_inq_nvars(this->getId(), &this->number_of_variables)) {
			nc_error("NetCDFFile::getNumberOfVariables calling nc_inq_nvars", retval);
		}
	}
	return this->number_of_variables;
}

int NetCDFFile::getNumberOfAttributes() {
	if (this->number_of_attributes == -1) {
		if (int retval = nc_inq_natts(this->getId(), &this->number_of_attributes)) {
			nc_error("NetCDFFile::getNumberOfAttributes call nc_inq_natts", retval);
		}
	}
	return this->number_of_attributes;
}


void NetCDFFile::populateName() {
	if (int retval = nc_inq_path(this->getId(), NULL, nc_local::temp_name)) {
		nc_error("NetCDFFile::populateName calling nc_inq_path", retval);
	}
	this->name = nc_local::temp_name;
}

nc_dimension NetCDFFile::getDimension(int dimid) {
	nc_dimension dim (this->getId(), dimid);
	return dim;
}

nc_variable NetCDFFile::getVariable(int varid) {
	nc_variable var(this->getId(), varid);
	return var;
}

nc_attribute NetCDFFile::getAttribute(int attid) {
	nc_attribute att(this->getId(), NC_GLOBAL, attid);
	return att;
}

void NetCDFFile::DumpTo(std::ostream& stream, std::string indent) {
	stream << "NetCDF Path = [" << this->getName() << "]" << std::endl
		<< "     has [" << this->getNumberOfAttributes() << "] attributes" << std::endl
		<< "         [" << this->getNumberOfDimensions() << "] dimensions" << std::endl
		<< "     and [" << this->getNumberOfVariables() << "] variables" << std::endl;
	for (int i = 0; i < this->getNumberOfAttributes(); i++) {
		nc_attribute att(this->getId(), NC_GLOBAL, i);
		att.DumpTo(stream, "  ");
	}

	for (int i = 0; i < this>getNumberOfDimensions(); i++) {
		nc_dimension dim;
	}
} 