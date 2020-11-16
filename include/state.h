#pragma once

#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>

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
			: acc_(acc), transitionsMap_()
		{
		}

		state(const state<CharT> &other)
			: acc_(other.acc_),
			  transitionsMap_()
		{
			std::for_each(std::cbegin(other.transitionsMap_), std::cend(other.transitionsMap_), [this](auto x) {
				std::vector<std::shared_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->transitionsMap_[x.first] = v;
			});
		}

		state(state<CharT> &&other)
			: acc_(other.acc_),
			  transitionsMap_(other.transitionsMap_)
		{
		}

		state<CharT> &operator=(const state<CharT> &other)
		{
			if(&other == this)
				return *this;

			acc_ = other.acc_;
			transitionsMap_.clear();

			std::for_each(std::cbegin(other.transitionsMap_), std::cend(other.transitionsMap_), [this](auto x) {
				std::vector<std::shared_ptr<state<CharT>>> v;
				std::for_each(std::cbegin(x.second), std::cend(x.second), [&v](auto y) {
					v.push_back(std::make_shared<state<CharT>>(*v));
				});
				this->transitionsMap_[x.first] = v;
			});

			return *this;
		}

		
		state<CharT> &operator=(state<CharT> other)
		{
			std::swap(acc_, other.acc_);
			std::swap(transitionsMap_, other.transitionsMap_);
			return *this;
		}

		void set(accepting acc)
		{
			acc_ = acc;
		}

		void connect(CharT symbol, std::shared_ptr<state<CharT>> st)
		{
			transitionsMap_[symbol].push_back(st);
		}

		match next(std::basic_string_view<CharT> str)
		{
			if (str.empty())
			{
				if (acc_ == accepting::accepting)
				{
					return match::accepted;
				}

				for (std::shared_ptr<state<CharT>> &next : transitionsMap_[0x01])
				{
					if (next->next(str) == match::accepted)
					{
						return match::accepted;
					}
				}

				return match::rejected;
			}

			for (std::shared_ptr<state<CharT>> &next : transitionsMap_[str[0]])
			{
				if (next->next(str.substr(1)) == match::accepted)
				{
					return match::accepted;
				}
			}

			for (std::shared_ptr<state<CharT>> &next : transitionsMap_[0x01])
			{
				if (next->next(str) == match::accepted)
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
} // namespace fa