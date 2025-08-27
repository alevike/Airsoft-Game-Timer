#include "BigFont02.h"

//************************************************************************
//*	A set of custom made large numbers for a 16x2 LCD using the
//*	LiquidCrystal library. Works with displays compatible with the
//*	Hitachi HD44780 driver.
//*
//*	orginal developed  by Michael Pilcher  2/9/2010
//*	there are 8 entries, 8 bytes per entry
//*	these are the building blocks to make the numbers
//*
//*	http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1265696343
//************************************************************************

const uint8_t	gBigFontShapeTable[] PROGMEM	=	{
//*	bar0[8] =
  B11100,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11100,
//*	bar1[8] =
  B00111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B00111,
//*	bar2[8] =
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
//*	bar3[8] =
  B11110,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11100,
//*	bar4[8] =
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00111,
//*	bar5[8] =
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
//*	bar6[8] =
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111,
//*	bar7[8] =
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000

};

//************************************************************************
//*	a single large character takes up 6 digits on the LCD (3 cols x 2 rows).
//*  0-7  refers to segments in gBigFontShapeTable
//*  32   all zeros
//*  254  all zeros  
//*  255  all ones
const uint8_t	gBigFontAsciiTable[] PROGMEM	=	{

	32,	32,	32,	32,	32,	32,		//	0x20	space
	32,	0,	32,	32,	5,	32,		//	0x21	!
	32,	32,	32,	32,	32,	32,		//	0x22
	32,	32,	32,	32,	32,	32,		//	0x23
	32,	32,	32,	32,	32,	32,		//	0x24
	32,	32,	32,	32,	32,	32,		//	0x25
	32,	32,	32,	32,	32,	32,		//	0x26
	32,	32,	32,	32,	32,	32,		//	0x27
	32,	32,	32,	32,	32,	32,		//	0x28
	32,	32,	32,	32,	32,	32,		//	0x29
	32,	32,	32,	32,	32,	32,		//	0x2A
	32,	32,	32,	32,	32,	32,		//	0x2B
	32,	32,	32,	32,	32,	32,		//	0x2C
	32,	32,	32,	32,	32,	32,		//	0x2D
	32,	32,	32,	32,	5,	32,		//	0x2E	. (period)
	32,	32,	32,	32,	32,	32,		//	0x2F

	1,	7,	0,	1,	5,	0,		//	0x30	0
	32,	32,	0,	32,	32,	1,		//	0x31	1
	4,	2,	0,	1,	5,	5,		//	0x32	2
	4,	2,	0,	6,	5,	0,		//	0x33	3
	1,	5,	0,	32,	32,	0,		//	0x34	4
	1,	2,	3,	6,	5,	0,		//	0x35	5
	1,	2,	3,	1,	5,	0,		//	0x36	6
	1,	7,	0,	32,	32,	1,		//	0x37	7
	1,	2,	0,	1,	5,	0,		//	0x38	8
	1,	2,	0,	6,	5,	0,		//	0x39	9
	32,	32,	32,	32,	32,	32,		//	0x3A
	32,	32,	32,	32,	32,	32,		//	0x3B
	32,	32,	32,	32,	32,	32,		//	0x3C
	32,	32,	32,	32,	32,	32,		//	0x3D
	32,	32,	32,	32,	32,	32,		//	0x3E
	32,	32,	32,	32,	32,	32,		//	0x3F	?
	32,	32,	32,	32,	32,	32,		//	0x40	@
	32,	32,	32,	32,	32,	32,		//	0x41	A
	32,	32,	32,	32,	32,	32,		//	0x42	B
	32,	32,	32,	32,	32,	32,		//	0x43	C
	32,	32,	32,	32,	32,	32,		//	0x44	D
	32,	32,	32,	32,	32,	32,		//	0x45	E
	32,	32,	32,	32,	32,	32,		//	0x46	F
	32,	32,	32,	32,	32,	32,		//	0x47	G
	32,	32,	32,	32,	32,	32,		//	0x48	H
	32,	32,	32,	32,	32,	32,		//	0x49	I
	32,	32,	32,	32,	32,	32,		//	0x4A	J
	32,	32,	32,	32,	32,	32,		//	0x4B	K
	32,	32,	32,	32,	32,	32,		//	0x4C	L
	32,	32,	32,	32,	32,	32,		//	0x4D	M	place holder
	32,	32,	32,	32,	32,	32,		//	0x4E	N	place holder
	32,	32,	32,	32,	32,	32,		//	0x4F	O	(try not to make same as zero)
	32,	32,	32,	32,	32,	32,		//	0x50	P
	32,	32,	32,	32,	32,	32,		//	0x51	Q
	32,	32,	32,	32,	32,	32,		//	0x52	R
	32,	32,	32,	32,	32,	32,		//	0x35	S	(try not to make same as five)
	32,	32,	32,	32,	32,	32,		//	0x54	T
	32,	32,	32,	32,	32,	32,		//	0x55	U
	32,	32,	32,	32,	32,	32,		//	0x56	V	place holder
	32,	32,	32,	32,	32,	32,		//	0x57	W	place holder
	32,	32,	32,	32,	32,	32,		//	0x58	X
	32,	32,	32,	32,	32,	32,		//	0x59	Y
	32,	32,	32,	32,	32,	32,		//	0x5A	Z
	0

};

