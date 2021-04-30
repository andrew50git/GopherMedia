run:
	g++ -fPIC -c `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf gopher_media.cpp
	gcc -shared gopher_media.o -o libgopher_media.so
	g++ -L/home/andrew/Documents/Programs/C++/GopherMedia/ test.cpp -lgopher_media `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
