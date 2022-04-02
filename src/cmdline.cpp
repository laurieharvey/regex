#include <filesystem>
#include <sstream>

#include "regex/utilities/cmdline.h"

namespace regex
{
    namespace cmd
    {
        exception::exception( const std::string &msg ) : std::runtime_error( msg )
        {
        }

        exception::exception( const char *msg ) : std::runtime_error( msg )
        {
        }

        static std::any parse_string( const std::string &val )
        {
            return std::any( val );
        }

        static std::string print_cmdline( const cmdline &args )
        {
            std::stringstream ss;
            ss << args;
            return ss.str();
        }

        cmdline::cmdline( const std::string &description ) : name_(), description_( description )
        {
        }

        void cmdline::add_positional( const std::string &var, const type t, const std::string &description )
        {
            positionals_.push_back( std::make_tuple( var, t, description ) );
        }

        void cmdline::add_optional( const std::string &arg, const std::string &var, const type t, const std::any &def,
                                    const std::string &description, const std::vector<std::any> &options )
        {
            args_[var] = def;
            optionals_.insert( { arg, std::make_tuple( var, t, description, options ) } );
        }

        void cmdline::add_flag( const std::string &arg, const std::string &var, const std::string &description )
        {
            args_[var] = std::any( false );
            flags_.insert( { arg, std::make_tuple( var, description ) } );
        }

        void cmdline::parse( int argc, const char **argv )
        {
            name_ = std::filesystem::path( argv[0] ).filename();

            auto positional = std::cbegin( positionals_ );

            for ( int i = 1; i < argc; ++i )
            {
                const auto arg = std::string( argv[i] );

                if ( arg.starts_with( "-" ) )
                {
                    const auto flag_iter = flags_.find( arg );

                    if ( flag_iter == std::cend( flags_ ) )
                    {
                        if ( i + 1 >= argc )
                        {
                            throw exception( "Expected a value to be supplied to an optional\n" +
                                             print_cmdline( *this ) );
                        }
                        else
                        {
                            const auto val = std::string( argv[i + 1] );

                            if ( val.starts_with( "-" ) )
                            {
                                throw exception( "Expected a value to be supplied to an optional\n" +
                                                 print_cmdline( *this ) );
                            }
                            else
                            {
                                const auto opt_iter = optionals_.find( arg );

                                if ( opt_iter == std::cend( optionals_ ) )
                                {
                                    throw exception( std::string( "Unknown optional " ) + arg +
                                                     std::string( " provided" ) + "\n" +
                                                     print_cmdline( *this ) ); // TODO use std::format when available
                                }
                                else
                                {
                                    args_[std::get<0>( opt_iter->second )] = parse_string( val );
                                }
                            }
                        }

                        ++i;
                    }
                    else
                    {
                        args_[std::get<0>( flag_iter->second )] = std::any( true );
                    }
                }
                else
                {
                    if ( positional != std::cend( positionals_ ) )
                    {
                        args_[std::get<0>( *positional )] = arg;
                        ++positional;
                    }
                    else
                    {
                        throw exception( "Too many positionals provided\n" + print_cmdline( *this ) );
                    }
                }
            }

            if ( positional != std::cend( positionals_ ) )
            {
                throw exception( "Not all positional arguments supplied\n" + print_cmdline( *this ) );
            }
        }

        static std::ostream &operator<<(
            std::ostream &os, const std::vector<std::tuple<std::string, cmdline::type, std::string>> &positionals )
        {
            for ( const auto &positional : positionals )
            {
                os << std::get<0>( positional ) << ' ';
            }
            return os;
        }

        static std::ostream &operator<<( std::ostream &os, const std::vector<std::any> &options )
        {
            for ( const auto &option : options )
            {
                os << std::any_cast<const char *>( option ) << ' ';
            }
            return os;
        }

        std::ostream &operator<<( std::ostream &os, const cmdline &c )
        {
            os << std::left;
            os << "Usage: " << c.name_ << " [OPTIONS|FLAGS] " << c.positionals_ << '\n';
            os << c.description_ << '\n';
            os << '\n';
            os << "Positionals\n";
            for ( const auto &positional : c.positionals_ )
            {
                os << std::setw( 10 ) << ' ' << std::setw( 15 ) << std::get<0>( positional )
                   << std::get<2>( positional ) << '\n';
            }
            os << "Optionals\n";
            for ( const auto &optional : c.optionals_ )
            {
                os << std::setw( 10 ) << optional.first << std::setw( 15 ) << std::get<0>( optional.second )
                   << std::get<2>( optional.second ) << " { " << std::get<3>( optional.second ) << '}' << '\n';
            }
            os << "Flags\n";
            for ( const auto &flag : c.flags_ )
            {
                os << std::setw( 10 ) << flag.first << std::setw( 15 ) << std::get<0>( flag.second )
                   << std::get<1>( flag.second ) << '\n';
            }
            return os;
        }
    } // namespace cmd
} // namespace regex