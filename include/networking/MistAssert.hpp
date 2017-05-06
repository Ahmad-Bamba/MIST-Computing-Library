#include <iostream>
#include <exception>

class MistAssertException : public std::exception {
	virtual const char* what() const throw() {
		return "Mist Assert Exception";
	}
} mist_aex;

#define mistassert(x, y) \
if (!(x)) { \
	std::cerr << (y) << std::endl \
	<< "Failed assertion triggered on line: " << __LINE__ << std::endl; \
	throw mist_aex; \
}
