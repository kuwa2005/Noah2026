
#include "stdafx.h"
#include "../kilib/kilib.h"
#include "resource.h"


//-- ?G?pFunctions ----------------------------------------------------------

void getProgramFiles( kiPath* path )
{
	// ?P?D???W?X?g??????
	kiRegKey key;
	if( key.open( HKEY_CLASSES_ROOT, "Software\\Microsoft\\Windows\\CurrentVersion", KEY_QUERY_VALUE ) )
		if( key.get( "ProgramFilesDir",  path )
		 || key.get( "ProgramFilesPath", path ) )
			return;

	// ?Q?DWindows?f?B???N?g?????????
	char buf[MAX_PATH];
	if( ::GetWindowsDirectory( buf, MAX_PATH ) )
	{
		for( char* p=buf; *p!='\\' && *p; p++ );
		*p='\0';

		*path= buf, *path += '\\', *path += "Program Files";
		if( ::GetFileAttributes( *path ) != 0xffffffff )
			return;
	}

	// ?R?D????[??????N???f?B???N?g??
	path->beSpecialPath( kiPath::Exe );
}

void createShortCut( const kiPath& original, const kiPath& at, const char* name )
{
	IShellLink* psl;
	if( SUCCEEDED(::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&psl)) )
	{
		psl->SetPath( original );

		IPersistFile* ppf;
		if( SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(void**)&ppf)) )
		{
			WORD wsz[MAX_PATH]; 
			kiPath lnkfile( at );
			lnkfile += name, lnkfile += ".lnk";
			::MultiByteToWideChar(CP_ACP,0,lnkfile,-1,wsz,MAX_PATH);
			ppf->Save(wsz,TRUE);
			ppf->Release();
		}
		psl->Release();
	}
}


//-- ???C?? -------------------------------------------------------------------

class CKInstApp : public kiApp, kiDialog // kiApp??R???X?g???N?^???????????????B?????????B
{
	CKInstApp() : kiDialog( IDD_MAIN ) {}
	friend void kilib_create_new_app();

//-- ???C?????[?`?? ---------

	void run( kiCmdParser& cmd )
	{
		if( cmd.option().len() )
			if( cmd.option()[0][1] == 'i' )
			{
				install();
				return;
			}
			else if( cmd.option()[0][1] == 'u' && cmd.param().len() )
			{
				uninstall( cmd.param()[0] );
				return;
			}
		boot_uninstaller();
	}

	void install()
	{
		doModal();
		if( IDCANCEL == getEndCode() )
			return;

		if( !copy() )
		{
			msgBox( kiStr().loadRsrc(IDS_COPYFAIL) );
			return;
		}

		if( !regist() )
			msgBox( kiStr().loadRsrc(IDS_REGISTFAIL) );

		do_caldix();

		msgBox( kiStr().loadRsrc(IDS_INSTALLFINISH),"Noah",MB_OK|MB_ICONINFORMATION );
	}

	void uninstall( const char* dir )
	{
		m_destdir = dir;
		for( int i=0; i!=sizeof(m_assoc)/sizeof(bool); i++ )
			m_assoc[i]=false;
		Sleep(200);
		unregist();
		remove();
		kill_later( kiPath(kiPath::Exe_name) );
		msgBox( kiStr().loadRsrc(IDS_UNINSTALLFINISH),"Noah",MB_OK|MB_ICONINFORMATION );
	}

