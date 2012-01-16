#ifndef __UTILS_H__
#define __UTILS_H__

/* import a few common headers */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

/* dummy macro for D like casting */
#define cast(x) (x)

/* if you need to handle some expections undef this macro and define own macro or function
 * but remember to define it back for another functions
**/
#define assert_catch() exit(EXIT_FAILURE)

/* a few lazy variants of printf */
#define printfln(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#define print(str) printf("%s", str)
#define println(str) puts(str)

#define fatal(msg) {                                                                               \
  fprintf(stderr, "%s, %d Fatal Error: %s\n", __FILE__, __LINE__, msg);                            \
  assert_catch();                                                                                  \
}


/* Error test macros TODO: Add some backtracing 
**________________________________________________________________________________________________*/

/* fatal error, when expr != 0 */
#define assertz(expr, ...) if(expr) {                                                              \
  int __errno = errno;                                                                             \
  if(errno) {                                                                                      \
    fprintf(                                                                                       \
      stderr, "%s, %d Fatal error "__VA_ARGS__": %s, %d\n",                                        \
      __FILE__, __LINE__,                                                                          \
      strerror(errno),                                                                             \
      __errno                                                                                      \
    );                                                                                             \
    assert_catch();                                                                                \
  }                                                                                                \
  else {                                                                                           \
    fatal( strlen(#__VA_ARGS__) ? #__VA_ARGS__ : #expr);                                           \
  }                                                                                                \
}

/* fatal error, when a == b */
#define assertv(a, b, ...) assertz((a) == (b), __VA_ARGS__)

/* fatal error, when expr == 0 */
#define assertnz(expr, ...) assertz(!(expr), __VA_ARGS__)

/* fatal error, when expr == -1 */
#define asserts(expr, ...) assertz((expr) == -1, __VA_ARGS__)

/* a few lazy typedefs */
typedef unsigned int   uint   ;
typedef unsigned long  ulong  ;
typedef unsigned short ushort ;
typedef unsigned char  ubyte  ;
typedef          char  byte   ;
typedef char*          string ;

/* Note: TRUE is just 1 so use BOOL to compare the value */
typedef enum {
  FALSE = 0,
  TRUE  = 1
} bool;

#define BOOL(expr) ((expr) == 0 ? FALSE : TRUE)

/* transitive const string */
typedef char const* const const_string_t;

/* transitive const string vector */
typedef char const* const* const const_stringv_t;


#endif /* __UTILS_H__ */
