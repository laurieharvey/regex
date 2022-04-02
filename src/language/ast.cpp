#include "regex/language/ast.h"

namespace regex
{
    namespace language
    {
        literal::literal( character_type value ) : value_( value )
        {
        }
        void literal::to_string( typename token::ostream &os ) const
        {
            os << get_character();
        }
        void literal::walk( std::function<void( const token & )> callback ) const
        {
            callback( *this );
        }
        character_type literal::get_character() const
        {
            return value_;
        }
        type literal::get_type() const
        {
            return type::literal;
        }
        parenthesis::parenthesis( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void parenthesis::to_string( typename token::ostream &os ) const
        {
            os << '(';
            lhs_->to_string( os );
            os << ")";
        }
        void parenthesis::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type parenthesis::get_character() const
        {
            return 0;
        }
        type parenthesis::get_type() const
        {
            return type::parenthesis;
        }
        kleene::kleene( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void kleene::to_string( typename token::ostream &os ) const
        {
            lhs_->to_string( os );
            os << '*';
        }
        void kleene::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type kleene::get_character() const
        {
            return '*';
        }
        type kleene::get_type() const
        {
            return type::kleene;
        }
        zero_or_one::zero_or_one( std::unique_ptr<token> lhs ) : lhs_( std::move( lhs ) )
        {
        }
        void zero_or_one::to_string( typename token::ostream &os ) const
        {
            lhs_->to_string( os );
            os << '?';
        }
        void zero_or_one::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            callback( *this );
        }
        character_type zero_or_one::get_character() const
        {
            return '?';
        }
        type zero_or_one::get_type() const
        {
            return type::zero_or_one;
        }
        one_or_more::one_or_more( std::unique_ptr<token> exp ) : exp_( std::move( exp ) )
        {
        }
        void one_or_more::to_string( typename token::ostream &os ) const
        {
            exp_->to_string( os );
            os << get_character();
        }
        void one_or_more::walk( std::function<void( const token & )> callback ) const
        {
            exp_->walk( callback );
            callback( *this );
        }
        character_type one_or_more::get_character() const
        {
            return '+';
        }
        type one_or_more::get_type() const
        {
            return type::one_or_more;
        }

        alternation::alternation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs )
            : lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
        {
        }
        void alternation::to_string( typename token::ostream &os ) const
        {
            lhs_->to_string( os );
            os << '|';
            rhs_->to_string( os );
        }
        void alternation::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            rhs_->walk( callback );
            callback( *this );
        }
        character_type alternation::get_character() const
        {
            return '|';
        }
        type alternation::get_type() const
        {
            return type::alternation;
        }
        concatenation::concatenation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs )
            : lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
        {
        }
        void concatenation::to_string( typename token::ostream &os ) const
        {
            lhs_->to_string( os );
            rhs_->to_string( os );
        }
        void concatenation::walk( std::function<void( const token & )> callback ) const
        {
            lhs_->walk( callback );
            rhs_->walk( callback );
            callback( *this );
        }
        character_type concatenation::get_character() const
        {
            return '-';
        }
        type concatenation::get_type() const
        {
            return type::concatenation;
        }

        any::any()
        {
        }
        void any::to_string( typename token::ostream &os ) const
        {
            os << '.';
        }
        void any::walk( std::function<void( const token & )> callback ) const
        {
            callback( *this );
        }
        character_type any::get_character() const
        {
            return '.';
        }
        type any::get_type() const
        {
            return type::any;
        }
    } // namespace language
} // namespace regex