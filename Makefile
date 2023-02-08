all:
	g++ -g event_display.cpp `pkg-config raylib --cflags --libs` `root-config --cflags --libs`
