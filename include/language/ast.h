#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <ostream>
#include <vector>

#include "alphabet.h"

namespace regex
{
    namespace language
    {
        enum class type
        {
            literal,
            any,
            parenthesis,
            kleene,
            zero_or_one,
            one_or_more,
            alternation,
            concatenation
        };

        class token
        {
          public:
            using ostream = std::basic_ostream<character_type, std::char_traits<character_type>>;

            virtual void to_string( ostream &os ) const = 0;

            virtual void walk( std::function<void( const token & )> callback ) const = 0;

            virtual character_type get_character() const = 0;

            virtual type get_type() const = 0;

            virtual ~token() = default;
        };

        class literal : public token
        {
            character_type value_;

          public:
            /*
             *  Literal {'a','b'...'z'}
             */
            explicit literal( character_type value );
            explicit literal( const literal & ) = delete;
            explicit literal( literal && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class any : public token
        {
          public:
            /*
             * Any '.'
             */
            explicit any();
            explicit any( const any & ) = delete;
            explicit any( any && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class parenthesis : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            /*
             * Parenthesis '()'
             * (expression)
             */
            explicit parenthesis( std::unique_ptr<token> lhs );
            explicit parenthesis( const parenthesis & ) = delete;
            explicit parenthesis( parenthesis && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class kleene : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            /*
             * Kleene '*'
             * (expression)*
             */
            explicit kleene( std::unique_ptr<token> lhs );
            explicit kleene( const kleene & ) = delete;
            explicit kleene( kleene && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class zero_or_one : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            /*
             * Zero or one '?'
             * (expression)?
             */
            explicit zero_or_one( std::unique_ptr<token> lhs );
            explicit zero_or_one( const zero_or_one & ) = delete;
            explicit zero_or_one( zero_or_one && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class one_or_more : public token
        {
            std::unique_ptr<token> exp_;

          public:
            /*
             * One or more '+'
             * (expression)+
             */
            explicit one_or_more( std::unique_ptr<token> exp );
            explicit one_or_more( const one_or_more & ) = delete;
            explicit one_or_more( one_or_more && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class alternation : public token
        {
            std::unique_ptr<token> lhs_, rhs_;

          public:
            /*
             * Alternation '|'
             * (expression 1)|(expression 2)
             */
            explicit alternation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs );
            explicit alternation( const alternation & ) = delete;
            explicit alternation( alternation && ) = delete;

            void to_string( typename token::ostream & os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };

        class concatenation : public token
        {
            std::unique_ptr<token> lhs_, rhs_;

          public:
            /*
             * Concatenation '•'
             * (expression 1)•(expression 2)
             */
            explicit concatenation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs );
            explicit concatenation( const concatenation & ) = delete;
            explicit concatenation( concatenation && ) = delete;

            void to_string( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_character() const override;

            type get_type() const override;
        };
    } // namespace language
} // namespace regex