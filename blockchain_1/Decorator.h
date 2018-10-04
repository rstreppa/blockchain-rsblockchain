/**
Decorator.h
Purpose:	Template classes to decorate memebrs with added functionalities:
			1) random sleep time within a specified limit
			2) logging to screen

@author rstreppa
@version 1.0 09/27/2018
*/

#pragma once
#include <functional>
#include <boost/random.hpp>
#include<boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>

//---------------------------------------------
// BEGIN random sleep decorator implementation
//---------------------------------------------

template <class> struct SleepDecorator;

template <class R, class... Args>
struct SleepDecorator<R(Args ...)>
{
	SleepDecorator(const size_t max,
		boost::random::mt19937_64& rng, 
		std::function<R(Args ...)> f) : m_max(max), m_rng(rng), m_f(f) {}

	R operator()(Args ... args)
	{
		boost::random::uniform_int_distribution<> N(1, max);
		int x = N(rng);
		boost::this_thread::sleep_for(boost::chrono::seconds(x));
		return m_f(args...);
	}

	const size_t m_max;
	boost::random::mt19937_64& m_rng;
	std::function<R(Args ...)> m_f;
};

template<class R, class... Args>
SleepDecorator<R(Args...)> makeSleepDecorator(const size_t max, boost::random::mt19937_64& rng, R(*f)(Args ...))
{
	return SleepDecorator<R(Args...)>(max, rng, std::function<R(Args...)>(f));
}

//-------------------------------------------
// END random sleep decorator implementation
//-------------------------------------------


//---------------------------------------------
// BEGIN another random sleep decorator
//---------------------------------------------






//-------------------------------------------
// END another random sleep decorator
//-------------------------------------------


#if 0
//-------------------------------
// Sample functions to decorate.
//-------------------------------

// Proposed solution doesn't work with default values.
// int decorated1(int a, float b = 0)
int decorated1(int a, float b)
{
	std::cout << "a = " << a << ", b = " << b << std::endl;
	return 0;
}

void decorated2(int a)
{
	std::cout << "a = " << a << std::endl;
}

int main()
{
	auto method1 = makeDecorator(decorated1);
	method1(10, 30.3);
	auto method2 = makeDecorator(decorated2);
	method2(10);
}

#endif