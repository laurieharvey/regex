#pragma once

#include <memory>
#include <ostream>
#include <functional>

namespace ast
{
	enum class type { character, parenthesis, kleene, zero_or_one, alternation, concatenation };

	template<typename CharT>
	class token
	{
	public:
		using ostream = std::basic_ostream<CharT, std::char_traits<CharT>>;

		virtual void print( ostream& os ) const = 0;

		virtual void walk( std::function<void( const ast::token<CharT>& )> callback ) const = 0;

		virtual CharT get_token( ) const = 0;

		virtual type get_type( ) const = 0;
	};

	template<typename CharT>
	class character : public token<CharT>
	{
		CharT value_;
	public:
		explicit character( CharT value )
			: value_( value )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			os << get_token( );
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return value_;
		}
		type get_type( ) const override
		{
			return type::character;
		}
	};

	template<typename CharT>
	class parenthesis : public token<CharT>
	{
		std::unique_ptr<token<CharT>> lhs_;
	public:
		explicit parenthesis( std::unique_ptr<token<CharT>> lhs )
			: lhs_( std::move( lhs ) )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			os << '('; lhs_->print( os ); os << ")";
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			lhs_->walk( callback );
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return 0;
		}
		type get_type( ) const override
		{
			return type::parenthesis;
		}
	};

	template<typename CharT>
	class kleene : public token<CharT>
	{
		std::unique_ptr<token<CharT>> lhs_;
	public:
		explicit kleene( std::unique_ptr<token<CharT>> lhs )
			: lhs_( std::move( lhs ) )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			lhs_->print( os ); os << '*';
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			lhs_->walk( callback );
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return '*';
		}
		type get_type( ) const override
		{
			return type::kleene;
		}
	};

	template<typename CharT>
	class zero_or_one : public token<CharT>
	{
		std::unique_ptr<token<CharT>> lhs_;
	public:
		explicit zero_or_one( std::unique_ptr<token<CharT>> lhs )
			: lhs_( std::move( lhs ) )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			lhs_->print( os ); os << '?';
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			lhs_->walk( callback );
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return '?';
		}
		type get_type( ) const override
		{
			return type::zero_or_one;
		}
	};

	template<typename CharT>
	class alternation : public token<CharT>
	{
		std::unique_ptr<token<CharT>> lhs_, rhs_;
	public:
		explicit alternation( std::unique_ptr<token<CharT>> lhs, std::unique_ptr<token<CharT>> rhs )
			: lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			lhs_->print( os ); os << '|'; rhs_->print( os );
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			lhs_->walk( callback );
			rhs_->walk( callback );
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return '|';
		}
		type get_type( ) const override
		{
			return type::alternation;
		}
	};

	template<typename CharT>
	class concatenation : public token<CharT>
	{
		std::unique_ptr<token<CharT>> lhs_, rhs_;
	public:
		explicit concatenation( std::unique_ptr<token<CharT>> lhs, std::unique_ptr<token<CharT>> rhs )
			: lhs_( std::move( lhs ) ), rhs_( std::move( rhs ) )
		{

		}
		void print( typename token<CharT>::ostream& os ) const override
		{
			lhs_->print( os ); rhs_->print( os );
		}
		void walk( std::function<void( const ast::token<CharT>& )> callback ) const override
		{
			lhs_->walk( callback );
			rhs_->walk( callback );
			callback( *this );
		}
		CharT get_token( ) const override
		{
			return '.';
		}
		type get_type( ) const override
		{
			return type::concatenation;
		}
	};
}