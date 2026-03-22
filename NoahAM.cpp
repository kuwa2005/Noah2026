// NoahAM.cpp
//-- control many archiver routines --

#include "stdafx.h"
#include <stdio.h>
#include "resource.h"
#include "NoahApp.h"
#include "NoahAM.h"
#include "ArcDLL.h"
#include "ArcAce.h"
#include "ArcMsc.h"
#include "ArcB2e.h"
#include "ArcCpt.h"

//----------------------------------------------//
//------ ??????????f?[?^?????????????? ------//
//----------------------------------------------//

void CNoahArchiverManager::init()
{
	const char* kl = mycnf().kill();
	static int dead[128];
	while( *kl ) dead[ 0x7f & (*(kl++)) ] = 1;

	// ????????`??
	if( !dead['L'] )	m_AList.add( new CArcLzh );
	if( !dead['7'] )	m_AList.add( new CArc7z ),
						m_AList.add( new CArc7zZip );
	if( !dead['Z'] )	m_AList.add( new CArcUnZip );
	if( !dead['z'] )	m_AList.add( new CArcZip );
	if( !dead['T'] )	m_AList.add( new CArcTar );
	if( !dead['C'] )	m_AList.add( new CArcCab );
	if( !dead['R'] )	m_AList.add( new CArcRar );
	if( !dead['A'] )	m_AList.add( new CArcArj );
	if( !dead['B'] )	m_AList.add( new CArcBga );
	if( !dead['Y'] )	m_AList.add( new CArcYz1 );
	if( !dead['G'] )	m_AList.add( new CArcGca );
	if( !dead['a'] )	m_AList.add( new CArcAce );
	if( !dead['M'] )	m_AList.add( new CArcMsc );
	if( !dead['c'] )	m_AList.add( new CArcCpt );

	// ?g???X?N???v?g???[?h
	char prev_cur[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, prev_cur);
	::SetCurrentDirectory( CArcB2e::init_b2e_path() );
	kiFindFile find;
	find.begin( "*.b2e" );
	WIN32_FIND_DATA fd;
	for( int t=0; find.next(&fd); t++ )
		m_AList.add( new CArcB2e(fd.cFileName) );
	m_b2e = (t>1);
	::SetCurrentDirectory(prev_cur);
}

//----------------------------------------------//
//------------ ?t?@?C?????X?g???L?? ------------//
//----------------------------------------------//

unsigned long CNoahArchiverManager::set_files( const cCharArray& files )
{
	//-- ?N???A
	m_FName.empty();
	m_BasePathList.empty();

	//-- ???p?X?????( ?o?????????p?????L??????A8.3?`???? )
	if( files.len() != 0 )
	{
		char spath[MAX_PATH];
		m_BasePath =
			( 0!=::GetShortPathName( files[0], spath, MAX_PATH ) )
			? spath : "";
		if( !m_BasePath.beDirOnly() )
		{
			m_BasePath.beSpecialPath( kiPath::Cur );
			m_BasePath.beBackSlash( true );
		}
	}

	//-- ?Z???t?@?C???????????????????????
	m_FName.alloc( files.len() );
	m_BasePathList.alloc( files.len() );
	for( unsigned int i=0,c=0; i!=files.len(); i++ )
		if( kiFindFile::findfirst( files[i], &m_FName[c] ) )
		{
			if( m_FName[c].cAlternateFileName[0] == '\0' )
				::strcpy_s( m_FName[c].cAlternateFileName, sizeof m_FName[c].cAlternateFileName, m_FName[c].cFileName );
			m_BasePathList[c] = files[i];
			if( !m_BasePathList[c].beDirOnly() )
			{
				m_BasePathList[c].beSpecialPath( kiPath::Cur );
				m_BasePathList[c].beBackSlash( true );
			}
			++c;
		}
	m_FName.forcelen( c );
	m_BasePathList.forcelen( c );
	return c;
}

