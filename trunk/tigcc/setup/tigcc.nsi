; Script generated by the HM NIS Edit Script Wizard.
; Edited by Kevin Kofler, based in part on the previous setup by Sebastian Reichelt.

; Copyright (C) 2005 Kevin Kofler

; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2, or (at your option)
; any later version.

; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software Foundation,
; Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */


; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "TIGCC"
!define PRODUCT_VERSION "0.96 Beta 1"
!define PRODUCT_PUBLISHER "TIGCC Team"
!define PRODUCT_WEB_SITE "http://tigcc.ticalc.org"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Other definitions
!define UPXDIR "E:\UPX"
!define TIGCCDIR "E:\TI-89\Compilers\TIGCC"
!define TIGCCCVSDIR "E:\TI-89\tigcccvs"

SetCompressor lzma

!packhdr tmpexe.tmp "${UPXDIR}\upx.exe --best -q tmpexe.tmp"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; Path manipulation
!include "pathmani.nsi"

; Section manipulation
!include "Sections.nsh"

; Windows messages
!include "WinMessages.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\classic.bmp"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "${TIGCCCVSDIR}\License.txt"
; Components page
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE CheckComponents
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "TIGCC SDK"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\TIGCC Team\TIGCC"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Program Group"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Path settings page
Page custom PathSettings
; Confirmation dialog
Page custom ConfirmSettings
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\Bin\ide.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\TIGCC"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Var INSTALLED_BEFORE

Var A68K_SELECTED
Var A68K_WAS_SELECTED
Var EXEPACK_SELECTED
Var EXEPACK_WAS_SELECTED

Var SET_PATH

; Detect previous installation
Function .onInit
!insertmacro MUI_INSTALLOPTIONS_EXTRACT "dlgpth9x.ini"
!insertmacro MUI_INSTALLOPTIONS_EXTRACT "dlgpthnt.ini"
!insertmacro MUI_INSTALLOPTIONS_EXTRACT "dlgcnfrm.ini"
  ReadRegStr $0 HKCU "Software\SeReSoft\TI-GCC IDE" "Program Folder"
  StrCmp $0 "" noseresoft
  StrCpy $INSTALLED_BEFORE 1
  StrCpy $INSTDIR $0
  ReadRegStr $0 HKCU "Software\SeReSoft\TI-GCC IDE" "Program Group"
  StrCmp $0 "" noseresoft
  StrCpy $ICONS_GROUP $0
noseresoft:
  ReadRegStr $0 HKLM "Software\TIGCC Team\TIGCC" "Program Folder"
  StrCmp $0 "" notigccteam
  StrCpy $INSTALLED_BEFORE 1
  StrCpy $INSTDIR $0
  ReadRegStr $0 HKLM "Software\TIGCC Team\TIGCC" "Program Group"
  StrCmp $0 "" notigccteam
  StrCpy $ICONS_GROUP $0
notigccteam:
  IntCmp $INSTALLED_BEFORE 0 not_installed_before
  IfFileExists "$INSTDIR\Examples" examples_exist
  !insertmacro UnselectSection 3
examples_exist:
  IfFileExists "$INSTDIR\tigcc.exe" tigcc_exists
  !insertmacro UnselectSection 2
tigcc_exists:
  IfFileExists "$INSTDIR\Bin\ide.exe" ide_exists
  !insertmacro UnselectSection 1
ide_exists:
  IfFileExists "$INSTDIR\Bin\a68k.exe" 0 a68k_doesnt_exist
  StrCpy $A68K_SELECTED 1
  !insertmacro SelectSection 5
a68k_doesnt_exist:
  IfFileExists "$INSTDIR\Bin\pack.exe" 0 exepack_doesnt_exist
  StrCpy $EXEPACK_SELECTED 1
  !insertmacro SelectSection 6
exepack_doesnt_exist:
not_installed_before:
FunctionEnd

; Stupid non-Free licenses
Function .onSelChange
  StrCpy $A68K_WAS_SELECTED $A68K_SELECTED
  SectionGetFlags 5 $0
  IntOp $A68K_SELECTED $0 & ${SF_SELECTED}
  IntCmp $A68K_SELECTED 0 a68k_not_selected
  IntCmp $A68K_WAS_SELECTED ${SF_SELECTED} a68k_not_selected
  MessageBox MB_ICONEXCLAMATION|MB_OKCANCEL "A68k License Agreement:$\n$\nThis program is Freely Distributable, as opposed to Public Domain. Permission is given to freely distribute this program provided no fee is charged, and the documentation file is included with the program.$\n$\n(The original text said $\"this documentation$\" instead of $\"the documentation.$\")$\n$\nPress OK if you accept this license." IDOK a68k_accepted
  !insertmacro UnselectSection 5
  IntOp $A68K_SELECTED $A68K_SELECTED & 0
