#include "aduc812.h"
#include "async.h"
#include "sio.h"
#include "sync.h"
#include "system.h"
#include "led.h"

#define ERR_OUT_OF_RANGE "\n\rnumber not 0-255\n\r\0"
#define ERR_INVALID_CHAR "\n\rinvalid operator\n\r\0"
#define READ_OK 0
#define READ_OUT_OF_RANGE_ERROR 1
#define READ_INVALID_CHAR_ERROR 2

#define ONE_CODE	49
#define ZERO_CODE	48

// Производит необходимую трансформацию литеральных символов
// Если русский литерал в верхнем регистре - вернуть тот же символ в нижнем регистре
// Если английский символ в нижнем регистре - вернуть тот же символ в верхнем регистре
// Вход: символ
// Выход: трансформированный символ
unsigned char TransformLetter(unsigned char symb) {
	if (symb >= 97 && symb <= 122) { // анлийский литерал нижний регистр
		return symb - 32;
	} else if (symb >= 128 && symb <= 143) { // русский литерал в верхнем, 1 часть
		return symb + 32;
	} else if (symb >= 144 && symb <= 159) { // русский литерал в верхнем, 2 часть
		return symb + 80;
	}
	return symb;
}

// Определяет, является ли символ десятичной цифрой
// Вход: проверяемый символ
// Выход: 0 - не является
//		  1 - является
bit IsDigit(unsigned char symb) {
	return symb >= '0' && symb <= '9';
}


unsigned char read_uart_symbol() {
	unsigned char c = 0;
	c = ReadUART();
	while (c == 0) {
		if (GetDIP() != 128)
		return 0;
		c = ReadUART();
	}
	return c;
}

// Функция последовательно считывает до 3х символов из последовательного канала
// Вход: 	signed char* num - указатель на число (используется для возврата
//			результата)
// 			signed char operator - оператор
// Выход: 	результат выполнения операции
signed char ReadNumber(unsigned short *num) {
	unsigned char c = 0, i = 0;
	*num = 0; 
	for (;i < 3; i++) {
		if (IsDigit(c = read_uart_symbol())) {
			WriteUART(c);
			*num *= 10;
			*num += c - '0'; 
		} else {
			if (c == '\r') {
				return READ_OK;
			}
			if (GetDIP() != 128)
			return 0;	
			WriteUART(c);
			WriteUART('\n');
			return READ_INVALID_CHAR_ERROR;
		}
	}
	if ((c = read_uart_symbol()) == '\r') {
		return *num > 255 ? READ_OUT_OF_RANGE_ERROR :  READ_OK;
 	} else {
		WriteUART(c);
		return IsDigit(c) ? READ_OUT_OF_RANGE_ERROR : READ_INVALID_CHAR_ERROR;
 	}
	
}


void OutputResult(unsigned short a) {
	unsigned char res[8], n; // TODO: remove later ?
	char i;
	WriteUART('=');
	leds(a);
	for (i = 7; i >= 0; i--) {
		n = a >> i;
		res[i] = (n & 1) ? ONE_CODE : ZERO_CODE;
		WriteUART(res[i]);
	}
}

// Преобразует десятичные числа в диапозоне [0, 255] 
// в двоичную систему счисления
void Calc(void) {
	unsigned short a = 0;
	if (GetDIP() != 128)
		return;
	switch (ReadNumber(&a)) {
		case READ_OK:
			if (GetDIP() != 128)
			return;
			OutputResult(a);
			break;
		case READ_INVALID_CHAR_ERROR:
			if (GetDIP() != 128)
			return;
			SendString(ERR_INVALID_CHAR);
			return;
		case READ_OUT_OF_RANGE_ERROR:
			if (GetDIP() != 128)
			return;
			SendString(ERR_OUT_OF_RANGE);
			return;
	}
			
	WriteUART('\r');
	WriteUART('\n');
}

void main(void) {
	unsigned char c;
	unsigned char i = 0;

	// TODO: init speed (???)
	init_sio(S9600);	
	
	while (1) {
		if (GetDIP() == 128) {
			ES = 1;
			EA = 1;
			while (1) {
				Calc();
				if (GetDIP() != 128) {
					ES = 0;
					break;
				}
			}
		}
		if (GetDIP() == 1) {
			ES = 0;
			EA = 0;
			while (1) {
				if (rsiostat()) {
					c = rsio();
					wsio(c);
					wsio('\n');
					if (GetDIP() != 1)
						break;
					
					c = TransformLetter(c);
					
					// write transformed letter 3 times
					for (i = 0; i < 3; i++) {
						wsio(c);
					}
					wsio('\r');
					wsio('\n');
					if (GetDIP() != 1)
						break;
				}
			}
		}
	}
}