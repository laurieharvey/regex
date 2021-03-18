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

	class state;

	using group = std::set<std::shared_ptr<state>>;

	class state
	{
	public:
		enum class context
		{
			accepting,
			rejecting,
		};

		explicit state(context ctx);

		context get_type() const;

		void set(state::context ctx);

		virtual group get_transitions(regex::character_type symbol) = 0;

		virtual group get_epsilon_closure() = 0;

		virtual std::map<character_type, group> get_transitions() = 0;

		virtual void walk(std::function<void(std::shared_ptr<state>)> callback, std::set<std::shared_ptr<state>> visited = std::set<std::shared_ptr<state>>()) = 0;

	private:
		context ctx_;
	};
} // namespace fa