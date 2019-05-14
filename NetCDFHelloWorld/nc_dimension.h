#pragma once
#ifndef NC_DIMENSION_H
#define NC_DIMENSION_H

#include "nc_super.h"

class nc_dimension : public nc_super {
protected:
	int ncid = -1;
	size_t size = 0;
public:
	nc_dimension();
	nc_dimension(int ncid, int dimid);
	nc_dimension(int ncid, std::string dimName);
	size_t getSize();
	void setSize(size_t size);
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent = "");
};

#endif // NC_DIMENSION_H