.set start,0x82555E00
.set DbgBreakPoint, 0x829E39CC

mflr 12
stw 12,-8(1)
stwu 1,-0x100(1)

lis 11,(DLL_STRING+start)@ha
addi 3,11,(DLL_STRING+start)@l
li 4,0x9
li 5,0
addi 6,1,0x50

lis 11,(DbgBreakPoint)@ha
addi 11,11,(DbgBreakPoint)@l
mtctr 11
bctrl

addi 1,1,0x100
lwz 12,-8(1)
mtlr 12
blr

DLL_STRING:.string "game:\\Core.xex"
.align 4