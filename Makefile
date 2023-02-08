all:
	g++ -g event_display.c `pkg-config raylib --cflags --libs` `root-config --cflags --libs`
