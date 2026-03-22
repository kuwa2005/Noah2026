# Regenerates Japanese block of Noah.rc as UTF-8 BOM. ASCII-only script.
$ErrorActionPreference = 'Stop'
function C([uint32[]]$codes) { -join ($codes | ForEach-Object { [char]$_ }) }

$MSPGothic = C @(0xFF2D,0xFF33,0x0020,0xFF30,0x30B4,0x30B7,0x30C3,0x30AF)

$jp = @"
/////////////////////////////////////////////////////////////////////////////
// Japanese resources (UTF-8 with BOM; /utf-8 for rc.exe)

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_JPN)
#ifdef _WIN32
LANGUAGE LANG_JAPANESE, SUBLANG_DEFAULT
#pragma code_page(65001)
#endif //_WIN32

#ifndef _MAC
/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION 3,1,9,9
 PRODUCTVERSION 3,1,9,9
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "041104b0"
        BEGIN
            VALUE "Comments", "INTO THE LEGEND\0"
            VALUE "CompanyName", "kMonos.NET\0"
            VALUE "FileDescription", "Noah( DnD Melter/Freezer )\0"
            VALUE "FileVersion", "3.199\0"
            VALUE "InternalName", "sakasai\0"
            VALUE "LegalCopyright", "Presented by k.inaba (1998-2010)\0"
            VALUE "LegalTrademarks", "\0"
            VALUE "OriginalFilename", "Noah.exe\0"
            VALUE "PrivateBuild", "\0"
            VALUE "ProductName", "Noah\0"
            VALUE "ProductVersion", "3.199\0"
            VALUE "SpecialBuild", "\0"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x411, 1200
    END
END

#endif    // !_MAC


#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE DISCARDABLE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE DISCARDABLE 
BEGIN
    "#include ""winres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE DISCARDABLE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//

IDD_CMPCFG DIALOG DISCARDABLE  0, 0, 219, 121
STYLE DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "$(C @(0x5727,0x7E2E))"
FONT 9, "$MSPGothic"
BEGIN
    GROUPBOX        "$(C @(0x5727,0x7E2E,0x5148,0x30D5,0x30A9,0x30EB,0x30C0))",IDC_STATIC,7,7,205,43
    CONTROL         "$(C @(0x5143,0x306E,0x30D5,0x30A1,0x30A4,0x30EB,0x3068,0x540C,0x3058,0x5834,0x6240))",IDC_DDIR1,"Button",
                    BS_AUTORADIOBUTTON | WS_GROUP,11,20,84,10
    CONTROL         "$(C @(0x6307,0x5B9A,0x3057,0x305F,0x5834,0x6240))",IDC_DDIR2,"Button",BS_AUTORADIOBUTTON | 
                    WS_TABSTOP,11,33,50,10
    EDITTEXT        IDC_DDIR,63,32,123,12,ES_AUTOHSCROLL | WS_GROUP
    PUSHBUTTON      "$(C @(0x53C2,0x7167))",IDC_REF,188,31,19,14,WS_GROUP
    CONTROL         "$(C @(0x5727,0x7E2E,0x5F8C,0x306B,0x958B,0x304F))",IDC_ODIR,"Button",BS_AUTOCHECKBOX | 
                    WS_GROUP | WS_TABSTOP,163,18,43,9
    GROUPBOX        "$(C @(0x5727,0x7E2E,0x5F62,0x5F0F))",IDC_STATIC,7,57,117,57
    COMBOBOX        IDC_CMPEXT,14,81,34,163,CBS_DROPDOWNLIST | CBS_SORT | 
                    CBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_GROUP | 
                    WS_TABSTOP
    COMBOBOX        IDC_CMPMHD,52,81,65,79,CBS_DROPDOWNLIST | 
                    CBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP
    GROUPBOX        "Noah$(C @(0x52D5,0x4F5C,0x30E2,0x30FC,0x30C9))",IDC_STATIC,133,57,79,57
    CONTROL         "$(C @(0x5727,0x7E2E,0x306E,0x307F))",IDC_MODE1,"Button",BS_AUTORADIOBUTTON | 
                    WS_GROUP,147,69,39,8
    CONTROL         "$(C @(0x5727,0x7E2E,0x512A,0x5148))",IDC_MODE2,"Button",BS_AUTORADIOBUTTON,147,79,
                    38,8
    CONTROL         "$(C @(0x5C55,0x958B,0x512A,0x5148))",IDC_MODE3,"Button",BS_AUTORADIOBUTTON | 
                    WS_TABSTOP,147,89,39,8
    CONTROL         "$(C @(0x5C55,0x958B,0x306E,0x307F))",IDC_MODE4,"Button",BS_AUTORADIOBUTTON,147,98,
                    39,12
