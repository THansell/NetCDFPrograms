#include "nc_local.h"
#include "nc_variable.h"

nc_variable::nc_variable() {
	this->ncid = -1;
	this->number_of_attributes = 0;
}

nc_variable::nc_variable(int ncid, int varid) {
	int ndims;
	if (int retval = nc_inq_var(ncid, varid, nc_local::temp_name, &this->type, &ndims, nc_local::temp_int, &this->number_of_attributes)) {
		nc_error("nc_variable constructor calling nc_inq_var", retval);
	}

	this->ncid = ncid;
	this->id = varid;
	this->name = nc_local::temp_name;
	for (int i = 0; i < ndims; i++) {
		this->dimids.push_back(nc_local::temp_int[i]);
	}
}

int nc_variable::getNumberOfAttributes() {
	return this->number_of_attributes;
}

int nc_variable::getNumberOfDimensions() {
	return (int)this->dimids.size();
}

int nc_variable::getDimId(int i) {
	if (i < this->dimids.size()) {
		return this->dimids[i];
	}
	return -1;
}

short nc_variable::getShortValueAt(size_t* dims) {
	short answer;
	if (int retval = nc_get_var1_short(this->ncid, this->id, dims, &answer)) {
		nc_error(this->getName() + ".getShortValue failed", retval);
	}
	return answer;
}

int nc_variable::getIntValueAt(size_t* dims) {
	int answer;
	if (int retval = nc_get_var1_int(this->ncid, this->id, dims, &answer)) {
		nc_error(this->getName() + ".getIntValueAt failed", retval);
	}
	return answer;
}

void nc_variable::putIntValueAt(int val, size_t* dims) {
	if (int retval = nc_put_var1_int(this->ncid, this->id, dims, &val)) {
		nc_error(this->getName() + ".putIntValueAt failed", retval);
	}
}

size_t nc_variable::getDataSize() {
	size_t size = 1;
	for (int i = 0; i < this->getNumberOfDimensions(); i++) {
		nc_dimension dim(this->ncid, this->dimids[0]);
		size *= dim.getSize();
	}
	return size;
}

void nc_variable::getDataInt(int* d) {
	if (int retval = nc_get_var(this->ncid, this->id, d)) {
		nc_error(this->getName() + " error fetching all data", retval);
	}
}

void nc_variable::putDataInt(int* d) {
	if (int retval = nc_put_var(this->ncid, this->id, d)) {
		nc_error(this->getName() + "error putting all data", retval);
	}
}

void nc_variable::getHyperSlabShort(size_t* s, size_t* c, short* d) {
	if (int retval = nc_get_vara(this->ncid, this->id, s, c, d)) {
		nc_error(this->getName() + " error getting HyperSlab", retval);
	}
}

void nc_variable::putHyperSlabShort(size_t* s, size_t* c, short* d) {
	if (int retval = nc_put_vara(this->ncid, this->id, s, c, d)) {
		nc_error(this->getName() + " error putting HyperSlab", retval);
	}
}
nc_type nc_variable::getType() {
	return this->type;
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
	for (int i = 0; i < this->getNumberOfDimensions(); i++) {
		nc_dimension dim = this->getDimension(this->dimids[i]);
		stream << sep << dim.getName();
		sep = ", ";
	}
	stream << ")" << std::endl;
	for (int i = 0; i < this->number_of_attributes; i++) {
		nc_attribute att = this->getAttribute(i);
		att.DumpTo(stream, indent + "  ");
	}
}