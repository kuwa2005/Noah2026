// Noah.cpp
// -- entrypoint etc for 'Noah'

#include "stdafx.h"
#include "NoahApp.h"
#include "resource.h"
#include <stdio.h>

#if _MSC_VER >= 1300
extern "C" { int _afxForceEXCLUDE; }
extern "C" BOOL WINAPI _imp__IsDebuggerPresent() { return FALSE; }
#endif

// ?v???Z?X????????]?[??
class ProcessNumLimitZone
{
	HANDLE m_han;
public:
	ProcessNumLimitZone(int Max, const char* name)
		: m_han( ::CreateSemaphore( NULL, Max, Max, name ) )
	{
		if( m_han )
			::WaitForSingleObject( m_han, INFINITE );
		else
			kiSUtil::msgLastError("CreateSemaphore Failed");
	}
	~ProcessNumLimitZone()
	{
		if( m_han )
		{
			::ReleaseSemaphore( m_han, 1, NULL );
			::CloseHandle( m_han );
		}
	}
};

//----------------------------------------------//
//--------- Noah ??G???g???|?C???g ------------//
//----------------------------------------------//

void kilib_create_new_app()
{
	//-- kilib ??A?v???P?[?V????????
	new CNoahApp;
}

void CNoahApp::run( kiCmdParser& cmd )
{
	//-- ??????
	m_cnfMan.init();
	m_arcMan.init();

	//-- ?R?}???h???C???p?????[?^???
	m_pCmd = &cmd;

	//-- ?u?t?@?C???????n??????? or Shift?????N???v???????\??
	if( cmd.param().len()==0 || keyPushed(VK_SHIFT) )
	{
		//-- Load-INI ( ?S?? )
		m_cnfMan.load( All );
		//-- ?????\??
		m_cnfMan.dialog();
	}
	else
	{
		//-- ???k???????
		do_cmdline( true );
	}

	//-- ?I??????
	m_tmpDir.remove();
}

//----------------------------------------------//
//------------- ???k/?? ?? ??? --------------//
//----------------------------------------------//

bool CNoahApp::is_writable_dir( const kiPath& path )
{
	// ?v?????ACDROM/DVDROM ???????B
	// FDD, PacketWrite??Disk ?????????????B

	// RAMDISK, REMOTE, FIXED, UNKNOWN ??f?B?X?N??????????\??????
	UINT drv = path.getDriveType();
	if( drv==DRIVE_REMOVABLE || drv==DRIVE_CDROM )
	{
		// ?fWin95???g????????????DynamicLoad
		typedef BOOL (WINAPI*pGDFSE)( LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER );
		pGDFSE pGetDiskFreeSpaceEx
			= (pGDFSE) ::GetProcAddress( ::GetModuleHandle("kernel32.dll"), "GetDiskFreeSpaceExA" );
		if( pGetDiskFreeSpaceEx )
		{
			// ??e???0???A????????s??????
			ULARGE_INTEGER fs, dummy;
			pGetDiskFreeSpaceEx( path, &dummy, &dummy, &fs );
			if( fs.QuadPart == 0 )
				return false;
		}
	}
	return true;
}

void CNoahApp::do_cmdline( bool directcall )
{
	do_files( m_pCmd->param(), &m_pCmd->option(), !directcall );
}

