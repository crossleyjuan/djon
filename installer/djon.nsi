; The name of the installer
Name "D-Jon"
Var QtDir  ;Qt Dir

; The file to write
OutFile "setup_djon.exe"

; The default installation directory
InstallDir $PROGRAMFILES\djon

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\djon" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin
;--------------------------------

; Pages

; Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

Function WriteToFile
 Exch $0 ;file to write to
 Exch
 Exch $1 ;text to write
 
  FileOpen $0 $0 a #open file
   FileSeek $0 0 END #go to end
   FileWrite $0 $1 #write to file
  FileClose $0
 
 Pop $1
 Pop $0
FunctionEnd
 
!macro WriteToFile String File
 Push "${String}"
 Push "${File}"
  Call WriteToFile
!macroend
!define WriteToFile "!insertmacro WriteToFile"

;--------------------------------

Function un.checkRunning
    FindProcDLL::FindProc "djon.exe"
    IntCmp $R0 1 0 notRunning
         MessageBox MB_OK|MB_ICONEXCLAMATION "d-jon is running. Please close it first" /SD IDOK
    Abort
notRunning:
FunctionEnd

;--------------------------------

; The stuff to install
Section "djon (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  StrCpy $QtDir "C:\Qt\2010.05\qt\bin"

  ; Put file there
  File "djon.exe"
  ; File "djon.nsi"
  File "djon-data.dll"
  File "djon-util.dll"
  File "djon-plugins.dll"
  ; File "updater.exe"
  
  File C:\Qt\2010.05\qt\bin\libgcc_s_dw2-1.dll
  File C:\Qt\2010.05\qt\bin\mingwm10.dll
  File C:\Qt\2010.05\qt\bin\QtCore4.dll
  File C:\Qt\2010.05\qt\bin\QtGui4.dll
  File C:\Qt\2010.05\qt\bin\QtXml4.dll
  File C:\Qt\2010.05\qt\bin\QtNetwork4.dll
  File C:\Qt\2010.05\qt\bin\QtWebKit4.dll
  File C:\Qt\2010.05\qt\bin\QtXmlPatterns4.dll
  File C:\Qt\2010.05\qt\bin\phonon4.dll
  File C:\Qt\2010.05\qt\bin\QtSql4.dll

  CreateDirectory $PROFILE\.djon
  CreateDirectory $PROFILE\.djon\templates
  CreateDirectory $PROFILE\.djon\Projects
  
  File /oname=$PROFILE\.djon\templates\Req.tpl Req.tpl
  File /oname=$PROFILE\.djon\templates\Bugs.tpl Bugs.tpl
  File /oname=$PROFILE\.djon\templates\cycle.tpl cycle.tpl
  File /oname=$PROFILE\.djon\templates\Tasks.tpl Tasks.tpl
  File /oname=$PROFILE\.djon\templates\CommonTask.tpl CommonTask.tpl
  
  ; if the conf file exists, remove it and replace with the new one
  ; Delete $PROFILE\.djon\djon.conf
  ; Push "last-project-dir:$PROFILE\.djon\Projects;$\r$\nidle-timeout:300;$\r$\nversion-file:http://d-jon.com/downloads/version.conf;$\r$\nupdater-address:http://d-jon.com/downloads/update_djon.exe;$\r$\n"
  ; Push "$PROFILE\.djon\djon.conf"
  ; Call WriteToFile
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\djon "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "DisplayName" "d-jon"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "NoRepair" 1
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Run" "djon" "$INSTDIR\djon.exe"
  WriteUninstaller "uninstall.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\djon"
  CreateShortCut "$SMPROGRAMS\djon\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\djon\djon.lnk" "$INSTDIR\djon.exe" "" "$INSTDIR\djon.exe" 0

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  call un.checkRunning

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon"
  DeleteRegKey HKLM SOFTWARE\djon

  ; Remove files and uninstaller
  Delete $INSTDIR\djon.exe
  Delete $INSTDIR\djon-data.dll
  Delete $INSTDIR\djon-util.dll
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\QtCored4.dll
  Delete $INSTDIR\QtGuid4.dll
  Delete $INSTDIR\QtXmld4.dll
  Delete $INSTDIR\QtSql4.dll

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\djon\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\djon"
  RMDir "$INSTDIR"

SectionEnd
