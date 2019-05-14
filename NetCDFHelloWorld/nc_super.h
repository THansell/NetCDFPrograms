#pragma once
#ifndef NC_SUPER_H
#define NC_SUPER_H

#include <string>
#include <iostream>

class nc_super {
protected:
	int id = -1;
	std::string name;
public:
	int getId();
	std::string getName();
	void setName(std::string name);
	virtual void populateName() = 0;
	virtual void DumpTo(std::ostream& stream, std::string indent = "") = 0;
};

#endif // NC_SUPER_H