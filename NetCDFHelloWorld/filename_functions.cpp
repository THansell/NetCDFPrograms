#include "filename_functions.h"

std::string BaseName(std::string fn, bool stripExt) {
	size_t f;
	std::string base = ((f = fn.find_last_of("\\")) == std::string::npos)? fn : fn.substr(f+1) ;
	
	if (stripExt) {
		base = ((f = base.find_last_of(".")) == std::string::npos)?base:base.substr(0,f) ;
	} 
	return base;
}