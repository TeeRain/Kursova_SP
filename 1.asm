.586
.model flat, stdcall
option casemap :none
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\masm32.inc
include \masm32\include\user32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\masm32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\msvcrt.lib

.DATA
;-------------------------------------------------------------------------
;------DataOdUser---------------------------------------------------------
;-------------------------------------------------------------------------
	_aaaa	dd	00h
	_bbbb	dd	00h
	_xxxx	dd	00h
	_yyyy	dd	00h

;-------------------------------------------------------------------------
;------AdditionData-------------------------------------------------------
;-------------------------------------------------------------------------
	hConsoleInput	dd	?
	hConsoleOutput	dd	?
	ErrorMessage	db	"Error: division by zero", 0
	InputBuf	db	15 dup (?)
	InMessage	db	"GET: ", 0
	OutMessage	db	"PUT: "," %d",0
	ResMessage	db	20 dup (?)
	NumberOfCharsRead	dd	?
	NumberOfCharsWrite	dd	?
	msg1310	dw	13,10
	buf	dd	0
	lb1	dd	?
	lb2	dd	?

.CODE
start:
invoke AllocConsole
invoke GetStdHandle, STD_INPUT_HANDLE
mov hConsoleInput, EAX
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hConsoleOutput, EAX
	call Input
	push buf
	pop _aaaa
	call Input
	push buf
	pop _bbbb
	push _aaaa
	push _bbbb
	pop edx
	pop eax
	add eax,edx
	push eax
	pop buf
	call Output
	push _aaaa
	push _bbbb
	pop edx
	pop eax
	sub eax,edx
	push eax
	pop buf
	call Output
	push _aaaa
	push _bbbb
	pop edx
	pop eax
	imul edx
	push eax
	pop buf
	call Output
	push _aaaa
	push _bbbb
	call Check_
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop buf
	call Output
	push _aaaa
	push _bbbb
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	pop buf
	call Output
	push _aaaa
	push _bbbb
	pop edx
	pop eax
	sub eax,edx
	push eax
	mov dword ptr buf, 10
	push buf
	pop edx
	pop eax
	imul edx
	push eax
	push _aaaa
	push _bbbb
	pop edx
	pop eax
	add eax,edx
	push eax
	mov dword ptr buf, 10
	push buf
	call Check_
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop edx
	pop eax
	add eax,edx
	push eax
	pop _xxxx
	push _xxxx
	push _xxxx
	mov dword ptr buf, 10
	push buf
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	pop edx
	pop eax
	add eax,edx
	push eax
	pop _yyyy
	push _xxxx
	pop buf
	call Output
	push _yyyy
	pop buf
	call Output
exit_label:
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0
invoke ExitProcess, 0

;-------------------------------------------------------------------------
;------ProcedureInput-----------------------------------------------------
;-------------------------------------------------------------------------
Input PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0
invoke  crt_atoi, addr InputBuf
mov dword ptr buf, eax
ret
Input ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------ProcedureOutput----------------------------------------------------
;-------------------------------------------------------------------------
Output PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf
invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0
ret
Output ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------ProcedureCheck-----------------------------------------------------
;-------------------------------------------------------------------------
Check_ PROC
	pop edx
	pop eax
	cmp eax,0
	jne end_check
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
	invoke WriteConsoleA, hConsoleOutput, ADDR ErrorMessage, SIZEOF ErrorMessage, ADDR NumberOfCharsWrite, 0
	jmp exit_label
end_check:
	push eax
	push edx
	ret
Check_ ENDP

;-------------------------------------------------------------------------

end start
