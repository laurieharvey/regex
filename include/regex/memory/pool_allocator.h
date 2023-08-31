#ifndef REGEX_POOL_ALLOCATOR_H
#define REGEX_POOL_ALLOCATOR_H

#include <cstdlib>
#include <memory>

template <typename T> class pool_allocator
{
  public:
    using pointer = T *;
    using size_type = std::size_t;

    pool_allocator( size_type n )
    {
        _begin = ::operator new( n * sizeof( T ) );
        _end = _begin;
        _end_of_storage = _begin + n;
    }

    ~pool_allocator()
    {
        ::operator delete( _begin );
    }

    /// Storage and lifetime operations

    pointer allocate( std::allocator_traits<pool_allocator>::size_type n )
    {
        if( _end + n >= _end_of_storage )
            throw std::bad_alloc();

        return _end++;
    }

    void deallocate( std::allocator_traits<pool_allocator>::pointer p,
                     std::allocator_traits<pool_allocator>::size_type n )
    {
        return;
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

#endif
