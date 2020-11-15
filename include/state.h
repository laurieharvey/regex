#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <vector>

namespace fa
{
	enum class match { accepted, rejected };

	template<typename CharT>
	class state
	{
	public:
		enum class accepting { accepting, nonaccepting };

		state( accepting acc = accepting::nonaccepting )
			: acc_( acc )
			, transitionsMap_( )
		{

		}

		void set( accepting acc )
		{
			acc_ = acc;
		}

		void connect( CharT symbol, std::shared_ptr<state<CharT>> st )
		{
			transitionsMap_[ symbol ].push_back( st );
		}

		match next( std::basic_string_view<CharT> str )
		{
			if( str.empty( ) )
			{
				if( acc_ == accepting::accepting )
				{
					return match::accepted;
				}

				for( std::shared_ptr<state<CharT>>& next : transitionsMap_[ 0x01 ] )
				{
					if( next->next( str ) == match::accepted )
					{
						return match::accepted;
					}
				}

				return match::rejected;
			}

			for( std::shared_ptr<state<CharT>>& next : transitionsMap_[ str[ 0 ] ] )
			{
				if( next->next( str.substr( 1 ) ) == match::accepted )
				{
					return match::accepted;
				}
			}

			for( std::shared_ptr<state<CharT>>& next : transitionsMap_[ 0x01 ] )
			{
				if( next->next( str ) == match::accepted )
				{
					return match::accepted;
				}
			}

			return match::rejected;
		}
	private:
		accepting acc_;
		std::map<CharT, std::vector<std::shared_ptr<state<CharT>>>> transitionsMap_;
	};
}