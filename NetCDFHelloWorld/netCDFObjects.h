#pragma once
#ifndef NET_CDF_OBJECTS_HPP
#define NET_CDF_OBJECTS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <netcdf.h>


// Local functions
std::string netcdf_types[];

void nc_error(std::string funname, const char e);

class nc_super {
protected:
	int id = -1;
	std::string name;
public:
	int getId();
	std::string getName();
	virtual void populateName() = 0;
	virtual void DumpTo(std::ostream& stream, std::string indent = "") = 0;
};

class nc_attribute : public nc_super {
protected:
	int ncid;
	int varid;
	nc_type type;
	size_t size = 0;
public:
	nc_attribute();
	nc_attribute(int ncid, int varid, int id);
	size_t getSize();
	nc_type getType();
	std::string getValue();
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent = "");
};

class nc_dimension : public nc_super {
protected:
	size_t size;
public:
	nc_dimension();
	nc_dimension(int ncid, int dimid);
	size_t getSize();
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent = "");
};

class nc_variable: public nc_super {
private:
	int ncid;
	size_t size;
	nc_type type;
	int number_of_attributes;
	int number_of_dimensions;
	std::vector<int> dimids;
public:
	nc_variable(int ncid, int varid);
	nc_dimension getDimension(int dimid);
	nc_attribute getAttribute(int attid);
	void populateName();
	void DumpTo(std::ostream&, std::string indent = "");
};

class NetCDFFile: public nc_super {
private:
	int number_of_dimensions = -1;
	int number_of_variables = -1;
	int number_of_attributes = -1;
	std::map<int, nc_dimension> dims;
public:
	NetCDFFile();
	bool isOpen();
	void open(std::string file);
	void close();
	int getNumberOfDimensions();
	int getNumberOfVariables();
	int getNumberOfAttributes();
	void populateName();
	nc_dimension getDimension(int dimid);
	nc_variable getVariable(int varid);
	nc_attribute getAttribute(int attid);
	void DumpTo(std::ostream& stream,std::string indent = "");
};

#endif // NET_CDF_OBJECTS_HPP