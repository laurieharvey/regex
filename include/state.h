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
	class state
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

		state(const state<CharT> &other)
			: acc_(other.acc_),
			  strong_transitions_(),
			  weak_transitions_()
		{
			std::for_each(std::cbegin(other.strong_transitions_), std::cend(other.strong_transitions_), [this](auto x) {
				std::vector<std::shared_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->strong_transitions_[x.first] = v;
			});

			std::for_each(std::cbegin(other.weak_transitions_), std::cend(other.weak_transitions_), [this](auto x) {
				std::vector<std::weak_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->weak_transitions_[x.first] = v;
			});
		}

		state(state<CharT> &&other)
			: acc_(other.acc_),
			  strong_transitions_(other.strong_transitions_),
			  weak_transitions_(other.weak_transitions_)
		{
		}

		state<CharT> &operator=(const state<CharT> &other)
		{
			if (&other == this)
				return *this;

			acc_ = other.acc_;
			strong_transitions_.clear();
			weak_transitions_.clear();

			std::for_each(std::cbegin(other.strong_transitions_), std::cend(other.strong_transitions_), [this](auto x) {
				std::vector<std::shared_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->strong_transitions_[x.first] = v;
			});

			std::for_each(std::cbegin(other.weak_transitions_), std::cend(other.weak_transitions_), [this](auto x) {
				std::vector<std::weak_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->weak_transitions_[x.first] = v;
			});

			return *this;
		}

		state<CharT> &operator=(state<CharT> other)
		{
			std::swap(acc_, other.acc_);
			std::swap(strong_transitions_, other.strong_transitions_);
			std::swap(weak_transitions_, other.weak_transitions_);
			return *this;
		}

		void set(accepting acc)
		{
			acc_ = acc;
		}

		void connect(CharT symbol, std::shared_ptr<state<CharT>> st)
		{
			strong_transitions_[symbol].push_back(st);
		}

		void connect(CharT symbol, std::weak_ptr<state<CharT>> st)
		{
			weak_transitions_[symbol].push_back(st);
		}

		const std::map<CharT, std::vector<std::shared_ptr<const state<CharT>>>> &get_transitions()
		{
			return strong_transitions_;
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

				for (std::shared_ptr<state<CharT>> &next : strong_transitions_[0x01])
				{
					if (next->next(str, visited) == match::accepted)
					{
						return match::accepted;
					}
				}

				for (std::weak_ptr<state<CharT>> &next : weak_transitions_[0x01])
				{
					if (next.lock()->next(str, visited) == match::accepted)
					{
						return match::accepted;
					}
				}

				return match::rejected;
			}

			for (std::shared_ptr<state<CharT>> &next : strong_transitions_[str[0]])
			{
				if (next->next(str.substr(1)) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (std::weak_ptr<state<CharT>> &next : weak_transitions_[str[0]])
			{
				if (next.lock()->next(str.substr(1)) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (std::shared_ptr<state<CharT>> &next : strong_transitions_[0x01])
			{
				if (next->next(str, visited) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (std::weak_ptr<state<CharT>> &next : weak_transitions_[0x01])
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
		std::map<CharT, std::vector<std::shared_ptr<state<CharT>>>> strong_transitions_;
		std::map<CharT, std::vector<std::weak_ptr<state<CharT>>>> weak_transitions_;
	};
} // namespace fa