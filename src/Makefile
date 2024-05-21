CC=gcc
CFLAGS=-Wall -g -I/usr/include/freetype2 -L/usr/local/lib
LIBS=-lfreetype -lGLESv2 -lEGL -lm $(shell pkg-config --cflags --libs x11)
TARGET=main
SRC=main.c main.h esUtil.c background.c background.h frame.c auxiliar.c auxiliar.h parameters.h compass.c compass.h SSI.c SSI.h airspeed.c airspeed.h altimeter.c altimeter.h VSI.c VSI.h waypoints.c waypoints.h text_generator.c text_generator.h misc.c misc.h

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(TARGET)

.PHONY: clean

clean:
	rm -f $(TARGET)