# Microsoft Developer Studio Generated NMAKE File, Based on jkcons32.dsp
!IF "$(CFG)" == ""
CFG=jkcons32 - Win32 Debug
!MESSAGE 構成が指定されていません。デフォルトの jkcons32 - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "jkcons32 - Win32 Release" && "$(CFG)" != "jkcons32 - Win32 Debug"
!MESSAGE 指定された ビルド モード "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE コマンド ライン上でマクロの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "jkcons32.mak" CFG="jkcons32 - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なビルド モード:
!MESSAGE 
!MESSAGE "jkcons32 - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "jkcons32 - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jkcons32 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\jkcons32.lib"


CLEAN :
	-@erase "$(INTDIR)\bschar32.obj"
	-@erase "$(INTDIR)\clrln32.obj"
	-@erase "$(INTDIR)\clrscr32.obj"
	-@erase "$(INTDIR)\cr32.obj"
	-@erase "$(INTDIR)\curbk32.obj"
	-@erase "$(INTDIR)\curon32.obj"
	-@erase "$(INTDIR)\curup32.obj"
	-@erase "$(INTDIR)\delay32.obj"
	-@erase "$(INTDIR)\erscsb32.obj"
	-@erase "$(INTDIR)\erseof32.obj"
	-@erase "$(INTDIR)\erseos32.obj"
	-@erase "$(INTDIR)\getcls32.obj"
	-@erase "$(INTDIR)\getcon32.obj"
	-@erase "$(INTDIR)\getcp32.obj"
	-@erase "$(INTDIR)\getmem32.obj"
	-@erase "$(INTDIR)\getnor32.obj"
	-@erase "$(INTDIR)\inicon32.obj"
	-@erase "$(INTDIR)\inscon32.obj"
	-@erase "$(INTDIR)\locate32.obj"
	-@erase "$(INTDIR)\pushcp32.obj"
	-@erase "$(INTDIR)\putcon32.obj"
	-@erase "$(INTDIR)\setttl32.obj"
	-@erase "$(INTDIR)\settxa32.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\jkcons32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "C:\project\lib\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\jkcons32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jkcons32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jkcons32.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bschar32.obj" \
	"$(INTDIR)\clrln32.obj" \
	"$(INTDIR)\clrscr32.obj" \
	"$(INTDIR)\cr32.obj" \
	"$(INTDIR)\curbk32.obj" \
	"$(INTDIR)\curon32.obj" \
	"$(INTDIR)\curup32.obj" \
	"$(INTDIR)\delay32.obj" \
	"$(INTDIR)\erscsb32.obj" \
	"$(INTDIR)\erseof32.obj" \
	"$(INTDIR)\erseos32.obj" \
	"$(INTDIR)\getcls32.obj" \
	"$(INTDIR)\getcon32.obj" \
	"$(INTDIR)\getcp32.obj" \
	"$(INTDIR)\getmem32.obj" \
	"$(INTDIR)\getnor32.obj" \
	"$(INTDIR)\inicon32.obj" \
	"$(INTDIR)\inscon32.obj" \
	"$(INTDIR)\locate32.obj" \
	"$(INTDIR)\pushcp32.obj" \
	"$(INTDIR)\putcon32.obj" \
	"$(INTDIR)\setttl32.obj" \
	"$(INTDIR)\settxa32.obj"

"$(OUTDIR)\jkcons32.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jkcons32 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\jkcons32.lib"


CLEAN :
	-@erase "$(INTDIR)\bschar32.obj"
	-@erase "$(INTDIR)\clrln32.obj"
	-@erase "$(INTDIR)\clrscr32.obj"
	-@erase "$(INTDIR)\cr32.obj"
	-@erase "$(INTDIR)\curbk32.obj"
	-@erase "$(INTDIR)\curon32.obj"
	-@erase "$(INTDIR)\curup32.obj"
	-@erase "$(INTDIR)\delay32.obj"
	-@erase "$(INTDIR)\erscsb32.obj"
	-@erase "$(INTDIR)\erseof32.obj"
	-@erase "$(INTDIR)\erseos32.obj"
	-@erase "$(INTDIR)\getcls32.obj"
	-@erase "$(INTDIR)\getcon32.obj"
	-@erase "$(INTDIR)\getcp32.obj"
	-@erase "$(INTDIR)\getmem32.obj"
	-@erase "$(INTDIR)\getnor32.obj"
	-@erase "$(INTDIR)\inicon32.obj"
	-@erase "$(INTDIR)\inscon32.obj"
	-@erase "$(INTDIR)\locate32.obj"
	-@erase "$(INTDIR)\pushcp32.obj"
	-@erase "$(INTDIR)\putcon32.obj"
	-@erase "$(INTDIR)\setttl32.obj"
	-@erase "$(INTDIR)\settxa32.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\jkcons32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\jkcons32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jkcons32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jkcons32.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bschar32.obj" \
	"$(INTDIR)\clrln32.obj" \
	"$(INTDIR)\clrscr32.obj" \
	"$(INTDIR)\cr32.obj" \
	"$(INTDIR)\curbk32.obj" \
	"$(INTDIR)\curon32.obj" \
	"$(INTDIR)\curup32.obj" \
	"$(INTDIR)\delay32.obj" \
	"$(INTDIR)\erscsb32.obj" \
	"$(INTDIR)\erseof32.obj" \
	"$(INTDIR)\erseos32.obj" \
	"$(INTDIR)\getcls32.obj" \
	"$(INTDIR)\getcon32.obj" \
	"$(INTDIR)\getcp32.obj" \
	"$(INTDIR)\getmem32.obj" \
	"$(INTDIR)\getnor32.obj" \
	"$(INTDIR)\inicon32.obj" \
	"$(INTDIR)\inscon32.obj" \
	"$(INTDIR)\locate32.obj" \
	"$(INTDIR)\pushcp32.obj" \
	"$(INTDIR)\putcon32.obj" \
	"$(INTDIR)\setttl32.obj" \
	"$(INTDIR)\settxa32.obj"

"$(OUTDIR)\jkcons32.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("jkcons32.dep")
!INCLUDE "jkcons32.dep"
!ELSE 
!MESSAGE Warning: cannot find "jkcons32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "jkcons32 - Win32 Release" || "$(CFG)" == "jkcons32 - Win32 Debug"
SOURCE=.\bschar32.c

"$(INTDIR)\bschar32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\clrln32.c

"$(INTDIR)\clrln32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\clrscr32.c

"$(INTDIR)\clrscr32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cr32.c

"$(INTDIR)\cr32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\curbk32.c

"$(INTDIR)\curbk32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\curon32.c

"$(INTDIR)\curon32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\curup32.c

"$(INTDIR)\curup32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\delay32.c

"$(INTDIR)\delay32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\erscsb32.c

"$(INTDIR)\erscsb32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\erseof32.c

"$(INTDIR)\erseof32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\erseos32.c

"$(INTDIR)\erseos32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getcls32.c

"$(INTDIR)\getcls32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getcon32.c

"$(INTDIR)\getcon32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getcp32.c

"$(INTDIR)\getcp32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getmem32.c

"$(INTDIR)\getmem32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getnor32.c

"$(INTDIR)\getnor32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\inicon32.c

"$(INTDIR)\inicon32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\inscon32.c

"$(INTDIR)\inscon32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\locate32.c

"$(INTDIR)\locate32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pushcp32.c

"$(INTDIR)\pushcp32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\putcon32.c

"$(INTDIR)\putcon32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\setttl32.c

"$(INTDIR)\setttl32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\settxa32.c

"$(INTDIR)\settxa32.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

