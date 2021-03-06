#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>
#include <math.h>

char key[32] = { 24, 10, 2, 8, 21, 26, 28, 0, 18, 12, 11, 25, 5, 22, 1, 16, 3, 9, 13, 4, 19, 17, 6, 15, 14, 20, 7, 23, 30, 31, 27, 29 };
//char key[32] = { 24, 6, 25, 31, 27, 19, 2, 26, 11, 3, 18, 30, 7, 5, 10, 16, 1, 17, 28, 20, 12, 0, 23, 4, 29, 13, 8, 14, 22, 9, 21, 15 };
//char key[32] = { 24, 25, 26, 27, 28, 29, 30, 31,  16, 17, 18, 19, 20, 21, 22, 23,  8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7 };
void perevod(char buff[], int c, int d, int a, bool shif, char buffOut[]) {
	unsigned int m = 0, out = 0;
	for (char j = c; j < d; j++) {
		for (char k = 0; k < 4; k++) {
			m = m << 8;
			if (k < a) {
				m = m + (unsigned char)buff[k + j * 4];
			}
			else 
				m = m + 48;
		}

		for (int i = 31; i >= 0; i--) {
			if (shif) {
				if (m & (1 << 31 - key[i])) 
					out |= (1 << 31 - i);
				
			}
			else {
				for (char t = 0; t < 32; t++) {
					if (key[t] == i) {
						if (m & (1 << 31 - t)) {
							out |= (1 << 31 - i);
						}
						break;
					}
				}
				
			}
		}

		for (char k = 0; k < 4; k++) {
				buffOut[3 - k + j * 4] = out;
				out = out >> 8;
		}
	}
}

int shifrator(const char source[], const char drain[]) {

	FILE* fp, * f;

	char buff[80], buffOut[80];

	//---------------------------------------------------------------
	// �������� ��� ������ 

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("�� �������� ������� ����.");
		return 1;
	}

	if ((f = fopen(drain, "wb")) == NULL) {
		printf("�� �������� ������� ����.");
		return 1;
	}

	//---------------------------------------------------------------

	strncpy(buffOut, " ", 8);
	strncpy(buff, "", 80);
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

	if (pos2 != 0) {
		int a = pos2 % 4, b = pos2 / 4, c = (b + (a == 0 ? 0 : 1)) * 4;

		strncpy(buffOut, "", 80);
		fread(buff, pos2, 1, fp);

		perevod(buff, 0, b, 8, true, buffOut);

		if (a != 0)
			perevod(buff, b, b + 1, a, true, buffOut);

		fwrite(buffOut, c, 1, f);

		//---------------------------------------------------------------

		for (char i = 0; i < a;) {
			buffOut[i] = rand() % 60 + 30;

			if (buffOut[i] != ' ')
				i++;
		}

		fseek(f, 0, SEEK_SET);
		fwrite(buffOut, a, 1, f);
	}

	//---------------------------------------------------------------

	fseek(fp, 0, SEEK_END);
	fseek(f, 0, SEEK_END);

	printf("\n\n�������� �����: %d ", ftell(fp));
	printf("\n������������� �����: %d \n", ftell(f));

	fclose(fp);
	fclose(f);

	return 0;
}

int deshifrator(const char source[], const char drain[]) {

	FILE* fp, * deshif;

	char buff[80], buffOut[80], plicity = 0;

	//---------------------------------------------------------------

	if ((fp = fopen(source, "rb")) == NULL) {
		printf("�� �������� ������� ����.");
		return 1;
	}

	if ((deshif = fopen(drain, "wb")) == NULL) {
		printf("�� �������� ������� ����.");
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

	fseek(fp, 0, SEEK_END);                          // ������������� ������� � ����� �����

	int pos2 = ftell(fp) - pos1;                            // �������� ������ � ������

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


	printf("\n������������� �����: %d ", ftell(fp));
	printf("\n�������������� �����: %d \n", ftell(deshif));

	fclose(fp);
	fclose(deshif);

	return 0;
}




int main(int argc, char* argv[]) {

	setlocale(0, "Rus");

	//char mas1[] = { "mi.txt" };
	//char mas2[] = { "db2.exe" };
	//char mas3[] = { "db3.exe" };
	//
	//system("\n fc mi.txt db3.exe");



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