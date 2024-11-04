<<<<<<< HEAD
OutFile "ViewLogInstaller.exe"
Icon ".\viewlog\ViewLog-logo.ico"


InstallDir "$PROGRAMFILES\ViewLog"
RequestExecutionLevel admin


Section "Install"

  SetOutPath $INSTDIR

  ; Include the files to be installed
  File /r ".\viewlog\*.*"

  CreateDirectory "$SMPROGRAMS\ViewLog"
  CreateShortCut "$SMPROGRAMS\ViewLog\ViewLog.lnk" "$INSTDIR\viewlog.exe" "" "$INSTDIR\ViewLog-logo.ico"


  CreateShortCut "$DESKTOP\ViewLog.lnk" "$INSTDIR\viewlog.exe" "" "$INSTDIR\ViewLog-logo.ico"

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
=======
OutFile "ViewLogInstaller.exe"
Icon ".\viewlog\ViewLog-logo.ico"


InstallDir "$PROGRAMFILES\ViewLog"
RequestExecutionLevel admin


Section "Install"

  SetOutPath $INSTDIR

  ; Include the files to be installed
  File /r ".\viewlog\*.*"

  CreateDirectory "$SMPROGRAMS\ViewLog"
  CreateShortCut "$SMPROGRAMS\ViewLog\ViewLog.lnk" "$INSTDIR\viewlog.exe" "" "$INSTDIR\ViewLog-logo.ico"


  CreateShortCut "$DESKTOP\ViewLog.lnk" "$INSTDIR\viewlog.exe" "" "$INSTDIR\ViewLog-logo.ico"

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
>>>>>>> ViewLog2
