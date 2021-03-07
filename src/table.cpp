#include <sstream>
#include <numeric>
#include <algorithm>
#include "table.h"

namespace regex
{
    std::string print(group gr)
    {
        std::stringstream ss;

        ss << "{ ";

        for (const auto &s : gr)
        {
            ss << s.get() << " ";
        }

        ss << "}";

        return ss.str();
    }

    std::string print(const schema &sch, std::string::size_type size)
    {
        std::string s;

        for (const auto name : sch)
        {
            s += name == 0x01 ? 'e' : name;
            s += std::string(size - 1, ' ');
        }

        return s;
    }

    schema::size_type schema::push_back(schema::name_type n)
    {
        m_names.push_back(n);

        return m_names.size() - 1;
    }

    schema::size_type schema::find(schema::name_type n) const
    {
        auto col = std::find(std::cbegin(m_names), std::cend(m_names), n);

        if (col == std::cend(m_names))
        {
            return m_names.size();
        }
        else
        {
            return std::distance(std::cbegin(m_names), col);
        }
    }

    schema::reference schema::operator[](schema::size_type pos)
    {
        return m_names[pos];
    }

    schema::size_type schema::size() const
    {
        return m_names.size();
    }

    schema::iterator schema::begin()
    {
        return std::begin(m_names);
    }

    schema::iterator schema::end()
    {
        return std::end(m_names);
    }

    schema::const_iterator schema::begin() const
    {
        return std::cbegin(m_names);
    }

    schema::const_iterator schema::end() const
    {
        return std::cend(m_names);
    }

    row::size_type row::push_back(group g)
    {
        m_elements.push_back(g);

        return m_elements.size() - 1;
    }

    row::size_type row::size() const
    {
        return m_elements.size();
    }

    void row::update(schema::size_type col, group g)
    {
        m_elements[col] = g;
    }

    row::reference row::operator[](size_type col)
    {
        return m_elements[col];
    }

    row::iterator row::begin()
    {
        return std::begin(m_elements);
    }

    row::iterator row::end()
    {
        return std::end(m_elements);
    }

    row::const_iterator row::begin() const
    {
        return std::cbegin(m_elements);
    }

    row::const_iterator row::end() const
    {
        return std::cend(m_elements);
    }

    void table::update(group index, table::name_type column, group element)
    {
        auto c = s_.find(column);

        if (c == s_.size())
        {
            c = s_.push_back(column);

            for (auto &row : r_)
            {
                row.second.push_back({});
            }
        }

        if (r_.find(index) == std::cend(r_))
        {
            row new_row;

            for (auto i = 0; i < s_.size(); ++i)
            {
                new_row.push_back({});
            }

            r_.insert({index, new_row});
        }

        r_.find(index)->second.update(c, element);
    }

    table::iterator table::find(const key_type &key)
    {
        return r_.find(key);
    }

    auto table::size() const
    {
        return std::pair<int, int>{s_.size(), r_.size()};
    }

    schema table::get_schema() const
    {
        return s_;
    }

    table::iterator table::begin()
    {
        return std::begin(r_);
    }

    table::iterator table::end()
    {
        return std::end(r_);
    }

    table::const_iterator table::begin() const
    {
        return std::cbegin(r_);
    }

    table::const_iterator table::end() const
    {
        return std::cend(r_);
    }

    enum class padding_flags
    {
        white_space, repeat
    };

    static std::string pad(std::string s, std::string::size_type l, padding_flags f)
    {
        std::string trimmed = s.substr(0, l);

        if( trimmed.size() == l )
        {
            return trimmed;
        }
        else if( f == padding_flags::white_space )
        {
            return trimmed + std::string(l - trimmed.size(), ' ');
        }
        else
        {
            return trimmed + std::string(l - trimmed.size(), trimmed.front());   
        }        
    }

    std::ostream &operator<<(std::ostream &os, const table &tb)
    {
        std::vector<std::string::size_type> boxes = std::vector<std::string::size_type>(tb.size().first + 2, 0);

        std::vector<std::vector<std::string>> output;

        std::vector<std::string> s;

        s.push_back("");
        s.push_back("|");

        for(const auto name: tb.get_schema())
        {
            s.push_back(" " + std::string(1, name == 0x01 ? 'e' : name));
        }

        for (const auto &r : tb)
        {
            std::vector<std::string> output_row;

            output_row.push_back(print(r.first) + " ");
            output_row.push_back("|");

            for (const auto &element : r.second)
            {
                output_row.push_back(" " + print(element));
            }

            output.push_back(output_row);
        }

        for (const auto &r : output)
        {
            for( std::vector<std::string>::size_type i = 0; i < r.size(); ++i)
            {
                boxes[i] = std::max(boxes[i], r[i].size());
            }
        }

        os << pad( s[0], boxes[0], padding_flags::white_space );
        os << pad( s[1], boxes[1], padding_flags::repeat );

        for( std::vector<std::string>::size_type i = 2; i < s.size(); ++i)
        {
            os << pad( s[i], boxes[i], padding_flags::white_space );
        }
        
        os << std::endl;
        os << std::string(std::accumulate(std::cbegin(boxes), std::cend(boxes), 0), '-') << std::endl;

        for (const auto &r : output)
        {
            os << pad( r[0], boxes[0], padding_flags::white_space );
            os << pad( r[1], boxes[1], padding_flags::repeat );

            for( std::vector<std::string>::size_type i = 2; i < r.size(); ++i)
            {
                os << pad( r[i], boxes[i], padding_flags::white_space );
            }

            os << std::endl;
        }

        return os;
    }
}