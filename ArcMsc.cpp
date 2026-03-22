
#include "stdafx.h"
#include "ArcMsc.h"
#include "NoahApp.h"

bool CArcMsc::header( kiFile& fp, unsigned long* siz, char* ext )
{
	// ???o??
	unsigned char p[14];
	if( 14!=fp.read(p,14) )
		return false;

	// ?w?b?_?\???FSZDD??'3A [ext3rd] [orisiz(dword)]
	static const unsigned char head[9]={ 0x53,0x5A,0x44,0x44,0x88,0xF0,0x27,0x33,0x41 };//="SZDD??'3A";

	for( int i=0; i!=9; i++ )
		if( p[i]!=head[i] )
			return false;

	if( ext )
		*ext = (char)p[9];

	DWORD x = p[10]+(p[11]<<8)+(p[12]<<16)+(p[13]<<24);
	if( siz )
		*siz=x;

	// ?????I??A16?{????????k????o???
	return (x <= fp.getSize()*16);
}

void CArcMsc::filename( char* oname, size_t onameCap, const char* ol, char ext )
{
	// ?R?s?[
	ki_strcpy( oname, onameCap, ol );

	// ???I '_' ????????????B
	bool bIs_=false;
	for( char* p=oname; *p; p=kiStr::next(p) ) 
		bIs_ = (*p=='_');
	if( bIs_ )
		*(--p)='\0';


	if( ext )
	{
		// ?g???q????
		*p++ = ext;
		*p   = '\0';
	}
	else
	{
		// ?g???q??????
		const char* x = kiPath::ext(oname);
		if( ki_strlen(x)==2 )
		{
			::CharLower(oname);

				 if( 0==ki_strcmp(x,"ex") )(*p++)='e';
			else if( 0==ki_strcmp(x,"co") )(*p++)='m';
			else if( 0==ki_strcmp(x,"sc") )(*p++)='r';

			else if( 0==ki_strcmp(x,"dl") )(*p++)='l';
			else if( 0==ki_strcmp(x,"oc") )(*p++)='x';
			else if( 0==ki_strcmp(x,"dr") )(*p++)='v';
			else if( 0==ki_strcmp(x,"vx") )(*p++)='d';
			else if( 0==ki_strcmp(x,"38") )(*p++)='6';
			else if( 0==ki_strcmp(x,"sy") )(*p++)='s';
			else if( 0==ki_strcmp(x,"cp") )(*p++)='l';
			else if( 0==ki_strcmp(x,"li") )(*p++)='b';
			else if( 0==ki_strcmp(x,"tt") )(*p++)='f';

			else if( 0==ki_strcmp(x,"ch") )(*p++)='m';
			else if( 0==ki_strcmp(x,"hl") )(*p++)='p';
			else if( 0==ki_strcmp(x,"cn") )(*p++)='t';
			else if( 0==ki_strcmp(x,"da") )(*p++)='t';

			else if( 0==ki_strcmp(x,"tx") )(*p++)='t';
			else if( 0==ki_strcmp(x,"wr") )(*p++)='i';
			else if( 0==ki_strcmp(x,"wa") )(*p++)='v';
			else if( 0==ki_strcmp(x,"mi") )(*p++)='d';
			else if( 0==ki_strcmp(x,"rm") )(*p++)='i';
			else if( 0==ki_strcmp(x,"bm") )(*p++)='p';
			else if( 0==ki_strcmp(x,"rl") )(*p++)='e';
			else if( 0==ki_strcmp(x,"cu") )(*p++)='r';
			else if( 0==ki_strcmp(x,"do") )(*p++)='c';
			else if( 0==ki_strcmp(x,"ic") )(*p++)='o';
			else if( 0==ki_strcmp(x,"re") )(*p++)='g';
			else if( 0==ki_strcmp(x,"rt") )(*p++)='f';
			else if( 0==ki_strcmp(x,"ht") )(*p++)='m';
			else						   (*p++)='#';

			*p = '\0';
		}
	}
}

bool CArcMsc::v_check( const kiPath& aname )
{
	kiFile fp;
	if( fp.open( aname ) )
		return header( fp,NULL,NULL );
	return false;
}

bool CArcMsc::v_list( const arcname& aname, aflArray& files )
{
	kiPath fname(aname.basedir); fname+=aname.lname;

	arcfile x;
	kiFile fp;
	char ext;
	if( !fp.open( fname ) || !header( fp, &x.inf.dwOriginalSize, &ext ) )
		return false;
	ki_strcpy( x.inf.szMode, "-msc-" );
	filename( x.inf.szFileName, sizeof x.inf.szFileName, aname.lname, ext );
	x.inf.dwCompressedSize = fp.getSize();
	x.isfile = true;

	// TODO: ?????????????H

	files.add( x );
	return true;
}

