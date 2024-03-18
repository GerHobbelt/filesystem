
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

	extern int fs_dir_main(int argc, const char** argv);
	extern int fs_du_main(int argc, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