a68k_accepted:
a68k_not_selected:
  StrCpy $EXEPACK_WAS_SELECTED $EXEPACK_SELECTED
  SectionGetFlags 6 $0
  IntOp $EXEPACK_SELECTED $0 & ${SF_SELECTED}
  IntCmp $EXEPACK_SELECTED 0 exepack_not_selected
  IntCmp $EXEPACK_WAS_SELECTED ${SF_SELECTED} exepack_not_selected
  MessageBox MB_ICONEXCLAMATION|MB_OKCANCEL "ExePack License Agreement:$\n$\n(This license applies to the TIGCC Tools Suite, which the ExePack System originates from.)$\n$\nThe Tools in this release may be distributed by any other website for non-commercial use only.$\n$\nThe author makes no representations or warranties about the suitability of the software, either express or implied. The author shall not be liable for any damages suffered as a result of using or distributing this software.$\n$\nIF YOU USE or RE-USE any PART of this SUITE, PLEASE GIVE CREDITS to US INCLUDING a LINK to our WEBSITE (http://tict.ticalc.org)$\n$\n($\"Any other website$\" probably refers to any website other than http://tict.ticalc.org.)$\n$\nPress OK if you accept this license." IDOK exepack_accepted
  !insertmacro UnselectSection 6
  StrCpy $EXEPACK_SELECTED 0
exepack_accepted:
exepack_not_selected:
FunctionEnd

; Make sure the user selected at least one interface
Function CheckComponents
  SectionGetFlags 2 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} tigcc_or_ide_selected
  SectionGetFlags 1 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 ${SF_SELECTED} tigcc_or_ide_selected
  MessageBox MB_ICONSTOP|MB_OK "You must select at least one interface: either the IDE or the command line compiler or both."
  Abort
tigcc_or_ide_selected:
FunctionEnd

; Don't allow installation into Program Files
Function .onVerifyInstDir
  StrCmp $INSTDIR $PROGRAMFILES 0 valid_path
  Abort
valid_path:
FunctionEnd

Section "Core TIGCC Components" SEC01
  SectionIn 1 RO
; delete obsolete files
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\Doc\tigcclib.chm"
  Delete "$INSTDIR\Bin\cpp.exe"
  Delete "$INSTDIR\Bin\cpp0.exe"
  Delete "$INSTDIR\Bin\link.exe"
  Delete "$INSTDIR\Bin\strip.exe"
  Delete "$INSTDIR\Bin\tovar.exe"
  Delete "$INSTDIR\Bin\ar.exe"
  Delete "$INSTDIR\Bin\objcopy.exe"
  Delete "$INSTDIR\Bin\cygwin1.dll"
  Delete "$INSTDIR\Lib\tipatch.lib"
  Delete "$INSTDIR\Lib\tipatchmain.lib"
  Delete "$INSTDIR\Lib\pstarter-titanium.o"
  Delete "$INSTDIR\Include\C\pstarter.s"
  Delete "$INSTDIR\Include\C\pstarter.o"
  Delete "$INSTDIR\Include\C\tipatch.lib"
  Delete "$INSTDIR\Include\C\tipatchmain.lib"
  Delete "$INSTDIR\Include\C\Sources"
; delete unselected files
  SectionGetFlags 3 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 examples_not_selected
  RmDir /r "$INSTDIR\Examples"
examples_not_selected:
  SectionGetFlags 2 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 tigcc_not_selected
  Delete "$INSTDIR\tigcc.exe"
tigcc_not_selected:
  SectionGetFlags 1 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 ide_not_selected
  Delete "$INSTDIR\Bin\ide.exe"
ide_not_selected:
  SectionGetFlags 5 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 a68k_not_selected
  Delete "$INSTDIR\Bin\a68k.exe"
  RmDir /r "$INSTDIR\Include\ASM"
a68k_not_selected:
  SectionGetFlags 6 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 exepack_not_selected
  Delete "$INSTDIR\Bin\pack.exe"
  Delete "$INSTDIR\Lib\pstarter.o"
exepack_not_selected:
; delete obsolete registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TI-GCC"
  DeleteRegValue HKLM "Software\TIGCC Team\TIGCC" "Program Group Existed"
