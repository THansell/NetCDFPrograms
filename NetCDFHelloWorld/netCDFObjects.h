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
	void setName(std::string name);
	virtual void populateName() = 0;
	virtual void DumpTo(std::ostream& stream, std::string indent="") = 0;
};

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
	void DumpTo(std::ostream& stream, std::string indent="");
};

class nc_dimension : public nc_super {
protected:
	size_t size = 0;
public:
	nc_dimension();
	nc_dimension(int ncid, int dimid);
	size_t getSize();
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent="");
};

class nc_variable: public nc_super {
private:
	int ncid;
	nc_type type = 0;
	int number_of_attributes;
	std::vector<int> dimids;
public:
	nc_variable();
	nc_variable(int ncid, int varid);
	int getNumberOfAttributes();
	int getNumberOfDimensions();
	int getDimId(int i);
	short getShortValueAt(size_t* dims);
	nc_type getType();
	nc_dimension getDimension(int dimid);
	nc_attribute getAttribute(int attid);
	void populateName();
	void DumpTo(std::ostream& stream, std::string indent="");
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
	void create(std::string file);
	void close();
	int getNumberOfDimensions();
	int getNumberOfVariables();
	int getNumberOfAttributes();
	void populateName();
	nc_dimension getDimension(int dimid);
	void addDimension(nc_dimension dim);
	nc_variable getVariable(int varid);
	nc_variable getVariableNamed(std::string);
	void addVariable(nc_variable var);
	nc_attribute getAttribute(int attid);
	void addAttribute(nc_attribute att);
	void copyAttribute(int attid, int outfileid);
	void copyAttributes(int outfileid);
	void DumpTo(std::ostream& stream, std::string indent="");
};

#endif // NET_CDF_OBJECTS_HPP