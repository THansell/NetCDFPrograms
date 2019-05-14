#include "nc_local.h"
#include "nc_attribute.h"

nc_attribute::nc_attribute() {
}

nc_attribute::nc_attribute(int ncid, int varid, int id) {
	this->id = id;
	this->varid = varid;
	this->ncid = ncid;
}

size_t nc_attribute::getSize() {
	if (!this->size) {
		if (this->ncid == -1) {
			this->size = this->value.size();
		}
		else if (int retval = nc_inq_attlen(this->ncid, this->varid, this->getName().c_str(), &this->size)) {
			nc_error("nc_attribute::getSize calling nc_inq_attlen", retval);
		}
	}
	return this->size;
}

nc_type nc_attribute::getType() {
	if (this->ncid != -1) {
		if (int retval = nc_inq_atttype(this->ncid, this->varid, this->getName().c_str(), &this->type)) {
			nc_error("nc_attribute::getSize calling nc_inq_atttype", retval);
		}
	}
	return this->type;
}

void nc_attribute::setType(nc_type type) {
	this->type = type;
}

std::string nc_attribute::getValue() {
	if (this->value.empty()) {
		char* stuff = new char[this->getSize() + 1];
		if (int retval = nc_get_att(this->ncid, this->varid, this->getName().c_str(), (void*)stuff)) {
			nc_error("nc_attribute::getValue calling nc_get_att", retval);
		}
		stuff[this->getSize()] = '\0';
		this->value = stuff;
		delete[] stuff;
	}
	return this->value;
}

void nc_attribute::setValue(std::string val) {
	this->value = val;
}

void nc_attribute::populateName() {
	if (int retval = nc_inq_attname(this->ncid, this->varid, this->id, nc_local::temp_name)) {
		nc_error("nc_attribute constructor calling nc_inq_attname", retval);
	}
	this->name = nc_local::temp_name;
}

void nc_attribute::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << ((this->varid == NC_GLOBAL) ? "Global " : "") << "Attribute["
		<< this->getId() << "] : "
		<< this->getName() << " = \"" << this->getValue() << "\""
		<< std::endl;
}