; install files
  SetOutPath "$INSTDIR\Bin"
  File "${TIGCCDIR}\Bin\as.exe"
  File "${TIGCCDIR}\Bin\cc1.exe"
  File "${TIGCCDIR}\Bin\gcc.exe"
  File "${TIGCCDIR}\Bin\link.dll"
  SetOutPath "$INSTDIR\Lib"
  File "${TIGCCDIR}\Lib\tigcc.a"
  SetOutPath "$INSTDIR\Include\C"
  File "${TIGCCDIR}\Include\C\*.*"
  SetOutPath "$INSTDIR\Include\S"
  File "${TIGCCDIR}\Include\S\*.*"
  SetOutPath "$INSTDIR\Doc"
  File "${TIGCCDIR}\Doc\tigcc.chm"
  CreateDirectory "$INSTDIR\Projects"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\TIGCC Documentation.lnk" "$INSTDIR\Doc\tigcc.chm"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "TIGCC IDE" SEC02
  SetOutPath "$INSTDIR\Bin"
  File "${TIGCCDIR}\Bin\ide.exe"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\TIGCC IDE.lnk" "$INSTDIR\Bin\ide.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
  File "${TIGCCDIR}\Bin\templates.dat"
  SetOutPath "$INSTDIR\Include\C\Completion"
  File "${TIGCCDIR}\Include\C\Completion\*.*"
SectionEnd

Section "Command Line Compiler (TIGCC.EXE)" SEC03
  SetOutPath "$INSTDIR"
  File "${TIGCCDIR}\tigcc.exe"
  File "${TIGCCDIR}\tprbuilder.exe"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\TIGCC Command Line Prompt.lnk" "command.com"
  !insertmacro MUI_STARTMENU_WRITE_END
  IntCmp $SET_PATH 0 no_path
  Push $INSTDIR
  Call AddToPath
no_path:
SectionEnd

Section "TIGCC Examples" SEC04
  SetOutPath "$INSTDIR\Examples"
  File "${TIGCCDIR}\Examples\*.tpr"
  File "${TIGCCDIR}\Examples\*.c"
  File "${TIGCCDIR}\Examples\*.asm"
SectionEnd

SubSection /e "Non-Free Components" NONFREE

Section /o "A68k Assembler" SEC05
  SetOutPath "$INSTDIR\Bin"
  File "${TIGCCDIR}\Bin\a68k.exe"
  SetOutPath "$INSTDIR\Include\ASM"
  File "${TIGCCDIR}\Include\ASM\*.*"
SectionEnd

Section /o "ExePack Compression" SEC06
  SetOutPath "$INSTDIR\Bin"
  File "${TIGCCDIR}\Bin\pack.exe"
  SetOutPath "$INSTDIR\Lib"
  File "${TIGCCDIR}\Lib\pstarter.o"
SectionEnd

SubSectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\Bin\ide.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr HKLM "Software\TIGCC Team\TIGCC" "Program Folder" "$INSTDIR"
  WriteRegStr HKCR ".tpr" "" "TIGCCProject"
  WriteRegStr HKCR "TIGCCProject" "" "TIGCC Project"
  WriteRegStr HKCR "TIGCCProject\DefaultIcon" "" "$INSTDIR\Bin\ide.exe,0"
  WriteRegStr HKCR "TIGCCProject\Shell\open\command" "" "$\"$INSTDIR\Bin\ide.exe$\" $\"%1$\""
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Components required to use TIGCC"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Integrated development environment"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "Command-line interface to TIGCC"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} "Examples can help you to understand TIGCC's functionality better. You can use the TIGCC IDE to open the example projects."
  !insertmacro MUI_DESCRIPTION_TEXT ${NONFREE} "These components are not free in the FSF sense, and therefore cannot be included as essential components. Therefore you can choose whether you want to install them, but be sure to read the license agreement of each component."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC05} "A widely used assembler by Charlie Gibbs"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC06} "A tool by Thomas Nussbaumer (TICT) which compresses calculator programs"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Convert an NSIS string to a form suitable for use by InstallOptions
; Usage:
;   Push <NSIS-string>
;   Call Nsis2Io
;   Pop <IO-string>
Function Nsis2Io
  Exch $0 ; The source
  Push $1 ; The output
  Push $2 ; Temporary char
  StrCpy $1 "" ; Initialise the output
loop:
  StrCpy $2 $0 1 ; Get the next source char
  StrCmp $2 "" done ; Abort when none left
    StrCpy $0 $0 "" 1 ; Remove it from the source
    StrCmp $2 "\" "" +3 ; Back-slash?
      StrCpy $1 "$1\\"
      Goto loop
    StrCmp $2 "$\r" "" +3 ; Carriage return?
      StrCpy $1 "$1\r"
      Goto loop
    StrCmp $2 "$\n" "" +3 ; Line feed?
      StrCpy $1 "$1\n"
      Goto loop
    StrCmp $2 "$\t" "" +3 ; Tab?
      StrCpy $1 "$1\t"
      Goto loop
    StrCpy $1 "$1$2" ; Anything else
    Goto loop
done:
  StrCpy $0 $1
  Pop $2
  Pop $1
  Exch $0
