#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "sem.h"

#include "../../commons/commons.h"



int 
main(int argc, char **argv) {
    // if (argc < 2) {
	// 	printf("In Writer: Usage: %s <test number>\n", argv[0]);
	// 	return -1;
	// }
	// char *name = atoi(argv[1]);
    int lock;
    if ((lock = open("/tmp/xpmem.lock", O_RDWR | O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    	if(id < 0)
    	{
        	perror("semget"); exit(11);
    	}
    	union semun u;
    	u.val = 1;
   	if(semctl(id, 0, SETVAL, u) < 0)
    	{
        	perror("semctl"); exit(12);
    	}
	
	char *s = "abcdefgh";
    int l = strlen(s);
    if(semop(id, &p, 1) < 0){
        perror("semop p"); 
        exit(15);
    }
	printf("   \n\n==== WRITER(SMALL): STARTS ====\n");

    for(int i = 0; i < l; ++i)
    {
        putchar(s[i]);
        fflush(stdout);
        // sleep(rand() % 2);
        putchar(s[i]);
        fflush(stdout);
        // sleep(rand() % 2);
    }
	lockf(lock, F_ULOCK, 0);

    if(semop(id, &v, 1) < 0){
        perror("semop p"); exit(16);
    }
	printf("   \n\n==== WRITER(SMALL): Ends ====\n");
    return 0;
}
