#ifndef ARGV_HPP
#define ARGV_HPP

#include <string>
#include <vector>

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

#endif // ARGV_HPP