FunctionEnd

; Path settings window
Function PathSettings
  SectionGetFlags 2 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 tigcc_not_selected
  !insertmacro MUI_HEADER_TEXT "PATH Settings" "Choose whether PATH settings for the command line compiler should be added automatically."
  Call IsNT
  Pop $0
  IntCmp $0 0 is9xme
  StrCpy $2 "dlgpthnt.ini"
  StrCpy $1 ";$INSTDIR"
  Goto isnt
is9xme:
  StrCpy $2 "dlgpth9x.ini"
  StrCpy $1 "SET PATH=%PATH%;$INSTDIR"
isnt:
  Push $1
  Call Nsis2Io
  Pop $1
  WriteINIStr "$PLUGINSDIR\$2" "Field 3" "Text" $1
  !insertmacro MUI_INSTALLOPTIONS_INITDIALOG $2
  Pop $0 ;HWND of dialog
  !insertmacro MUI_INSTALLOPTIONS_SHOW
  ReadINIStr $SET_PATH "$PLUGINSDIR\$2" "Field 6" "State"
tigcc_not_selected:
FunctionEnd

; Confirm settings window
Function ConfirmSettings
  !insertmacro MUI_HEADER_TEXT "Confirm Settings" "Please double-check and confirm your settings."
  Push $INSTDIR
  Call Nsis2Io
  Pop $1
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 4" "Text" $1
  Push $ICONS_GROUP
  Call Nsis2Io
  Pop $1
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 6" "Text" $1
  StrCpy $2 ""
  SectionGetFlags 1 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 ide_not_selected
  StrCpy $2 "$2IDE; "
ide_not_selected:
  SectionGetFlags 2 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 tigcc_not_selected
  StrCpy $2 "$2Command Line Compiler; "
tigcc_not_selected:
  SectionGetFlags 3 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 examples_not_selected
  StrCpy $2 "$2Examples; "
examples_not_selected:
  SectionGetFlags 5 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 a68k_not_selected
  StrCpy $2 "$2A68k; "
a68k_not_selected:
  SectionGetFlags 6 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 exepack_not_selected
  StrCpy $2 "$2ExePack Compression; "
exepack_not_selected:
  StrLen $1 $2
  IntOp $1 $1 - 2
  StrCpy $2 $2 $1
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 8" "Text" $2
  SectionGetFlags 2 $0
  IntOp $0 $0 & ${SF_SELECTED}
  IntCmp $0 0 tigcc_not_selected2
  IntCmp $SET_PATH 0 set_path_no
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 10" "Text" "Yes"
  Goto tigcc_selected
set_path_no:
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 10" "Text" "No"
  Goto tigcc_selected
tigcc_not_selected2:
  WriteINIStr "$PLUGINSDIR\dlgcnfrm.ini" "Field 10" "Text" "No, command line compiler not installed"
tigcc_selected:
  !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "dlgcnfrm.ini"
  Pop $0 ;HWND of dialog
  !insertmacro MUI_INSTALLOPTIONS_SHOW
FunctionEnd

; Uninstallation
Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\uninst.exe"

; try to get the real install dir from the registry
  ReadRegStr $0 HKLM "Software\TIGCC Team\TIGCC" "Program Folder"
  StrCmp $0 "" 0 have_inst_dir
  StrCpy $0 $INSTDIR
have_inst_dir:

  Push $0
  Call un.RemoveFromPath

  Delete "$SMPROGRAMS\$ICONS_GROUP\TIGCC Command Line Prompt.pif"
  Delete "$SMPROGRAMS\$ICONS_GROUP\TIGCC Documentation.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\TIGCC IDE.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  Delete "$0\Bin\as.exe"
  Delete "$0\Bin\cc1.exe"
  Delete "$0\Bin\gcc.exe"
  Delete "$0\Bin\link.dll"
  Delete "$0\Bin\ide.exe"
  Delete "$0\Bin\templates.dat"
  Delete "$0\Bin\a68k.exe"
  Delete "$0\Bin\pack.exe"
  RmDir "$0\Bin"
  Delete "$0\Lib\tigcc.a"
  Delete "$0\Lib\pstarter.o"
  RmDir "$0\Lib"
  RmDir /r "$0\Include"
  Delete "$0\Doc\tigcc.chm"
  RmDir "$0\Doc"
  RmDir /r "$0\Examples"
  Delete "$0\tigcc.exe"
  Delete "$0\tprbuilder.exe"

; WARNING: These should NEVER be "/r". The old uninstaller did that and it sucked.
  RmDir "$0\Projects"
  RMDir "$0"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKCR ".tpr"
  DeleteRegKey HKCR "TIGCCProject"

  SetAutoClose true
SectionEnd