	void boot_uninstaller()
	{
		if( IDNO==msgBox( kiStr(500).loadRsrc(IDS_UNINSTOK), "Noah", MB_YESNO|MB_ICONQUESTION ) )
			return;

		kiPath self(kiPath::Exe_name), to(kiPath::Tmp), pos(kiPath::Exe);
		to += "noaunins.exe";
		::CopyFile( self, to, FALSE );
		to += " -u \"", to += pos, to+='"';

		PROCESS_INFORMATION pi;
		STARTUPINFO si;	ki_memzero( &si,sizeof(si) ); si.cb=sizeof(si);
		if( !::CreateProcess( NULL,const_cast<char*>((const char*)to),
			NULL,NULL,FALSE,CREATE_NEW_PROCESS_GROUP|NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi ) )
			return; // ?N?????????????c?B

		::CloseHandle( pi.hThread );
		::CloseHandle( pi.hProcess );

		// ???W?X?g????A???C???X?g?[??????
		kiRegKey key;
		if( key.open( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall" ) )
			key.delSubKey( "Noah" );
	}

//-- ???C???_?C?A???O????? --

	bool onOK()
	{
		char str[MAX_PATH];
		sendMsgToItem( IDC_INSTALLTO, WM_GETTEXT, MAX_PATH, (LPARAM)str );
		if( str[1]!=':' || str[2]!='\\' )
		{
			msgBox( kiStr().loadRsrc(IDS_BADPATH) );
			return false;
		}

		m_destdir = str;
		m_destdir.beBackSlash(true);
		for( UINT i=IDC_LZH; i<=IDC_DSK; i++ )
			m_assoc[i-IDC_LZH] = (BST_CHECKED==sendMsgToItem( i, BM_GETCHECK ));
		m_assoc[IDC_STT-IDC_LZH] = true;

		return true;
	}
	BOOL onInit()
	{
		for( int j=0; j<18; j++ )
			m_assoc[j] = true;

		kiPath prg;
		getPrevPos( &prg );
		if( prg.len()==0 ) // ?V?K?C???X?g?[??
			getProgramFiles( &prg ), prg.beBackSlash(true), prg += "Noah\\";
		else // ????????????
		{
			HINSTANCE hDLL = kiSUtil::loadLibrary( "NoahXt.dll" );
			if( hDLL )
			{
				typedef bool (WINAPI * XT_IA)();
				XT_IA Init = (XT_IA)::GetProcAddress( hDLL, "Init" );
				if( Init() )
				{
					typedef void (WINAPI * XT_LS)(bool*,bool*);
					typedef void (WINAPI * XT_AS)(bool*);
					typedef void (WINAPI * XT_LSEX)(const char*,bool*);
					XT_LS LoadSE    = (XT_LS)::GetProcAddress( hDLL, "LoadSE" );
					XT_AS LoadAssoc = (XT_AS)::GetProcAddress( hDLL, "LoadAS" );
					XT_LSEX LoadASEx = (XT_LSEX)::GetProcAddress( hDLL, "LoadASEx" );
					LoadSE( &m_assoc[13], &m_assoc[14] );
					LoadASEx( "7z\0", &m_assoc[12] );
					LoadAssoc( m_assoc );
				}
				::FreeLibrary( hDLL );
			}
			m_assoc[15] = kiSUtil::exist( kiPath(kiPath::Snd)+="Noah.lnk" );
			m_assoc[16] = kiSUtil::exist( kiPath(kiPath::Dsk)+="Noah.lnk" );
		}

		sendMsgToItem( IDC_INSTALLTO, WM_SETTEXT, 0, (LPARAM)(const char*)prg );
		for( UINT i=IDC_LZH; i<=IDC_DSK; i++ )
			if( m_assoc[i-IDC_LZH] )
				sendMsgToItem( i, BM_SETCHECK, BST_CHECKED );
		return FALSE;
	}
	BOOL CALLBACK proc( UINT msg, WPARAM wp, LPARAM lp )
	{
		if( msg != WM_COMMAND )
			return FALSE;
		switch( LOWORD(wp) )
		{
		case IDC_REF:{
			char str[MAX_PATH];
			sendMsgToItem( IDC_INSTALLTO, WM_GETTEXT, MAX_PATH, (LPARAM)str );
			if( kiSUtil::getFolderDlg( str, hwnd(), kiStr().loadRsrc(IDS_DIR), str ) )
			{
				kiPath x(str); x.beBackSlash(true); x+="Noah\\";
				sendMsgToItem( IDC_INSTALLTO, WM_SETTEXT, 0, (LPARAM)(const char*)x );
			}
			}break;
		case IDC_ALL:{
			for( UINT i=IDC_LZH; i<=IDC_DSK; i++ )
				sendMsgToItem( i, BM_SETCHECK, BST_UNCHECKED );
			}break;
		default:
			return FALSE;
		}
		return TRUE;
	}

//-- ?C???X?g?[?? ---------

	bool m_assoc[18]; // LZH-JAK, 7Z, CMP MLT, SND DSK STT
	kiPath m_destdir;

	bool copy()
	{
		kiPath exe(kiPath::Exe,false), inst(m_destdir);
		inst.beBackSlash(false);

		// ????????S?R?s
		bool r = copy_dir2dir(exe,inst);

		// manual???????????[?????
		kiPath manE(m_destdir), manJ(m_destdir);
		manE.beBackSlash(true), manJ.beBackSlash(true);
		manE += "manual-e.htm", manJ += "manual.htm";
		if( ::GetACP() != 932 )
			::CopyFile( manE, manJ, FALSE );
		::DeleteFile( manE );

		return r;
	}

	bool regist()
	{
		// NoahXt.dll????Y??
		kiPath xtdll( m_destdir );
		xtdll += "NoahXt.dll";
		HINSTANCE hDLL = kiSUtil::loadLibrary( xtdll );
		if( hDLL )
		{
			typedef bool (WINAPI * XT_IA)();
			typedef void (WINAPI * XT_LS)(bool*,bool*);
			typedef void (WINAPI * XT_SS)(bool,bool);
			typedef void (WINAPI * XT_AS)(bool*);
			typedef void (WINAPI * XT_SSEX)(const char*,bool);
			XT_IA Init = (XT_IA)::GetProcAddress( hDLL, "Init" );
			if( Init() )
			{
				XT_SS SaveSE = (XT_SS)::GetProcAddress( hDLL, "SaveSE" );
				XT_AS SaveAssoc = (XT_AS)::GetProcAddress( hDLL, "SaveAS" );
				XT_SSEX SaveASEx = (XT_SSEX)::GetProcAddress( hDLL, "SaveASEx" );
				SaveSE( m_assoc[13], m_assoc[14] );
				SaveASEx( "7z\0", m_assoc[12] );
				SaveAssoc( m_assoc );
			}
			::FreeLibrary( hDLL );
		}

		// ?V???[?g?J?b?g
		::CoInitialize( NULL );
			kiPath tmp(m_destdir); tmp += "Noah.exe";
			if( m_assoc[15] )
				createShortCut( tmp, kiPath(kiPath::Snd), "Noah" );
			if( m_assoc[16] )
				createShortCut( tmp, kiPath(kiPath::Dsk), "Noah" );
			if( m_assoc[17] )
			{
				kiPath StartMenu( CSIDL_PROGRAMS ),rsrc;
				StartMenu += "Noah\\";
				StartMenu.mkdir();

				createShortCut( tmp, StartMenu, "Noah" );
				tmp = m_destdir, tmp += "caldix.exe";
				createShortCut( tmp, StartMenu, rsrc.loadRsrc(IDS_CALDIX) );
				tmp = m_destdir, tmp += "manual.htm";
				createShortCut( tmp, StartMenu, rsrc.loadRsrc(IDS_HELP) );
				tmp = m_destdir, tmp += "uninst.exe";
				createShortCut( tmp, StartMenu, rsrc.loadRsrc(IDS_UNINSTALLER) );
			}
		::CoUninitialize();

		// ?A???C???X?g?[?????????W?X?g????
		kiPath uninst( m_destdir );
		uninst += "uninst.exe";

		kiRegKey key;
		if( !key.create( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Noah", KEY_WRITE ) )
			return false;
		key.set( "DisplayName", kiStr().loadRsrc(IDS_NOAH) );
		key.set( "UninstallString", uninst );
		return true;
	}

	void do_caldix()
	{
		if( IDYES==msgBox( kiStr(1000).loadRsrc(IDS_DLLINST),"Noah",MB_YESNO|MB_ICONQUESTION ) )
		{
			kiStr cld;
			cld += '"';
			cld += m_destdir;
			cld += "caldix.exe";
			cld += '"';

			// ?v???Z?X?J?n
			PROCESS_INFORMATION pi;
			STARTUPINFO si;	ki_memzero( &si,sizeof(si) ); si.cb=sizeof(si);
			if( !::CreateProcess( NULL,const_cast<char*>((const char*)cld),
				NULL,NULL,FALSE,CREATE_NEW_PROCESS_GROUP|NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi ) )
				return;

			// ?I????@
			::CloseHandle( pi.hThread );
			::WaitForSingleObject( pi.hProcess, INFINITE );
			::CloseHandle( pi.hProcess );
		}
	}

	bool copy_dir2dir( kiPath& from, kiPath& to )
	{
		if( !kiSUtil::isdir(from) )
		{
			if( ::CopyFile( from, to, FALSE ) )
				return true;
			if( 0==ki_strcmpi( from.ext(), "dll" ) )
				return copy_later(from, to);
			return false;
		}

		from += '\\', to += '\\', to.mkdir();
		if( !kiSUtil::isdir(to) )
			return false;

		kiFindFile find;
		WIN32_FIND_DATA fd;
		kiPath src, dst, wild(from); wild+="\\*";
		for( find.begin( wild ); find.next(&fd); )
		{
			src = from, src+='\\', src += fd.cFileName;
			dst = to  , dst+='\\', dst += fd.cFileName;
			if( !copy_dir2dir( src, dst ) )
				return false;
		}
		return true;
	}

	void getPrevPos( kiPath* path )
	{
		*path = "";

		kiRegKey key;
		if( !key.open( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Noah", KEY_READ ) )
			return;
		key.get( "UninstallString", path );
		path->beDirOnly();
	}

//-- ?A???C???X?g?[?? ---------

	typedef bool (WINAPI * XT_IA)();
	typedef void (WINAPI * XT_LS)(bool*,bool*);
	typedef void (WINAPI * XT_SS)(bool,bool);
	typedef void (WINAPI * XT_AS)(bool*);
	typedef void (WINAPI * XT_SAX)(const char*,bool);
	typedef void (WINAPI * XT_LAX)(const char*,bool*);
	bool unregist()
	{
		// NoahXt.dll????Y??
		kiPath xtdll( m_destdir );
		xtdll += "NoahXt.dll";
		HINSTANCE hDLL = kiSUtil::loadLibrary( xtdll );
		if( hDLL )
		{
			XT_IA Init = (XT_IA)::GetProcAddress( hDLL, "Init" );
			if( Init() )
			{
				XT_SAX SaveASEx = (XT_SAX)::GetProcAddress( hDLL, "SaveASEx" );
				XT_LAX LoadASEx = (XT_LAX)::GetProcAddress( hDLL, "LoadASEx" );
				unregist_b2e( SaveASEx, LoadASEx );

				XT_SS SaveSE = (XT_SS)::GetProcAddress( hDLL, "SaveSE" );
				XT_AS SaveAssoc = (XT_AS)::GetProcAddress( hDLL, "SaveAS" );
				SaveSE( m_assoc[13], m_assoc[14] );
				SaveAssoc( m_assoc );
			}
			::FreeLibrary( hDLL );
		}

		// ?V???[?g?J?b?g
		::CoInitialize( NULL );
		kiPath snd(kiPath::Snd); snd += "Noah.lnk";
		kiPath dsk(kiPath::Dsk); dsk += "Noah.lnk";
		kiPath stt(CSIDL_PROGRAMS); stt += "Noah";
		::DeleteFile(snd), ::DeleteFile(dsk), stt.remove();

		return true;
	}

	static void crack_str( char* p )
	{
		for( ; *p; p=kiStr::next(p) )
			if( *p=='.' )
				*p++ = '\0';
		*++p = '\0';
	}

	void unregist_b2e( XT_SAX SaveASEx, XT_LAX LoadASEx )
	{
		char* first_dot;
		kiFindFile f;
		WIN32_FIND_DATA fd;
		kiPath b2ewild(m_destdir);
		b2ewild.beBackSlash(true);

		f.begin( b2ewild+="b2e\\*.b2e" );
		while( f.next(&fd) )
			if( fd.cFileName[0] != '#' ) // # ?t??????k??p
			{
				// ?g???q????o??
				::CharLower( fd.cFileName );
				first_dot = const_cast<char*>(kiPath::ext_all(fd.cFileName)-1);
				*first_dot = '\0';
				crack_str( fd.cFileName );
				// ??A?????
				SaveASEx( fd.cFileName, false );
			}
		SaveASEx( "7z\0", false );
	}

	bool remove()
	{
		::SetCurrentDirectory( kiPath(kiPath::Exe) );

		kiPath tmp;
		m_destdir.beBackSlash(true);
		// ?C???X?g?[?????????m?????S??
		tmp=m_destdir, tmp+="Noah.exe",   ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="Noah.ini",   ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="uninst.exe", ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="caldix.exe", ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="caldix.ini", ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="ReadMe.txt", ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="manual.htm", ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="html",       tmp.remove();
		tmp=m_destdir, tmp+="b2e\\jak.b2e",     ::DeleteFile(tmp);
		tmp=m_destdir, tmp+="b2e\\aboutb2e.txt",::DeleteFile(tmp);
		tmp=m_destdir, tmp+="b2e",        ::RemoveDirectory(tmp);
		tmp=m_destdir, tmp+="NoahXt.dll", ::DeleteFile(tmp);
		::RemoveDirectory(m_destdir);
		// NoahXt.dll???N??????????m????
		if( kiSUtil::exist(tmp) )
		{
			kill_later(tmp);
			tmp.beDirOnly();
			tmp.beBackSlash(false);
			kill_later(tmp);
		}

		return true;
	}

	void kill_later( const char* pszFile )
	{
		// "MoveFileEx Not Supported in Windows 95 But Functionality Is"
		if( ::MoveFileEx( pszFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT ) )
			return;
		char shortPath[MAX_PATH];
		if( !::GetShortPathName( pszFile, shortPath, sizeof(shortPath) ) )
			return;

		kiPath inifile( kiPath::Win ); inifile+="wininit.ini";

		char buf[30000];
		DWORD n = ::GetPrivateProfileSection( "Rename", buf, sizeof(buf), inifile );
		if( n >= sizeof(buf) - 2 )
			return;
		char* p = buf;
		while(*p)while(*p++);

		size_t rem = sizeof(buf) - (size_t)(p - buf);
		if( FAILED( StringCchCopyA( p, rem, "NUL=" ) )
		 || FAILED( StringCchCatA( p, rem, shortPath ) ) )
			return;
		while(*p++);
		if( sizeof(buf) - (size_t)(p - buf) < 1u )
			return;
		*p='\0';

		::WritePrivateProfileSection( "Rename", buf, inifile );

		// ?m???????????
		::WritePrivateProfileString( NULL, NULL, NULL, inifile );
	}

	bool copy_later( const char* from, const char* to )
	{
		char from_temp[MAX_PATH];
		if( FAILED( StringCchCopyA( from_temp, sizeof(from_temp), to ) )
		 || FAILED( StringCchCatA( from_temp, sizeof(from_temp), ".new" ) ) )
			return false;
		if( !::CopyFile( from, from_temp, FALSE ) )
			return false;

		char shortFrom[MAX_PATH];
		char shortTo[MAX_PATH];
		if( !::GetShortPathName( from_temp, shortFrom, sizeof(shortFrom) )
		 || !::GetShortPathName( to, shortTo, sizeof(shortTo) ) )
			return false;
		kiPath inifile( kiPath::Win ); inifile+="wininit.ini";

		char buf[30000];
		DWORD n = ::GetPrivateProfileSection( "Rename", buf, sizeof(buf), inifile );
		if( n >= sizeof(buf) - 2 )
			return false;
		char* p = buf;
		while(*p)while(*p++);

		size_t rem = sizeof(buf) - (size_t)(p - buf);
		if( FAILED( StringCchCopyA( p, rem, "NUL=" ) )
		 || FAILED( StringCchCatA( p, rem, shortTo ) ) )
			return false;
		while(*p++);
		if( sizeof(buf) - (size_t)(p - buf) < 2u )
			return false;
		*p++='\r',*p++='\n';
		rem = sizeof(buf) - (size_t)(p - buf);
		if( FAILED( StringCchCopyA( p, rem, shortTo ) )
		 || FAILED( StringCchCatA( p, rem, "=" ) )
		 || FAILED( StringCchCatA( p, rem, shortFrom ) ) )
			return false;
		while(*p++);
		if( sizeof(buf) - (size_t)(p - buf) < 1u )
			return false;
		*p='\0';

		::WritePrivateProfileSection( "Rename", buf, inifile );

		// ?m???????????
		::WritePrivateProfileString( NULL, NULL, NULL, inifile );
		return true;
	}
};

void kilib_create_new_app()
{
	new CKInstApp;
}
