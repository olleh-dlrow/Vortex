@echo off
:: 设定批处理所在目录为工作目录
pushd %~dp0
call vendor\premake\bin\premake5.exe vs2022
popd
PAUSE