END

IDD_ARCVIEW DIALOG DISCARDABLE  0, 0, 311, 157
STYLE WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_CAPTION | WS_SYSMENU | 
    WS_THICKFRAME
FONT 9, "$MSPGothic"
BEGIN
    CONTROL         "List1",IDC_FILELIST,"SysListView32",LVS_REPORT | 
                    LVS_SHOWSELALWAYS | WS_BORDER | WS_TABSTOP,0,22,312,120
    PUSHBUTTON      "$(C @(0x3059,0x3079,0x3066))",IDOK,5,3,26,14
    PUSHBUTTON      "$(C @(0x5C55,0x958B))",IDC_MELTEACH,34,3,19,14
    PUSHBUTTON      "$(C @(0x53CD,0x8EE2))",IDC_SELECTINV,78,3,19,14
    EDITTEXT        IDC_DDIR,102,3,145,14,ES_AUTOHSCROLL
    PUSHBUTTON      "...",IDC_REF,249,3,8,14
    DEFPUSHBUTTON   "$(C @(0x8868,0x793A))",IDC_SHOW,56,3,19,14
    LTEXT           "",IDC_STATUSBAR,0,146,309,11
END

IDD_MLTCFG DIALOG DISCARDABLE  0, 0, 219, 121
STYLE DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "$(C @(0x89E3,0x51CD))"
FONT 9, "$MSPGothic"
BEGIN
    GROUPBOX        "$(C @(0x89E3,0x51CD,0x5148,0x30D5,0x30A9,0x30EB,0x30C0))",IDC_STATIC,7,7,205,43
    CONTROL         "$(C @(0x5727,0x7E2E,0x30D5,0x30A1,0x30A4,0x30EB,0x306E,0x5834,0x6240))",IDC_DDIR1,"Button",BS_AUTORADIOBUTTON | 
                    WS_GROUP,11,20,65,10
    CONTROL         "$(C @(0x6307,0x5B9A,0x3057,0x305F,0x5834,0x6240))",IDC_DDIR2,"Button",BS_AUTORADIOBUTTON | 
                    WS_TABSTOP,11,33,50,10
    EDITTEXT        IDC_DDIR,63,32,123,12,ES_AUTOHSCROLL | WS_GROUP
    PUSHBUTTON      "$(C @(0x53C2,0x7167))",IDC_REF,188,31,19,14,WS_GROUP
    CONTROL         "$(C @(0x89E3,0x51CD,0x5F8C,0x306B,0x958B,0x304F))",IDC_ODIR,"Button",BS_AUTOCHECKBOX | 
                    WS_GROUP | WS_TABSTOP,163,18,42,8
    GROUPBOX        "",IDC_STATIC,7,52,205,62
    CONTROL         "$(C @(0x30D5,0x30A9,0x30EB,0x30C0,0x3092,0x4F5C,0x6210,0x3059,0x308B))",IDC_MKDIR,"Button",BS_AUTOCHECKBOX | 
                    WS_GROUP | WS_TABSTOP,16,62,63,10
    CONTROL         "$(C @(0x30D5,0x30A1,0x30A4,0x30EB,0x304C,0x0031,0x3064,0x306E,0x3068,0x304D,0x306F,0x4F5C,0x3089,0x306A,0x3044))",IDC_MKDIR1,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,32,73,86,10
    CONTROL         "$(C @(0x30D5,0x30A9,0x30EB,0x30C0,0x304C,0x0031,0x3064,0x306E,0x3068,0x304D,0x306F,0x4F5C,0x3089,0x306A,0x3044))",IDC_MKDIR2,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,32,84,86,10
    CONTROL         "$(C @(0x672B,0x5C3E,0x306E,0x9023,0x756A,0x3092,0x4ED8,0x3051,0x306A,0x3044))",IDC_MKDIR3,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,32,95,64,10
