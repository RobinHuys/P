#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t DWORD;	// DWORD = unsigned integer 32 bit value
typedef uint16_t WORD;	// WORD = unsigned integer 16 bit value
typedef int32_t LONG;	// LONG = signed integer 32 bit value

#pragma pack(push, 1) //disables padding
typedef struct temp_save_rgb //struct to save 1 byte value temporarily
{
	unsigned char temp_rgb;
}RGBSAVE;

#pragma pack(pop) //struct to save fileheader

#pragma pack(push, 1) //disables padding
typedef struct bmp_header
{
	WORD hType;		//file type
	DWORD hSize;	//file size (bytes)
	WORD hZero;		//reserved (0)
	WORD hZero2;	//reserved (0)
	DWORD hOffset;	//offset fileheader to bitmap (bits)
}HEADER;

#pragma pack(pop) //struct to save infoheader

#pragma pack(push, 1) //disables padding
typedef struct info_header
{
	DWORD iSize;		//struct size (byte)
	LONG iWidth;		//bitmap width (pixel)
	LONG iHeight;		//bitmap height (pixel)
	WORD iColorPlane;	//amount of color planes (1)
	WORD iBits;			//amount of bits per pixel
	DWORD iCompress;	//compression type
	DWORD iImageSize;	//image size (bytes)
	LONG iPPMX;			//X-axis pixels per meter
	LONG iPPMY;			//Y-axis pixels per meter
	DWORD iColors;		//amount of colors used in bitmap
	DWORD iColorsImp;	//amount of important colors
}INFOHEADER;

#pragma pack(pop)

int main()
{
	FILE *filePointerIn = NULL;

	filePointerIn = fopen("input.bmp", "rb"); //opens input file as read-only
	if (filePointerIn == NULL)
	{
		printf("file not found\n");
		exit(EXIT_FAILURE);
		return 0;
	}
	
	FILE *filePointerOut = NULL;

	filePointerOut = fopen("output.bmp", "wb"); //opens/creates an output file as writable
	if (filePointerOut == NULL)
	{
		printf("can't create file\n");
		exit(EXIT_FAILURE);
		return 0;
	}

	HEADER headFile;
	INFOHEADER infoHead;

	//copying header info
	fread(&headFile, sizeof(headFile), 1, filePointerIn);
	fwrite(&headFile, sizeof(headFile), 1, filePointerOut);
	fread(&infoHead, sizeof(infoHead), 1, filePointerIn);
	fwrite(&infoHead, sizeof(infoHead), 1, filePointerOut);

	RGBSAVE saveVariable;

	unsigned char testClause = 0; //checks error at the end of file to stop the loop

	do
	{
		testClause = fread(&saveVariable.temp_rgb, 1, 1, filePointerIn); //read value
		saveVariable.temp_rgb = saveVariable.temp_rgb ^ 0xFF; //inverts value
		fwrite(&saveVariable.temp_rgb, 1, 1, filePointerOut); //writes value
	}

	while(testClause == 1);

	//end of transmission
	fclose(filePointerIn);
	fclose(filePointerOut);
	return 0;
}

//-------------------------------------------------------------------------------------
//bronvermelding
//https://stackoverflow.com/questions/23725180/pixel-manipulation-on-bitmap-file-in-c
//-------------------------------------------------------------------------------------
