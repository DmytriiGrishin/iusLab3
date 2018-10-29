#include "aduc812.h"
#include "max.h"

unsigned char dip(void) {
  return 255 - read_max(EXT_LO);
}
