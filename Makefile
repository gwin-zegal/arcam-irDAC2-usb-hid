all: arcamusbhid

arcamusbhid: event.cpp
	gcc -Wall event.cpp -lmpdclient -o arcamusbhid