END

IDD_WINCFG DIALOG DISCARDABLE  0, 0, 219, 121
STYLE DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Windows$(C @(0x9023,0x643A))"
FONT 9, "$MSPGothic"
BEGIN
    GROUPBOX        "$(C @(0x30D5,0x30A1,0x30A4,0x30EB,0x306E,0x95A2,0x9023,0x4ED8,0x3051))",IDC_STATIC,7,7,205,39
    CONTROL         "&LZH",IDC_LZH,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,13,
                    18,23,9
    CONTROL         "&ZIP",IDC_ZIP,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,13,
                    30,23,10
    CONTROL         "&CAB",IDC_CAB,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,40,
                    18,26,10
    CONTROL         "&RAR",IDC_RAR,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,40,
                    30,25,9
    CONTROL         "&TAR",IDC_TAR,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,68,
                    18,26,10
    CONTROL         "&YZ1",IDC_YZ1,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,68,
                    30,23,9
    CONTROL         "&GCA",IDC_GCA,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,97,
                    18,25,10
    CONTROL         "AR&J",IDC_ARJ,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,97,
                    30,24,10
    CONTROL         "&BGA",IDC_BGA,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,126,
                    18,25,9
    CONTROL         "AC&E",IDC_ACE,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,126,
                    30,25,9
    CONTROL         "C&PT",IDC_CPT,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,155,
                    18,26,9
    CONTROL         "JA&K",IDC_JAK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,155,
                    30,26,9
    CONTROL         "&7Z",IDC_7Z,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,182,
                    18,23,10
    PUSHBUTTON      "$(C @(0x8A73,0x7D30))",IDC_ASS,190,30,17,14
    GROUPBOX        "$(C @(0x30C9,0x30E9,0x30C3,0x30B0,0xFF06,0x30C9,0x30ED,0x30C3,0x30D7))",IDC_STATIC,7,51,132,29
    CONTROL         "$(C @(0x3053,0x3053,0x3067,0x5727,0x7E2E,0x0028,0x0026,0x0048,0x0029))",IDC_CMP,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,15,63,57,10
    CONTROL         "$(C @(0x3053,0x3053,0x3067,0x89E3,0x51CD,0x0028,0x0026,0x0058,0x0029))",IDC_MLT,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,75,63,56,10
    GROUPBOX        "$(C @(0x30B7,0x30E7,0x30FC,0x30C8,0x30AB,0x30C3,0x30C8))",IDC_STATIC,7,86,132,28
    CONTROL         "$(C @(0x9001,0x308B,0x0028,0x0026,0x0053,0x0029))",IDC_SND,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,
                    15,98,37,10
    CONTROL         "$(C @(0x30C7,0x30B9,0x30AF,0x30C8,0x30C3,0x30D7,0x0028,0x0026,0x0044,0x0029))",IDC_DSK,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,75,97,60,10
    LTEXT           "$(C @(0x7BA1,0x7406,0x8005,0x6B0A,0x9650,0x304C,0x306A,0x3044,0x305F,0x3081,0x3001,0x30D5,0x30A1,0x30A4,0x30EB,0x306E,0x95A2,0x9023,0x4ED8,0x3051,0x3092,0x5909,0x66F4,0x3067,0x304D,0x307E,0x305B,0x3093,0x3002))",
                    IDC_NOADMIN,146,57,62,49
END

