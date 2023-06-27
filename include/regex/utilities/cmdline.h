#pragma once

#include <any>
#include <exception>
#include <filesystem>
#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace regex::cmd
{
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const std::string &msg );
        explicit exception( const char *msg );
    };

    class cmdline
    {
      public:
        explicit cmdline( const std::string &description );
        explicit cmdline( const cmdline & ) = delete;
        explicit cmdline( cmdline && ) = delete;

        friend std::ostream &operator<<( std::ostream &os, const cmdline & );

        enum class type { boolean, integral, floating, string };
        /*
         *  Add positional argument
         *  e.g. add_positional( "filename", type::string );
         */
        void add_positional( const std::string &var, type, const std::string &description = std::string() );
        /*
         *  Add optional argument
         *  e.g. add_optional( "-f", "filename", type::integral );
         */
        void add_optional( const std::string &arg, const std::string &var, type, const std::any &def,
                           const std::string &description = std::string(),
                           const std::vector<std::any> &options = std::vector<std::any>() );
        /*
         *  Add flag
         *  e.g. add_flag( "-v", "verbose" );
         */
        void add_flag( const std::string &arg, const std::string &var, const std::string &description = std::string() );
        /*
         *  Parse command line args
         *  Throw if incorrect
         */
        void parse( int argc, const char **argv );
        /*
         *  Fetch argument assigned to var
         *
         */
        template <typename T>
        T get_argument( const std::string &var )
        {
            return std::any_cast<T>( args_.find( var )->second );
        };

        bool get_flag( const std::string &flag )
        {
            return args_.contains( flag );
        };

      private:
        std::string name_, error_, description_;
        std::map<std::string, std::any> args_;
        std::vector<std::tuple<std::string, type, std::string>> positionals_;
        std::map<std::string, std::tuple<std::string, type, std::string, std::vector<std::any>>> optionals_;
        std::map<std::string, std::tuple<std::string, std::string>> flags_;
    };

    std::ostream &operator<<( std::ostream &os, const cmdline & );
} // namespace regex::cmd