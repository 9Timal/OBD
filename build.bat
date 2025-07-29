@echo off
echo === Compilation en cours ===

:: Définition des variables
set COMPILER=g++
set SRC=src\*.cpp
set INCLUDE=-Iinclude
set LIBPATH=-LC:\dev\libs\boost_1_88_0\stage\lib
set LIBS=-lboost_system-mgw15-mt-x64-1_88 -lws2_32 -lsetupapi
set FLAGS=-std=c++17 -Wall -Wextra -O2
set OUTPUT=build\obd_app.exe

:: Compilation
%COMPILER% main.cpp %SRC% %INCLUDE% %FLAGS% %LIBPATH% %LIBS% -o %OUTPUT%

if %errorlevel% neq 0 (
    echo ❌ Échec de la compilation
) else (
    echo ✅ Compilation réussie !
)
pause