IDD_INFCFG DIALOG DISCARDABLE  0, 0, 219, 97
STYLE DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "About"
FONT 9, "Courier New"
BEGIN
    ICON            IDI_MAIN,IDC_STATIC,28,21,20,20,SS_REALSIZEIMAGE
    CTEXT           "Noah 3.199\n\npresented by k.inaba",IDC_STATIC,13,45,53,
                    36,WS_TABSTOP
    EDITTEXT        IDC_VERSION,78,7,134,83,ES_MULTILINE | ES_AUTOHSCROLL | 
                    ES_READONLY | WS_VSCROLL
END

IDD_PROGRESS DIALOG DISCARDABLE  0, 0, 214, 43
STYLE DS_ABSALIGN | DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | 
    WS_SYSMENU
CAPTION "$(C @(0x5C55,0x958B,0x4E2D,0x002E,0x002E,0x002E))"
FONT 9, "$MSPGothic"
BEGIN
    DEFPUSHBUTTON   "$(C @(0x505C,0x6B62))",IDCANCEL,157,7,50,14
    CONTROL         "Progress1",IDC_BAR,"msctls_progress32",PBS_SMOOTH | 
                    WS_BORDER,7,25,200,12
    LTEXT           "",IDC_FNAME,7,7,142,11
END

IDD_PASSWORD DIALOG DISCARDABLE  0, 0, 206, 64
STYLE DS_ABSALIGN | DS_MODALFRAME | DS_SETFOREGROUND | DS_CENTER | WS_POPUP | 
    WS_CAPTION | WS_SYSMENU
CAPTION "Noah"
FONT 9, "$MSPGothic"
BEGIN
    LTEXT           "$(C @(0x30D1,0x30B9,0x30EF,0x30FC,0x30C9,0x3092,0x5165,0x529B,0x3057,0x3066,0x304F,0x3060,0x3055,0x3044,0x003A))",IDC_MESSAGE,4,5,196,20
    EDITTEXT        IDC_EDIT,7,29,194,12,ES_AUTOHSCROLL
    CONTROL         "$(C @(0x30DE,0x30B9,0x30AF,0x8868,0x793A))",IDC_MASK,"Button",BS_AUTOCHECKBOX | WS_TABSTOP,15,
                    47,31,10
    PUSHBUTTON      "OK",IDOK,96,46,50,12
    PUSHBUTTON      "$(C @(0x30AD,0x30E3,0x30F3,0x30BB,0x30EB))",IDCANCEL,149,46,50,12
END

IDD_ANYASS DIALOG DISCARDABLE  0, 0, 207, 97
STYLE DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "$(C @(0x305D,0x306E,0x4ED6,0x306E,0x5F62,0x5F0F,0x306E,0x95A2,0x9023,0x4ED8,0x3051))"
FONT 9, "$MSPGothic"
BEGIN
    LISTBOX         IDC_NASSOC,7,7,77,62,LBS_SORT | LBS_NOINTEGRALHEIGHT | 
                    LBS_EXTENDEDSEL | WS_VSCROLL | WS_TABSTOP
    PUSHBUTTON      "$(C @(0x8FFD,0x52A0)) >>",IDC_ADD,90,18,26,14
    LISTBOX         IDC_ASSOC,123,7,77,62,LBS_SORT | LBS_NOINTEGRALHEIGHT | 
                    LBS_EXTENDEDSEL | WS_VSCROLL | WS_TABSTOP
    PUSHBUTTON      "<< $(C @(0x524A,0x9664))",IDC_DEL,90,38,27,14
    DEFPUSHBUTTON   "$(C @(0x5B9F,0x884C))",IDOK,95,76,50,14
    PUSHBUTTON      "$(C @(0x4E2D,0x6B62))",IDCANCEL,150,76,50,14
END

