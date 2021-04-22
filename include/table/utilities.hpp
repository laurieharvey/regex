#include <ostream>
#include <ios>
#include <vector>
#include <numeric>

#include "table.h"

namespace regex
{
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const table<T> &tb)
    {
        os << std::left;

        std::vector<std::string::size_type> widths = std::vector<std::string::size_type>(tb.size().first + 2, 0);

        std::vector<std::vector<std::string>> output;

        for (const auto &row : tb)
        {
            std::vector<std::string> output_row;

            output_row.push_back(print(row.first) + " ");
            output_row.push_back("|");

            for (const auto &element : row.second)
            {
                output_row.push_back(" " + print(element));
            }

            output.push_back(output_row);
        }

        for (const auto &r : output)
        {
            for (std::vector<std::string>::size_type i = 0; i < r.size(); ++i)
            {
                widths[i] = std::max(widths[i], r[i].size());
            }
        }

        const auto total_width = std::accumulate(std::cbegin(widths), std::cend(widths), 0);

        auto print_break = [ &os, total_width ]( )
        {
            os << std::setw( total_width ) << std::setfill( '-' ) << '-' << '\n' << std::setfill( ' ' );
        };

        auto print_schema = [ &os, &tb, &widths ]( )
        {
            os << std::setw( widths[ 0 ] ) << "";
            os << '|';

            int i = 2;
            
            for( const auto& name : tb.get_schema( ) )
            {
                os << ' ' << std::setw( widths[ i++ ] ) << ( name == 0x01 ? 'e' : name );
            }

            os << '\n';
        };

        auto print_row = [ &os, &widths ]( const std::vector<std::string>& row )
        {   
            for( auto i = 0; i < row.size( ); ++i )
            {
                os << std::setw( widths[ i ] ) << row[ i ];
            }
            
            os << '\n';
        };

        print_schema( );
        print_break( );

        for( const auto& row : output )
        {
            print_row( row );
        }

        return os;
    }

}