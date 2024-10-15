#sudo openfortivpn gw.bsc.es:443 -u jpedraja
#ssh jpedraj@192.168.10.27
#scp opengl_es_sc.zip jpedraj@192.168.10.27:/home/jpedraj
#git clone https://gitlab.freedesktop.org/freetype/freetype.git
#scp opengl_es_sc.zip jpedraja@84.88.51.153:/home/jpedraja
#ssh jpedraja@84.88.51.153
#export DISPLAY=:0


CC = gcc
CFLAGS = -Wall -g -I./include
ARCH = $(shell uname -m)
TARGET = scs
SRC_DIR = src
ARCH := ./x86_64

ifeq ($(shell uname -m), aarch64)
	ARCH = ./aarch64
	CFLAGS += -DSDL_DISABLE_IMMINTRIN_H
endif

LIBS = -L./$(ARCH)/libs -lEGL -lcjson -lm $(shell pkg-config --cflags --libs x11) -lz

# Check API only if we are not running the "clean" target
ifneq ($(MAKECMDGOALS), clean)
    ifndef API
        $(error API not specified)
    endif
    ifeq ($(API), OpenGL_ES)
        CFLAGS += -DOPENGL_ES_MODE
        LIBS += -lGLESv2
    else ifeq ($(API), OpenGL_SC)
        CFLAGS += -DOPENGL_SC_MODE
        LIBS += -pthread -lpthread -Wl,--no-as-needed -ldl -lSDL2 -DC_OPENCL_HOST $(ARCH)/libGLSCv2.opencl.so
    else
        $(error API must be either OpenGL_ES or OpenGL_SC)
    endif
endif

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRC:.c=.o)

# Check for GEN_OCL flag and run the required command
ifdef GEN_OCL
.PHONY: gen_ocl
gen_ocl:
	rm -f kernel.ocl | $(ARCH)/clcompiler kernel.cl kernel.ocl -cl-kernel-arg-info

# Ensure gen_ocl is run before the main target
$(TARGET): gen_ocl $(OBJS)
else
$(TARGET): $(OBJS)
endif
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
