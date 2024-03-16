namespace Slyvina {
	namespace Units {
#ifdef SlyvWindows
#define WinOnly(t,f) t f
#else
#define WinOnly(t, f) inline t f {}
#endif


		WinOnly(void, ReleaseConsole());
	}
}