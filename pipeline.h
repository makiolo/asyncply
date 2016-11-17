// Modified from work of Aldrin D'Souza.

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <vector>
#include <memory>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/coroutine2/coroutine.hpp>

namespace asyncply {

template <typename T>
using asymm_coro = boost::coroutines2::asymmetric_coroutine<T>;

template <typename T>
using pull_type = typename asymm_coro<T>::pull_type;

template <typename T>
using yield_type = typename asymm_coro<T>::push_type;

template <typename T>
using link = boost::function<void(asyncply::pull_type<T>&, asyncply::yield_type<T>&)>;

template <typename T>
using coroutine = std::shared_ptr< asyncply::pull_type<T> >;

template <typename T, typename Function>
coroutine<T> make_coroutine(Function&& f)
{
	return std::make_shared< asyncply::pull_type<T> >(std::forward<Function>(f));
}

template <typename T>
class pipeline
{
public:
	using in = asyncply::pull_type<T>;
	using out = asyncply::yield_type<T>;
	using link = asyncply::link<T>;

	template <typename Function>
	pipeline(Function&& f)
	{
		std::vector<coroutine<T> > coros;
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					[](asyncply::yield_type<T>&) { ; }
				)
		);
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
	}

	template <typename Function, typename ... Functions>
	pipeline(Function&& f, Functions&& ... fs)
	{
		std::vector<coroutine<T> > coros;
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					[](asyncply::yield_type<T>&) { ; }
				)
		);
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
		// recursion
		_add(coros, std::forward<Functions>(fs)...);
	}

protected:
	template <typename Function>
	void _add(std::vector<coroutine<T> >& coros, Function&& f)
	{
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
	}

	template <typename Function, typename ... Functions>
	void _add(std::vector<coroutine<T> >& coros, Function&& f, Functions&& ... fs)
	{
		coros.emplace_back(
				asyncply::make_coroutine<T>(
					boost::bind(f, boost::ref(*coros.back().get()), _1)
				)
		);
		// recursion
		_add(coros, std::forward<Functions>(fs)...);
	}
};

}

#endif

