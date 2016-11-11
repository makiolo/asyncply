#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/filesystem.hpp>
#include <asyncply/parallel.h>
#include "../pipeline.h"
#include "../cmd.h"

int main()
{
	using namespace asyncply;
	std::cout.sync_with_stdio(false);

	// std::vector<std::string> lines;
	// cmd(find("../tests"), grep("test_"), out(lines));
	// for (auto& line : lines)
	// 	std::cout << line << std::endl;

	cmd({
		find(".."),
		grep(".*\\.cpp$|.*\\.h$"),
		cat(),
		grep("class|struct|typedef|using|void|int|double|float"),
		grep_v("enable_if|;|\"|\'"),
		trim(),
		split(" "),
		uniq(),
		join(" "),
		out()
	});

	// ssh_session my_ssh_session;
	// int rc;
	// char* password;
	// // Open session and set options
	// my_ssh_session = ssh_new();
	// if (my_ssh_session == NULL)
	// 	exit(-1);
	// ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");
	// // Connect to server
	// rc = ssh_connect(my_ssh_session);
	// if (rc != SSH_OK)
	// {
	// 	fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(my_ssh_session));
	// 	ssh_free(my_ssh_session);
	// 	exit(-1);
	// }
	// // Verify the server's identity
	// // For the source code of verify_knowhost(), check previous example
	// if (verify_knownhost(my_ssh_session) < 0)
	// {
	// 	ssh_disconnect(my_ssh_session);
	// 	ssh_free(my_ssh_session);
	// 	exit(-1);
	// }
	// // Authenticate ourselves
	// password = getpass("Password: ");
	// rc = ssh_userauth_password(my_ssh_session, NULL, password);
	// if (rc != SSH_AUTH_SUCCESS)
	// {
	// 	fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(my_ssh_session));
	// 	ssh_disconnect(my_ssh_session);
	// 	ssh_free(my_ssh_session);
	// 	exit(-1);
	// }
	//
	// std::cout << "connected!" << std::endl;
	// show_remote_processes(my_ssh_session);
	//
	// ssh_disconnect(my_ssh_session);
	// ssh_free(my_ssh_session);


	//
	// fes::async_fast< std::shared_ptr<asyncply::coro<int> > > _channel;
	// std::atomic<bool> _exit;
	// _exit = false;
	// asyncply::parallel(
	// 		[&](){
	// 			while(!_exit)
	// 			{
	// 				_channel.connect([&](const std::shared_ptr<asyncply::coro<int> >& coro) {
	// 					while(*coro)
	// 					{
	// 						int x = coro->get();
	// 						_exit = (x == 1);
	// 						(*coro)();
	// 					}
	// 				});
	// 				_channel.update();
	// 			}
	// 		},
	// 		[&](){
	// 			asyncply::run(
	// 				[&](){
	// 					for(int i=0; i<1000; ++i)
	// 					{
	// 						_channel(asyncply::corun<int>(
	// 							[](asyncply::yield_type<int>& yield)
	// 							{
	// 								std::cout << "create " << std::endl;
	// 								yield(0);
	// 								std::cout << "download " << std::endl;
	// 								yield(0);
	// 								std::cout << "patching " << std::endl;
	// 								yield(0);
	// 								std::cout << "compile " << std::endl;
	// 								yield(0);
	// 								std::cout << "tests " << std::endl;
	// 								yield(0);
	// 								std::cout << "packing " << std::endl;
	// 								yield(0);
	// 							}
	// 						));
	// 					}
	// 				}
	// 				,
	// 				[&](){
	// 					_channel(asyncply::corun<int>(
	// 						[](asyncply::yield_type<int>& yield)
	// 						{
	// 							std::cout << "request exit" << std::endl;
	// 							yield(1);
	// 						}
	// 					));
	// 				}
	// 			);
	// 		},
	// 		[&](){
	// 			_channel(asyncply::corun<int>(
	// 				[](asyncply::yield_type<int>& yield)
	// 				{
	// 					std::cout << "step1 - thread3 " << std::endl;
	// 					yield(0);
	// 					std::cout << "step2 - thread3 " << std::endl;
	// 					yield(0);
	// 					std::cout << "step3 - thread3 " << std::endl;
	// 					yield(0);
	// 				}
	// 			));
	// 		}
	// );

	// pipelines in parallel
	// try
	// {
	// 	auto result = asyncply::parallel(
	// 		[](){
	// 			return "one";
	// 		},
	// 		[](){
	// 			cmd({
	// 				find(".."),
	// 				grep(".*\\.cpp$|.*\\.h$"),
	// 				cat(),
	// 				grep("class|struct|typedef|using|void|int|double|float"),
	// 				grep_v("enable_if|;|\"|\'"),
	// 				trim(),
	// 				split(" "),
	// 				uniq(),
	// 				join(" "),
	// 				out()
	// 			});
	// 			return "two";
	// 		}
	// 	);
	// 	std::cout << result.size() << std::endl;
	// 	for(auto& r : result)
	// 	{
	// 		std::cout << r << std::endl;
	// 	}
	// }
	// catch(boost::filesystem::filesystem_error& e)
	// {
	// 	std::cout << "exception: " << e.what() << std::endl;
	// }
}
