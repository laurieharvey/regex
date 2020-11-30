#pragma once

#include <ostream>
#include <istream>
#include <array>
#include <memory>

#include "ast.h"
#include "stack.h"

namespace ast
{
	/* 
	 * 3 { '(', ')' } 2 { '*', '?', '+' } 1 { '-' } 
	 */
	const std::array<int, 128> precedence
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,3,3,2,2,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};

	template<typename CharT>
	bool is_operator( CharT token )
	{
		switch( token )
		{
		case '*':
		case '?':
		case '+':
		case '-':
		case '|':
		case '(':
		case ')':
			return true;
		default:
			return false;
		}
	}

	template<typename CharT>
	bool is_unary_operator( CharT token )
	{
		switch( token )
		{
		case '*':
		case '?':
		case '+':
		case '-':
		case ')':
			return true;
		default:
			return false;
		}
	}

	template<typename CharT>
	bool is_character( CharT token )
	{
		return !is_operator( token ) && token != 0;
	}

	/*
	 *	Convert an implicit regex expression to an explicit expression
	 * 	Implicit concatonation operator -> explicit '-'
	 */
	template<typename CharT>
	std::basic_stringstream<CharT> make_explicit( std::basic_istream<CharT>& input )
	{
		CharT current_token, previous_token = 0;

		std::basic_stringstream<CharT> output;

		while( input.get( current_token ) )
		{
			if( ( is_character( current_token ) || current_token == '(' ) && ( is_unary_operator( previous_token ) || is_character( previous_token ) ) )
				output.put( '-' );
			output.put( current_token );
			previous_token = current_token;
		}

		return output;
	}

	/*
	 *	Parse an explicit regex expression using the shunting yard algorithm to produce the AST
	 *	https://en.wikipedia.org/wiki/Shunting-yard_algorithm
	 */
	template<typename CharT>
	std::unique_ptr<ast::token<CharT>> parse( std::basic_istream<CharT, std::char_traits<CharT>>& expression )
	{
		CharT token;
		stack<std::unique_ptr<ast::token<CharT>>> output;
		stack<CharT> ops;

		std::unique_ptr<ast::token<CharT>> lhs;
		std::unique_ptr<ast::token<CharT>> rhs;

		while( expression.get( token ) )
		{
			if( token == '*' || token == '?' || token == '|' || token == '-' || token == '+' )
			{
				while( ops.size( ) && ops.top( ) != '(' && precedence[ token ] < precedence[ ops.top( ) ] )
				{
					switch( ops.top( ) )
					{
					case '*':
						output.push( std::make_unique<kleene<CharT>>( output.pop( ) ) );
						break;
					case '?':
						output.push( std::make_unique<zero_or_one<CharT>>( output.pop( ) ) );
						break;
					case '+':
						output.push( std::make_unique<one_or_more<CharT>>( output.pop( ) ) );
						break;
					case '-':
						rhs = output.pop( );
						lhs = output.pop( );
						output.push( std::make_unique<concatenation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
						break;
					case '|':
						rhs = output.pop( );
						lhs = output.pop( );
						output.push( std::make_unique<alternation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
						break;
					}
					ops.pop( );
				}
				ops.push( token );
			}
			else if( token == '(' )
			{
				ops.push( token );
			}
			else if( token == ')' )
			{
				while( ops.top( ) != '(' )
				{
					switch( ops.top( ) )
					{
					case '*':
						output.push( std::make_unique<kleene<CharT>>( output.pop( ) ) );
						break;
					case '?':
						output.push( std::make_unique<zero_or_one<CharT>>( output.pop( ) ) );
						break;
					case '+':
						output.push( std::make_unique<one_or_more<CharT>>( output.pop( ) ) );
						break;
					case '-':
						rhs = output.pop( );
						lhs = output.pop( );
						output.push( std::make_unique<concatenation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
						break;
					case '|':
						rhs = output.pop( );
						lhs = output.pop( );
						output.push( std::make_unique<alternation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
						break;
					}
					ops.pop( );
				}
				output.push( std::make_unique<parenthesis<CharT>>( output.pop( ) ) );
				ops.pop( );
			}
			else
			{
				if( token == '.' )
				{
					output.push( std::make_unique<any<CharT>>( ) );
				}	
				else
				{
					output.push( std::make_unique<character<CharT>>( token ) );		
				}			
			}
		}
		while( ops.size( ) )
		{
			switch( ops.top( ) )
			{
			case '*':
				output.push( std::make_unique<kleene<CharT>>( output.pop( ) ) );
				break;
			case '?':
				output.push( std::make_unique<zero_or_one<CharT>>( output.pop( ) ) );
				break;
			case '+':
				output.push( std::make_unique<one_or_more<CharT>>( output.pop( ) ) );
				break;
			case '-':
				rhs = output.pop( );
				lhs = output.pop( );
				output.push( std::make_unique<concatenation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
				break;
			case '|':
				rhs = output.pop( );
				lhs = output.pop( );
				output.push( std::make_unique<alternation<CharT>>( std::move( lhs ), std::move( rhs ) ) );
				break;
			case ')':
				output.push( std::make_unique<parenthesis<CharT>>( output.pop( ) ) );
				break;
			case '(':
				output.push( std::make_unique<parenthesis<CharT>>( output.pop( ) ) );
				break;
			}
			ops.pop( );
		}

		return std::move( output.top( ) );

	}
}