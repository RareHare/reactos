
#include <mbstring.h>
#include <mbctype.h>
#include <ctype.h>


/*
 * @implemented
 */
int _ismbbpunct(unsigned int c)
{
// (0xA1 <= c <= 0xA6)
  return (ispunct(c) || _ismbbkana(c));
}

 //iskana()     :(0xA1 <= c <= 0xDF)
 //iskpun()     :(0xA1 <= c <= 0xA6)
 //iskmoji()    :(0xA7 <= c <= 0xDF)
