#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <windows.h>


int sizeText = 6;
char text[] = { "012345" };
char text2[50];
int temp = 0;
void CreateTestBMP(LPTSTR szFileName, PRGBTRIPLE pColor)
{
	// Объявим нужные структуры
	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(bfh));
	BITMAPINFOHEADER bih;
	BYTE Palette[1024]; // Палитра
	memset(Palette, 0, 1024);
	// Пусть у нас будет картинка размером 35 x 50 пикселей
	int Width = 101;
	int Height = 100;
	bfh.bfType = 0x4D42; // Обозначим, что это bmp 'BM'
	bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024; // Палитра занимает 1Kb, но мы его использовать не будем
	bfh.bfSize = bfh.bfOffBits +
		sizeof(*pColor) * Width * Height +
		Height * ((sizeof(*pColor) * Width) % 4); // Посчитае размер конечного файла
	memset(&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biBitCount = 24;
	bih.biClrUsed = 0;
	bih.biCompression = BI_RGB; // Без сжатия
	bih.biHeight = Height;
	bih.biWidth = Width;
	bih.biPlanes = 1; // Должно быть 1
	// Остальные поля остаются 0
	HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD RW;
	// Запишем заголовки
	WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
	WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);
	// Запишем палитру
	WriteFile(hFile, Palette, sizeof(Palette), &RW, NULL);
	// Запишем растр

	// Обеспечим выравнивание строки
	WriteFile(hFile, Palette, (sizeof(*pColor) * Width) % 4, &RW, NULL);

	for (int i = 0, p = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			if (i == 0 && j < 8) {
				if (sizeText & (1 << 0)) {
					pColor->rgbtRed |= (1 << 0);
				}
				else {
					pColor->rgbtRed &= ~(1 << 0);
				}
				if (sizeText & (1 << 1)) {
					pColor->rgbtGreen |= (1 << 0);
				}
				else {
					pColor->rgbtGreen &= ~(1 << 0);
				}
				if (sizeText & (1 << 2)) {
					pColor->rgbtBlue |= (1 << 0);
				}
				else {
					pColor->rgbtBlue &= ~(1 << 0);
				}
				sizeText = sizeText >> 3;
			}
			else {
				if (p % 8 == 0) {
					temp = 0;
					for (char k = 0; k < 3; k++) {
						temp = temp << 8;
						temp = temp + (unsigned char)text[k + (j - 8) * 3];
					}
				}
				p++;
				_asm {
					push eax
					mov eax, temp
					pop eax
				}

				if (temp & (1 << 0)) {
					pColor->rgbtRed |= (1 << 0);
				}
				else {
					pColor->rgbtRed &= ~(1 << 0);
				}
				if (temp & (1 << 1)) {
					pColor->rgbtGreen |= (1 << 0);
				}
				else {
					pColor->rgbtGreen &= ~(1 << 0);
				}
				if (temp & (1 << 2)) {
					pColor->rgbtBlue |= (1 << 0);
				}
				else {
					pColor->rgbtBlue &= ~(1 << 0);
				}
				temp = temp >> 3;

			}
			//pColor->rgbtRed = rand() % 255;
			WriteFile(hFile, pColor, sizeof(*pColor), &RW, NULL);
		}
		//	// Обеспечим выравнивание строки
		WriteFile(hFile, Palette, (sizeof(*pColor) * Width) % 4, &RW, NULL);
	}
	CloseHandle(hFile);
}




