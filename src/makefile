CC=gcc
EXE_DIR=../out/
W_FLAGS=-Wall

all: shm_e1 shm_e2 shm_e3 shm_e4 shm_srvcli tube_1 tube_2 tube_3 tube_srvcli_n1 tube_srvcli_n2

shm_srvcli: shm_cli shm_srv

tube_srvcli_n1: tube_srv_n1 tube_cli_n1

tube_srvcli_n2: tube_srv_n2 tube_cli_n2

shm_utils.o :
	gcc -c shm_utils.c
	
utils.o :
	gcc -c utils.c
	
shm_e1: shm_e1.c
	$(CC) $< $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
shm_e2: shm_e2.c shm_utils.o utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe

shm_e3: shm_e3.c shm_utils.o utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
shm_e4: shm_e4.c shm_utils.o utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe -lpthread
	
shm_srv: shm_srv.c shm_utils.o utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
shm_cli: shm_cli.c shm_utils.o utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
tube_1: tube_1.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
tube_2: tube_2.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
tube_3: tube_3.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe

tube_srv_n1: tube_srv_n1.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe	
	
tube_cli_n1: tube_cli_n1.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe	

tube_srv_n2: tube_srv_n2.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe	
	
tube_cli_n2: tube_cli_n2.c utils.o
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe	

	
clean :
	rm -f $(EXE_DIR)*.exe *.o