//----------------------------------------------//
//--- ?t?@?C?????X?g?????[?`??????????? ---//
//----------------------------------------------//

// ?w?????g???q????????????[?`??????`?T??
CArchiver* CNoahArchiverManager::fromExt( const char* ext )
{
	kiStr tmp = ext;
	tmp.lower();

	for( unsigned int i=0; i!=m_AList.len(); i++ )
		if( m_AList[i]->extCheck( tmp ) 
		 && (m_AList[i]->ability() & aMelt) )
			return m_AList[i];
	return NULL;
}

bool CNoahArchiverManager::map_melters( int mode ) // 1:cmp 2:mlt 3:must_mlt
{
	// ?N???A
	m_Melters.empty();

#define attrb (m_FName[ct].dwFileAttributes)
#define lname (m_FName[ct].cFileName)
#define sname (m_FName[ct].cAlternateFileName[0]==0 ? m_FName[ct].cFileName : m_FName[ct].cAlternateFileName)

	kiPath fnm;
	const char* ext;
	for( unsigned int ct=0, bad=0; ct!=file_num(); ct++ )
	{
//		fnm = m_BasePath, fnm += sname;
		fnm = m_BasePathList[ct], fnm += sname;

		//-- 0byte?t?@?C?? / ?f?B???N?g????e??
		if( !(attrb & FILE_ATTRIBUTE_DIRECTORY) && 0!=kiFile::getSize( fnm, 0 ) )
		{
			//-- ???????g???q???????????`?????I?o
			CArchiver* x = fromExt( ext=kiPath::ext(lname) );

			//-- ???`??A?t?@?C?????e????`?F?b?N
			if( x && x->check( fnm ) )
			{
				m_Melters.add( x );
				continue;
			}

			//-- ???`?????e?`?F?b?N?s?????????????b????g??
			if( x && !(x->ability() & aCheck) )
			{
				m_Melters.add( x );
				continue;
			}

			//-- ???`???_?????A?????????e?`?F?b?N??\????[?`???S??????
			if( mode!=1 || 0==ki_strcmpi( "exe", ext ) )
			{
				for( unsigned long j=0; j!=m_AList.len(); j++ )
					if( m_AList[j]!=x && m_AList[j]->check( fnm ) )
					{
						m_Melters.add( m_AList[j] );
						break;
					}
				if( m_Melters.len() == ct+1 )
					continue;
			}
		}

		//-- ?`?F?b?N?????A??s?\????????
		if( mode!=3 )
			return false; //-- ???p???[?h???????I??
		m_Melters.add( NULL ), bad++;
	}
#undef sname
#undef lname
#undef attrb

	return (ct!=bad);
}

//----------------------------------------------//
//--- ?t?@?C?????X?g????k???[?`??????????? ---//
//----------------------------------------------//

bool CNoahArchiverManager::map_compressor( const char* ext, const char* method, bool sfx )
{
	int m;
	m_Method = -1;
	m_Sfx    = sfx;

	for( unsigned int i=0; i!=m_AList.len(); i++ )
		if( -1 != (m=m_AList[i]->cancompressby(ext,method,sfx)) )
			if( m!=-2 ) // ???S??v
			{
				m_Compressor = m_AList[i];
				m_Method = m;
				break;
			}
			else if( m_Method == -1 ) // ?`????????v???????????m
			{
				m_Compressor = m_AList[i];
				m_Method = m_AList[i]->cmp_mhd_default();
			}
	return (m_Method != -1);
}

//----------------------------------------------//
//------------ ?o?[?W?????????? ------------//
//----------------------------------------------//

void CNoahArchiverManager::get_version( kiStr& str )
{
	kiStr tmp;
	for( unsigned int i=0; i!=m_AList.len(); i++ )
		if( m_AList[i]->ver( tmp ) )
			str+=tmp, str+="\r\n";
}

