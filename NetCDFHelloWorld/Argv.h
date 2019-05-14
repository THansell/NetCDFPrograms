#ifndef ARGV_HPP
#define ARGV_HPP

#include <string>
#include <vector>
#include <map>

class Argv {
private:
    std::vector<std::string> command_line;
public:
    // Constructor
    Argv();
    Argv(int argc, char* argv[]);
    // 
    size_t size();
	size_t numArgs();
	std::string program_name();
    void erase(size_t n);
    void clear();
    void process(int argc, char* argv[]);
    std::string operator[] (int n);
    void replace(int n, std::string str);
    void push_back(std::string str);
	void push_back(char * cstr);
};

class CommandLineFlags {
private:
	std::map<std::string, std::vector<std::string>> flagDefinitions;
	std::vector<std::string> flags;
public:
	void addFlag(std::string name, std::vector<std::string> options);
	std::string matchesFlag(std::string arg);
	bool testFlag(std::string f);
	void setFlag(std::string);
	void process(Argv& args);
};

#endif // ARGV_HPP
