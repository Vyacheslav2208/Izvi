#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <time.h>

char key[10] = { 2, 8, 10, 4, 3, 7, 1, 6, 9, 5 };

int shifrator(const char source[], const char drain[]) {

	FILE* fp, * f;

	char buff[256], buffOut[256];

	srand(time(NULL));

	//---------------------------------------------------------------
	// �������� ��� ������ 

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
	strncpy(buff, "" , 256);
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

	fseek(fp, 0, SEEK_END);                          // ������������� ������� � ����� �����
	int pos2 = ftell(fp) - pos1;;                            // �������� ������ � ������
	fseek(fp, pos1, SEEK_SET);
	if (pos2 != 0) {
		int a = pos2 % 8, b = pos2 / 8, c = (b + (a == 0 ? 0 : 1)) * 8, d = b * 8 + a;

		fread(buff, pos2, 1, fp);

		//for (int j = 0; j < b; j++)
		//	for (int r = 0; r < 32; r++)
		//		//buffOut[r + j * 32] = buff[key[r] - 1 + j * 32];



		//if (a != 0) {
		//	for (int r = 0; r < 32; r++) {
		//		if (key[r] - 1 < a)
		//			buffOut[r + b * 32] = buff[key[r] - 1 + b * 32];
		//		else
		//			buffOut[r + b * 32] = ' ';
		//	}

		//}

		//fwrite(buffOut, c, 1, f);
		fwrite(buff, c, 1, f);

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
	printf("\n\n�������� �����: %d", ftell(fp));
	printf("\n������������� �����: %d", ftell(f));

	

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

	fseek(fp, 0, SEEK_END);                          // ������������� ������� � ����� �����

	int pos2 = ftell(fp) - pos1;                            // �������� ������ � ������

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
	printf("\n������������� �����: %d", ftell(fp));
	printf("\n�������������� �����: %d", ftell(deshif));

	fclose(fp);
	fclose(deshif);

	return 0;
}

int main(int argc, char* argv[]) {

		shifrator("db1.txt", "db2.txt");

	_getch();
	return 0;
}

