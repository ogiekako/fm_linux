# Microsoft Developer Studio Generated NMAKE File, Based on jkmisc.dsp
!IF "$(CFG)" == ""
CFG=jkmisc - Win32 Debug
!MESSAGE 構成が指定されていません。デフォルトの jkmisc - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "jkmisc - Win32 Release" && "$(CFG)" != "jkmisc - Win32 Debug"
!MESSAGE 指定された ビルド モード "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE コマンド ライン上でマクロの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "jkmisc.mak" CFG="jkmisc - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なビルド モード:
!MESSAGE 
!MESSAGE "jkmisc - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "jkmisc - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "jkmisc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\jkmisc.lib"

!ELSE 

ALL : "$(OUTDIR)\jkmisc.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\dosopt.obj"
	-@erase "$(INTDIR)\getcpjk.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\jis2sj.obj"
	-@erase "$(INTDIR)\nthctype.obj"
	-@erase "$(INTDIR)\optvar.obj"
	-@erase "$(INTDIR)\strchr.obj"
	-@erase "$(INTDIR)\strpbrk.obj"
	-@erase "$(INTDIR)\strrchr.obj"
	-@erase "$(INTDIR)\strstr.obj"
	-@erase "$(INTDIR)\strzeq.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\jkmisc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "D:\project\lib\include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\jkmisc.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jkmisc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jkmisc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\dosopt.obj" \
	"$(INTDIR)\getcpjk.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\jis2sj.obj" \
	"$(INTDIR)\nthctype.obj" \
	"$(INTDIR)\optvar.obj" \
	"$(INTDIR)\strchr.obj" \
	"$(INTDIR)\strpbrk.obj" \
	"$(INTDIR)\strrchr.obj" \
	"$(INTDIR)\strstr.obj" \
	"$(INTDIR)\strzeq.obj"

"$(OUTDIR)\jkmisc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jkmisc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\jkmisc.lib"

!ELSE 

ALL : "$(OUTDIR)\jkmisc.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\dosopt.obj"
	-@erase "$(INTDIR)\getcpjk.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\jis2sj.obj"
	-@erase "$(INTDIR)\nthctype.obj"
	-@erase "$(INTDIR)\optvar.obj"
	-@erase "$(INTDIR)\strchr.obj"
	-@erase "$(INTDIR)\strpbrk.obj"
	-@erase "$(INTDIR)\strrchr.obj"
	-@erase "$(INTDIR)\strstr.obj"
	-@erase "$(INTDIR)\strzeq.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\jkmisc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /I "D:\project\lib\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\jkmisc.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jkmisc.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jkmisc.lib" 
LIB32_OBJS= \
	"$(INTDIR)\dosopt.obj" \
	"$(INTDIR)\getcpjk.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\jis2sj.obj" \
	"$(INTDIR)\nthctype.obj" \
	"$(INTDIR)\optvar.obj" \
	"$(INTDIR)\strchr.obj" \
	"$(INTDIR)\strpbrk.obj" \
	"$(INTDIR)\strrchr.obj" \
	"$(INTDIR)\strstr.obj" \
	"$(INTDIR)\strzeq.obj"

"$(OUTDIR)\jkmisc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "jkmisc - Win32 Release" || "$(CFG)" == "jkmisc - Win32 Debug"
SOURCE=.\dosopt.c
DEP_CPP_DOSOP=\
	"..\include\getoptjk.h"\
	"..\include\sjis.h"\
	

"$(INTDIR)\dosopt.obj" : $(SOURCE) $(DEP_CPP_DOSOP) "$(INTDIR)"


SOURCE=.\getcpjk.c

"$(INTDIR)\getcpjk.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\getopt.c

!IF  "$(CFG)" == "jkmisc - Win32 Release"

DEP_CPP_GETOP=\
	"..\include\getoptjk.h"\
	"..\include\sjis.h"\
	

"$(INTDIR)\getopt.obj" : $(SOURCE) $(DEP_CPP_GETOP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jkmisc - Win32 Debug"

DEP_CPP_GETOP=\
	"..\include\getoptjk.h"\
	"..\include\sjis.h"\
	

"$(INTDIR)\getopt.obj" : $(SOURCE) $(DEP_CPP_GETOP) "$(INTDIR)"


!ENDIF 

SOURCE=.\jis2sj.c

"$(INTDIR)\jis2sj.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\nthctype.c
DEP_CPP_NTHCT=\
	"..\include\sjis.h"\
	

"$(INTDIR)\nthctype.obj" : $(SOURCE) $(DEP_CPP_NTHCT) "$(INTDIR)"


SOURCE=.\optvar.c

"$(INTDIR)\optvar.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\strchr.c
DEP_CPP_STRCH=\
	"..\include\sjis.h"\
	"..\include\stringjk.h"\
	

"$(INTDIR)\strchr.obj" : $(SOURCE) $(DEP_CPP_STRCH) "$(INTDIR)"


SOURCE=.\strpbrk.c
DEP_CPP_STRPB=\
	"..\include\sjis.h"\
	"..\include\stringjk.h"\
	

"$(INTDIR)\strpbrk.obj" : $(SOURCE) $(DEP_CPP_STRPB) "$(INTDIR)"


SOURCE=.\strrchr.c
DEP_CPP_STRRC=\
	"..\include\sjis.h"\
	"..\include\stringjk.h"\
	

"$(INTDIR)\strrchr.obj" : $(SOURCE) $(DEP_CPP_STRRC) "$(INTDIR)"


SOURCE=.\strstr.c
DEP_CPP_STRST=\
	"..\include\sjis.h"\
	"..\include\stringjk.h"\
	

"$(INTDIR)\strstr.obj" : $(SOURCE) $(DEP_CPP_STRST) "$(INTDIR)"


SOURCE=.\strzeq.c
DEP_CPP_STRZE=\
	"..\include\strzeq.h"\
	

"$(INTDIR)\strzeq.obj" : $(SOURCE) $(DEP_CPP_STRZE) "$(INTDIR)"



!ENDIF 

