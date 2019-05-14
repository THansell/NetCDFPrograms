#pragma once
#ifndef NC_LOCAL_H
#define NC_LOCAL_H

#include <string>

#define ERRCODE 2

std::string netcdf_types[];

namespace nc_local {
	char temp_name[];
	int temp_int[];
};

void nc_error(std::string funname, const char e);

#endif // NC_LOCAL_H