//----------------------------------------------//
//--------------- ???k?`?????X?g ---------------//
//----------------------------------------------//

static unsigned int find( const cCharArray& x, const char* o )
{
	for( unsigned int i=0; i!=x.len(); i++ )
		if( 0==ki_strcmp( x[i], o ) )
			return i;
	return 0xffffffff;
}

static unsigned int find( const StrArray& x, const char* o )
{
	for( unsigned int i=0; i!=x.len(); i++ )
		if( x[i]==o )
			return i;
	return 0xffffffff;
}

void CNoahArchiverManager::get_cmpmethod(
		const char* set,
		int& def_mhd,
		StrArray& mhd_list,
		bool need_ext,
		cCharArray* ext_list )
{
	def_mhd = -1;

	const char* x;
	for( unsigned int i=0; i!=m_AList.len(); i++ )
	{
		if( *(x = m_AList[i]->cmp_ext())=='\0' )
			continue;
		if( need_ext )
		{
			if( -1 == find( *ext_list, x ) )
				ext_list->add( x );
		}
		if( 0 == ki_strcmp( set, x ) )
		{
			if( mhd_list.len()==0 )
			{
				def_mhd = m_AList[i]->cmp_mhd_default();
				for( unsigned int j=0; j!=m_AList[i]->cmp_mhd_list().len(); j++ )
					mhd_list.add( (m_AList[i]->cmp_mhd_list())[j] );
			}
			else
			{
				for( unsigned int j=0; j!=m_AList[i]->cmp_mhd_list().len(); j++ )
					if( -1 == find( mhd_list, (m_AList[i]->cmp_mhd_list())[j] ) )
						mhd_list.add( (m_AList[i]->cmp_mhd_list())[j] );
			}
		}
	}

	if( def_mhd == -1 )
		def_mhd = 0;
}

//----------------------------------------------//
//--------------- ????????[?h ---------------//
//----------------------------------------------//

#include "SubDlg.h"

void CNoahArchiverManager::do_listing( kiPath& destdir )
{
	kiWindow* mptr = app()->mainwnd();
	kiPath ddir;
	int    mdf = mycnf().mkdir();
	bool   rmn = mycnf().mnonum();
	destdir.beBackSlash( true );

	//-- ?_?C?A???O?????J?E???^???N???A
	kiArray<CArcViewDlg*> views;
	CArcViewDlg::clear();

	//-- ?_?C?A???O?N??
	for( unsigned int i=0; i!=m_FName.len(); i++ )
	{
		if( !m_Melters[i] )
			continue;

		arcname an(
			m_BasePathList[i],
//			m_BasePath,
			m_FName[i].cAlternateFileName[0]==0 ? m_FName[i].cFileName : m_FName[i].cAlternateFileName,
			m_FName[i].cFileName );
		ddir = destdir;

		if( mdf )
			generate_dirname( m_FName[i].cFileName, ddir, rmn );

		CArcViewDlg* x = new CArcViewDlg( m_Melters[i],an,ddir );
		views.add( x );
		x->createModeless( NULL );
	}

	//-- ?S???I?????????@
	kiWindow::msgLoop( kiWindow::GET );

	//-- ???I??
	app()->setMainWnd( mptr );
	for( i=0; i!=views.len(); i++ )
		delete views[i];
}

//----------------------------------------------//
//----------------- ???? -------------------//
//----------------------------------------------//