//************************************************************************
//*	separate table for wide characters
const byte	gBigFontAsciiTableWide[] PROGMEM	=	{
//*	this table is 10 bytes, 2 rows of 5
//	---top------------|
	32,	32,	32,	32,	32,	32,	32,	32,	32,	32,		//	0x4D	M		5-wide
	32,	32,	32,	32,	32,	32,	32,	32,	32,	32,		//	0x4E	N		4-wide
	32,	32,	32,	32,	32,	32,	32,	32,	32,	32,		//	0x51	Q		4-wide
	32,	32,	32,	32,	32,	32,	32,	32,	32,	32,		//	0x56	V		4-wide
	32,	32,	32,	32,	32,	32,	32,	32,	32,	32,		//	0x57	W		5-wide
	0
};

//************************************************************************
//*	main class definition
BigFont02::BigFont02(LiquidCrystal* lcd) {
	_lcd = lcd;
}

//************************************************************************
//* generate custom characters
void BigFont02::begin() {
byte	customCharDef[10];
byte	ii;
int		jj;

	for (ii=0; ii<8; ii++)
	{
		for (jj=0; jj<8; jj++)
		{
			customCharDef[jj]	=	pgm_read_byte_near(gBigFontShapeTable + (ii * 8) + jj);
		}
		_lcd->createChar(ii, customCharDef);
	}
}

//************************************************************************
//*	write a large character on the LCD display.
//*	note: method returns the width of the character.
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
//*   c       single ascii character to display
int BigFont02::writechar(int y, int x, char c) {
int		offset;
int		i;
char	b;
boolean	specialCase;
int		specialIndex;
int		charWidth;

	if (c == 0x20)
	{
		return(2);
	}
	else if (c < 0x20)
	{
		return(0);
	}

	if (c >= 'A')
	{
		c	=	c & 0x5F;	//*	force to upper case
	}
	specialCase	=	true;
	switch (c)
	{
		case 'M':	charWidth	=	5;	specialIndex	=	0;	break;
		case 'N':	charWidth	=	4;	specialIndex	=	1;	break;
		case 'Q':	charWidth	=	4;	specialIndex	=	2;	break;
		case 'V':	charWidth	=	4;	specialIndex	=	3;	break;
		case 'W':	charWidth	=	5;	specialIndex	=	4;	break;


		default:
			charWidth	=	3;
			specialCase	=	false;
			offset		=	6 * (c - 0x20);
			_lcd->setCursor(x, y);
			for (i=0; i<3; i++)
			{
				b	=	pgm_read_byte_near(gBigFontAsciiTable + offset + i);
				_lcd->write(b);
			}

			_lcd->setCursor(x, y + 1);
			offset	+=	3;
			for (i=0; i<3; i++)
			{
				b	=	pgm_read_byte_near(gBigFontAsciiTable + offset + i);
				_lcd->write(b);
			}
			break;
	}
	if (specialCase)
	{
		//*
		offset		=	10 * specialIndex;
		_lcd->setCursor(x, y);
		for (i=0; i<charWidth; i++)
		{
			b	=	pgm_read_byte_near(gBigFontAsciiTableWide + offset + i);
			_lcd->write(b);
		}

		_lcd->setCursor(x, y + 1);
		offset	+=	5;
		for (i=0; i<charWidth; i++)
		{
			b	=	pgm_read_byte_near(gBigFontAsciiTableWide + offset + i);
			_lcd->write(b);
		}
	}
	return(charWidth + 1);
}

//************************************************************************
//* clear leading digits
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
void BigFont02::clear(int y, int x)
{
  _lcd->setCursor(y,x);
  _lcd->print("   ");
  _lcd->setCursor(y,x+1); 
  _lcd->print("   ");
}

//************************************************************************
//* prints an integer to the display using large characters
//* Parameters:
//*   y       yposition or column
//*   x       xposition or row
//*   n       number to display (integer)
//*   d       number of digits.  For example, 4 digits => [   9] [  99] [ 999] [9999]
//*   l       leading zeros (false = no, true = yes)
void BigFont02::writeint(int y, int x, int n, byte d, bool l)
{
  boolean isNegative = false;
  if(n < 0)
  {
    isNegative = true;
    n = abs(n);
  }
  byte numString[d];
  byte index = d - 1;
  while(index)
  {
    numString[index] = n % 10;
	n /= 10;
	index--;
  }
  numString[0] = n % 10;
  
  for (int i = 0; i < d; i++)
  {
    if(numString[i] == 0 && !l && i < d - 1)
    {
      //clear(y, (i * 3) + x);
    }
    else
    {
	  writechar(y, (i * 3) + x, numString[i] + '0');
      l = true;
	  //if (isNegative)
      //{
       // _lcd->setCursor(max(0, (i * 3) - 2 + x), 0);
		//if(numString[i] == 1)
		//{
		//  
		//}
		//_lcd->write(45); // "-"
		//isNegative = false;
      //}
    }
  }
}
