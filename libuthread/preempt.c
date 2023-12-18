#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "private.h"
#include "uthread.h"

// Frequency of preemption: 100Hz is 100 times per second
#define HZ 100
static sigset_t signal_set;
static struct itimerval preemption_timer;

void signal_handler(int signal)
{
	(void) signal;
	// Yield to next thread
	uthread_yield();
}

void preempt_disable(void)
{
	// Initialize empty signal set
	sigemptyset(&signal_set);
	// Include Alarm signal in signal set
	sigaddset(&signal_set, SIGVTALRM);
	// Block the set of signals
	sigprocmask(SIG_BLOCK, &signal_set, NULL);
}

void preempt_enable(void)
{
	// Initialize empty signal set
	sigemptyset(&signal_set);
	// Include Alarm signal in signal set
	sigaddset(&signal_set, SIGVTALRM);
	// Unblock the set of signals (not automatically done)
	sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
}

void preempt_start(bool preempt)
{
	if (preempt)
	{
		struct sigaction sa;
		// Function to call when signal is received
		sa.sa_handler = signal_handler;
		// No signals will be blocked while signal handler is running
		sigemptyset(&sa.sa_mask);
		// Set new signal handler for Alarm signal
		sigaction(SIGVTALRM, &sa, NULL);
		// Set Interval: every 0 seconds, 10000 microseconds, which is 100 times per second
		preemption_timer.it_interval.tv_sec = 0;
		preemption_timer.it_interval.tv_usec = HZ * 100;
		preemption_timer.it_value = preemption_timer.it_interval;
		// Start timer
		setitimer(ITIMER_VIRTUAL,&preemption_timer, NULL);
	}
}

void preempt_stop(void)
{
	// Remove Alarm signal and replace with NULL
	sigaction(SIGVTALRM, NULL, NULL);
}