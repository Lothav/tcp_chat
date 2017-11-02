#include <iostream>
#include <memory>
#include "Module.hpp"

int main(int argc, char* argv[])
{
    try {

        {
            std::unique_ptr<Server::Module> server_module (new Server::Module(argv[1]));
        }
    } catch (int a) {

    }

    return 0;
}