void CNoahApp::do_files( const cCharArray& files,
						 const cCharArray* opts,
						 bool  basicaly_ignore )
{
	struct local {
		~local() {kiSUtil::switchCurDirToExeDir(); } // ?f?B???N?g?????b?N?????????
	} _;

	//-- Archiver Manager ??t?@?C???????X?g???L??????
	if( 0 == m_arcMan.set_files( files ) )
		return;

	//-- ???p???
	enum { unknown, melt, compress }
			whattodo = unknown;
	bool	ctrl_mlt = keyPushed( VK_CONTROL );
	bool	ctrl_cmp = ctrl_mlt;
	bool	alt      = keyPushed( VK_MENU ) || keyPushed( VK_RWIN ) || keyPushed( VK_LWIN );
	const char *cmptype=NULL, *method=NULL;
	kiPath  destdir;
	kiStr tmp(300);

	//-- ( ????????? )?R?}???h???C???I?v?V??????????
	if( opts )
		for( unsigned int i=0; i!=opts->len(); i++ )
			switch( (*opts)[i][1] )
			{
			case 'a':	if( !basicaly_ignore )
			case 'A':		whattodo = compress;	break;

			case 'x':	if( !basicaly_ignore )
			case 'X':		whattodo = melt;		break;

			case 'd':	if( !basicaly_ignore )
			case 'D':		destdir = (*opts)[i]+2;	break;

			case 'w':	if( !basicaly_ignore )
			case 'W':		alt = true;				break;

			case 't':	if( !basicaly_ignore )
			case 'T':		cmptype = (*opts)[i]+2;	break;

			case 'm':	if( !basicaly_ignore )
			case 'M':		method = (*opts)[i]+2;	break;

			case 'c':	if( !basicaly_ignore ) {
			case 'C':		if((*opts)[i][2]!='x') ctrl_cmp = true;
							if((*opts)[i][2]!='a') ctrl_mlt = true;
						break;}
			}

	//-- Load-INI ( ?????[?h??? )
	m_cnfMan.load( Mode );

	//-- ???k?????????s??????????B???????????B
	//
	// ?E?R?}???h???C????A???k??w?????????????????k??
	//
	// ?E???????????A???Noah??????[?h???
	// ?@m0:???k??p  m1:???k?D??  m2:??D??  m3:???p
	// ?@?@?R?}???h???C??????w????????? m3?B
	// ?@?@?w??b???????Am_cnfMan ?????????B
	//
	// ?Em0 ???A'm1????????t?@?C????????' ??????????????k??
	//
	// ?E???????????A????[?`??????????????B
	// ?@?????Am3 ??O???????????????????s??????G???[?????k??
	// ?@m3 ????A??????????????????G???[?B???????I??

	if( whattodo != compress )
	{
		int mode = 3;
		if( whattodo != melt )
			mode = m_cnfMan.mode();

		if( mode==0 || ( mode==1 && m_arcMan.file_num()>=2 ) )
			whattodo = compress;
		else
		{
			//-- ????[?`????????????
			bool suc = m_arcMan.map_melters( mode );
			if( suc )
				whattodo = melt;
			else
			{
				if( mode != 3 )
					whattodo = compress;
				else
				{
					//-- ???p???[?h???????s??I?I
					msgBox( tmp.loadRsrc(IDS_M_ERROR) );
					return;
				}
			}
		}
	}

	if( whattodo == melt )
	{
		//-- ????????m_cnfMan.init()????[?h????????c

		if( destdir.len()==0 )
		{
			//-- ???f?B???N?g?????
			if( m_cnfMan.mdirsm() )
				if( is_writable_dir(m_arcMan.get_basepath()) )
					destdir = m_arcMan.get_basepath();
			if( destdir.len()==0 )
				destdir = m_cnfMan.mdir();
		}

		//-- ??
		if( ctrl_mlt )	m_arcMan.do_listing( destdir );
		else {
			ProcessNumLimitZone zone( mycnf().multiboot_limit(), "LimitterForNoahAtKmonosNet" );
			m_arcMan.do_melting( destdir );
		}
	}
	else
	{
		//-- Load-INI( ???k??? )
		m_cnfMan.load( Compress );

		if( destdir.len()==0 )
		{
			//-- ???k??f?B???N?g?????
			if( m_cnfMan.cdirsm() )
				if( is_writable_dir(m_arcMan.get_basepath()) )
					destdir = m_arcMan.get_basepath();
			if( destdir.len()==0 )
				destdir = m_cnfMan.cdir();
		}
		if( !cmptype ) cmptype = m_cnfMan.cext();
		else if( !method ) method = "";
		if( !method  ) method  = m_cnfMan.cmhd();

		//-- ???k?p???[?`???????????
		if( !m_arcMan.map_compressor( cmptype, method, ctrl_cmp ) )
		{
			//-- ???k?s?\??`???I?I
			msgBox( tmp.loadRsrc(IDS_C_ERROR) );
			return;
		}

		//-- ???k
		ProcessNumLimitZone zone( mycnf().multiboot_limit(), "LimitterForNoahAtKmonosNet" );
		m_arcMan.do_compressing( destdir, alt );
	}
}

