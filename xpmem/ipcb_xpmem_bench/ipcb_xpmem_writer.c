
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem_writer.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <xpmem.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "ipcb_xpmem.h"

const ull memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );
struct timeval start, end;

int test_base(test_args* t) { return 0; }
int test_base_one(test_args* t) { return 0; }
int test_two_attach(test_args* t) { return 0; }
int test_two_shares(test_args* t) { return 0; }
int test_fork(test_args* t) { return 0; }


/*
 *  Lorem Ipsum
 */
int 
main(int argc, char **argv) {

    char *str;
    char **buf;
    pid_t otherChildId;
	test_args* xpmem_args;
    struct timeval start, end;
    int fd, fdSync, fdTime, test_nr;

    if (argc < 2) {
		printf("Usage: %s <test number>\n", argv[0]);
		return -1;
	}
	test_nr = atoi(argv[1]);
	
	ipcb_xpmem_arg_generator(memSize, xpmem_args);
    memset(xpmem_args->share, '\0', TMP_SHARE_SIZE);
    xpmem_args->buf = ipcb_fake_data_generator(XPMEM_ROW_SIZE, XPMEM_COL_SIZE);

    printf("==== %s STARTS ====\n", xpmem_test[0].name);

    return (*xpmem_test[test_nr].function)(xpmem_args);
}


/**
 * test_base - a simple test to share and attach
 * Description:
 *      Creates a share (initialized to a random value), calls a second process
 *	to attach to the shared address and increment its value.
 * Return Values:
 *	Success: 0
 *	Failure: -1
 */
int 
ipcb_test_base_one (test_args *xpmem_args)
{
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;

	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		// xpmem_args->share[LOCK_INDEX] = 1;
		return -1;
	}

	printf("xpmem_proc_writer: mypid = %d\n", getpid());
	printf("xpmem_proc_writer: sharing %ld bytes\n", SHARE_SIZE);
	printf("xpmem_proc_writer: segid = %llx at %p\n\n", segid, data);

	ipcb_get_time(&start, "\ntest_base:start: "); /* Start. */
	/* Copy data to mmap share */
    for (int i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy((data + (i * XPMEM_COL_SIZE) ), xpmem_args->buf[i], 
				XPMEM_COL_SIZE);

	sprintf(xpmem_args->share, "%llx", segid);

	/* Give control back to xpmem_master */
	// xpmem_args->share[LOCK_INDEX] = 1;



	unmake_share(segid, data, SHARE_SIZE);

	return ret;
}



/*
 *  Lorem Ipsum
 */
int
ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args) {

	if ((xpmem_args->fd = open(SHARE_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.share");

	if ((xpmem_args->lock = open(LOCK_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.lock");

	xpmem_args->share = ipcb_map_memory_to_fd(memSize, xpmem_args->fd, 0); 
    return 1;
}


/*
 *  Lorem Ipsum
 */
char*
ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset) {
    char* str;

    str = mmap(NULL, 2ull * memorySize, PROT_READ|PROT_WRITE, 
                MAP_SHARED, fd, offset);
    if (MAP_FAILED == str) 
        ipcb_print_error("ipcb_master:ipcb_map_memory_to_fd: mmap");    
    return str;
}


    // write(fdTime, &start.tv_sec, sizeof(long int));
    // write(fdTime, &start.tv_usec, sizeof(long int));
    
    // printf("\nWriting Data into memory is done.\n");
    // printf("Time in microseconds: %ld microseconds\n",
    //         ((end.tv_sec - start.tv_sec)*1000000L
    //        +end.tv_usec) - start.tv_usec
    //       ); // Added semicolon





	/* Wait for xpmem_proc2 to finish */
	// lockf(xpmem_args->lock, F_LOCK, 0);
	// lockf(xpmem_args->lock, F_ULOCK, 0);

	// printf("xpmem_proc1: verifying data...");
	// expected = (xpmem_args->add == 2 ? 2 : 1); /* Slightly hackish */
	// for (i = 0; i < SHARE_INT_SIZE; i++) {
	// 	if (*(data + i) != i + expected) {
	// 		printf("xpmem_proc1: ***mismatch at %d: expected %d "
	// 			"got %d\n", i, i + expected, *(data + i));
	// 		ret = -1;
	// 	}
	// }