#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.action_button"), aligned (8)))
#else
# define SECTION
#endif

#ifdef _MSC_VER
static const SECTION union { const guint8 data[529]; const double alignment; void * const ptr;}  action_button_resource_data = { {
  0107, 0126, 0141, 0162, 0151, 0141, 0156, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 
  0030, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 0000, 0000, 0000, 0050, 0007, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0002, 0000, 0000, 0000, 0003, 0000, 0000, 0000, 
  0005, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 0006, 0000, 0000, 0000, 0113, 0120, 0220, 0013, 
  0002, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 0004, 0000, 0114, 0000, 0350, 0000, 0000, 0000, 
  0354, 0000, 0000, 0000, 0322, 0005, 0350, 0052, 0006, 0000, 0000, 0000, 0354, 0000, 0000, 0000, 
  0014, 0000, 0114, 0000, 0370, 0000, 0000, 0000, 0374, 0000, 0000, 0000, 0324, 0265, 0002, 0000, 
  0377, 0377, 0377, 0377, 0374, 0000, 0000, 0000, 0001, 0000, 0114, 0000, 0000, 0001, 0000, 0000, 
  0004, 0001, 0000, 0000, 0272, 0272, 0177, 0171, 0001, 0000, 0000, 0000, 0004, 0001, 0000, 0000, 
  0010, 0000, 0114, 0000, 0014, 0001, 0000, 0000, 0020, 0001, 0000, 0000, 0170, 0367, 0004, 0275, 
  0005, 0000, 0000, 0000, 0020, 0001, 0000, 0000, 0026, 0000, 0166, 0000, 0050, 0001, 0000, 0000, 
  0360, 0001, 0000, 0000, 0160, 0172, 0221, 0040, 0003, 0000, 0000, 0000, 0360, 0001, 0000, 0000, 
  0016, 0000, 0114, 0000, 0000, 0002, 0000, 0000, 0004, 0002, 0000, 0000, 0260, 0267, 0044, 0060, 
  0000, 0000, 0000, 0000, 0004, 0002, 0000, 0000, 0006, 0000, 0114, 0000, 0014, 0002, 0000, 0000, 
  0020, 0002, 0000, 0000, 0157, 0162, 0147, 0057, 0006, 0000, 0000, 0000, 0147, 0156, 0157, 0155, 
  0145, 0055, 0160, 0141, 0156, 0145, 0154, 0057, 0003, 0000, 0000, 0000, 0057, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0155, 0157, 0144, 0165, 0154, 0145, 0163, 0057, 0005, 0000, 0000, 0000, 
  0147, 0160, 0055, 0154, 0157, 0143, 0153, 0055, 0163, 0143, 0162, 0145, 0145, 0156, 0055, 0155, 
  0145, 0156, 0165, 0056, 0165, 0151, 0000, 0000, 0152, 0002, 0000, 0000, 0001, 0000, 0000, 0000, 
  0170, 0332, 0255, 0222, 0061, 0016, 0302, 0060, 0014, 0105, 0367, 0236, 0042, 0312, 0136, 0162, 
  0201, 0020, 0211, 0235, 0001, 0211, 0003, 0040, 0067, 0030, 0051, 0042, 0115, 0253, 0330, 0255, 
  0304, 0355, 0161, 0133, 0001, 0031, 0120, 0207, 0302, 0226, 0157, 0377, 0374, 0377, 0006, 0333, 
  0220, 0030, 0363, 0015, 0074, 0272, 0112, 0051, 0333, 0142, 0032, 0124, 0270, 0356, 0165, 0354, 
  0374, 0275, 0046, 0237, 0021, 0123, 0075, 0015, 0365, 0264, 0026, 0003, 0241, 0347, 0320, 0245, 
  0105, 0211, 0016, 0214, 0355, 0113, 0210, 0004, 0346, 0034, 0232, 0201, 0121, 0045, 0150, 0121, 
  0142, 0240, 0301, 0250, 0025, 0147, 0110, 0024, 0201, 0241, 0211, 0062, 0174, 0040, 0151, 0167, 
  0071, 0112, 0203, 0072, 0317, 0015, 0326, 0274, 0377, 0255, 0144, 0301, 0334, 0254, 0135, 0201, 
  0266, 0053, 0336, 0137, 0102, 0254, 0051, 0361, 0176, 0140, 0075, 0110, 0365, 0010, 0342, 0134, 
  0170, 0011, 0106, 0314, 0333, 0241, 0151, 0065, 0344, 0157, 0320, 0247, 0334, 0365, 0230, 0071, 
  0040, 0155, 0107, 0355, 0327, 0062, 0112, 0122, 0153, 0212, 0313, 0260, 0146, 0072, 0031, 0127, 
  0211, 0341, 0163, 0135, 0117, 0102, 0243, 0306, 0207, 0000, 0050, 0165, 0165, 0141, 0171, 0051, 
  0141, 0143, 0164, 0151, 0157, 0156, 0055, 0142, 0165, 0164, 0164, 0157, 0156, 0057, 0000, 0000, 
  0004, 0000, 0000, 0000, 0147, 0156, 0157, 0155, 0145, 0057, 0000, 0000, 0001, 0000, 0000, 0000
} };
#else /* _MSC_VER */
static const SECTION union { const guint8 data[529]; const double alignment; void * const ptr;}  action_button_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\344\000\000\000\000\000\000\050\007\000\000\000"
  "\000\000\000\000\000\000\000\000\002\000\000\000\003\000\000\000"
  "\005\000\000\000\005\000\000\000\006\000\000\000\113\120\220\013"
  "\002\000\000\000\344\000\000\000\004\000\114\000\350\000\000\000"
  "\354\000\000\000\322\005\350\052\006\000\000\000\354\000\000\000"
  "\014\000\114\000\370\000\000\000\374\000\000\000\324\265\002\000"
  "\377\377\377\377\374\000\000\000\001\000\114\000\000\001\000\000"
  "\004\001\000\000\272\272\177\171\001\000\000\000\004\001\000\000"
  "\010\000\114\000\014\001\000\000\020\001\000\000\170\367\004\275"
  "\005\000\000\000\020\001\000\000\026\000\166\000\050\001\000\000"
  "\360\001\000\000\160\172\221\040\003\000\000\000\360\001\000\000"
  "\016\000\114\000\000\002\000\000\004\002\000\000\260\267\044\060"
  "\000\000\000\000\004\002\000\000\006\000\114\000\014\002\000\000"
  "\020\002\000\000\157\162\147\057\006\000\000\000\147\156\157\155"
  "\145\055\160\141\156\145\154\057\003\000\000\000\057\000\000\000"
  "\000\000\000\000\155\157\144\165\154\145\163\057\005\000\000\000"
  "\147\160\055\154\157\143\153\055\163\143\162\145\145\156\055\155"
  "\145\156\165\056\165\151\000\000\152\002\000\000\001\000\000\000"
  "\170\332\255\222\061\016\302\060\014\105\367\236\042\312\136\162"
  "\201\020\211\235\001\211\003\040\067\030\051\042\115\253\330\255"
  "\304\355\161\133\001\031\120\207\302\226\157\377\374\377\006\333"
  "\220\030\363\015\074\272\112\051\333\142\032\124\270\356\165\354"
  "\374\275\046\237\021\123\075\015\365\264\026\003\241\347\320\245"
  "\105\211\016\214\355\113\210\004\346\034\232\201\121\045\150\121"
  "\142\240\301\250\025\147\110\024\201\241\211\062\174\040\151\167"
  "\071\112\203\072\317\015\326\274\377\255\144\301\334\254\135\201"
  "\266\053\336\137\102\254\051\361\176\140\075\110\365\010\342\134"
  "\170\011\106\314\333\241\151\065\344\157\320\247\334\365\230\071"
  "\040\155\107\355\327\062\112\122\153\212\313\260\146\072\031\127"
  "\211\341\163\135\117\102\243\306\207\000\050\165\165\141\171\051"
  "\141\143\164\151\157\156\055\142\165\164\164\157\156\057\000\000"
  "\004\000\000\000\147\156\157\155\145\057\000\000\001\000\000\000"
  "" };
#endif /* !_MSC_VER */

static GStaticResource static_resource = { action_button_resource_data.data, sizeof (action_button_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *action_button_get_resource (void);
GResource *action_button_get_resource (void)
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
