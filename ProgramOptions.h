#ifndef __PROGRAM_OPTIONS_H
#define __PROGRAM_OPTIONS_H

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include "ISolutionMethod.h"

class ProgramOptions
{
public:
    ProgramOptions(int argc, char* argv[], const std::vector<ISolutionMethod*>& solvers) : 
        m_RequiredArgs("Required Arguments"),
        m_OptionalArgs("Optional Arguments"),
        m_rows(0),
        m_columns(0),
        m_p1Strategy(""),
        m_p2Strategy(""),
        m_Method("")
    {
        std::string methodHelpMessage("Solution method, may be one of the following:\n");

        for (std::vector<ISolutionMethod*>::const_iterator it = solvers.begin(); it != solvers.end(); ++it)
            methodHelpMessage += "\n" + (*it)->Name();

        m_RequiredArgs.add_options()
            ("rows,r", boost::program_options::value<int>(), "Number of rows in the game")
            ("columns,c", boost::program_options::value<int>(), "Number of columns in the game")
            ("method,m", boost::program_options::value<std::string>(), methodHelpMessage.c_str());
        m_OptionalArgs.add_options()
            ("help,h", "Display program options")
            ("player1,1", boost::program_options::value<std::string>(), "Player 1 strategy values, comma delimited")
            ("player2,2", boost::program_options::value<std::string>(), "Player 2 strategy values, comma delimited");

        try
        {
            ParseCommandLine(argc, argv);
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << std::endl << std::endl;
            std::cout << m_RequiredArgs << m_OptionalArgs << std::endl;
            return;
        }        
    }

    int GetRows() const { return m_rows; }
    int GetColumns() const { return m_columns; }
    std::string GetP1Strategy() const { return m_p1Strategy; }
    std::string GetP2Strategy() const { return m_p2Strategy; }
    std::string GetMethod() const {return m_Method; }

private:
    boost::program_options::options_description m_RequiredArgs;
    boost::program_options::options_description m_OptionalArgs;
    int m_rows;
    int m_columns;
    std::string m_p1Strategy;
    std::string m_p2Strategy;
    std::string m_Method;

    void ParseCommandLine(int argc, char* argv[])
    {
        boost::program_options::variables_map vm;
        boost::program_options::options_description cmdLineOptions;
        cmdLineOptions.add(m_RequiredArgs).add(m_OptionalArgs);
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdLineOptions).run(), vm);
        boost::program_options::notify(vm);    

        if (vm.count("help")) 
        {
            std::cout << m_RequiredArgs << m_OptionalArgs << std::endl;
            return;
        }

        if (vm.count("rows")) 
            m_rows = vm["rows"].as<int>();
        else
            throw std::runtime_error("The number of rows must be specified.");

        if (vm.count("columns")) 
            m_columns = vm["columns"].as<int>();
        else
            throw std::runtime_error("The number of columns must be specified.");

        if (vm.count("method"))
            m_Method = vm["method"].as<std::string>();
        else
            throw std::runtime_error("A solution method must be specified.");
        
        if (vm.count("player1") && vm.count("player2"))
            throw std::runtime_error("Strategies for both player 1 and player 2 cannot be specified.");

        if (vm.count("player1"))
            m_p1Strategy = vm["player1"].as<std::string>();

        if (vm.count("player2"))
            m_p2Strategy = vm["player2"].as<std::string>();
    }
};

#endif //__PROGRAM_OPTIONS_H
