#include <ghc/filesystem.hpp>


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      fs_exception_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
	return 0;
}
