set "projectdir=%~dp0"
set "preprocess_bl2_file=%projectdir%\image_macros_preprocessed_bl2.c"
set "hardening=%projectdir%\hardening.bat"
set "regression=%projectdir%\regression.bat"
set "update=%projectdir%\SBSFU_UPDATE.bat"
set "postbuild=%projectdir%\..\..\SBSFU_Appli\EWARM\postbuild.bat"
pushd %projectdir%\..\..\..\..\..\..\Middlewares\Third_Party\mcuboot
set mcuboot=%cd%
popd

:start
goto exe:
goto py:
:exe
::line for window executable
echo Postbuild with windows executable
set "imgtool=%mcuboot%\scripts\dist\imgtool\imgtool.exe"
set "python="
if exist %imgtool% (
goto postbuild
)
:py
::line for python
echo Postbuild with python script
set "imgtool=%mcuboot%\scripts\imgtool.py"
set "python=python "
:postbuild
set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b secbootadd0 -m  RE_BL2_BOOT_ADDRESS  -d 0x80  -s 1 %regression% > %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b boot -m  RE_BL2_PERSO_ADDRESS -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b slot0 -m  RE_IMAGE_FLASH_ADDRESS_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot1 -m  RE_IMAGE_FLASH_ADDRESS_NON_SECURE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file%  -b slot2 -m  RE_IMAGE_FLASH_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b slot3 -m  RE_IMAGE_FLASH_NON_SECURE_UPDATE -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_start -m  RE_BL2_WRP_START -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_end -m  RE_BL2_WRP_END -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec1_end -m  RE_BL2_SEC1_END -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec2_start -m  RE_BL2_SEC2_START -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b sec2_end -m  RE_BL2_SEC2_END -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b hdp_end -m  RE_BL2_HDP_END -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_s_size -m  RE_IMAGE_FLASH_SECURE_IMAGE_SIZE -s 1 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_ns_size -m  RE_IMAGE_FLASH_NON_SECURE_IMAGE_SIZE -s 1 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b external_flash_enable -m  RE_EXTERNAL_FLASH_ENABLE -s 1 -p 0 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b ns_code_start -m  RE_CODE_START_NON_SECURE -s 1 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b loader -m  RE_IMAGE_FLASH_LOADER_ADDRESS -s 1 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_bank2_start -m  RE_LOADER_WRP_START -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b wrp_bank2_end -m  RE_LOADER_WRP_END -d 0x800 -s 1 -p 0 %hardening% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b primary_only -m  RE_PRIMARY_ONLY -s 1 -p 0 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_number -m  RE_IMAGE_NUMBER -s 1 -p 0 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b cfg_loader -m  RE_EXT_LOADER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b image_number -m  RE_IMAGE_NUMBER -s 1 -p 0 %update% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error

set "command=%python%%imgtool% flash --layout %preprocess_bl2_file% -b crypto_scheme -m  RE_CRYPTO_SCHEME -s 1 -p 0 %postbuild% >> %projectdir%\output.txt 2>&1"
%command%
IF %ERRORLEVEL% NEQ 0 goto :error
exit 0
:error
echo "%command% : failed" >> %projectdir%\\output.txt
exit 1

