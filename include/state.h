#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

namespace fa
{
	enum class match
	{
		accepted,
		rejected
	};

	template <typename CharT>
	class state : public std::enable_shared_from_this<state<CharT>>
	{
	public:
		enum class accepting
		{
			accepting,
			nonaccepting
		};

		state(accepting acc = accepting::nonaccepting)
			: acc_(acc), strong_transitions_(), weak_transitions_()
		{
		}

		void set(accepting acc)
		{
			acc_ = acc;
		}

		void connect(CharT symbol, std::shared_ptr<state<CharT>> st)
		{
			strong_transitions_[symbol].insert(st);
		}

		void connect(CharT symbol, std::weak_ptr<state<CharT>> st)
		{
			weak_transitions_[symbol].insert(st);
		}

		auto get_transitions(CharT symbol)
		{
			std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>> result;

			typename std::map<CharT, std::set<std::shared_ptr<state<CharT>>>>::const_iterator strong_transitions = strong_transitions_.find(symbol);

			if (strong_transitions != std::cend(strong_transitions_))
			{
				std::copy(strong_transitions->second.cbegin(), strong_transitions->second.cend(), std::inserter(result, result.begin()));
			}

			typename std::map<CharT, std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>>>::const_iterator weak_transitions = weak_transitions_.find(symbol);

			if (weak_transitions != std::cend(weak_transitions_))
			{
				std::copy(weak_transitions->second.cbegin(), weak_transitions->second.cend(), std::inserter(result, result.begin()));
			}

			return result;
		}

		std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>> get_epsilon_closure()
		{
			std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>> epsilon_closure;

			epsilon_closure.insert(std::enable_shared_from_this<state<CharT>>::weak_from_this());

			std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>> epsilon_transitions = get_transitions(0x01);

			for (const std::weak_ptr<state<CharT>> &epsilon_transition : epsilon_transitions)
			{
				if (epsilon_closure.find(epsilon_transition) == std::end(epsilon_closure))
				{
					epsilon_closure.insert(epsilon_transition);
					std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>> next_closure = epsilon_transition.lock()->get_epsilon_closure();
					std::copy(next_closure.begin(), next_closure.end(), std::inserter(epsilon_closure, epsilon_closure.begin()));
				}
			}

			return epsilon_closure;
		}

		auto get_transitions()
		{
			std::map<CharT, std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>>> result = weak_transitions_;

			for (auto &states_for_transition : strong_transitions_)
			{
				for (const std::weak_ptr<state<CharT>> &transition : states_for_transition.second)
				{
					result[states_for_transition.first].insert(transition);
				}
			}

			return result;
		}

		void walk(std::function<void(std::weak_ptr<state<CharT>>)> callback, std::set<const state<CharT> *> visited = std::set<const state<CharT> *>())
		{
			if (visited.find(this) != std::end(visited))
			{
				return;
			}
			else
			{
				visited.insert(this);

				std::weak_ptr<state<CharT>> temp = std::enable_shared_from_this<state<CharT>>::weak_from_this();

				callback(temp);

				for (auto &transitionsForCharacter : strong_transitions_)
				{
					for (auto &transition : transitionsForCharacter.second)
					{
						transition->walk(callback, visited);
					}
				}

				for (auto &transitionsForCharacter : weak_transitions_)
				{
					for (auto &transition : transitionsForCharacter.second)
					{
						transition.lock()->walk(callback, visited);
					}
				}
			}
		}

		match next(std::basic_string_view<CharT> str, std::set<state *> visited = std::set<state *>())
		{
			if (visited.find(this) != std::end(visited))
			{
				return match::rejected;
			}

			visited.insert(this);

			if (str.empty())
			{
				if (acc_ == accepting::accepting)
				{
					return match::accepted;
				}

				for (const std::shared_ptr<state<CharT>> &next : strong_transitions_[0x01])
				{
					if (next->next(str, visited) == match::accepted)
					{
						return match::accepted;
					}
				}

				for (const std::weak_ptr<state<CharT>> &next : weak_transitions_[0x01])
				{
					if (next.lock()->next(str, visited) == match::accepted)
					{
						return match::accepted;
					}
				}

				return match::rejected;
			}

			for (const std::shared_ptr<state<CharT>> &next : strong_transitions_[str[0]])
			{
				if (next->next(str.substr(1)) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (const std::weak_ptr<state<CharT>> &next : weak_transitions_[str[0]])
			{
				if (next.lock()->next(str.substr(1)) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (const std::shared_ptr<state<CharT>> &next : strong_transitions_[0x01])
			{
				if (next->next(str, visited) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (const std::weak_ptr<state<CharT>> &next : weak_transitions_[0x01])
			{
				if (next.lock()->next(str, visited) == match::accepted)
				{
					return match::accepted;
				}
			}

			return match::rejected;
		}

	private:
		accepting acc_;
		std::map<CharT, std::set<std::shared_ptr<state<CharT>>>> strong_transitions_;
		std::map<CharT, std::set<std::weak_ptr<state<CharT>>, std::owner_less<std::weak_ptr<state<CharT>>>>> weak_transitions_;
	};
} // namespace fa