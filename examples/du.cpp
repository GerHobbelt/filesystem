
// to prevent mishaps, use the GHC-provided filesystem selector headerfile: ghc/fs_std.hpp
// which listens to both system availability of std::filesystem and the GHC_DO_NOT_USE_STD_FS
// overriding switch/define.
#include <ghc/fs_std.hpp>  // namespace fs = std::filesystem;   or   namespace fs = ghc::filesystem;

#include <iostream>
#include <iomanip>
#include <chrono>

#include "monolithic_examples.h"


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      fs_du_main(cnt, arr)
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
		std::cerr << "USAGE: du <path>" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		fs::path dir{"."};
		if (argc == 2) {
			dir = fs::path((const char8_t *)(argv[1]));
		}

		uint64_t totalSize = 0;
		int totalDirs = 0;
		int totalFiles = 0;
		int maxDepth = 0;

		try {
			auto rdi = fs::recursive_directory_iterator(dir);
			for (auto de : rdi) {
				if (rdi.depth() > maxDepth) {
					maxDepth = rdi.depth();
				}
				if (de.is_regular_file()) {
					totalSize += de.file_size();
					++totalFiles;
				} else if (de.is_directory()) {
					++totalDirs;
				}
			}
		}
		catch (fs::filesystem_error fe) {
			std::cerr << "Error: " << fe.what() << std::endl;
			return 1;
		}
		std::cout << totalSize << " bytes in " << totalFiles << " files and " << totalDirs << " directories, maximum depth: " << maxDepth << std::endl;
	}
	catch (fs::filesystem_error &ex)
	{
		std::cerr << "filesystem error " << ex.code().value() << ": " << ex.code().message() << " :: " << ex.what() << " (path: '" << ex.path1().string() << "')" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
