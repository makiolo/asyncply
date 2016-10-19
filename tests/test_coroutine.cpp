#include <iostream>
#include <stackless_coroutine.hpp>

auto make_test1()
{
	static auto block = stackless_coroutine::make_block(
		[](auto& context, auto& variables, const std::string& text) {
			std::cout << text << ": paso 1" << std::endl;
			context("Mary");
			context("Mary2");
			return context.do_async();
		},
		[](auto& context, auto& variables, const std::string& text) {
			std::cout << text << ": paso 2" << std::endl;
			context("nobody");
			return context.do_async();
		},
		[](auto& context, auto& variables, const std::string& text) {
			std::cout << "final" << std::endl;
			return context.do_async();
		}	
	);
	struct dummy {};
	auto co = stackless_coroutine::make_coroutine<dummy>(block, [](auto& variables, std::exception_ptr e, auto op){
		std::cout << "fin" << std::endl;
	});
	return co("Ricardo");
}

int main(int argc, const char* argv[])
{
	make_test1();
	return 0;
}

