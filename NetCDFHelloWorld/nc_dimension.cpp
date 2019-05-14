#include <netcdf.h>
#include "nc_local.h"
#include "nc_dimension.h"

nc_dimension::nc_dimension() {}

nc_dimension::nc_dimension(int ncid, int dimid) {
	if (int retval = nc_inq_dim(ncid, dimid, nc_local::temp_name, &this->size)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimname]", retval);
	}
	this->ncid = ncid;
	this->id = dimid;
	this->name = nc_local::temp_name;
}

nc_dimension::nc_dimension(int ncid, std::string dimName) {
	this->ncid = ncid;
	this->name = dimName;
	if (int retval = nc_inq_dimid(this->ncid, dimName.c_str(), &this->id)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimid]", retval);
	}
	if (int retval = nc_inq_dimlen(this->ncid, this->id, &this->size)) {
		nc_error("nc_dimension constructor calling [nc_inq_dimlen]", retval);
	}
}

size_t nc_dimension::getSize() {
	return this->size;
}

void nc_dimension::setSize(size_t size) {
	this->size = size;
}

void nc_dimension::populateName() {
	if (this->name.empty()) {
		if (this->ncid != -1) {
			if (int retval = nc_inq_dimname(this->ncid, this->id, nc_local::temp_name)) {
				this->name = nc_local::temp_name;
			}
		} else {
			this->name = "BogusDimension";
		}
	}
}

void nc_dimension::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << "Dimension[" << this->id << "] : "
		<< this->name << ":size = [" << this->size << "]" << std::endl;
}