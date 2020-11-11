@ECHO off
path = %~dp0..\..\%
pushd %path%
call Vendors\bin\premake\premake5.exe vs2019
popd
PAUSE