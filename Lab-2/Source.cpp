#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>
#include <math.h>

char key[32] = { 24, 6, 25, 31, 27, 19, 2, 26, 11, 3, 18, 30, 7, 5, 10, 16, 1, 17, 28, 20, 12, 0, 23, 4, 29, 13, 8, 14, 22, 9, 21, 15 };

void perevod(char buff[], int c, int d, int a, bool shif,  char buffOut[]) {
	unsigned char m[8], p, temp[32];
	int e = 0;
	
	for (char j = c; j < d; j++) {
		for (char k = 0; k < 4; k++) {
			
			if (k < a) {
				p = buff[k + j * 4];
				e = p;
			}		
			else 
				e = 48;
			
			for (char h = 7; h >= 0; h--) {
				if (e > 0)
				{
					m[h] = e % 2;
					e /= 2;
				}
				else
					m[h] = 0;
			}

			for (char l = 0; l < 8; l++) 
				if (shif)
					temp[key[l + k * 8]] = m[l];
				else 
					for (char i = 0; i < 32; i++)
						if (key[i] == l + k * 8) 
							temp[i] = m[l];
							
		}
		for (char k = 0; k < 4; k++) {

			e = 0;
			for (char h = 0; h < 8; h++) {

				e += (temp[k * 8 + 7 - h]) * pow(2, h);
			}

			buffOut[k + j * 4] = e;
		}
	}
}

int shifrator(const char source[], const char drain[]) {

	FILE* fp, * f;

	char buff[80], buffOut[80];

	//---------------------------------------------------------------
	// открытие для чтения 

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("Не возможно открыть файл.");
		return 1;
	}

	if ((f = fopen(drain, "wb")) == NULL) {
		printf("Не возможно открыть файл.");
		return 1;
	}

	//---------------------------------------------------------------

	strncpy(buffOut, " " , 8);
	strncpy(buff, "" , 80);
	fwrite(buffOut, 4, 1, f);

	//---------------------------------------------------------------

	int pos1 = ftell(fp);

	while (fread(buff, 80, 1, fp) != 0) {
		pos1 = ftell(fp);

		perevod(buff, 0, 20, 8, true, buffOut);
		
		fwrite(buffOut, sizeof buffOut, 1, f); 
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);									
	int pos2 = ftell(fp) - pos1;;                          
	fseek(fp, pos1, SEEK_SET);
	
	if (pos2 != 0 ) {
		int a = pos2 % 4, b = pos2 / 4, c = (b + (a == 0 ? 0 : 1)) * 4;
		
		fread(buff, pos2, 1, fp);
			
		perevod(buff, 0, b, 8, true, buffOut);

		if (a != 0) 
			perevod(buff, b, b + 1, a, true, buffOut);

		fwrite(buffOut, c, 1, f);

		//---------------------------------------------------------------

		for (char i = 0; i < a;) {
			buffOut[i] =  rand() % 60 + 30;

			if (buffOut[i] != ' ')
				i++;
		}

		fseek(f, 0, SEEK_SET);
		fwrite(buffOut, a, 1, f);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(f, 0, SEEK_END);

	printf("\n\nИсходный текст: %d ", ftell(fp));
	printf("\nЗашифрованный текст: %d \n", ftell(f));

	fclose(fp);
	fclose(f);

	return 0;
}

int deshifrator(const char source[], const char drain[]) {

	FILE* fp, * deshif;

	char buff[80], buffOut[80], plicity = 0;
	
	//---------------------------------------------------------------

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("Не возможно открыть файл.");
		return 1;
	}

	if ((deshif = fopen(drain, "wb")) == NULL) {
		printf("Не возможно открыть файл.");
		return 1;
	}

	//---------------------------------------------------------------

	fread(buffOut, 4, 1, fp);
	while (plicity < 4 && buffOut[plicity] != '\0' && buffOut[plicity] != ' ') {
		plicity++;
	}

	strncpy(buff, "", 80);
	strncpy(buffOut, "", 80);

	//---------------------------------------------------------------

	int pos1 = ftell(fp);
	while (fread(buff, 80, 1, fp) != 0) {
		pos1 = ftell(fp);

		perevod(buff, 0, 20, 8, false, buffOut);

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) - pos1 > 0)
			fwrite(buffOut, 80, 1, deshif);

		fseek(fp, pos1, SEEK_SET);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);                          // устанавливаем позицию в конец файла

	int pos2 = ftell(fp) - pos1;                            // получаем размер в байтах

	fseek(fp, pos1, SEEK_SET);

	if (pos2 == 0) {
		if (plicity != 0) {
			pos2 = 72 + plicity;
			fseek(deshif, pos1 - 88, SEEK_SET);
		}
		else
			pos2 = 80;

		fwrite(buffOut, pos2, 1, deshif);
	}
	else if (pos2 == 4 && plicity != 0) {

		perevod(buff, 0, plicity, 8, false, buffOut);
		fwrite(buffOut, pos2 + plicity - 4, 1, deshif);

	}
	else if (pos2 != 0) {

		perevod(buff, 0, pos2 / 4, 8, false, buffOut);

		if (plicity != 0)
			pos2 = pos2 - 4 + plicity;
		
		fwrite(buffOut, pos2, 1, deshif);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(deshif, 0, SEEK_END);


	printf("\nЗашифрованный текст: %d ", ftell(fp));
	printf("\nРасшифрованный текст: %d \n", ftell(deshif));

	fclose(fp);
	fclose(deshif);

	return 0;
}

int main(int argc, char* argv[]) {

	setlocale(0, "Rus");

	int d = atoi(argv[1]);
	if (d == 0) {
		shifrator(argv[2], argv[3]);
	}
	else if (d == 1) {
		deshifrator(argv[2], argv[3]);
	}

	_getch();
	return 0;
}