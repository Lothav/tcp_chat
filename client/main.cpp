#include <iostream>
#include <memory>
#include "Module.hpp"

int main(int argc, char* argv[])
{
    try {

        {
            std::unique_ptr<Client::Module> client_module (new Client::Module(argv[1]));
        }
    } catch (int a) {


    }

    return 0;
}