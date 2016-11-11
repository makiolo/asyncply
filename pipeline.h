// Modified from work of Aldrin D'Souza.

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <vector>
#include <memory>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/coroutine/coroutine.hpp>

namespace asyncply {

template <typename T>
using coro = boost::coroutines::asymmetric_coroutine<T()>;

template <typename T>
using yield_type = typename coro<T>::push_type;

template <typename T>
using link = boost::function<void(asyncply::coro<T>&, asyncply::yield_type<T>&)>;

template <typename T, typename Function>
std::shared_ptr< coro<T> > corun(Function&& f)
{
	return std::make_shared<coro<T> >(std::forward<Function>(f));
}

template <typename T>
class pipeline
{
private:
	using coro = asyncply::coro<T>;
	using coroptr = std::shared_ptr<coro>;

public:
	using in = asyncply::coro<T>;
	using out = asyncply::yield_type<T>;
	using link = asyncply::link<T>;

	template <typename Function>
	pipeline(Function&& f)
	{
		std::vector<coroptr> coros;
		coros.emplace_back(
				asyncply::corun<T>(
					[](asyncply::yield_type<T>&) { ; }
				)
		);
		coros.emplace_back(
				asyncply::corun<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
	}

	template <typename Function, typename ... Functions>
	pipeline(Function&& f, Functions&& ... fs)
	{
		std::vector<coroptr> coros;
		coros.emplace_back(
				asyncply::corun<T>(
					[](asyncply::yield_type<T>&) { ; }
				)
		);
		coros.emplace_back(
				asyncply::corun<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
		// recursion
		_add(coros, std::forward<Functions>(fs)...);
	}

protected:
	template <typename Function>
	void _add(std::vector<coroptr>& coros, Function&& f)
	{
		coros.emplace_back(
				asyncply::corun<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
	}

	template <typename Function, typename ... Functions>
	void _add(std::vector<coroptr>& coros, Function&& f, Functions&& ... fs)
	{
		coros.emplace_back(
				asyncply::corun<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
		// recursion
		_add(coros, std::forward<Functions>(fs)...);
	}
};

}

#endif

