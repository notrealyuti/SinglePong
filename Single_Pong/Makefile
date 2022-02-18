#windres icon\file.rc -O coff -o icon\file.res
#g++ -std=c++11 main.cpp icon\file.res -o Single_Pong.exe -lmingw32 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_test

build:
	windres icon/file.rc -O coff -o icon/file.res
	g++ -std=c++17 icon/file.res src/main.cpp -o build/Single_Pong.exe -lmingw32 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf 


run:
	Single_Pong.exe

