#include "nc_super.h"

int nc_super::getId() {
	return this->id;
}

std::string nc_super::getName() {
	if (this->name.empty()) {
		this->populateName();
	}
	return this->name;
}

void nc_super::setName(std::string name) {
	this->name = name;
}