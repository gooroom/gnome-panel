#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.menu"), aligned (8)))
#else
# define SECTION
#endif

#ifdef _MSC_VER
static const SECTION union { const guint8 data[685]; const double alignment; void * const ptr;}  menu_resource_data = { {
  0107, 0126, 0141, 0162, 0151, 0141, 0156, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 
  0030, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0000, 0000, 0050, 0010, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0003, 0000, 0000, 0000, 
  0006, 0000, 0000, 0000, 0007, 0000, 0000, 0000, 0007, 0000, 0000, 0000, 0010, 0000, 0000, 0000, 
  0260, 0267, 0044, 0060, 0004, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0006, 0000, 0114, 0000, 
  0010, 0001, 0000, 0000, 0014, 0001, 0000, 0000, 0322, 0005, 0350, 0052, 0000, 0000, 0000, 0000, 
  0014, 0001, 0000, 0000, 0014, 0000, 0114, 0000, 0030, 0001, 0000, 0000, 0034, 0001, 0000, 0000, 
  0272, 0272, 0177, 0171, 0001, 0000, 0000, 0000, 0034, 0001, 0000, 0000, 0010, 0000, 0114, 0000, 
  0044, 0001, 0000, 0000, 0050, 0001, 0000, 0000, 0203, 0251, 0176, 0033, 0007, 0000, 0000, 0000, 
  0050, 0001, 0000, 0000, 0020, 0000, 0166, 0000, 0070, 0001, 0000, 0000, 0327, 0001, 0000, 0000, 
  0113, 0120, 0220, 0013, 0006, 0000, 0000, 0000, 0327, 0001, 0000, 0000, 0004, 0000, 0114, 0000, 
  0334, 0001, 0000, 0000, 0340, 0001, 0000, 0000, 0223, 0075, 0033, 0260, 0007, 0000, 0000, 0000, 
  0340, 0001, 0000, 0000, 0021, 0000, 0166, 0000, 0370, 0001, 0000, 0000, 0227, 0002, 0000, 0000, 
  0324, 0265, 0002, 0000, 0377, 0377, 0377, 0377, 0227, 0002, 0000, 0000, 0001, 0000, 0114, 0000, 
  0230, 0002, 0000, 0000, 0234, 0002, 0000, 0000, 0136, 0063, 0061, 0332, 0002, 0000, 0000, 0000, 
  0234, 0002, 0000, 0000, 0005, 0000, 0114, 0000, 0244, 0002, 0000, 0000, 0254, 0002, 0000, 0000, 
  0147, 0156, 0157, 0155, 0145, 0057, 0000, 0000, 0001, 0000, 0000, 0000, 0147, 0156, 0157, 0155, 
  0145, 0055, 0160, 0141, 0156, 0145, 0154, 0057, 0002, 0000, 0000, 0000, 0155, 0157, 0144, 0165, 
  0154, 0145, 0163, 0057, 0007, 0000, 0000, 0000, 0155, 0145, 0156, 0165, 0055, 0142, 0141, 0162, 
  0055, 0155, 0145, 0156, 0165, 0056, 0165, 0151, 0003, 0001, 0000, 0000, 0001, 0000, 0000, 0000, 
  0170, 0332, 0175, 0117, 0061, 0016, 0302, 0060, 0014, 0334, 0373, 0212, 0310, 0173, 0311, 0007, 
  0334, 0154, 0214, 0274, 0001, 0071, 0255, 0221, 0054, 0045, 0101, 0112, 0334, 0201, 0337, 0343, 
  0264, 0052, 0145, 0100, 0154, 0167, 0076, 0337, 0371, 0214, 0122, 0224, 0353, 0203, 0146, 0016, 
  0203, 0163, 0230, 0271, 0254, 0116, 0226, 0011, 0072, 0030, 0043, 0325, 0261, 0003, 0350, 0232, 
  0251, 0215, 0147, 0225, 0147, 0331, 0231, 0161, 0121, 0316, 0007, 0061, 0112, 0252, 0125, 0342, 
  0252, 0354, 0012, 0145, 0236, 0040, 0121, 0344, 0004, 0116, 0053, 0225, 0226, 0110, 0051, 0046, 
  0033, 0276, 0270, 0101, 0270, 0137, 0027, 0121, 0167, 0263, 0350, 0206, 0376, 0143, 0373, 0023, 
  0105, 0333, 0141, 0010, 0107, 0255, 0013, 0133, 0300, 0326, 0355, 0127, 0000, 0372, 0263, 0031, 
  0372, 0257, 0326, 0350, 0273, 0045, 0014, 0266, 0160, 0276, 0375, 0006, 0072, 0273, 0121, 0100, 
  0000, 0050, 0165, 0165, 0141, 0171, 0051, 0157, 0162, 0147, 0057, 0000, 0000, 0000, 0000, 0000, 
  0155, 0141, 0151, 0156, 0055, 0155, 0145, 0156, 0165, 0055, 0155, 0145, 0156, 0165, 0056, 0165, 
  0151, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0005, 0001, 0000, 0000, 0001, 0000, 0000, 0000, 
  0170, 0332, 0175, 0217, 0075, 0016, 0302, 0060, 0014, 0205, 0367, 0236, 0042, 0312, 0016, 0271, 
  0200, 0233, 0215, 0221, 0063, 0040, 0247, 0065, 0222, 0245, 0304, 0110, 0211, 0073, 0160, 0173, 
  0234, 0040, 0132, 0006, 0304, 0142, 0371, 0363, 0317, 0363, 0063, 0260, 0050, 0325, 0073, 0056, 
  0024, 0047, 0347, 0240, 0220, 0154, 0216, 0327, 0331, 0027, 0144, 0071, 0165, 0032, 0301, 0367, 
  0246, 0265, 0033, 0055, 0312, 0017, 0171, 0223, 0061, 0053, 0225, 0017, 0030, 0242, 0152, 0345, 
  0264, 0051, 0071, 0301, 0102, 0263, 0317, 0230, 0050, 0173, 0247, 0025, 0245, 0145, 0124, 0114, 
  0331, 0212, 0117, 0152, 0076, 0336, 0056, 0053, 0253, 0273, 0232, 0164, 0203, 0260, 0257, 0375, 
  0221, 0302, 0161, 0330, 0307, 0335, 0327, 0231, 0114, 0141, 0144, 0277, 0024, 0040, 0034, 0326, 
  0040, 0174, 0331, 0206, 0320, 0127, 0342, 0144, 0003, 0307, 0343, 0057, 0005, 0076, 0122, 0040, 
  0000, 0050, 0165, 0165, 0141, 0171, 0051, 0057, 0004, 0000, 0000, 0000, 0155, 0145, 0156, 0165, 
  0057, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 0003, 0000, 0000, 0000
} };
#else /* _MSC_VER */
static const SECTION union { const guint8 data[685]; const double alignment; void * const ptr;}  menu_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\000\001\000\000\000\000\000\050\010\000\000\000"
  "\000\000\000\000\001\000\000\000\001\000\000\000\003\000\000\000"
  "\006\000\000\000\007\000\000\000\007\000\000\000\010\000\000\000"
  "\260\267\044\060\004\000\000\000\000\001\000\000\006\000\114\000"
  "\010\001\000\000\014\001\000\000\322\005\350\052\000\000\000\000"
  "\014\001\000\000\014\000\114\000\030\001\000\000\034\001\000\000"
  "\272\272\177\171\001\000\000\000\034\001\000\000\010\000\114\000"
  "\044\001\000\000\050\001\000\000\203\251\176\033\007\000\000\000"
  "\050\001\000\000\020\000\166\000\070\001\000\000\327\001\000\000"
  "\113\120\220\013\006\000\000\000\327\001\000\000\004\000\114\000"
  "\334\001\000\000\340\001\000\000\223\075\033\260\007\000\000\000"
  "\340\001\000\000\021\000\166\000\370\001\000\000\227\002\000\000"
  "\324\265\002\000\377\377\377\377\227\002\000\000\001\000\114\000"
  "\230\002\000\000\234\002\000\000\136\063\061\332\002\000\000\000"
  "\234\002\000\000\005\000\114\000\244\002\000\000\254\002\000\000"
  "\147\156\157\155\145\057\000\000\001\000\000\000\147\156\157\155"
  "\145\055\160\141\156\145\154\057\002\000\000\000\155\157\144\165"
  "\154\145\163\057\007\000\000\000\155\145\156\165\055\142\141\162"
  "\055\155\145\156\165\056\165\151\003\001\000\000\001\000\000\000"
  "\170\332\175\117\061\016\302\060\014\334\373\212\310\173\311\007"
  "\334\154\214\274\001\071\255\221\054\045\101\112\334\201\337\343"
  "\264\052\145\100\154\167\076\337\371\214\122\224\353\203\146\016"
  "\203\163\230\271\254\116\226\011\072\030\043\325\261\003\350\232"
  "\251\215\147\225\147\331\231\161\121\316\007\061\112\252\125\342"
  "\252\354\012\145\236\040\121\344\004\116\053\225\226\110\051\046"
  "\033\276\270\101\270\137\027\121\167\263\350\206\376\143\373\023"
  "\105\333\141\010\107\255\013\133\300\326\355\127\000\372\263\031"
  "\372\257\326\350\273\045\014\266\160\276\375\006\072\273\121\100"
  "\000\050\165\165\141\171\051\157\162\147\057\000\000\000\000\000"
  "\155\141\151\156\055\155\145\156\165\055\155\145\156\165\056\165"
  "\151\000\000\000\000\000\000\000\005\001\000\000\001\000\000\000"
  "\170\332\175\217\075\016\302\060\014\205\367\236\042\312\016\271"
  "\200\233\215\221\063\040\247\065\222\245\304\110\211\073\160\173"
  "\234\040\132\006\304\142\371\363\317\363\063\260\050\325\073\056"
  "\024\047\347\240\220\154\216\327\331\027\144\071\165\032\301\367"
  "\246\265\033\055\312\017\171\223\061\053\225\017\030\242\152\345"
  "\264\051\071\301\102\263\317\230\050\173\247\025\245\145\124\114"
  "\331\212\117\152\076\336\056\053\253\273\232\164\203\260\257\375"
  "\221\302\161\330\307\335\327\231\114\141\144\277\024\040\034\326"
  "\040\174\331\206\320\127\342\144\003\307\343\057\005\076\122\040"
  "\000\050\165\165\141\171\051\057\004\000\000\000\155\145\156\165"
  "\057\000\000\000\005\000\000\000\003\000\000\000" };
#endif /* !_MSC_VER */

static GStaticResource static_resource = { menu_resource_data.data, sizeof (menu_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *menu_get_resource (void);
GResource *menu_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a usable way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for Win32 (x86) and x64 programs, as symbols on Win32 are prefixed
 * with an underscore but symbols on x64 are not.
 */
#ifdef _WIN64
#define G_MSVC_SYMBOL_PREFIX ""
#else
#define G_MSVC_SYMBOL_PREFIX "_"
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
