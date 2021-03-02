#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "ast.h"

namespace regex
{
	enum class match
	{
		accepted,
		rejected
	};

	class state : public std::enable_shared_from_this<state>
	{
	public:
		enum class accepting
		{
			accepting,
			nonaccepting
		};

		state(accepting acc = accepting::nonaccepting);

		void set(accepting acc);

		void connect(regex::character_type symbol, std::shared_ptr<state> st);

		void connect(regex::character_type symbol, std::weak_ptr<state> st);

		auto get_transitions(regex::character_type symbol);

		std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> get_epsilon_closure();

		auto get_transitions();

		void walk(std::function<void(std::weak_ptr<state>)> callback, std::set<const state *> visited = std::set<const state *>());

		match next(std::basic_string_view<regex::character_type> str, std::set<state *> visited = std::set<state *>());

	private:
		accepting acc_;
		std::map<regex::character_type, std::set<std::shared_ptr<state>>> strong_transitions_;
		std::map<regex::character_type, std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>>> weak_transitions_;
	};
} // namespace fa