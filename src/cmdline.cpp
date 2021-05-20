#include "cmdline.h"

namespace regex
{
    namespace cmd
    {
        static std::any parse_string( const std::string& val )
        {
            return std::any( val );
        }

        void cmdline::add_positional( const std::string& var,
                                      const type t,
                                      const std::string& description )
        {
            positionals_.push_back( { var, t } );
        }

        void cmdline::add_optional( const std::string& arg,
                                    const std::string& var,
                                    const type t,
                                    const std::string& description )
        {
            optionals_.insert( { arg, { var, t } } );
        }

        void cmdline::add_flag( const std::string& arg, 
                                const std::string& var, 
                                const std::string& description )
        {
            args_[ var ] = std::any( false );
            flags_.insert( { arg, var } );
        }

        void cmdline::parse( int argc, char **argv )
        {
            name_ = argv[ 0 ];

            auto positional = std::cbegin( positionals_ );

            for( int i = 1; i < argc; ++i )
            {
                const auto arg = std::string( argv[ i ] );

                if( arg.starts_with( "-" ) )
                {
                    const auto flag_iter = flags_.find( arg );

                    if( flag_iter == std::cend( flags_ ) )
                    {
                        if( i + 1 >= argc )
                        {
                            throw exception( );
                        }
                        else
                        {
                            const auto val = std::string( argv[ i + 1 ] );

                            if( val.starts_with( "-" ) )
                            {
                                throw exception( );
                            }
                            else
                            {
                                const auto opt_iter = optionals_.find( arg );

                                if( opt_iter == std::cend( optionals_ ) )
                                {
                                    throw exception( );
                                }
                                else
                                {
                                    args_[ opt_iter->second.first ] = parse_string( val );
                                }
                            }
                        }
                    }
                    else
                    {
                        args_[ flag_iter->second ] = std::any( true );
                    }
                }
                else
                {
                    if( positional != std::cend( positionals_ ) )
                    {
                        args_[ positional->first ] = arg;
                        ++positional;
                    }
                    else
                    {
                        throw exception( );
                    }
                }
            }
        }

        std::ostream& operator<<( std::ostream& os, const cmdline& c )
        {
            os << c.name_ << ": " << c.error_ << '\n';
            os << "usage: \n";
            
        }
    }
}