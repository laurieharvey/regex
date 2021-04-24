#pragma once

#include <string>
#include <vector>
#include <map>
#include <initializer_list>

namespace regex::cmd
{
    class parser
    {
        std::map<std::string, std::string> args_;
        std::vector<std::string> positionals_;
        std::map<std::string, std::string> optionals_;
        std::map<std::string, std::string> flags_;
    public:
        explicit parser( );
        explicit parser( const parser& ) = delete;
        explicit parser( parser&& ) = delete;
        /*
         *  Add positional argument
         *  e.g. add_positional( "filename" );
         */
        void add_positional( const std::string& var, 
                             const std::string& description = std::string( ) );
        /*
         *  Add optional argument
         *  e.g. add_optional( "-f", "filename" );
         */
        void add_optional( const std::string& arg, 
                           const std::string& var, 
                           const std::string& description = std::string( ) ;
        /*
         *  Add flag
         *  e.g. add_flag( "-v", "verbose" );
         */
        void add_flag( const std::string& arg, 
                       const std::string& var, 
                       const std::string& description = std::string( ) );

        template<typename T>
        T get_argument( const std::string& var );

        template<>
        int get_argument( const std::string& var )
        {
            
        }
    };
}