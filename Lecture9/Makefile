
LDFLAGS=-L/opt/local/lib -framework Cocoa -framework OpenGL  -framework GLUT  -lGLEW -lm

CC = g++
CFLAGS=-g -I/opt/local/include 


cube2: cube2.o  InitShader.o 
	$(CC) $@.o InitShader.o $(LDFLAGS) -o $@

cube6: cube6.o  InitShader.o 
	$(CC) $@.o InitShader.o $(LDFLAGS) -o $@


sierpinski2D: sierpinski2D.o  InitShader.o 
	$(CC) $@.o InitShader.o $(LDFLAGS) -o $@

sierpinski3D: sierpinski3D.o  InitShader.o 
	$(CC) $@.o InitShader.o $(LDFLAGS) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -w


clean: 
	-rm -r example0 *.o core *.dSYM
