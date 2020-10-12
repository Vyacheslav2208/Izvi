#include<iostream>
#include<conio.h>
#include<time.h>

void generator() {

	short mas[10];

	for (char i = 0; i < 10; i++)
		mas[i] = 0;

	unsigned short  a, b = rand() % 65534;
	unsigned int r;

	printf("Введите первое число: ");
	std::cin >> a;

	for (int i = 0; i < 2000; i++) {

		r = a * b;

		printf("\nr = %.8x", r);

		r = r << 8;
		r = r >> 16;
		printf(" \tres = %.4x", r);
		printf(" \tres = %d", r);

		mas[r / 6553]++;
		a = b;
		b = r;
	}

	for (char i = 0; i < 10; i++)
		printf("\n %d\t- %d\t= %d ", 6553 * (i), 6553 * (i + 1) - 1, mas[i]);

}

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	do {
		system("cls");
		generator();
		printf("\n\n1 - Генерировать занова числа\n2 - Выход\n");
	} while (_getch() != '2');

	return 0;
}