void CNoahArchiverManager::do_melting( kiPath& destdir )
{
	//-- ????[?h
	const int  mdf = mycnf().mkdir();  // Make Directory Flag( 0:no 1:no1file 2: noddir 3:yes )
	const bool rmn = mycnf().mnonum(); // Remove NuMber ?

	//-- ?o???
	destdir.beBackSlash( true );
	destdir.mkdir(), destdir.beShortPath();

	for( unsigned int i=0; i!=m_FName.len(); i++ )
		if( m_Melters[i] )
		{
			//-- ?o???

			int mk=2; // 0:no 1:yes 2:???
			kiPath ddir( destdir ), dnm;
			if( mdf==0 )
				mk=0;
			else if( mdf==3 )
				mk=1;
			else
			{
				kiPath anm(m_BasePathList[i]);
//				kiPath anm(m_BasePath);
				anm+=m_FName[i].cFileName;
				int c = m_Melters[i]->contents( anm, dnm );
				if( c==aSingleDir || (c==aSingleFile && mdf==1) )
					mk=0; // ?Q?d?t?H???_?h?~????(??)
				else if( c==aMulti )
					mk=1;
			}
			if( mk )
			{
				generate_dirname( m_FName[i].cFileName, ddir, rmn );
				if( mk==2 && kiSUtil::exist(ddir) )
					mk=1;
				ddir+='\\';
				ddir.mkdir();
				ddir.beShortPath();
			}

			//-- ??I

			arcname an( m_BasePathList[i],
//			arcname an( m_BasePath,
				m_FName[i].cAlternateFileName[0]==0 ? m_FName[i].cFileName : m_FName[i].cAlternateFileName,
				m_FName[i].cFileName );
			int result = m_Melters[i]->melt( an, ddir );
			if( result<0x8000 )
			{
				if( mk==2 ) // ?Q?d?t?H???_?h?~????(??)
					break_ddir( ddir, mdf==2 );
				else if( mk==0 && dnm.len() ) // ?Q?d?t?H???_?h?~????(??)
					if( dnm.len()<=1 || dnm[1]!=':' ) // ???p?X??J?????
						ddir+=dnm, ddir+='\\';
				// ?o?????J??????
				myapp().open_folder( ddir, 1 );
			}
			else if( result!=0x8020 )
			{
				//?G???[?I
				char str[255];
				_snprintf_s( str, sizeof str, _TRUNCATE, "%s\nError No: [%x]",
					(const char*)kiStr().loadRsrc( IDS_M_ERROR ), result );
				app()->msgBox( str );
			}
		}
}

void CNoahArchiverManager::generate_dirname( const char* src, kiPath& dst, bool rmn )
{
	// src????????????????f?B???N?g??????????A
	// dst??????Brmn==true??????????????

	// ?????? . ??????????? . ??T??
	const char *fdot=NULL, *sdot=NULL, *tail;
	for( tail=src; *tail; tail=kiStr::next(tail) )
		if( *tail=='.' )
			sdot=fdot, fdot=tail;

	// .tar.xxx ???A.xxx.gz/.xxx.z/.xxx.bz2 ??????
	if( fdot )
	{
		tail = fdot;
		if( sdot )
			if( 0==::lstrcmpi(fdot,".gz")
			 || 0==::lstrcmpi(fdot,".z")
			 || 0==::lstrcmpi(fdot,".bz2")
			 || (sdot+4==fdot
			 && (sdot[1]=='t'||sdot[1]=='T')
			 && (sdot[2]=='a'||sdot[2]=='A')
			 && (sdot[3]=='r'||sdot[3]=='R')
			))
				tail = sdot;
	}

	// ???????????'-'??'_'??'.'???B???p?X?y?[?X???B
	bool del[256];
	ki_memzero( del, sizeof(del) );
	if( rmn )
	{
		del['-'] = del['_'] = del['.'] = true;
		for( char c='0'; c<='9'; ++c )
			del[c] = true;
	}
	del[' '] = true;

	const char* mjs=NULL;
	for( const char *x=src; x<tail; x=kiStr::next(x) )
		if( !del[(unsigned char)(*x)] )
			mjs = NULL;
		else if( !mjs )
			mjs = x;
	if( mjs && mjs!=src )
		tail = mjs;

	// ??????????????? "noahmelt" ????????O??????????B
	if( src==tail )
		dst += "noahmelt";
	else
		while( src!=tail )
			dst += *src++;
}

