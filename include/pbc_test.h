// Useful for tests.

#ifndef __PBC_TEST_H__
#define __PBC_TEST_H__

/*@manual test
Initializes pairing from file specified as first argument, or from standard
input if there is no first argument.
*/
static inline void pbc_demo_pairing_init(pairing_t pairing, int argc, char **argv) {
  char s[16384];
  FILE *fp = stdin;

   HINSTANCE hdll;
   hdll = LoadLibrary("pbc.dll");
   typedef void (*LPpbc_die)(char[], ...);
//  typedef int  (* LPpairing_init_set_buf)(pairing_t, const char[], size_t);
   LPpbc_die pbc_die;
//  LPpairing_init_set_buf pairing_init_set_buf;
//  pairing_init_set_buf = (LPpairing_init_set_buf)GetProcAddress(hdll,"pairing_init_set_buf");
   pbc_die = (LPpbc_die)GetProcAddress(hdll, "pbc_die");
// 

  if (argc > 1) {
    fp = fopen("F:\\pbc-0.5.8\\a.param", "r");
    if (!fp) pbc_die("error opening %s", argv[1]);
  }
  size_t count = fread(s, 1, 16384, fp);
  if (!count) pbc_die("input error");
  fclose(fp);

  if (pairing_init_set_buf(pairing, s, count)) pbc_die("pairing init failed");
}

/*@manual test
Returns seconds elapsed since the first call to this function.
Returns 0 the first time.
*/
double pbc_get_time(void);

/*@manual test
Macro: if `condition` evaluates to 0 then print an error.
*/
#define EXPECT(condition) \
  if (condition); else pbc_err_count++, fprintf(stderr, "\n*** FAIL ***\n  %s:%d: %s\n\n", __FILE__, __LINE__, #condition)

/*@manual test
Total number of failed EXPECT checks.
*/
int pbc_err_count;

#endif //__PBC_TEST_H__