int CArcMsc::v_melt( const arcname& aname, const kiPath& ddir, const aflArray* files )
{
	kiFile fp;
	kiPath fname(aname.basedir); fname+=aname.lname;
	kiPath oname(ddir);
	char tmp[MAX_PATH];

	// ?w?b?_??????
	char ext;
	unsigned long alllen;
	if( !fp.open( fname ) || !header(fp,&alllen,&ext) )
		return 0xffff;
	fp.close();
	filename( tmp, sizeof tmp, aname.lname, ext );
	oname += tmp;

	// ????E?o?????J??
	OFSTRUCT of;
	of.cBytes = sizeof(of);
	int FROM = ::LZOpenFile( const_cast<char*>((const char*)fname),&of,OF_READ );
	int TO   = ::LZOpenFile( const_cast<char*>((const char*)oname),&of,OF_WRITE|OF_CREATE );
	if( FROM<0 || TO<0 )
		return 0xffff;
	// ??
	bool ans = (0<=::LZCopy( FROM,TO ));
	// ?I??
	::LZClose( TO );
	::LZClose( FROM );
	return ans?0:0xffff;
}

int CArcMsc::v_compress( const kiPath& base, const wfdArray& files, const kiPath& ddir, int method, bool sfx )
{
	::SetCurrentDirectory( base );

	// ???t?@?C?????J??
	kiFile in;
	if( !in.open( files[0].cFileName ) )
		return 0xffff;

	// ?w?b?_???E???k??t?@?C????
	char h_Ext3 = '\0';
	unsigned long h_Len = in.getSize();

	char aname[MAX_PATH];
	ki_strcpy( aname, files[0].cFileName );
	for( char *x=aname, *last=aname; *x; x=kiStr::next(x) )
		last = x;
	if( !IsDBCSLeadByte(*last) )
		h_Ext3=*last;
	*last = '_', *(last+1) = '\0';

	// ???k??J??
	::SetCurrentDirectory( ddir );

	// ???k????J??
	kiFile out;
	if( !out.open( aname, false ) )
		return 0xffff;

	// ?^?C???X?^???v?R?s?[
	FILETIME ct, at, mt;
	::GetFileTime(in.getHandle(), &ct, &at, &mt);
	::SetFileTime(out.getHandle(), &ct, &at, &mt);

	// ?w?b?_????????
	unsigned char head[14]={ 0x53,0x5A,0x44,0x44,0x88,0xF0,0x27,0x33,0x41,(unsigned char)h_Ext3,
							 (unsigned char)(h_Len&0xff),(unsigned char)((h_Len>>8)&0xff),
							 (unsigned char)((h_Len>>16)&0xff),(unsigned char)((h_Len>>24)&0xff) };
	out.write( head, 14 );

	// ???k???
	CArcProgressDlg dlg( h_Len, true );
	dlg.change( files[0].cFileName );
	if( !do_lzss( in, out, dlg ) )
	{
		out.close();
		::DeleteFile(aname);
		return 0x8020;
	}
	return 0;
}


//-- 12bit LZSS -----------------------------------------------//


#define N 4096 // slide????T?C?Y 2^12 bytes
#define F 18   // ?????v?? 2^(16-12)+2 bytes

static unsigned char window[N+F-1];
static int dad[N+1], lson[N+1], rson[N+257];
static int matchpos, matchlen;

static void init_tree()
{
	//-- ?????????

	int i;
	for( i=N+1; i<=N+256; i++ ) // root: 0x00 -- 0xff
		rson[i] = N;
	for( i=0; i<N ; i++ )
		 dad[i] = N;
}