int openBMP(LPTSTR szFileName, PRGBTRIPLE pColor2)
{
	sizeText = 0;
	RGBTRIPLE pColor1 = { 0,0,0 };
	PRGBTRIPLE pColor = &pColor1;
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL;

	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// Read bitmap info
	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB')
	{
		CloseHandle(file);
		return NULL;
	}

	// get image measurements
	int Width = bmpinfo.biWidth;
	int Height = abs(bmpinfo.biHeight);

	// Check if bmp iuncompressed
	if (bmpinfo.biCompression != BI_RGB)
	{
		CloseHandle(file);
		return NULL;
	}

	// Check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24)
	{
		CloseHandle(file);
		return NULL;
	}

	// create buffer to hold the data
	int size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	temp = 0;
	for (int i = 0, p = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			ReadFile(file, pColor, sizeof(*pColor), &bytesread, NULL);

			if (i == 0 && j < 8) {

				if (pColor->rgbtRed & (1 << 0)) {
					sizeText |= (1 << 21);
				}
				else {
					sizeText &= ~(1 << 21);
				}
				if (pColor->rgbtGreen & (1 << 0)) {
					sizeText |= (1 << 22);
				}
				else {
					sizeText &= ~(1 << 22);
				}
				if (pColor->rgbtBlue & (1 << 0)) {
					sizeText |= (1 << 23);
				}
				else {
					sizeText &= ~(1 << 23);
				}
				sizeText = sizeText >> 3;
			}
			else {


				if (pColor->rgbtRed & (1 << 0)) {
					temp |= (1 << 21);
				}
				else {
					temp &= ~(1 << 21);
				}
				if (pColor->rgbtGreen & (1 << 0)) {
					temp |= (1 << 22);
				}
				else {
					temp &= ~(1 << 22);
				}
				if (pColor->rgbtBlue & (1 << 0)) {
					temp |= (1 << 23);
				}
				else {
					temp &= ~(1 << 23);
				}

				if (p % 8 == 0) {
					for (char k = 0; k < 3; k++) {
						text2[3 - k + j * 4] = temp;
						temp = temp >> 8;
					}
					temp = 0;
				}
				p++;


				//temp = temp << 3;
			}
			//pColor->rgbtRed = rand() % 255;
			//WriteFile(hFile, pColor, sizeof(*pColor), &RW, NULL);
		}
		//	// Обеспечим выравнивание строки
		//WriteFile(hFile, Palette, (sizeof(*pColor) * Width) % 4, &RW, NULL);
	}

	//if (ReadFile(file, Buffer, size, &bytesread, NULL) == false)
	//{
	//	delete[] Buffer;
	//	CloseHandle(file);
	//	return NULL;
	//}

	// everything successful here: close file and return buffer

	CloseHandle(file);

	for (char k = 0; k < sizeText; k++) {
		printf(" %d", text2[k]);
	}
	//// Объявим нужные структуры
	//BITMAPFILEHEADER bfh;
	//memset(&bfh, 0, sizeof(bfh));
	//BITMAPINFOHEADER bih;
	//BYTE Palette[1024]; // Палитра
	//memset(Palette, 0, 1024);
	//// Пусть у нас будет картинка размером 35 x 50 пикселей
	//int Width = 100;
	//int Height = 100;
	////bfh.bfType = 0x4D42; // Обозначим, что это bmp 'BM'
	////bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024; // Палитра занимает 1Kb, но мы его использовать не будем
	////bfh.bfSize = bfh.bfOffBits +
	////	sizeof(*pColor) * Width * Height +
	////	Height * ((sizeof(*pColor) * Width) % 4); // Посчитае размер конечного файла
	////memset(&bih, 0, sizeof(bih));
	////bih.biSize = sizeof(bih);
	////bih.biBitCount = 24;
	////bih.biClrUsed = 0;
	////bih.biCompression = BI_RGB; // Без сжатия
	////bih.biHeight = Height;
	////bih.biWidth = Width;
	////bih.biPlanes = 1; // Должно быть 1
	//// Остальные поля остаются 0

	//HANDLE hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL,
	//	OPEN_ALWAYS, 0, NULL);
	//if (hFile == INVALID_HANDLE_VALUE)
	//	return;
	//DWORD RW;
	//// Запишем заголовки
	//ReadFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
	//ReadFile(hFile, &bih, sizeof(bih), &RW, NULL);
	//// Запишем палитру
	//ReadFile(hFile, Palette, sizeof(Palette), &RW, NULL);
	//// Запишем растр

	//// Обеспечим выравнивание строки
	//ReadFile(hFile, Palette, (sizeof(*pColor) * Width) % 4, &RW, NULL);

	//for (int i = 0; i < Height; ++i)
	//{
	//	for (int j = 0; j < Width; ++j)
	//	{
	//		if (i == 0 && j < 8) {
	//			if (sizeText & (1 << 0)) {
	//				pColor->rgbtRed |= (1 << 0);
	//			}
	//			else {
	//				pColor->rgbtRed &= ~(1 << 0);
	//			}
	//			if (sizeText & (1 << 1)) {
	//				pColor->rgbtGreen |= (1 << 0);
	//			}
	//			else {
	//				pColor->rgbtGreen &= ~(1 << 0);
	//			}
	//			if (sizeText & (1 << 2)) {
	//				pColor->rgbtBlue |= (1 << 0);
	//			}
	//			else {
	//				pColor->rgbtBlue &= ~(1 << 0);
	//			}
	//			sizeText = sizeText >> 3;
	//		}
	//		else {
	//			pColor->rgbtRed = 100;
	//			pColor->rgbtGreen = 10;
	//			pColor->rgbtBlue = 1;
	//		}
	//		//pColor->rgbtRed = rand() % 255;
	//		//WriteFile(hFile, pColor, sizeof(*pColor), &RW, NULL);
	//	}
	//	//	// Обеспечим выравнивание строки
	//	//WriteFile(hFile, Palette, (sizeof(*pColor) * Width) % 4, &RW, NULL);
	//}
	//CloseHandle(hFile);
}




int wmain(int argc, _TCHAR* argv[])
{

	RGBTRIPLE color;
	color.rgbtRed = 0;
	color.rgbtGreen = 0;
	color.rgbtBlue = 0;
	wchar_t mas[] = { L"test.bmp" };
	CreateTestBMP(mas, &color);
	openBMP(mas, &color);
	printf("\n %d = sizeText ", sizeText);
	_getch();
	system("test.bmp");
	return 0;
}