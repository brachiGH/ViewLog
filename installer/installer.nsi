; Define the output installer file name
OutFile "Installer.exe"

; Define the default installation directory
InstallDir "$PROGRAMFILES\ViewLog"

; Request administrative privileges for installation
RequestExecutionLevel admin

; Default section for installation
Section "Install"

  ; Create installation directory
  SetOutPath $INSTDIR

  ; Include the files to be installed
  File /r ".\viewlog\*.*"

  ; Create a shortcut on the Start Menu
  ; Modify the target path if necessary
  CreateDirectory "$SMPROGRAMS\ViewLog"
  CreateShortCut "$SMPROGRAMS\ViewLog\ViewLog.lnk" "$INSTDIR\viewlog.exe"

  ; Create a shortcut on the desktop
  ; Modify the target path if necessary
  CreateShortCut "$DESKTOP\ViewLog.lnk" "$INSTDIR\viewlog.exe"

  ; Write the uninstaller executable
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

; Uninstallation section
Section "Uninstall"

  ; Remove installation directory and its contents
  RMDir /r $INSTDIR

  ; Remove shortcuts
  Delete "$SMPROGRAMS\ViewLog\ViewLog.lnk"
  RMDir "$SMPROGRAMS\ViewLog"
  Delete "$DESKTOP\ViewLog.lnk"

SectionEnd