IDD_PATHCHECK DIALOG DISCARDABLE  0, 0, 298, 69
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Noah $(C @(0x8B66,0x544A))"
FONT 9, "$MSPGothic"
BEGIN
    PUSHBUTTON      "$(C @(0x306F,0x3044,0x0028,0x0026,0x0059,0x0029))",IDYES,111,48,50,14
    PUSHBUTTON      "$(C @(0x30AD,0x30E3,0x30F3,0x30BB,0x30EB,0x0028,0x0026,0x0043,0x0029))",ID_ALLNO,241,48,50,14
    LTEXT           "$(C @(0x7D76,0x5BFE,0x30D1,0x30B9,0x3001,0x307E,0x305F,0x306F,0x89AA,0x30D5,0x30A9,0x30EB,0x30C0,0x3078,0x306E,0x53C2,0x7167,0xFF08,0xFF0E,0xFF0E,0xFF09,0x3092,0x542B,0x307F,0x307E,0x3059,0x3002,0x672C,0x5F53,0x306B,0x5C55,0x958B,0x3057,0x307E,0x3059,0x304B,0xFF1F))",IDC_STATIC,7,7,264,10
    LTEXT           "",IDC_PATHNAME,16,17,275,23
    PUSHBUTTON      "$(C @(0x3059,0x3079,0x3066,0x306F,0x3044,0x0028,0x0026,0x0041,0x0029))",ID_ALLYES,166,48,50,14
END


/////////////////////////////////////////////////////////////////////////////
//
// DESIGNINFO
//

#ifdef APSTUDIO_INVOKED
GUIDELINES DESIGNINFO DISCARDABLE 
BEGIN
    IDD_CMPCFG, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 212
        TOPMARGIN, 7
        BOTTOMMARGIN, 114
    END

    IDD_ARCVIEW, DIALOG
    BEGIN
        RIGHTMARGIN, 304
        TOPMARGIN, 3
        BOTTOMMARGIN, 148
    END

    IDD_MLTCFG, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 212
        TOPMARGIN, 7
        BOTTOMMARGIN, 114
    END

    IDD_WINCFG, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 212
        TOPMARGIN, 7
        BOTTOMMARGIN, 114
    END

    IDD_INFCFG, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 212
        TOPMARGIN, 7
        BOTTOMMARGIN, 90
    END

    IDD_PROGRESS, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 207
        TOPMARGIN, 7
        BOTTOMMARGIN, 36
    END

    IDD_PASSWORD, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 199
        TOPMARGIN, 7
        BOTTOMMARGIN, 57
    END

    IDD_ANYASS, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 200
        TOPMARGIN, 7
        BOTTOMMARGIN, 90
    END

    IDD_PATHCHECK, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 291
        TOPMARGIN, 7
        BOTTOMMARGIN, 62
    END
END
#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_MAIN                ICON    DISCARDABLE     "noah.ico"
IDI_B2E                 ICON    DISCARDABLE     "b2e.ico"

/////////////////////////////////////////////////////////////////////////////
//
// Accelerator
//

IDR_ACCEL ACCELERATORS DISCARDABLE 
BEGIN
    "F",            IDA_MYDIR,              VIRTKEY, CONTROL, NOINVERT
    VK_F1,          IDA_HELP,               VIRTKEY, NOINVERT
END


/////////////////////////////////////////////////////////////////////////////
//
// 24
//

1                       24      MOVEABLE PURE   "manifest.xml"

