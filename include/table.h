#pragma once

#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "state/nstate.h"

namespace regex
{
    std::string print( group );

    class schema
    {
        std::vector<char> m_names;

    public:
        using name_type = char;
        using size_type = std::vector<char>::size_type;
        using iterator = std::vector<char>::iterator;
        using const_iterator = std::vector<char>::const_iterator;
        using reference = std::vector<char>::reference;

        size_type push_back( name_type n );

        size_type find( name_type n ) const;

        reference operator[]( size_type pos );

        size_type size( ) const;

        iterator begin( );

        iterator end( );

        const_iterator begin( ) const;

        const_iterator end( ) const;
    };

    std::string print( const schema& sch, std::string::size_type size );

    class row
    {
        std::vector<group> m_elements;

    public:
        using value_type = group;
        using size_type = schema::size_type;
        using reference = value_type&;
        using iterator = std::vector<group>::iterator;
        using const_iterator = std::vector<group>::const_iterator;

        size_type push_back( group g );

        size_type size( ) const;

        void update( size_type col, group g );

        reference operator[]( size_type pos );

        iterator begin( );

        iterator end( );

        const_iterator begin( ) const;

        const_iterator end( ) const;
    };

    bool operator<( const row& lhs, const row& rhs );

    using rows = std::map<group, row>;

    class table
    {
        schema s_;
        rows r_;

    public:
        using size_type = schema::size_type;
        using name_type = schema::name_type;
        using reference = row&;
        using iterator = rows::iterator;
        using const_iterator = rows::const_iterator;
        using key_type = rows::key_type;

        void update( group index, name_type column, group element );

        iterator find( const key_type& key );

        auto size( ) const;

        schema get_schema( ) const;

        reference operator[]( size_type pos );

        iterator begin( );

        iterator end( );

        const_iterator begin( ) const;

        const_iterator end( ) const;
    };

    std::ostream& operator<<( std::ostream& os, const table& tb );
}