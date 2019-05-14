#pragma once
#ifndef NC_ATTRIBUTE_H
#define NC_ATTRIBUTE_H

#include <netcdf.h>
#include "nc_super.h"

class nc_attribute : public nc_super {
protected:
	int ncid = -1;
	int varid = -1;
	nc_type type = 0;
	size_t size = 0;
	std::string value;
public:
	nc_attribute();
	nc_attribute(int ncid, int varid, int id);
	size_t getSize();
	nc_type getType();
	void setType(nc_type type);
	std::string getValue();
	void setValue(std::string val);
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent = "");
};

#endif // NC_ATTRIBUTE_H
