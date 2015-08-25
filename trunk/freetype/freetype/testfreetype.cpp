#include "stdafx.h"
#include "testfreetype.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iconv.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define WIDTH   80
#define HEIGHT  80

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];


void mydraw_bitmap1( FT_Bitmap* bitmap)
{
	FT_Int  i, j;
	FT_Int  x_max = bitmap->width;
	FT_Int  y_max = bitmap->rows;


	for ( i = 0; i < y_max; i++ )
	{
		for ( j = 0; j < x_max; j++)
		{
			fprintf(stderr, "%02x ", bitmap->buffer[i * bitmap->width + j]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}


void mydraw_bitmap2( FT_Bitmap* bitmap)
{
	FT_Int  i, j;
	FT_Int  x_max = bitmap->width;
	FT_Int  y_max = bitmap->rows;


	for ( i = 0; i < y_max; i++ )
	{
		for ( j = 0; j < x_max; j++)
		{
			unsigned char bi = bitmap->buffer[i * bitmap->width + j];
			fprintf(stderr, "%c", (bi == 0) ? ' ' : ( bi < 128 ? '+' : '*' ));
		}
		/*
		fprintf(stderr, "\t\t\t");
		for ( j = 0; j < x_max; j++)
		{
		unsigned char bi = bitmap->buffer[i * bitmap->width + j];
		if( bi > 0 )
		{
		fprintf(stderr, "%02x", bi);
		}
		else
		{
		fprintf(stderr, "  ");
		}
		}
		*/
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}


void draw_bitmap( FT_Bitmap*  bitmap, int x_offset,int y_offset)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = bitmap->width;
	FT_Int  y_max = bitmap->rows;


	for ( i = 0; i < y_max; i++ )
	{
		for ( j = 0; j < x_max; j++ )
		{
			if ( j + x_offset >= WIDTH || i + y_offset >= HEIGHT )
				continue;

			image[i + y_offset][j + x_offset] = bitmap->buffer[i * bitmap->width + j];
		}
	}
}


void show_image( void )
{
	int  i, j;


	for ( i = 0; i < HEIGHT; i++ )
	{
		for ( j = 0; j < WIDTH; j++ )
		{
			unsigned char bi = image[i][j];

			fprintf(stderr, "%c", (bi == 0) ? ' ' : (bi < 128 ? '+'	: '*' ) );
		}		
	}
}


int
testfreetype(char* filename, unsigned long* text, int nLen)
{
	FT_Library    library;
	FT_Face       face;

	FT_GlyphSlot  slot;
	FT_Matrix     matrix;                 /* transformation matrix */
	FT_Vector     pen;                    /* untransformed origin  */
	FT_Error      error;

	double        angle;
	int           target_height;
	int           n, num_chars;

	angle         = ( 25.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
	target_height = HEIGHT;

	error = FT_Init_FreeType( &library );              /* initialize library */
	/* error handling omitted */

	error = FT_New_Face( library, filename, 0, &face ); /* create face object */
	/* error handling omitted */

	/* use 50pt at 100dpi */
	error = FT_Set_Char_Size( face, 24 * 64, 0,	72, 0 );                /* set character size */
	/* error handling omitted */

	slot = face->glyph;

	/* set up matrix */
	//matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	//matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	//matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	//matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	//pen.x = 300 * 64;
	//pen.y = ( target_height - 200 ) * 64;


	int x_offset = 0;
	int y_offset = 0;

	for ( n = 0; n < nLen; n++ )
	{
		/* set transformation */
		//FT_Set_Transform( face, &matrix, &pen );

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
		if ( error )
			continue;                 /* ignore errors */

		/* now, draw to our target surface (convert position) */
		//draw_bitmap( &slot->bitmap,slot->bitmap_left,target_height - slot->bitmap_top );
#define	MYDRAW  0
#if MYDRAW
		mydraw_bitmap2(&slot->bitmap);
#else
		x_offset += slot->bitmap_left;//以pixel为单位
		draw_bitmap(&slot->bitmap,x_offset, y_offset);
		x_offset += slot->advance.x / 64;//以1/64 pixel为单位
#endif
		/* increment pen position */
		//pen.x += slot->advance.x;
		//pen.y += slot->advance.y;
	}

#if !MYDRAW
	show_image();
#endif

	FT_Done_Face    ( face );
	FT_Done_FreeType( library );

	return 0;
}


static void FromWide2 (const wchar_t *wide, char* utf8, int nLen)
{
	size_t len;
	len = WideCharToMultiByte (CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);

	if( len < nLen)	
		WideCharToMultiByte (CP_UTF8, 0, wide, -1, utf8, len, NULL, NULL);

	return ;
}


int test()
{
	char chineseutf8[256] = "中文";

#ifdef WIN32
	FromWide2(L"中文", chineseutf8, 255);
#endif //WIN32

	iconv_t iconvh = iconv_open("ucs-4le", "utf-8");

	unsigned long ucs4[256] = {0};

	char* in = chineseutf8;
	size_t nIn = strlen(chineseutf8);

	char* out = (char*)ucs4;
	size_t nOut = 255*4;

	size_t ret = iconv(iconvh, (const char**)&in, &nIn, &out, &nOut);
	if( ret < 0 )
	{
		return 0;
	}

	iconv_close(iconvh);

	/*
	ucs4[0] = 'a';
	ucs4[1] = 'b';
	ucs4[2] = 'c';
	ucs4[3] = 'g';
	ucs4[4] = 0;
	*/

	int ucs4Len = 0;
	for(int i=0; i<255; i++ )
	{
		if( ucs4[i] > 0 )
		{
			ucs4Len++;
		}
		else
		{
			break;
		}
	}

	testfreetype("C:\\WINDOWS\\Fonts\\SIMSUN.TTC"/* SIMSUN "SIMHEI.TTF" "ARIAL.TTF"*/, ucs4, ucs4Len);		
}
