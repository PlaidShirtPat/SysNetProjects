We did not get graceful exits working on the code, but the program will accept infinite inputs and deliver reliably within the RDT 3.0 spec

There is the possiblity of false acks with large delays, but we belive this simply the already known weakness in the RDT3 protocol (We did this on the midterm)

The code is not extreemly well factored, but attempts were made to keep it reasonably clean. We ask that it be taken into account that we have been developing a library (MyNetworking.c/h) and common.c/h over the course of this semester when deducting points for code factorization. It's not perfect, but we are attempting to keep our code clean and readable.

We also resolved some memory leaks through better usage of stack versus heap space, utilizing the auto memory management that comes with the stack. 



