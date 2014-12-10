Project 2 
=======
By using multi-thread/multi-process mechanism to speed up this program. Also, manage the share memory to protect critical section. 

This program reads four textdata files containing lots of numbers. Read threads will calculate the sum
of integers before "wait" term in each file, and a write entity will print the added sum in "Result.txt".


Review
---------
First of all, use main thread to create four read entity and one write entity. While every read thread encounter "wait" term, it wait for other read thread updating global sum. After updating, sem_post the file sem, and set local sum to zero. At last, signal the write thread, and wait for other read entity.

Write thread will wait until all of the read threads have updated global sum, and then write the global sum to Result.txt.

Reference
---------
https://computing.llnl.gov/tutorials/pthreads

http://www.csc.villanova.edu/~mdamian/threads/posixsem.html

http://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/shm.h.html
