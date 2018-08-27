@setlocal EnableExtensions EnableDelayedExpansion
@set PATH_ORIGINAL=%PATH%
@set MINGWPATH_64_THEN_32=C:\Lab\MinGW\mingw64\bin;C:\Lab\MinGW\mingw32\bin;
@set MINGWPATH_32_THEN_64=C:\Lab\MinGW\mingw32\bin;C:\Lab\MinGW\mingw64\bin;
@set "PATH_SUBST=!PATH_ORIGINAL:%MINGWPATH_64_THEN_32%=%MINGWPATH_32_THEN_64%!"
@set PATH=!PATH_SUBST!
@echo Alter PATH: put 32bit MinGW bin folder first.
@set MAKECMD=make DIRBIT=32 %1 %2 %3 %4 %5 %6 %7 %8 %9
@echo %MAKECMD%
@call %MAKECMD%
@echo Restore PATH.
@set PATH=%PATH_ORIGINAL%
@endlocal
