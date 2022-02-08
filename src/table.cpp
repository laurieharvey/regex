#include "table.h"

#include <algorithm>
#include <numeric>
#include <sstream>

namespace regex
{
    std::string print( group gr )
    {
        std::stringstream ss;

        ss << "{ ";

        for ( const auto &s : gr )
        {
            ss << s.get() << " ";
        }

        ss << "}";

        return ss.str();
    }

    std::string print( std::shared_ptr<dstate> s )
    {
        std::stringstream ss;

        if ( s )
        {
            ss << "{ " << s.get() << " }";
        }
        else
        {
            ss << "{ }";
        }

        return ss.str();
    }

    std::string print( const schema &sch, std::string::size_type size )
    {
        std::string s;

        for ( const auto name : sch )
        {
            s += name == 0x01 ? 'e' : name;
            s += std::string( size - 1, ' ' );
        }

        return s;
    }

    schema::size_type schema::push_back( schema::name_type n )
    {
        m_names.push_back( n );

        return m_names.size() - 1;
    }

    schema::size_type schema::find( schema::name_type n ) const
    {
        auto col = std::find( std::cbegin( m_names ), std::cend( m_names ), n );

        if ( col == std::cend( m_names ) )
        {
            return m_names.size();
        }
        else
        {
            return std::distance( std::cbegin( m_names ), col );
        }
    }

    schema::reference schema::operator[]( schema::size_type pos )
    {
        return m_names[pos];
    }

    schema::size_type schema::size() const
    {
        return m_names.size();
    }

    schema::iterator schema::begin()
    {
        return std::begin( m_names );
    }

    schema::iterator schema::end()
    {
        return std::end( m_names );
    }

    schema::const_iterator schema::begin() const
    {
        return std::cbegin( m_names );
    }

    schema::const_iterator schema::end() const
    {
        return std::cend( m_names );
    }
} // namespace regex