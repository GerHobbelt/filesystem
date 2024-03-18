
// to prevent mishaps, use the GHC-provided filesystem selector headerfile: ghc/fs_std.hpp
// which listens to both system availability of std::filesystem and the GHC_DO_NOT_USE_STD_FS
// overriding switch/define.
#include <ghc/fs_std.hpp>  // namespace fs = std::filesystem;   or   namespace fs = ghc::filesystem;

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "monolithic_examples.h"


template <typename TP>
std::time_t to_time_t(TP tp)
{
    // Based on trick from: Nico Josuttis, C++17 - The Complete Guide
    std::chrono::system_clock::duration dt = std::chrono::duration_cast<std::chrono::system_clock::duration>(tp - TP::clock::now());
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + dt);
}

static std::string perm_to_str(fs::perms prms)
{
    std::string result;
    result.reserve(9);
    for (int i = 0; i < 9; ++i) {
        result = ((static_cast<int>(prms) & (1 << i)) ? "xwrxwrxwr"[i] : '-') + result;
    }
    return result;
}



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      fs_dir_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
#ifdef GHC_FILESYSTEM_VERSION
    fs::u8arguments u8guard(argc, argv);
    if (!u8guard.valid()) {
        std::cerr << "Invalid character encoding, UTF-8 based encoding needed." << std::endl;
        return EXIT_FAILURE;
    }
#endif
    if (argc > 2) {
        std::cerr << "USAGE: dir <path>" << std::endl;
        return EXIT_FAILURE;
    }
	try
	{
		fs::path dir{"."};
		if (argc == 2) {
			dir = (const char8_t *)(argv[1]);
		}
		for (auto de : fs::directory_iterator(dir)) {
			auto ft = to_time_t(de.last_write_time());
			auto ftm = *std::localtime(&ft);
			std::cout << (de.is_directory() ? "d" : "-") << perm_to_str(de.symlink_status().permissions()) << "  " << std::setw(8) << (de.is_directory() ? "-" : std::to_string(de.file_size())) << "  " << std::put_time(&ftm, "%Y-%m-%d %H:%M:%S") << "  "
				<< de.path().filename().string() << std::endl;
		}
	}
	catch (fs::filesystem_error &ex)
	{
		std::cerr << "filesystem error " << ex.code().value() << ": " << ex.code().message() << " :: " << ex.what() << " (path: '" << ex.path1().string() << "')" << std::endl;
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}
