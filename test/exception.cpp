#include <ghc/filesystem.hpp>

#include <iostream>

#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr) fs_exception_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv)
{
    std::cerr << "Done.\n";
    return 0;
}
