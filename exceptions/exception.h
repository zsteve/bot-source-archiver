
#include <exception>

#ifndef EXCEPTION_H
#define EXCEPTION_H

class FileNonExistentException : public std::exception
{
};


class FormatInvalidException : public std::exception
{
};

class FileEmptyException : public std::exception
{
};

#endif
