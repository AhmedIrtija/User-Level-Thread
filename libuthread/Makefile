# Target library
lib := libuthread.a
targets := queue uthread context sem preempt
objs := queue.o uthread.o context.o sem.o preempt.o
CC := gcc
CFLAGS := -Wall -Wextra -MMD -Werror
CFLAGS += -g


ifneq ($(v), 1)
Q = @
endif

all: $(lib)
deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)


$(targets): $(objs)
	@echo "CC $@"
	$(Q) $(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	@echo "CC $@"
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
	
$(lib): $(objs)
	@echo "CC $@"
	$(Q) ar rcs $@ $^
clean:
	@echo "clean"
	$(Q) rm -f $(targets) $(objs) $(deps) $(lib)
