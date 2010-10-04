; The name of the installer
Name "d-jon updater"

SilentInstall silent

; The file to write
OutFile "shared\update_djon.exe"

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
;Page directory
;Page instfiles

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

; The stuff to install
Section "djon (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "djon.exe"
  File "djon-util.dll"
  File "updater.exe"
  
  File /oname=$PROFILE\.djon\templates\AnaRequerimientos.tpl AnaRequerimientos.tpl
  File /oname=$PROFILE\.djon\templates\Bugs.tpl Bugs.tpl
  File /oname=$PROFILE\.djon\templates\ciclos.tpl ciclos.tpl
  File /oname=$PROFILE\.djon\templates\Tasks.tpl Tasks.tpl
  File /oname=$PROFILE\.djon\templates\CommonTask.tpl CommonTask.tpl
  
  ; if the conf file exists, remove it and replace with the new one
;  Delete $PROFILE\.djon\djon.conf
;  Push "last-project-dir:$PROFILE\.djon\Projects;$\r$\nidle-timeout:300;$\r$\n"
;  Push "$PROFILE\.djon\djon.conf"
;  Call WriteToFile
  
  ; Write the installation path into the registry
;  WriteRegStr HKLM SOFTWARE\djon "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
;  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "DisplayName" "d-jon"
;  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "UninstallString" '"$INSTDIR\uninstall.exe"'
;  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "NoModify" 1
;  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\djon" "NoRepair" 1
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Run" "djon" "$INSTDIR\djon.exe"

;  WriteUninstaller "uninstall.exe"
  
  Exec '"$INSTDIR\djon.exe"'

SectionEnd