//----------------------------------------------//
//----------------- ??????G?p -----------------//
//----------------------------------------------//

static bool build_open_folder_cmd( char* out, size_t outChars, const char* tmpl, const char* dirPath )
{
	if( !out || outChars==0 || !dirPath )
		return false;
	out[0] = '\0';
	if( !tmpl || !*tmpl )
		tmpl = "explorer.exe \"%s\"";
	const char* pct = NULL;
	for( const char* p = tmpl; *p; p++ )
	{
		if( *p != '%' )
			continue;
		if( p[1] == '%' )
		{
			p++;
			continue;
		}
		if( p[1] == 's' )
		{
			if( pct )
				return false;
			pct = p;
			p++;
			continue;
		}
		return false;
	}
	if( pct )
	{
		const ptrdiff_t pre = pct - tmpl;
		const char* post = pct + 2;
		_snprintf_s( out, outChars, _TRUNCATE, "%.*s%s%s", (int)pre, tmpl, dirPath, post );
	}
	else
		_snprintf_s( out, outChars, _TRUNCATE, "%s \"%s\"", tmpl, dirPath );
	return true;
}

// from= 0:normal 1:melt 2:compress
void CNoahApp::open_folder( const kiPath& path, int from )
{
	if( from==1 || from==2 ) //-- Shell??X?V??m
		::SHChangeNotify( SHCNE_UPDATEDIR, SHCNF_PATH, (const void*)(const char*)path, NULL );

	//-- ?f?X?N?g?b?v????????J?????
	kiPath dir(path), tmp(kiPath::Dsk,false);
	dir.beBackSlash(false), dir.beShortPath(), tmp.beShortPath();

	if( !tmp.isSame( dir ) )
	{
		//-- Load-INI( ?t?H???_?J????? )
		m_cnfMan.load( OpenDir );
		if( (from==1 && !m_cnfMan.modir())
		 || (from==2 && !m_cnfMan.codir()) )
			return;
		
		char cmdline[1000];
		const char* tmpl = m_cnfMan.openby();
		if( !build_open_folder_cmd( cmdline, sizeof cmdline, tmpl, (const char*)dir ) )
			build_open_folder_cmd( cmdline, sizeof cmdline, "explorer.exe \"%s\"", (const char*)dir );
		STARTUPINFOA si = { sizeof(si) };
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWDEFAULT;
		PROCESS_INFORMATION pi = {};
		if( ::CreateProcessA( NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
		{
			::CloseHandle( pi.hThread );
			::CloseHandle( pi.hProcess );
		}
	}
}

// ?S?V?X?e?????????e???|?????t?H???_?????????
void CNoahApp::get_tempdir( kiPath& tmp )
{
	char buf[MAX_PATH];

	if( m_tmpDir.len()==0 )
	{
		::GetTempFileName( kiPath( kiPath::Tmp ), "noa", 0, buf );
		::DeleteFile( buf );
		m_tmpDir = buf;
		m_tmpDir.beBackSlash( true );
		m_tmpDir.mkdir();
		m_tmpID = ::GetCurrentProcessId();
	}

	::GetTempFileName( m_tmpDir, "noa", m_tmpID++, buf );
	::DeleteFile( buf );
	tmp = buf;
	tmp.beBackSlash( true );
	tmp.mkdir();
}
