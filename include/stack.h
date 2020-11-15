#pragma once

#include <stack>

template <class _Ty>
class stack : public std::stack<_Ty>
{
public:
    _Ty pop( )
    {
        _Ty t = std::move( std::stack<_Ty>::top( ) );
        std::stack<_Ty>::pop( );
        return t;
    }
};