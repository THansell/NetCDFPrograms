#include "NetCDFFile.h"
#include "filename_functions.h"

NetCDFFile::NetCDFFile() {
}

bool NetCDFFile::isOpen() {
	return (this->id != -1);
}

void NetCDFFile::open(std::string file) {
	if (int retval = nc_open(file.c_str(), NC_NOWRITE, &this->id)) {
		nc_error("NetCDFFile::open calling nc_open", retval);
	}	
}

void NetCDFFile::create(std::string file) {
	if (int retval = nc_create(file.c_str(), NC_NOCLOBBER | NC_NETCDF4, &this->id)) {
		if (retval == NC_EEXIST) {
			std::cerr << "Error: File [" << BaseName(file) << "] exists. Can't create." << std::endl;
			exit(ERRCODE);
		}
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

std::string NetCDFFile::getDimensionName(int dimid) {
	return this->getDimension(dimid).getName();
}

nc_dimension NetCDFFile::getDimensionNamed(std::string dimName) {
	nc_dimension dim(this->getId(), dimName);
	return dim;
}

std::vector<std::string> NetCDFFile::getDimensionNames() {
	std::vector<std::string> dimNames;
	for (int i = 0; i < this->getNumberOfDimensions(); i++) {
		dimNames.push_back(this->getDimensionName(i));
	}
	return dimNames;
}

void NetCDFFile::addDimension(nc_dimension dim) {
	int dimid;
	if (int retval = nc_def_dim(this->getId(), dim.getName().c_str(), dim.getSize(), &dimid)) {
		nc_error("NetCDFFile [" + BaseName(this->getName()) + "] can't add dimension", retval);
	}
}

nc_variable NetCDFFile::getVariable(int varid) {
	nc_variable var(this->getId(), varid);
	return var;
}

nc_variable NetCDFFile::getVariableNamed(std::string str) {
	nc_variable var;
	for (int i = 0; i < this->getNumberOfVariables(); i++) {
		var = this->getVariable(i);
		if (str.compare(var.getName()) == 0) {
			break;
		}
	}
	return var;
}

std::vector<std::string> NetCDFFile::getVariableNames() {
	std::vector<std::string> vars;
	for (int i = 0; i < this->getNumberOfVariables(); i++) {
		vars.push_back(this->getVariable(i).getName());
	}
	return vars;
}

void NetCDFFile::addVariable(nc_variable var) {
	size_t dimsize = var.getNumberOfDimensions();
	int* dims = new int[dimsize];
	for (size_t i = 0; i < dimsize; i++) {
		dims[i] = var.getDimId((int)i);
	}
	int varid;
	if (int retval = nc_def_var(this->getId(), var.getName().c_str(), var.getType(), var.getNumberOfDimensions(), dims, &varid)) {
		nc_error("NetCDFFile [" + BaseName(this->getName()) + "] can't create variable [" + var.getName() + "]", retval);
	}
	short fill = -1;
	for (int i = 0; i < var.getNumberOfAttributes(); i++) {
		nc_attribute att = var.getAttribute(i);
		if (int retval = nc_put_att(this->getId(), varid, att.getName().c_str(), att.getType(), att.getSize(), att.getValue().c_str())) {
			nc_error("NetCDFFile [" + BaseName(this->getName()) + "] can't add attribute to variable", retval);
		}
	}
	delete[] dims;
}

nc_attribute NetCDFFile::getAttribute(int attid) {
	nc_attribute att(this->getId(), NC_GLOBAL, attid);
	return att;
}

void NetCDFFile::addAttribute(nc_attribute att) {
	if (int retval = nc_put_att(this->getId(), NC_GLOBAL, att.getName().c_str(), att.getType(), att.getSize(), (void*)att.getValue().c_str())) {
		nc_error("NetCDFFile [" + BaseName(this->getName()) + "] can't add attribute", retval);
	}
}

void NetCDFFile::copyAttribute(int attid, int outfileid) {
	if (int retval = nc_copy_att(this->getId(), NC_GLOBAL, this->getAttribute(attid).getName().c_str(), outfileid, NC_GLOBAL)) {
		nc_error("NetCDFFile copyAttribute calling nc_copy_att", retval);
	}
}

void NetCDFFile::copyAttributes(int outfileid) {
	for (int i = 0; i < this->getNumberOfAttributes(); i++) {
		this->copyAttribute(i, outfileid);
	}
}

void NetCDFFile::DumpTo(std::ostream& stream, std::string indent) {
	stream << indent << "NetCDF Path = [" << this->getName() << "]" << std::endl
		<< indent << "     has [" << this->getNumberOfAttributes() << "] attributes" << std::endl
		<< indent << "         [" << this->getNumberOfDimensions() << "] dimensions" << std::endl
		<< indent << "     and [" << this->getNumberOfVariables() << "] variables" << std::endl;

	stream << "  ----" << std::endl;

	for (int i = 0; i < this->getNumberOfAttributes(); i++) {
		nc_attribute att(this->getId(), NC_GLOBAL, i);
		att.DumpTo(stream, indent + "  ");
	}

	stream << "  ----" << std::endl;

	for (int i = 0; i < this->getNumberOfDimensions(); i++) {
		nc_dimension dim(this->getId(),i);
		dim.DumpTo(stream, indent + "  ");
	}

	stream << "  ----" << std::endl;

	for (int i = 0; i < this->getNumberOfVariables(); i++) {
		nc_variable var(this->getId(), i);
		var.DumpTo(stream, indent + "  ");
	}

	stream << "  ----" << std::endl;

}
 