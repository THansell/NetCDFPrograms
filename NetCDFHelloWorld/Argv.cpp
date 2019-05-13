#include "Argv.h"
#include <algorithm>

Argv::Argv() {
}

Argv::Argv(int argc, char* argv[]) {
    this->process(argc, argv);
}


size_t Argv::size() {
    return this->command_line.size() ;
}

size_t Argv::numArgs() {
	if (this->size() == 0) {
		return(this->size());
	}
	return(this->size() - 1);
}

void Argv::erase(size_t n) {
    this->command_line.erase(this->command_line.begin()+n);
}

std::string Argv::program_name() {
	if (this->size() == 0) {
		return("not set");
	}
	return(this->command_line[0]);
}

void Argv::clear() {
    this->command_line.clear();
}

void Argv::process(int argc, char* argv[]) {
	for (int i = 0; i < argc; i++) {
		this->command_line.push_back(std::string(argv[i]));
	}
}

std::string Argv::operator[] (int n) {
    if (this->command_line.size() > 0 && n < this->command_line.size()) {
		return(this->command_line[n]);
    }
    return "" ;
}

void Argv::replace(int n, std::string str) {
    this->command_line[n] = str;
}

void Argv::push_back(std::string str) {
    this->command_line.push_back(str);
}

void Argv::push_back(char * cstr) {
	std::string str(cstr);
	this->command_line.push_back(str);
}

void CommandLineFlags::addFlag(std::string name, std::vector<std::string> options) {
	this->flagDefinitions[name] = options;
}

std::string CommandLineFlags::matchesFlag(std::string arg) {
	for (auto const& i : this->flagDefinitions) {
		for (auto const& j : i.second) {
			if (arg.compare(j) == 0) {
				return i.first;
			}
		}
	}
	return "";
}

bool CommandLineFlags::testFlag(std::string f) {
	for (auto const& x : this->flags) {
		if (x.compare(f) == 0) {
			return true;
		}
	}
	return false;
}

void CommandLineFlags::setFlag(std::string f) {
	if (!this->testFlag(f)) {
		this->flags.push_back(f);
	}
}

void CommandLineFlags::process(Argv& args) {
	for (int i = 0; i < args.size(); i++ ) {
		std::string flag = matchesFlag(args[i]);
		if (!flag.empty()) {
			this->setFlag(flag);
			args.erase(i);
		}
	}
}

std::string BaseName(std::string fn) {
	size_t f = fn.find_last_of("\\");
	return fn.substr(f + 1);
}