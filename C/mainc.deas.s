
mainc.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main> (File Offset: 0x40):
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	c7 45 fc 0f 00 00 00 	movl   $0xf,-0x4(%rbp)
   b:	83 45 fc 0a          	addl   $0xa,-0x4(%rbp)
   f:	8b 45 fc             	mov    -0x4(%rbp),%eax
  12:	5d                   	pop    %rbp
  13:	c3                   	ret

Disassembly of section .comment:

0000000000000000 <.comment> (File Offset: 0x54):
   0:	00 47 43             	add    %al,0x43(%rdi)
   3:	43 3a 20             	rex.XB cmp (%r8),%spl
   6:	28 47 4e             	sub    %al,0x4e(%rdi)
   9:	55                   	push   %rbp
   a:	29 20                	sub    %esp,(%rax)
   c:	31 34 2e             	xor    %esi,(%rsi,%rbp,1)
   f:	31 2e                	xor    %ebp,(%rsi)
  11:	31 20                	xor    %esp,(%rax)
  13:	32 30                	xor    (%rax),%dh
  15:	32 34 30             	xor    (%rax,%rsi,1),%dh
  18:	35                   	.byte 0x35
  19:	32 32                	xor    (%rdx),%dh
	...

Disassembly of section .note.gnu.property:

0000000000000000 <.note.gnu.property> (File Offset: 0x70):
   0:	04 00                	add    $0x0,%al
   2:	00 00                	add    %al,(%rax)
   4:	20 00                	and    %al,(%rax)
   6:	00 00                	add    %al,(%rax)
   8:	05 00 00 00 47       	add    $0x47000000,%eax
   d:	4e 55                	rex.WRX push %rbp
   f:	00 02                	add    %al,(%rdx)
  11:	00 01                	add    %al,(%rcx)
  13:	c0 04 00 00          	rolb   $0x0,(%rax,%rax,1)
	... (skipping 8 zeroes, resuming at file offset: 0x8f)
  1f:	00 01                	add    %al,(%rcx)
  21:	00 01                	add    %al,(%rcx)
  23:	c0 04 00 00          	rolb   $0x0,(%rax,%rax,1)
  27:	00 01                	add    %al,(%rcx)
  29:	00 00                	add    %al,(%rax)
  2b:	00 00                	add    %al,(%rax)
  2d:	00 00                	add    %al,(%rax)
	...

Disassembly of section .eh_frame:

0000000000000000 <.eh_frame> (File Offset: 0xa0):
   0:	14 00                	adc    $0x0,%al
   2:	00 00                	add    %al,(%rax)
   4:	00 00                	add    %al,(%rax)
   6:	00 00                	add    %al,(%rax)
   8:	01 7a 52             	add    %edi,0x52(%rdx)
   b:	00 01                	add    %al,(%rcx)
   d:	78 10                	js     1f <.eh_frame+0x1f> (File Offset: 0xbf)
   f:	01 1b                	add    %ebx,(%rbx)
  11:	0c 07                	or     $0x7,%al
  13:	08 90 01 00 00 1c    	or     %dl,0x1c000001(%rax)
  19:	00 00                	add    %al,(%rax)
  1b:	00 1c 00             	add    %bl,(%rax,%rax,1)
  1e:	00 00                	add    %al,(%rax)
  20:	00 00                	add    %al,(%rax)
  22:	00 00                	add    %al,(%rax)
  24:	14 00                	adc    $0x0,%al
  26:	00 00                	add    %al,(%rax)
  28:	00 41 0e             	add    %al,0xe(%rcx)
  2b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
  31:	4f 0c 07             	rex.WRXB or $0x7,%al
  34:	08 00                	or     %al,(%rax)
	...
