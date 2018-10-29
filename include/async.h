#include "aduc812.h" 


//#define FIFOSize 16

void SetVector(unsigned char xdata * Address, void * Vector);
void SIO_ISR( void ) __interrupt ( 4 );

//struct FIFOb{
//	unsigned char buf[FIFOSize];
//	char RP;
//	char WP;
//};

//struct FIFOb wFIFO, rFIFO;
//bit TRANSFER_NOW;				//Флаг для разрешения проблемы начальной передачи


void init_sio( unsigned char speed );

bit PushFIFO(struct FIFOb* a, unsigned char c);

unsigned char PopFIFO(struct FIFOb* a);

bit WriteUART(unsigned char c);

void APIString(const unsigned char* str);

unsigned char ReadUART(void);