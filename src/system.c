#include "aduc812.h"

#define MAXBASE 8

// Запись байта в регистр ПЛИС
//Вход:
// regnum - адрес регистра ПЛИС
// val - записываемое значение
//Выход: нет
void WriteMax(unsigned char xdata *regnum, unsigned char val) {
	unsigned char oldDPP = DPP;
	DPP = MAXBASE;
	*regnum = val;
	DPP = oldDPP;
}

//Функция установки состояния линейки светодиодов
//Вход:
// value - состояние светодиодов
//Выход: нет
void WriteLED(unsigned char value) { WriteMax(7, value); }

// Чтение байта из регистра ПЛИС
//Вход:
// regnum - адрес регистра ПЛИС
// Результат: прочитанное значение
unsigned char ReadMax(unsigned char xdata *regnum) {
	unsigned char oldDPP = DPP;
	unsigned char val1;
	DPP = MAXBASE;
	val1 = *regnum;
	DPP = oldDPP;
	return val1;
}

//Функция чтения состояния DIP - перключателей
//Вход: нет
//Выход:
//состояние DIP-переключателей
unsigned char GetDIP() {
	unsigned char val = ReadMax(2);
	return ~val;
}