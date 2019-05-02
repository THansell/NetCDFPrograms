#pragma once
#ifndef NET_CDF_OBJECTS_HPP
#define NET_CDF_OBJECTS_HPP

#include <string>

// Local functions
void nc_error(std::string funname, const char e);

class nc_dimension {
private:
	int id;
	std::string name;
	size_t size;
public:
	nc_dimension(int ncid, int dimid);
	int getId();
	size_t getSize();
	std::string getName();
};

#endif // NET_CDF_OBJECTS_HPP