bool CNoahArchiverManager::break_ddir( kiPath& dir, bool onlydir )
{
// ?Q?d?t?H???_ or ?P??t?@?C?? ????????
//
// ?f????i?[?t?@?C????????x???????????{??????????A
// ??????????????x?????????????AFindFirst?n??
// ?T?|?[?g????DLL??????G???W????O???????????????
// ???_????????A??????y Noah 2.xx ???????@????B

//-- ????P???????????????????m?F -----------------
	char wild[MAX_PATH];
	ki_strcpy( wild, (const char*)dir );
	ki_strcat( wild, "*.*" );
	kiFindFile find;
	if( !find.begin( wild ) )
		return false;
	WIN32_FIND_DATA fd,fd2,fd3;
	find.next( &fd );
	if( find.next( &fd2 ) )
		return false;
	find.close();
//----------------------------------------------------

//-- to:??I?????t?@?C?????B??????A?J?????gDir???????????????? -----
	kiPath to(dir); to.beBackSlash( false ), to.beDirOnly();
	::SetCurrentDirectory( to );
	to += fd.cFileName;
//-------------------------------------------------------------------------

//-- ?t?@?C?????????? --------------------------------------
	if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	{
		if( !onlydir )
		{
			// now ?????t?@?C????
			kiStr now=dir; now+=fd.cFileName;

			// now -> to ???
			if( ::MoveFile( now, to ) )
			{
				dir.remove();
				dir.beBackSlash( false ), dir.beDirOnly();
				return true;
			}
		}
	}
//-- ?t?H???_???????? ----------------------------------------
	else
	{
		// 'base/aaa/aaa/' ???????aaa???O??move???????B
		// ?????A?????B-> 'base/aaa_noah_tmp_178116/aaa/'

		dir.beBackSlash( false );
		kiFindFile::findfirst( dir, &fd3 );
		kiPath dirx( dir ); dirx+="_noah_tmp_178116";

		if( ::MoveFile( dir, dirx ) )
		{
			// now ?????t?@?C????
			kiStr now( dirx ); now+='\\', now+=fd.cFileName;

			// ?f?B???N?g???????
			if( ::MoveFile( now, to ) )
			{
				dirx.remove();
				dir=to, dir.beBackSlash( true );
				return true;
			}
			else
			{
				// 'base/aaa_noah_tmp_178116/aaa/' -> 'base/aaa/aaa/'
				dir.beDirOnly(), dir+=fd3.cFileName;
				::MoveFile( dirx, dir );
			}
		}

		dir.beBackSlash( true );
	}
//------------------------------------------------------------
	return false;
}

//----------------------------------------------//
//----------------- ???k??? -------------------//
//----------------------------------------------//

void CNoahArchiverManager::do_compressing( kiPath& destdir, bool each )
{
	int result = 0xffff, tr;

	// ?o?????m???????????
	destdir.beBackSlash( true );
	destdir.mkdir();
	destdir.beShortPath();

	// ?????k???[?h???AArchiving?s???`??????????
	if( each || !(m_Compressor->ability() & aArchive) )
	{
		wfdArray templist;

		for( unsigned int i=0; i!=m_FName.len(); i++ )
		{
			templist.empty();
			templist.add( m_FName[i] );
			tr = m_Compressor->compress( m_BasePath,templist,destdir,m_Method,m_Sfx );
			if( tr<0x8000 || tr==0x8020 )
				result = tr;
		}
	}
	else
		result = m_Compressor->compress( m_BasePath,m_FName,destdir,m_Method,m_Sfx );

	// ?J??????
	if( result<0x8000 )
		myapp().open_folder( destdir, 2 );
	else if( result!=0x8020 )
	{
		//?G???[?I
		char str[255];
		_snprintf_s( str, sizeof str, _TRUNCATE, "%s\nError No: [%x]", "Compression Error", result );
		app()->msgBox( str );
	}
}
