CC = g++

LDFLAGS		=-L/usr/local/opt/opencv@3/lib
CPPFLAGS	=-I/usr/local/opt/opencv@3/include
LIBS 		=-lopencv_core -lopencv_imgcodecs

BIN			= mat2b64
OBJS		= main.o

$(BIN):	$(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) $(LDFLAGS) $(LIBS) -o $(BIN)

test:;
	./$(BIN) -i test.png
	./$(BIN) -i test.png -o test.txt

clean:;
	rm -f *.o *~ $(BIN) test.txt