static void insert_node( int r )
{
	//-- [??ur]????n???[??str]????o?^
	unsigned char* str = window + r;
	rson[r] = lson[r] = N;

	//-- ???????v???E??u???L?^????
	matchlen = 2;

	// ???????root??I??
	int i, p = N+1+str[0], cmp=1;
	for(; ;)
	{
		if( cmp >= 0 )
		{
			// ?E??i??
			if( rson[p] != N )
				p = rson[p];
			// ?E??????node?????????????o?^????I??
			else
			{
				rson[p] = r;
				 dad[r] = p;
				return;
			}
		}
		else
		{
			// ????i??
			if( lson[p] != N )
				p = lson[p];
			// ????????node?????????????o?^????I??
			else
			{
				lson[p] = r;
				 dad[r] = p;
				return;
			}
		}

		// ?????node??str???r( i==??v?? )
		for( i=1; i<F; i++ )
			if( cmp = str[i] - window[p+i] )
				break;

		// ?????????????????????????????L??
		if( i > matchlen )
		{
			matchpos = p;
			if( (matchlen=i) == F )
				break;
		}
	}

	// [??up]???????F???v???????A?????????
	// p ???????????? r ??u????????

	 dad[r] =  dad[p];
	lson[r] = lson[p];
	rson[r] = rson[p];
	 dad[lson[p]] = r;
	 dad[rson[p]] = r;

	if( rson[dad[p]]==p )
		rson[dad[p]] = r;
	else
		lson[dad[p]] = r;

	dad[p] = N;
}

static void delete_node( int p )
{
	//-- [??up]??v?f???????

	if( dad[p] == N ) // ??????????????????????
		return;

	int q;

	if( rson[p] == N )
		q = lson[p]; // ?B???q??????????O??
	else if( lson[p] == N )
		q = rson[p]; // ?B???q??????????O??
	else
	{
		q = lson[p];

		if( rson[q] != N )
		{
			// ????}???E?A????????????????node????????O??
			do
				q = rson[q];
			while( rson[q] != N );

			rson[dad[q]] = lson[q];
			dad[lson[q]] = dad[q];
			lson[q] = lson[p];
			dad[lson[p]] = q;
		}
		rson[q] = rson[p];
		dad[rson[p]] = q;
	}

	dad[q] = dad[p];
	if( rson[dad[p]] == p )
		rson[dad[p]] = q;
	else
		lson[dad[p]] = q;
	dad[p] = N;
}

bool CArcMsc::do_lzss( kiFile& in, kiFile& out, CArcProgressDlg& dlg )
{
	int i, c, len, r, s;
	unsigned char code[17]={0}, mask=1, codeptr=1;

	s = 2;		// s = ?f?[?^?????u
	r = N - 16;	// r = ????}????u

	// ?N???A
	init_tree();
	ki_memset( window+2, ' ', N-F );

	// ??18bytes????
	for( len=0 ; len<F ; len++ )
	{
		if( -1 == (c = in.getc()) )
			break;
		window[r+len] = c;
	}
	window[0] = window[r+16];
	window[1] = window[r+17];
	if( len==0 )
		return true;

	// ???}??
	for( i=F ; i>=0 ; i-- )
		insert_node( r-i );

	// ???[?v
	unsigned int total_read=18,prgr_read=0;

	do
	{
		if( prgr_read > 5000 )
		{
			dlg.change( NULL, total_read+=prgr_read );
			prgr_read-=5000;
			if( !dlg.msgloop() )
			{
				prgr_read=0xffffffff;
				break;
			}
		}

		if( matchlen > len )
			matchlen=len;

		if( matchlen < 3 )	// ??v???
		{
			matchlen = 1;
			code[0] |= mask;
			code[codeptr++] = window[r];
		}
		else				// ??v????
		{
			// [pos&0xff] [pos&0xf00 | len-3]
			code[codeptr++] = (unsigned char)matchpos;
			code[codeptr++] = (unsigned char)(((matchpos>>4)&0xf0) | (matchlen-3));
		}

		if( (mask<<=1)==0 ) // code ?? 8Block ???????????o??
		{
			out.write( code, codeptr );
			// ?R?[?h?o?b?t?@??????
			code[0] = 0;
			codeptr = mask = 1;
		}

		// ?o?????????????
		int lastmatchlen = matchlen;
		for( i=0 ; i<lastmatchlen; i++ )
		{
			if( -1 == (c=in.getc()) )
				break;
			prgr_read++;

			// [??us]?????????????
			delete_node( s );
			window[s] = c;
			if( s < F-1 ) window[ N+s ] = c;

			s = (s+1) & (N-1); // s++
			r = (r+1) & (N-1); // r++

			// ???[??ur]??f?[?^??}??
			insert_node( r );
		}

		// EOF????
		while( i++ < lastmatchlen )
		{
			delete_node( s );
			s = (s+1) & (N-1); // s++
			r = (r+1) & (N-1); // r++
			if( --len )
				insert_node( r );
		}

	} while( len > 0 );

	if( prgr_read==0xffffffff )
		return false;

	// 8block???E???????
	if( mask != 1 )
	{
		while( mask<<=1 )
			code[codeptr++] = 0;
		out.write( code, codeptr );
	}

	return true;
}

