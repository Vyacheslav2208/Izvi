#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>
#include <math.h>

char key[32] = { 28, 29, 30, 31, 24, 25, 26, 27, 20, 21, 22, 23, 16, 17, 18, 19, 12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3 };

int shifrator(const char source[], const char drain[]) {

	FILE* fp, * f;

	char buff[80], buffOut[80], temp[32];

	srand(time(NULL));

	//---------------------------------------------------------------
	// открытие для чтения 

	FILE* pq;
	if ((pq = fopen(source, "wb")) == NULL) {
		printf("Cannot open file.");
		return 0;
	}
	char pw[] = { "12345678" };
	fwrite(pw, 8, 1, pq);
	fclose(pq);

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("cannot open file");
		return 1;
	}

	if ((f = fopen(drain, "wb")) == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	////---------------------------------------------------------------

	//for (int j = 0; j < 32; j++)
		//buffOut[j] = ' ';
	strncpy(buffOut, " " , 8);
	strncpy(buff, "" , 80);
	fwrite(buffOut, 8, 1, f);


	//---------------------------------------------------------------

	int pos1 = ftell(fp);

	while (fread(buff, sizeof buff, 1, fp) != 0) {
		pos1 = ftell(fp);

		/*for (int j = 0; j < 10; j++)
			for (int r = 0; r < 10; r++)
				buffout[r + j * 10] = buff[key[r] - 1 + j * 10];*/

		fwrite(buffOut, sizeof buffOut, 1, f);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);                          // устанавливаем позицию в конец файла
	int pos2 = ftell(fp) - pos1;;                            // получаем размер в байтах
	fseek(fp, pos1, SEEK_SET);
	if (pos2 != 0) {
		int a = pos2 % 8, b = pos2 / 8, c = (b + (a == 0 ? 0 : 1)) * 8, d = b * 8 + a, e;

		fread(buff, pos2, 1, fp);
		char m[4], p[2], * s = &p[0];
		for (int j = 0; j < b; j++) {
			for (int k = 0; k < 8; k++) {
				p[0] = buff[k + j * 8];
				e = atoi(s);


				for (int h = 3; h >= 0 ; h--) {
					if (e > 0)
					{
						m[h] = e % 2;
						e /= 2;
					}
					else
						m[h] = 0;
				}
				for (int l = 0; l < 4; l++) {
					temp[key[l + k * 4]] = m[l];
				}
				
			}
			for (int k = 0; k < 8; k++) {
				
				e = 0;
				for (int h = 0; h < 4; h++) {
					
					e += temp[k * 4 + 3 - h] * pow(2, h);
				}
				_itoa(e, s, 10);
				buffOut[k + j * 8] = p[0];
			}
			
		}
		int huy = 0;
			/*for (int r = 0; r < 32; r++)
				buffOut[r + j * 32] = buff[key[r] - 1 + j * 32];*/



		//if (a != 0) {
		//	for (int r = 0; r < 32; r++) {
		//		if (key[r] - 1 < a)
		//			buffOut[r + b * 32] = buff[key[r] - 1 + b * 32];
		//		else
		//			buffOut[r + b * 32] = ' ';
		//	}

		//}

		//fwrite(buffOut, c, 1, f);
		fwrite(buffOut, c, 1, f);

		//---------------------------------------------------------------

		for (char i = 0; i < a;) {
			buffOut[i] = buff[(rand() % 9) + (rand() % 9) * 8] + rand() % 9 + 1;

			if (buffOut[i] != ' ')
				i++;
		}

		fseek(f, 0, SEEK_SET);
		fwrite(buffOut, a, 1, f);

	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(f, 0, SEEK_END);

	setlocale(0, "Rus");
	printf("\n\nИсходный текст: %d", ftell(fp));
	printf("\nЗашифрованный текст: %d", ftell(f));

	

	fclose(fp);
	fclose(f);

	return 0;
}

int deshifrator(const char source[], const char drain[]) {

	FILE* fp, * deshif;

	char buff[100], buffOut[100];
	char plicity = 0;

	//---------------------------------------------------------------

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	if ((deshif = fopen(drain, "wb")) == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	//---------------------------------------------------------------

	fread(buffOut, 10, 1, fp);

	while (plicity < 10 && buffOut[plicity] != ' ') {
		plicity++;
	}

	//---------------------------------------------------------------

	int pos1 = ftell(fp);
	while (fread(buff, sizeof buff, 1, fp) != 0) {
		pos1 = ftell(fp);

		for (int j = 0; j < 10; j++)
			for (int r = 0; r < 10; r++)
				buffOut[key[r] - 1 + j * 10] = buff[r + j * 10];

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) - pos1 > 0)
			fwrite(buffOut, sizeof buffOut, 1, deshif);

		fseek(fp, pos1, SEEK_SET);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);                          // устанавливаем позицию в конец файла

	int pos2 = ftell(fp) - pos1;                            // получаем размер в байтах

	fseek(fp, pos1, SEEK_SET);

	if (pos2 == 0) {
		if (plicity != 0) {
			pos2 = 90 + plicity;
			fseek(deshif, pos1 - 110, SEEK_SET);
		}
		else
			pos2 = 100;

		fwrite(buffOut, pos2, 1, deshif);
	}
	else if (pos2 == 10 && plicity != 0) {

		pos2 = pos2 + plicity - 10;
		fwrite(buffOut, pos2, 1, deshif);

	}
	else if (pos2 != 0) {

		if (plicity != 0)
			pos2 = pos2 - 10 + plicity;

		fwrite(buff, pos2, 1, deshif);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(deshif, 0, SEEK_END);

	setlocale(0, "Rus");
	printf("\nЗашифрованный текст: %d", ftell(fp));
	printf("\nРасшифрованный текст: %d", ftell(deshif));

	fclose(fp);
	fclose(deshif);

	return 0;
}

int main(int argc, char* argv[]) {

		shifrator("db1.txt", "db2.txt");

	_getch();
	return 0;
}

