#pragma once
#ifndef NET_CDF_OBJECTS_HPP
#define NET_CDF_OBJECTS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>

// Local functions
void nc_error(std::string funname, const char e);

class nc_super {
protected:
	int id = -1;
	std::string name;
public:
	int getId();
	std::string getName();
	virtual void DumpTo(std::ostream& stream, std::string indent = "") = 0;
};

class nc_dimension: public nc_super {
protected:
	size_t size;
public:
	nc_dimension();
	nc_dimension(int ncid, int dimid);
	size_t getSize();
	void DumpTo(std::ostream& stream, std::string indent="");
};

class nc_dimensions {
private:
	std::map<int, nc_dimension> dims;
public:
	nc_dimensions();
	void readDimensions(int ncid, int dimcount);
	void add_dimension(int id, nc_dimension d);
	size_t getSize();
	std::string getNameOf(int dimdid);
	nc_dimension& operator[] (int id);
};

class nc_variable: nc_super {
private:
	size_t size;
	nc_type type;
	int number_of_dimensions;
	int number_of_attributes;
	int* dimension_ids;
	nc_dimensions* dims;
public:
	nc_variable(int ncid, int varid, nc_dimensions * dims);
	void DumpTo(std::ostream&, std::string indent = "");
};

class NetCDFFile: nc_super {
private:
	int number_of_dimensions;
	int number_of_variables;
	nc_dimensions dimensions;
	std::vector<nc_variable> variables;
public:
	NetCDFFile();
	bool isOpen();
	void open(std::string file);
	int getNumberOfDimension();
	void DumpTo(std::ostream& stream,std::string indent = "");
};

#endif // NET_CDF_OBJECTS_HPP