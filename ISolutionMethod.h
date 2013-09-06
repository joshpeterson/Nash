#ifndef __ISOLUTION_METHOD_H
#define __ISOLUTION_METHOD_H

#include <iosfwd>
class ProgramOptions;

class ISolutionMethod
{
public:
    virtual void Run(const ProgramOptions& options, std::ostream& out, int argc, char* argv[]) = 0;
    virtual std::string Name() const = 0;
};

#endif //__ISOLUTION_METHOD_H