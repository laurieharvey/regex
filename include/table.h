#pragma once

#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "state/nstate.h"
#include "state/dstate.h"

namespace regex
{
    std::string print(group);
    std::string print(std::shared_ptr<dstate>);

    class schema
    {
        std::vector<char> m_names;

    public:
        using name_type = char;
        using size_type = std::vector<char>::size_type;
        using iterator = std::vector<char>::iterator;
        using const_iterator = std::vector<char>::const_iterator;
        using reference = std::vector<char>::reference;

        size_type push_back(name_type n);

        size_type find(name_type n) const;

        reference operator[](size_type pos);

        size_type size() const;

        iterator begin();

        iterator end();

        const_iterator begin() const;

        const_iterator end() const;
    };

    std::string print(const schema &sch, std::string::size_type size);

    template <typename T>
    class row
    {
        std::vector<T> m_elements;

    public:
        using value_type = T;
        using size_type = schema::size_type;
        using reference = value_type &;
        using const_reference = const value_type&;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        size_type push_back(const_reference g)
        {
            m_elements.push_back(g);
            return m_elements.size() - 1;
        }

        size_type size() const
        {
            return m_elements.size();
        }

        reference operator[](size_type col)
        {
            return m_elements[col];
        }

        iterator begin()
        {
            return std::begin(m_elements);
        }

        iterator end()
        {
            return std::end(m_elements);
        }

        const_iterator begin() const
        {
            return std::cbegin(m_elements);
        }

        const_iterator end() const
        {
            return std::cend(m_elements);
        }
    };

    template <typename T>
    class table
    {
        schema s_;
        std::map<T, row<T>> r_;

    public:
        using size_type = schema::size_type;
        using name_type = schema::name_type;
        using reference = row<T> &;
        using iterator = typename std::map<T, row<T>>::iterator;
        using const_iterator = typename std::map<T, row<T>>::const_iterator;
        using key_type = typename std::map<T, row<T>>::key_type;
        using value_type = typename std::map<T, row<T>>::value_type;

        T& element( key_type index, name_type column )
        {
            auto c = s_.find(column);

            if (c == s_.size())
            {
                c = s_.push_back(column);

                for (auto &row : r_)
                {
                    row.second.push_back( T( ) );
                }
            }

            if (r_.find(index) == std::cend(r_))
            {
                row<T> new_row;

                for (auto i = 0; i < s_.size(); ++i)
                {
                    new_row.push_back( T( ) );
                }

                r_.insert({index, new_row});
            }

            return r_.find(index)->second[ c ];
        }

        iterator find(const key_type &key)
        {
            return r_.find(key);
        }

        auto size() const
        {
            return std::pair<int, int>{s_.size(), r_.size()};
        }

        schema get_schema() const
        {
            return s_;
        }

        reference operator[](size_type pos);

        iterator begin()
        {
            return std::begin(r_);
        }

        iterator end()
        {
            return std::end(r_);
        }

        const_iterator begin() const
        {
            return std::cbegin(r_);
        }

        const_iterator end() const
        {
            return std::cend(r_);
        }
    };
}