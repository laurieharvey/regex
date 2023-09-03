#pragma once

#include <cstdlib>
#include <memory>

template <typename T> class pool_allocator
{
  public:
    using pointer = T *;
    using size_type = std::size_t;
    using value_type = T;
    using is_always_equal = std::false_type;

    pool_allocator( pool_allocator &&rhs ) noexcept
    {
        std::swap( _begin, rhs._begin );
        std::swap( _end, rhs._end );
        std::swap( _end_of_storage, rhs._end_of_storage );
    }

    pool_allocator( size_type n )
    {
        _begin = static_cast<pointer>( ::operator new( n * sizeof( T ) ) );
        _end = _begin;
        _end_of_storage = _begin + n;
    }

    ~pool_allocator()
    {
        if( _begin )
            ::operator delete( _begin );
    }

    /// Storage and lifetime operations

    pointer allocate( size_type n )
    {
        if( _end + n > _end_of_storage )
            throw std::bad_alloc();

        return _end++;
    }

    void deallocate( pointer p, size_type n )
    {
    }

    size_type max_size() const
    {
        return sizeof( T );
    }

    /// Relationship between instances

    bool operator==( const pool_allocator &rhs )
    {
        return this == &rhs;
    }

    bool operator!=( const pool_allocator &rhs )
    {
        return !( *this == rhs );
    }

    /// Influence on container operations
  private:
    pointer _begin = nullptr;
    pointer _end = nullptr;
    pointer _end_of_storage = nullptr;
};
