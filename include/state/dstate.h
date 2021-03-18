#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "ast.h"
#include "state/state.h"

namespace regex
{
	class dstate : public state, public std::enable_shared_from_this<dstate>
	{
	public:
		enum class accepting
		{
			accepting,
			nonaccepting
		};

		dstate(state::context ctx = state::context::rejecting);

		void connect(regex::character_type symbol, std::shared_ptr<dstate> st);

		friend void merge(std::shared_ptr<dstate> src, std::shared_ptr<dstate> target);

        friend void copy(std::shared_ptr<const dstate> src, std::shared_ptr<dstate> target);

		group get_transitions(regex::character_type symbol) override;

		group get_epsilon_closure() override;

		std::map<character_type, group> get_transitions() override;

        // std::map<character_type, std::shared_ptr<dstate>> get_transitions();

		void walk(std::function<void(std::shared_ptr<state>)> callback, std::set<std::shared_ptr<state>> visited = std::set<std::shared_ptr<state>>()) override;

		match next(std::basic_string_view<regex::character_type> str);

	private:
		std::map<regex::character_type, std::shared_ptr<dstate>> transitions_;
	};
} // namespace regex