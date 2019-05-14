#pragma once
#ifndef NC_VARIABLE_H
#define NC_VARIABLE_H

#include <netcdf.h>
#include <vector>
#include "nc_super.h"
#include "nc_attribute.h"
#include "nc_dimension.h"

class nc_variable : public nc_super {
private:
	int ncid = -1;
	nc_type type = 0;
	int number_of_attributes = 0;
	std::vector<int> dimids;
public:
	nc_variable();
	nc_variable(int ncid, int varid);
	int getNumberOfAttributes();
	int getNumberOfDimensions();
	int getDimId(int i);
	short getShortValueAt(size_t* dims);
	int getIntValueAt(size_t* dims);
	void putIntValueAt(int val, size_t* dims);
	size_t getDataSize();
	void getDataInt(int* d);
	void putDataInt(int* d);
	void getHyperSlabShort(size_t* s, size_t* count, short* d);
	void putHyperSlabShort(size_t* s, size_t* count, short* d);
	nc_type getType();
	nc_dimension getDimension(int dimid);
	nc_attribute getAttribute(int attid);
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent = "");
};

#endif // NC_VARIABLE_H