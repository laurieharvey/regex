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
            character,
            parenthesis,
            kleene,
            zero_or_one,
            one_or_more,
            alternation,
            concatenation,
            any
        };

        class token
        {
          public:
            using ostream = std::basic_ostream<character_type, std::char_traits<character_type>>;

            virtual void print( ostream &os ) const = 0;

            virtual void walk( std::function<void( const token & )> callback ) const = 0;

            virtual character_type get_token() const = 0;

            virtual type get_type() const = 0;

            virtual ~token();
        };

        class character : public token
        {
            character_type value_;

          public:
            explicit character( character_type value );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class parenthesis : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            explicit parenthesis( std::unique_ptr<token> lhs );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class kleene : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            explicit kleene( std::unique_ptr<token> lhs );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class zero_or_one : public token
        {
            std::unique_ptr<token> lhs_;

          public:
            explicit zero_or_one( std::unique_ptr<token> lhs );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class one_or_more : public token
        {
            std::unique_ptr<token> exp_;

          public:
            explicit one_or_more( std::unique_ptr<token> exp );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class alternation : public token
        {
            std::unique_ptr<token> lhs_, rhs_;

          public:
            explicit alternation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class concatenation : public token
        {
            std::unique_ptr<token> lhs_, rhs_;

          public:
            explicit concatenation( std::unique_ptr<token> lhs, std::unique_ptr<token> rhs );

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };

        class any : public token
        {
          public:
            explicit any();

            void print( typename token::ostream &os ) const override;

            void walk( std::function<void( const token & )> callback ) const override;

            character_type get_token() const override;

            type get_type() const override;
        };
    } // namespace language
} // namespace regex