/////////////////////////////////////////////////////////////////////////////
//
// String Table
//

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_M_ERROR             "$(C @(0x89E3,0x51CD,0x3067,0x304D,0x307E,0x305B,0x3093,0xFF01,0x000A,0x5727,0x7E2E,0x30D5,0x30A1,0x30A4,0x30EB,0x3067,0x306A,0x3044,0x304B,0x3001,0x58CA,0x308C,0x3066,0x3044,0x307E,0x3059,0x3002))"
    IDS_C_ERROR             "$(C @(0x3053,0x306E,0x5727,0x7E2E,0x65B9,0x5F0F,0x306B,0x306F,0x5BFE,0x5FDC,0x3057,0x3066,0x3044,0x307E,0x305B,0x3093,0xFF01,0x000A,0x5FC5,0x8981,0x306A,0x0020,0x0044,0x004C,0x004C,0x0020,0x304C,0x4E0D,0x8DB3,0x3057,0x3066,0x3044,0x308B,0x53EF,0x80FD,0x6027,0x304C,0x3042,0x308A,0x307E,0x3059,0x3002,0x004E,0x006F,0x0061,0x0068,0x0020,0x7528,0x30D7,0x30E9,0x30B0,0x30A4,0x30F3,0x306E,0x30A4,0x30F3,0x30B9,0x30C8,0x30FC,0x30EB,0x3092,0x78BA,0x8A8D,0x3057,0x3066,0x304F,0x3060,0x3055,0x3044,0x3002))"
    IDS_CHOOSEDIR           "$(C @(0x5C55,0x958B,0x5148,0x306E,0x30D5,0x30A9,0x30EB,0x30C0,0x3092,0x9078,0x629E,0x3057,0x3066,0x304F,0x3060,0x3055,0x3044,0x3002))"
    IDS_FNAME               "$(C @(0x30D5,0x30A1,0x30A4,0x30EB,0x540D))"
    IDS_SIZE                "$(C @(0x30B5,0x30A4,0x30BA))"
    IDS_DATETIME            "$(C @(0x65E5,0x6642))"
    IDS_RATIO               "$(C @(0x5727,0x7E2E,0x7387))"
    IDS_METHOD              "$(C @(0x65B9,0x5F0F))"
    IDS_PATH                "$(C @(0x30D1,0x30B9))"
    IDS_EXECUTING           "$(C @(0x004E,0x006F,0x0061,0x0068,0x0020,0x304B,0x3089,0x8D77,0x52D5,0x3057,0x305F,0x30A2,0x30D7,0x30EA,0x30B1,0x30FC,0x30B7,0x30E7,0x30F3,0x304C,0x5B9F,0x884C,0x4E2D,0x3067,0x3059,0x3002,0x7D42,0x4E86,0x3059,0x308B,0x3068,0x30C7,0x30A3,0x30B9,0x30AF,0x4E0A,0x306B,0x4F5C,0x696D,0x30D5,0x30A1,0x30A4,0x30EB,0x304C,0x6B8B,0x308B,0x5834,0x5408,0x304C,0x3042,0x308A,0x307E,0x3059,0x3002,0x7D42,0x4E86,0x3057,0x3066,0x3082,0x3088,0x308D,0x3057,0x3044,0x3067,0x3059,0x304B,0xFF1F))"
    IDS_NOLIST              "$(C @(0x30A8,0x30E9,0x30FC,0x003A,0x0020,0x30A2,0x30FC,0x30AB,0x30A4,0x30D6,0x306E,0x5185,0x5BB9,0x3092,0x5217,0x6319,0x3067,0x304D,0x307E,0x305B,0x3093,0x3002))"
    IDS_ARCVIEW_MSG         "[%d $(C @(0x30D5,0x30A1,0x30A4,0x30EB))] [$(C @(0x5727,0x7E2E,0x7387)) %d%%] [%s]"
END

#endif    // Japanese resources
/////////////////////////////////////////////////////////////////////////////


"@

$utf8Bom = New-Object System.Text.UTF8Encoding $true
$rcPath = Join-Path (Split-Path $PSScriptRoot -Parent) 'Noah.rc'
$all = [System.IO.File]::ReadAllText($rcPath, $utf8Bom)
$markerJ = '// English (U.S.) resources'
$ix = $all.IndexOf($markerJ)
if ($ix -lt 0) { throw "Marker not found: $markerJ" }
$head = @"
//Microsoft Developer Studio generated resource script.
//
#include `"resource.h`"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include `"winres.h`"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

"@
$tail = $all.Substring($ix)
$out = $head + $jp + $tail
[System.IO.File]::WriteAllText($rcPath, $out, $utf8Bom)

# Verify CAPTION UTF-8 for 圧縮
$check = [regex]::Match($out, 'IDD_CMPCFG[\s\S]*?CAPTION "([^"]*)"').Groups[1].Value
$bytes = $utf8Bom.GetBytes($check)
Write-Host "CAPTION check: len=$($check.Length) UTF8=$([BitConverter]::ToString($bytes))"
