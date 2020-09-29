#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>
#include <math.h>

int pow2to16(int h) {
	int e = 1;

	for (int i = 0; i < h; i++) {
		e = e + i * 16;
	}
	return e;
}

//char key[32] = { 28, 29, 30, 31, 24, 25, 26, 27, 20, 21, 22, 23, 16, 17, 18, 19, 12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3 };
char key[32] = { 24, 25, 26, 27, 28, 29, 30, 31,  16, 17, 18, 19, 20, 21, 22, 23,  8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7 };
int sab = 1, bas = 0;
void perevod(char buff[], int j, int a,  char buffOut[]) {
	unsigned char m[8], p[2], temp[32];
	int e = 0;
	for (int k = 0; k < 4; k++) {
		if (k < a) {
			p[0] = buff[k + j * 4];
			e = p[0];
			//e = atoi(&p[0]);
		}
		else {
			e = 48;
		}


		for (int h = 7; h >= 0; h--) {
			if (e > 0)
			{
				m[h] = e % 2;
				//m[h] = e % 2;
				e /= 2;
			}
			else
				m[h] = 0;
		}
		for (int l = 0; l < 8; l++) {
			//temp[key[l + k * 8]] = m[l];
			temp[l + k * 8] = m[l];
			//temp[l + k * 8] = m[l];
		}
	}
	for (int k = 0; k < 4; k++) {

		e = 0;
		for (int h = 0; h < 8; h++) {

			e += (temp[k * 8 + 7 - h]) * pow(2, h);
		}
		p[0] = e;
		//_itoa(e, &p[0], 10);
		buffOut[k + j * 4] = p[0];
	}
}

void perevod2(char buff[], int j,  int b, char buffOut[]) {
	unsigned char m[8], p[2], temp[32];
	int e = 0;

	//for (int j = 0; j < b; j++) {
		for (int k = 0; k < 4; k++) {

			p[0] = buff[k + j * 4];
			e = p[0];


			for (int h = 7; h >= 0; h--) {
				if (e > 0)
				{
					m[h] = e % 2;
					//m[h] = e % 2;
					e /= 2;
				}
				else
					m[h] = 0;
			}
			for (int l = 0; l < 8; l++) {

				temp[l + k * 8] = m[l];
				//temp[key[l + k * 8]] = m[l];
			}
		}
		for (int k = 0; k < 4; k++) {
			e = 0;
			for (int h = 0; h < 8; h++) {
				e += (temp[k * 8 + 7 - h]) * pow(2, h);
			}
			p[0] = e;
			buffOut[k + j * 4] = p[0];
		}
	//}
}

