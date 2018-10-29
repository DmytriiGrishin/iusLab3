#include "aduc812.h"
#include "system.h"

unsigned char rsiostat(void);

void wsio( unsigned char c );

unsigned char rsio(void);

void SendString(const char * str);