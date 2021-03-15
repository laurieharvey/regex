#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "ast.h"
#include "state.h"

namespace regex
{
	class dstate
	{
	public:
		enum class accepting
		{
			accepting,
			nonaccepting
		};

		dstate(accepting acc = accepting::nonaccepting);

		void set(accepting acc);

		bool is_accepting() const;

		void connect(regex::character_type symbol, std::shared_ptr<dstate> st);

		friend void merge(std::shared_ptr<dstate> src, std::shared_ptr<dstate> target);

        friend void copy(std::shared_ptr<const dstate> src, std::shared_ptr<dstate> target);

        std::map<character_type, std::shared_ptr<dstate>> get_transitions();

		match next(std::basic_string_view<regex::character_type> str);

	private:
		accepting acc_;
		std::map<regex::character_type, std::shared_ptr<dstate>> transitions_;
	};
} // namespace regex