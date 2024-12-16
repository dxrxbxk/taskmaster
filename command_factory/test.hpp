#ifndef TEST_HPP
# define TEST_HPP

#include "command.hpp"
#include <iostream>

class test : public command {
	public:
		ret_type execute() const override {
			std::cout << "test command" << std::endl;
		}

		std::unique_ptr<command> clone() const override {
			return std::make_unique<test>(*this);
		}
};

#endif // TEST_HPP
