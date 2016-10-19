#include <iostream>
#include <stackless_coroutine.hpp>

typedef enum {
	RUNNING = 0,
	COMPLETED = 1,
	FAILED = 2,
	ABORTED = 3,
	PANIC_ERROR = 4
} StatusEnum;

struct node {
	node(const std::function<int()>& _f)
		: f(_f)
	{
		;
	}

	int update()
	{
		return f();
	}

	std::function<int()> f;
};

struct sequence_data {
	sequence_data(std::vector<node> _c) : i(0), init(false), c(std::move(_c))
	{
		;
	}
	int i;
	bool init;
	std::vector<node> c;
	std::weak_ptr<node> parent;
};

struct childs_data {
	childs_data()
	{
		;
	}
};

auto make_sequence()
{
	return stackless_coroutine::make_block(
			stackless_coroutine::make_while_true(
				[](auto &context, sequence_data &v) {
					if(v.i < 3)
					{
						auto child = v.c[v.i];
						auto code = child.update();
						switch(code)
						{
							case RUNNING:
							{
								return context.do_async_yield(RUNNING);
							}
							case COMPLETED:
							{
								++v.i;
								return context.do_async_yield(RUNNING);
							}
							case FAILED:
							{
								return context.do_async_yield(FAILED);
							}
							case ABORTED:
							{
								return context.do_async_yield(ABORTED);
							}
							default:
							{
								return context.do_async_yield(PANIC_ERROR);
							}
						}
					}
					else
					{
						return context.do_async_yield(COMPLETED);
					}
				}
			)
		);
}

// auto make_childs_examples()
// {
// 	return stackless_coroutine::make_block(
// 			stackless_coroutine::make_while_true(
// 				[](auto &context, auto &value) {
// 					std::cout << "child 1" << std::endl;
// 					return context.do_next();
// 				}
// 			)
// 		);
// }

void make_test1()
{
	// auto childs = stackless_coroutine::make_coroutine<childs_data>(make_childs_examples(),
	// 		[](auto &value, std::exception_ptr ep, stackless_coroutine::operation op) {
	// 			;
	// 		}
	// 	);
	// childs();
	// return;
    //
	std::vector<node> c;
	c.emplace_back([](){
		std::cout << "child 1" << std::endl;
		return COMPLETED;
	});
	c.emplace_back([](){
		std::cout << "child 2" << std::endl;
		return COMPLETED;
	});
	c.emplace_back([](){
		std::cout << "child 3" << std::endl;
		return COMPLETED;
	});

	// initial state
	auto range = stackless_coroutine::make_generator<int, sequence_data>(make_sequence(), c);
	while(range.valid())
	{
		auto code = range.value();
		if(code == COMPLETED)
			break;
		range.next();
	}
}

int main(int argc, const char* argv[])
{
	make_test1();
	return 0;
}

