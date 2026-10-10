/* CRT thread helpers. Definitions arrive with checklist item 3. */
#ifndef _PROCESS_H_SHIM_
#define _PROCESS_H_SHIM_
typedef void (*_beginthread_proc)(void*);
unsigned long _beginthread(_beginthread_proc start, unsigned stack_size, void* arg);
void _endthread(void);
#endif
