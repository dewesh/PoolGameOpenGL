a.out: readobj.cpp libBall.a libWorld.a libCamera.a libStick.a libTable.a
	g++ -Wall readobj.cpp -L. -lWorld -lBall -lCamera -lStick -lTable -lglut -lGLU

libWorld.a: World.o
	ar -rcs libWorld.a World.o
World.o: World.cpp World.h libCamera.a libStick.a libBall.a libTable.a
	g++ -Wall -c World.cpp -L. -lCamera -lStick -lBall -lTable

libTable.a: Table.o
	ar -rcs libTable.a Table.o
Table.o: Table.cpp Table.h Game.h
	g++ -Wall -c Table.cpp

libCamera.a: Camera.o
	ar -rcs libCamera.a Camera.o
Camera.o: Camera.cpp Camera.h Game.h
	g++ -Wall -c Camera.cpp


libStick.a: Stick.o
	ar -rcs libStick.a Stick.o
Stick.o: Stick.cpp Stick.h Game.h
	g++ -Wall -c Stick.cpp


libBall.a: Ball.o
	ar -rcs libBall.a Ball.o
Ball.o: Ball.cpp Ball.h Game.h
	g++ -Wall -c Ball.cpp

clean:
	rm *.o a.out *.a
