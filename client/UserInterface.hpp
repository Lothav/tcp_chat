//
// Created by luizorv on 11/6/17.
//

#ifndef TCP_CHAT_USERINTERFACE_HPP
#define TCP_CHAT_USERINTERFACE_HPP

#include <iostream>
#include <zconf.h>
#include <vector>

namespace Client
{
	class UserInterface
	{

	private:
		UserInterface(){}

	public:

		static void withLoading(std::string msg)
		{
			std::cout << "Loading Please Wait";
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "." << std::flush;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "." << std::flush;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "." << std::flush;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "\b\b\b   \b\b\b" << std::flush;
			}
		}


	};
}
#endif //TCP_CHAT_USERINTERFACE_HPP
