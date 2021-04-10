#include "language/ast.h"

namespace regex
{
    namespace language
    {
        token::~token( )
        {
        }

        character::character( character_type value ) : value_( value )
        {
        }
        void character::print( typename token::ostream &os ) const
        {
            os << get_token( );
        }
        void character::walk( std::function<void( const token & )> callback ) const
        {
            callback( *this );
        }
        character_type character::get_token( ) const
        {
            return value_;
        }
        type character::get_type( ) const
        {
            return type::character;
        }
        parenthesis::parenthesis( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void parenthesis::print( typename token::ostream &os ) const
        {
            os << '(';
            lhs_->print( os );
            os << ")";
        }
        void parenthesis::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type parenthesis::get_token( ) const
        {
            return 0;
        }
        type parenthesis::get_type( ) const
        {
            return type::parenthesis;
        }
        kleene::kleene( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void kleene::print( typename token::ostream &os ) const
        {
            lhs_->print( os );
            os << '*';
        }
        void kleene::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type kleene::get_token( ) const
        {
            return '*';
        }
        type kleene::get_type( ) const
        {
            return type::kleene;
        }
        zero_or_one::zero_or_one( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void zero_or_one::print( typename token::ostream &os ) const
        {
            lhs_->print( os );
            os << '?';
        }
        void zero_or_one::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type zero_or_one::get_token( ) const
        {
            return '?';
        }
        type zero_or_one::get_type( ) const
        {
            return type::zero_or_one;
        }
        one_or_more::one_or_more( std::unique_ptr<token> exp ) : exp_( std::move( exp ) )
        {
        }
        void one_or_more::print( typename token::ostream &os ) const
        {
            exp_->print( os );
            os << get_token( );
        }
        void one_or_more::walk( std::function<void( const token & )> callback ) const
        {
            exp_->walk( callback );
            callback( *this );
        }
        character_type one_or_more::get_token( ) const
        {
            return '+';
        }
        type one_or_more::get_type( ) const
        {
            return type::one_or_more;
        }

        alternation::alternation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs ) : lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
        {
        }
        void alternation::print( typename token::ostream &os ) const
        {
            lhs_->print( os );
            os << '|';
            rhs_->print( os );
        }
        void alternation::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            rhs_->walk( callback );
            callback( *this );
        }
        character_type alternation::get_token( ) const
        {
            return '|';
        }
        type alternation::get_type( ) const
        {
            return type::alternation;
        }
        concatenation::concatenation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs ) : lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
        {
        }
        void concatenation::print( typename token::ostream &os ) const
        {
            lhs_->print( os );
            rhs_->print( os );
        }
        void concatenation::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            rhs_->walk( callback );
            callback( *this );
        }
        character_type concatenation::get_token( ) const
        {
            return '-';
        }
        type concatenation::get_type( ) const
        {
            return type::concatenation;
        }

        any::any( )
        {
        }
        void any::print( typename token::ostream &os ) const
        {
            os << '.';
        }
        void any::walk( std::function<void( const token & )> callback ) const
        {
            callback( *this );
        }
        character_type any::get_token( ) const
        {
            return '.';
        }
        type any::get_type( ) const
        {
            return type::any;
        }
    }
}