GCC_FLAGS = -Wall -Wextra -Werror -std=c2x

tester: tester.c
	gcc $^ $(GCC_FLAGS) -lcap -o $@
	sudo chown root:root $@
	sudo setcap 'cap_sys_nice,cap_setpcap=ep' $@

clean:
	sudo rm -f -- tester  # sudo needed to remove root owned files