int shifrator(const char source[], const char drain[]) {

	FILE* fp, * f;

	char buff[80], buffOut[80];

	//---------------------------------------------------------------
	// открытие для чтения 
	//srand(time(NULL));
	//FILE* pq;
	//if ((pq = fopen(source, "wb")) == NULL) {
	//	printf("Cannot open file.");
	//	return 0;
	//}
	//char pw[] = { "12345678" };
	//for (int j = 0; j < sab; j++) {
	//		if (j == sab - 1) fwrite(pw, bas, 1, pq);
	//		else fwrite(pw, 8, 1, pq);
	//	}
	////fwrite(pw, 10, 1, pq);
	//fclose(pq);

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("cannot open file");
		return 1;
	}

	if ((f = fopen(drain, "wb")) == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	//---------------------------------------------------------------

	strncpy(buffOut, " " , 8);
	strncpy(buff, "" , 80);

	//---------------------------------------------------------------

	int pos1 = ftell(fp);

	while (fread(buff, 80, 1, fp) != 0) {
		pos1 = ftell(fp);

		for (int j = 0; j < 20; j++) {

			perevod(buff, j, 8, buffOut);

		}
		
		fwrite(buffOut, sizeof buffOut, 1, f); //strcpy(buffOut, buff); 	//fwrite(buff, sizeof buff, 1, f);
	
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);									// устанавливаем позицию в конец файла
	int pos2 = ftell(fp) - pos1;;                           // получаем размер в байтах
	fseek(fp, pos1, SEEK_SET);
	
	if (pos2 != 0 ) {
		int a = pos2 % 4, b = pos2 / 4, c = (b + (a == 0 ? 0 : 1)) * 4;
		strcpy(buffOut, buff);
		fread(buff, pos2, 1, fp);
		for (int j = 0; j < b; j++) {
			
			perevod(buff, j, 8, buffOut);
			
		}

		if (a != 0) {

			perevod(buff, b, a, buffOut);

		}

		fwrite(buffOut, c, 1, f);

		//---------------------------------------------------------------

		for (char i = 0; i < a;) {
			buffOut[i] =  rand() % 60 + 30;

			if (buffOut[i] != ' ')
				i++;
		}

		fseek(f, 0, SEEK_SET);
		//fwrite(buffOut, a, 1, f);

	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(f, 0, SEEK_END);

	setlocale(0, "Rus");
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
		printf("Cannot open file.");
		return 1;
	}

	if ((deshif = fopen(drain, "wb")) == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	//---------------------------------------------------------------

	// fread(buffOut, 4, 1, fp);
	strncpy(buffOut, "", 80);
	strncpy(buff, "", 80);
	/*while (plicity < 4 && buffOut[plicity] != '\0' && buffOut[plicity] != ' ') {
		plicity++;
	}*/

	//---------------------------------------------------------------

	int pos1 = ftell(fp);
	while (fread(buff, 80, 1, fp) != 0) {
		pos1 = ftell(fp);
		//perevod2(buff, 20, buffOut);

		for (int j = 0; j < 20; j++) {

			perevod2(buff, j, 8, buffOut);

		}

		//strcpy(buffOut, buff);

		fseek(fp, 0, SEEK_END);
		if (ftell(fp) - pos1 > 0)
			fwrite(buffOut, 80, 1, deshif);

		fseek(fp, pos1, SEEK_SET);
	}

	//while (fread(buff, 80, 1, fp) != 0) {
	//	pos1 = ftell(fp);

	//	for (int j = 0; j < 20; j++) {

	//		perevod2(buff, j, 8, buffOut);

	//	}

	//	fwrite(buffOut, sizeof buffOut, 1, deshif); //strcpy(buffOut, buff); 	//fwrite(buff, sizeof buff, 1, f);

	//}

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


		for (int j = 0; j < plicity; j++) {

			perevod2(buff, j, 8, buffOut);

		}

		//perevod2(buff, plicity, buffOut);
		pos2 = pos2 + plicity - 4;
		fwrite(buffOut, pos2, 1, deshif);

	}
	else if (pos2 != 0) {

		for (int j = 0; j < pos2 / 4; j++) {
			perevod2(buff, j, plicity, buffOut);
		}
		if (plicity != 0)
			pos2 = pos2 - 4 + plicity;
		
		fwrite(buffOut, pos2, 1, deshif);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(deshif, 0, SEEK_END);

	setlocale(0, "Rus");
	printf("\nЗашифрованный текст: %d ", ftell(fp));
	printf("\nРасшифрованный текст: %d \n", ftell(deshif));

	fclose(fp);
	fclose(deshif);

	return 0;
}

void hyi(const char source[]) {
	FILE* fp;
	if ((fp = fopen(source, "wb")) == NULL) {
		printf("Cannot open file.");
	}
	char mas[2];
	for (int a = 0; a < 255; a++) {
		mas[0] = a;
		fwrite(mas, 1, 1, fp);

	}
	


	fclose(fp);

}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	a1:
	sab += 1;
	//sab=11;
	bas = rand() % 9;
	//bas = 0;
	//for (int j = 0; j < 1000; j++) {
		sab += 1;
		//sab=11;
		bas = rand() % 9;
		char mas1[] = {"111.exe"};
		char mas2[] = {"db2.txt"};
		char mas3[] = {"db3.txt"};
		char mas4[] = {"\n fc  "};
		//hyi(mas1);
		shifrator(mas1,mas2);
		deshifrator(mas2, mas3);
		strcat(mas4, mas1);
		strcat(mas4, "  ");
		strcat(mas4, mas3);
			if( system("\n fc 111.exe db3.txt") != 0 )
			{
				printf("Залупа!");
			_getch();
			 }
		//}
	_getch();
	goto a1;
	return 0;
}



/*

	if (pos2 == 0) {
		if (plicity != 0) {
			pos2 = 72 + plicity;
			fseek(deshif, pos1 - 88, SEEK_SET);
		}
		else
			pos2 = 80;

		fwrite(buffOut, pos2, 1, deshif);
	}
	else if (pos2 == 8 && plicity != 0) {

		pos2 = pos2 + plicity - 8;
		fwrite(buffOut, pos2, 1, deshif);

	}
	else if (pos2 != 0) {
		perevod2(buff, pos2/8, buffOut);
		if (plicity != 0)
			pos2 = pos2 - 8 + plicity;

		fwrite(buffOut, pos2, 1, deshif);
	}

	*/