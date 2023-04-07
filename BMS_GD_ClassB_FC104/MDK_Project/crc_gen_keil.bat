@echo off
ECHO Computing CRC
ECHO -------------------------------------
REM Batch script for generating CRC in KEIL project
REM Must be placed at MDK-ARM folder (project folder)

REM Path configuration
SET SREC_PATH=..\Tools\SREC
SET TARGET_NAME=BMS_GD_ClassB_FC104
SET TARGET_PATH=Output
SET BYTE_SWAP=1
SET COMPARE_HEX=1
SET CRC_ADDR_FROM_MAP=1
REM Not used when CRC_ADDR_FROM_MAP=1
SET CRC_ADDR=0x08007ce0

REM Derived configuration
SET MAP_FILE=%TARGET_PATH%\%TARGET_NAME%.map
SET INPUT_AXF=%TARGET_PATH%\%TARGET_NAME%.axf
SET INPUT_HEX=%TARGET_PATH%\%TARGET_NAME%.hex
SET OUTPUT_HEX=%TARGET_PATH%\%TARGET_NAME%_CRC.hex
SET TMP_FILE=crc_tmp_file.txt


if not exist %SREC_PATH% (
    ECHO %SREC_PATH% is not exit, exit
    ECHO --del %INPUT_HEX% -- %INPUT_AXF% --
    DEL %INPUT_HEX% %INPUT_AXF%
    EXIT
)

IF NOT "%CRC_ADDR_FROM_MAP%"=="1" goto:end_of_map_extraction
REM Extract CRC address from MAP file
REM -----------------------------------------------------------
REM Load line with checksum location to crc_search variable
ECHO Extracting CRC address from MAP file
FINDSTR /R /C:"^  *CHECKSUM" %MAP_FILE%>%TMP_FILE%
SET /p crc_search=<%TMP_FILE%
DEL %TMP_FILE%
REM remove '(' character and string after, which causes errors
for /f "tokens=1 delims=(" %%a in ("%crc_search%") do set crc_search=%%a
REM remove CHECKSUM string from variable
SET crc_search=%crc_search:CHECKSUM=%
REM get first word at line, which should be CRC address in HEX format
for /f "tokens=1 delims= " %%a in ("%crc_search%") do set CRC_ADDR=%%a
REM -----------------------------------------------------------
REM End of CRC address extraction
:end_of_map_extraction

REM Compute CRC and store it to new HEX file
ECHO CRC address: %CRC_ADDR%
if "%BYTE_SWAP%"=="1" (
REM ECHO to see what is going on
ECHO %SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel ^
	-crop 0x08000000 %CRC_ADDR% ^
	-byte_swap 4 ^
	-stm32-b-e %CRC_ADDR% ^
	-byte_swap 4 ^
	-o %TMP_FILE% -intel	
%SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel ^
	-crop 0x08000000 %CRC_ADDR% ^
	-byte_swap 4 ^
	-stm32-b-e %CRC_ADDR% ^
	-byte_swap 4 ^
	-o %TMP_FILE% -intel	
) else (
REM ECHO to see what is going on
ECHO %SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel ^
	-crop 0x08000000 %CRC_ADDR% ^
	-stm32-l-e %CRC_ADDR% ^
	-o %TMP_FILE% -intel
%SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel ^
	-crop 0x08000000 %CRC_ADDR% ^
	-stm32-l-e %CRC_ADDR% ^
	-o %TMP_FILE% -intel
)
ECHO %SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel -exclude -within %TMP_FILE% -intel ^
	%TMP_FILE% -intel ^
	-o %OUTPUT_HEX% -intel
%SREC_PATH%\srec_cat.exe ^
	%INPUT_HEX% -intel -exclude -within %TMP_FILE% -intel ^
	%TMP_FILE% -intel ^
	-o %OUTPUT_HEX% -intel
REM Delete temporary file
DEL %TMP_FILE%
ECHO Modified HEX file with CRC stored at %OUTPUT_HEX%

REM Compare input HEX file with output HEX file
if "%COMPARE_HEX%"=="1" (
ECHO Comparing %INPUT_HEX% with %OUTPUT_HEX%
%SREC_PATH%\srec_cmp.exe ^
	%INPUT_HEX% -intel %OUTPUT_HEX% -intel -v
)


ECHO --del %INPUT_HEX% -- %INPUT_AXF% --
REM DEL %INPUT_HEX%

ECHO -------------------------------------