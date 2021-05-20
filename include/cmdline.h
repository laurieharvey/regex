#pragma once

#include <string>
#include <vector>
#include <any>
#include <utility>
#include <map>
#include <initializer_list>
#include <ostream>

namespace regex::cmd
{
    class exception
    {

    };

    class cmdline
    {
    public:
        explicit cmdline( );
        explicit cmdline( const cmdline& ) = delete;
        explicit cmdline( cmdline&& ) = delete;

        friend std::ostream& operator<<( std::ostream& os, const cmdline& );

        enum class type { boolean, integral, floating, string };
        /*
         *  Add positional argument
         *  e.g. add_positional( "filename", type::string );
         */
        void add_positional( const std::string& var,
                             const type,
                             const std::string& description = std::string( ) );
        /*
         *  Add optional argument
         *  e.g. add_optional( "-f", "filename", type::integral );
         */
        void add_optional( const std::string& arg,
                           const std::string& var,
                           const type,
                           const std::string& description = std::string( ) );
        /*
         *  Add flag
         *  e.g. add_flag( "-v", "verbose" );
         */
        void add_flag( const std::string& arg, 
                       const std::string& var, 
                       const std::string& description = std::string( ) );
        /*
         *  Parse command line args 
         *  Throw if incorrect
         */
        void parse( int argc, char **argv );
        /*
         *  Fetch argument assigned to var
         */
        template<typename T>
        T get_argument( const std::string& var )
        {
            return std::any_cast<T>( args_.find( var )->second );
        };
    private:
        std::string name_, error_;
        std::map<std::string, std::any> args_;
        std::vector<std::pair<std::string, type>> positionals_;
        std::map<std::string, std::pair<std::string, type>> optionals_;
        std::map<std::string, std::string> flags_;
    };

    std::ostream& operator<<( std::ostream& os, const cmdline& );
}