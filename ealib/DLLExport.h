#ifdef EALIB_EXPORTS
#define CLASS_DECLSPEC __declspec(dllexport)
#else
#define CLASS_DECLSPEC __declspec(dllimport)
#endif

#pragma warning(disable:4251)