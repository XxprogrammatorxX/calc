@echo off
color 02
echo ################################calc#####################################
g++ calc.cpp token.cpp variable.cpp grammar.cpp -o calc.exe
echo .
echo .
echo .
echo ################################Done!######################################
pause
echo runing...
echo .  
echo . 
echo . 
calc.exe
pause