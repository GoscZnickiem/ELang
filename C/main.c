int main(void) {
	return 0/0;
}

void _start() {
	main();
	asm("mov $69, %rbp; mov $7, %rsp; mov $60,%rax; mov $0,%rdi; syscall");
    __builtin_unreachable();
}

