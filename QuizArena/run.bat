@echo off
cd /d "%~dp0"

set "PATH=C:\msys64\ucrt64\bin;%PATH%"

g++ -std=c++17 -o quizarena.exe main.cpp Question.cpp MultipleChoiceQuestion.cpp TrueFalseQuestion.cpp Player.cpp ConsoleUI.cpp QuizGame.cpp SaveManager.cpp Leaderboard.cpp

if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

quizarena.exe
pause