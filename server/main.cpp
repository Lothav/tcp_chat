#include <iostream>
#include <memory>
#include "Module.hpp"

int main(int argc, char* argv[])
{
    try {

        int port = atoi(argv[1]);

        {
            std::unique_ptr<Server::Module> server_module (new Server::Module(port));
            server_module->run();
        }
    } catch (int a) {

    }

    return 0;
}