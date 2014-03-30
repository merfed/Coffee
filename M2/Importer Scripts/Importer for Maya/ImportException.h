#ifndef IMPORT_EXCEPTION_H
#define IMPORT_EXCEPTION_H

#include <string>

class ImportException
{
private:
	std::string msg;

public:
	ImportException(const std::string& message)
	{
		msg = message;
	}

	const std::string& getMessage() const
	{
		return msg;
	}
};

#endif
