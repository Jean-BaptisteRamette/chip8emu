#include <iostream>


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "[ERROR]: No ROM input file provided !" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
