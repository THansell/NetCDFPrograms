#pragma once
#ifndef NET_CDF_FILE_H
#define NET_CDF_FILE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <netcdf.h>
#include "nc_local.h"
#include "nc_super.h"
#include "nc_variable.h"

class NetCDFFile: public nc_super {
private:
	int number_of_dimensions = -1;
	int number_of_variables = -1;
	int number_of_attributes = -1;
public:
	NetCDFFile();
	bool isOpen();
	void open(std::string file);
	void create(std::string file);
	void close();
	int getNumberOfDimensions();
	int getNumberOfVariables();
	int getNumberOfAttributes();
	void populateName();
	nc_dimension getDimension(int dimid);
	std::string getDimensionName(int dimid);
	nc_dimension getDimensionNamed(std::string dimName);
	std::vector<std::string> getDimensionNames();
	void addDimension(nc_dimension dim);
	nc_variable getVariable(int varid);
	nc_variable getVariableNamed(std::string varName);
	std::vector<std::string> getVariableNames();
	void addVariable(nc_variable var);
	nc_attribute getAttribute(int attid);
	void addAttribute(nc_attribute att);
	void copyAttribute(int attid, int outfileid);
	void copyAttributes(int outfileid);
	void DumpTo(std::ostream& stream, std::string indent="");
};

#endif // NET_CDF_FILE_H