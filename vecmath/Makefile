CC = g++
CFLAGS = -c -Wall
PROJECT = ../lib/libvecmath.a
SRC = Matrix2f.cpp Matrix3f.cpp Matrix4f.cpp Quat4f.cpp Vector2f.cpp Vector3f.cpp Vector4f.cpp
OBJ = $(SRC:.cpp=.o)

INCFLAGS  = -I ../include/vecmath

all: $(PROJECT)

.cpp.o:
	CC $(CFLAGS) $< $(INCFLAGS)

$(PROJECT): $(OBJ)
	libtool -static $(OBJ) -o $(PROJECT)