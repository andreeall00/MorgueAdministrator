#pragma once
#include <string>

class RepositoryException: public std::exception
{
private:
	std::string message;

public:
	RepositoryException(std::string message) : message{ message } {}
	const char* what() const noexcept override { return this->message.c_str(); }
};


class ValidatorException : public std::exception
{
private:
	std::string message;

public:
	ValidatorException(std::string message) : message{ message } {}
	const char* what() const noexcept override { return this->message.c_str(); }
};

