; |-------------------------------------------------|
; | Program: bootloader                             |
; | Register Description:                           |
; |-------------------------------------------------|
; | Type of Register:                               |
; |     - General Register                          |
; |     - Segment Register                          |
; |     - Indexes and pointers                      |
; |     - EFLAGS register                           |
; |-------------------------------------------------|
; | General Register:                               |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     32 bits:                                    |
; |        EAX                                      |
; |        EBX                                      |
; |        ECX                                      |
; |        EDX                                      |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     16 bits:                                    |
; |        AX                                       |
; |        BX                                       |
; |        CX                                       |
; |        DX                                       |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     8 bits:                                     |
; |        AH                                       |
; |        AL                                       |
; |        BH                                       |
; |        BL                                       |
; |        CH                                       |
; |        CL                                       |
; |        DH                                       |
; |        DL                                       |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; | At the link it is described also the X to H & L |
; | http://www.sce.carleton.ca/courses/94201/f01/   |
; | 94201.lecture9-12-processor.pdf                 |
; |     AX = (AH) << 8 || AL                        |
; |     BX = (BH) << 8 || BL                        |
; |     CX = (CH) << 8 || CL                        |
; |     DX = (DH) << 8 || DL                        |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; | EAX, AX, AH, AL:                                |
; | Called the Accumulator register.                |
; | It is used for I/O port access, arithmetic,     |
; | interrupt calls, etc...                         |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; | EBX, BX, BH, BL:                                |
; | Called the Base register.                       |
; | It is used as a base pointer for memory access  |
; | Gets some interrupt return values               |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; | ECX, CX, CH, CL:                                |
; | Called the Counter register.                    |
; | It is used as a loop counter and for shifts     |
; | Gets some interrupt values                      |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; | EDX, DX, DH, DL:                                |
; | Called the Data register.                       |
; | It is used for I/O port access, arithmetic,     |
; | some interrupt calls.                           |
; |-------------------------------------------------|
; | Segment Register:                               |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     CS:                                         |
; |     Holds the Code segment in which your        |
; |     programm runs. Changing its value might     |
; |     make the computer hang.                     |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     DS:                                         |
; |     Holds the Data segment that your programm   |
; |     accesses. Changing its value might give     |
; |     erronous data.                              |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     ES, FS, GS:                                 |
; |     These are extra segment registers available |
; |     for far pointer addressing like video       |
; |     memory and such.                            |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     SS:                                         |
; |     Hodlds the Stack segment your programm      |
; |     uses. Sometimes has the same value as OS.   |
; |     Changing its value can give unpredictable   |
; |     results. mostly data related.               |
; |-------------------------------------------------|
; | Indexes and pointers:                           |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     ES:EDI EDI DI:                              |
; |     Destination index register                  |
; |     Used for string, memory array copying and   |
; |     setting and for far pointer addressing with |
; |     ES.                                         |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     DS:ESI EDI SI:                              |
; |     Source index register                       |
; |     Used for string and memory array copying.   |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     SS:EBP EBP BP:                              |
; |     Stack Base pointer register                 |
; |     Holds the base address of the stack.        |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     SS:ESP ESP SP:                              |
; |     Stack pointer register                      |
; |     Holds the top address of the stack.         |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |     CS:EIP EIP IP:                              |
; |     Index Pointer                               |
; |     Holds the offset of the next instruction    |
; |     It can only be read.                        |
; |-------------------------------------------------|
; | EFLAGS Register:                                |
; | - - - - - - - - - - - - - - - - - - - - - - - - |
; |   Bit   Label    Desciption                     |
; |   ---------------------------                   |
; |   0      CF      Carry flag                     |
; |   2      PF      Parity flag                    |
; |   4      AF      Auxiliary carry flag           |
; |   6      ZF      Zero flag                      |
; |   7      SF      Sign flag                      |
; |   8      TF      Trap flag                      |
; |   9      IF      Interrupt enable flag          |
; |   10     DF      Direction flag                 |
; |   11     OF      Overflow flag                  |
; |   12-13  IOPL    I/O Priviledge level           |
; |   14     NT      Nested task flag               |
; |   16     RF      Resume flag                    |
; |   17     VM      Virtual 8086 mode flag         |
; |   18     AC      Alignment check flag (486+)    |
; |   19     VIF     Virutal interrupt flag         |
; |   20     VIP     Virtual interrupt pending flag |
; |   21     ID      ID flag                        |
; |-------------------------------------------------|

; Also interesting is the Memory Address Allocation:
;
; 0x00000000 - 0x000003FF - Real Mode Interrupt Vector Table
; 0x00000400 - 0x000004FF - BIOS Data Area
; 0x00000500 - 0x00007BFF - Unused
; 0x00007C00 - 0x00007DFF - Our Bootloader
; 0x00007E00 - 0x0009FFFF - Unused
; ---> Using for Stage2:
;      Start ASM to load other parts: 0x7E50 (again only 512Byte)
;      Other ASM Informations for protected mode etc: 0xA000
; 0x000A0000 - 0x000BFFFF - Video RAM (VRAM) Memory
; 0x000B0000 - 0x000B7777 - Monochrome Video Memory
; 0x000B8000 - 0x000BFFFF - Color Video Memory
; 0x000C0000 - 0x000C7FFF - Video ROM BIOS
; 0x000C8000 - 0x000EFFFF - BIOS Shadow Area
; 0x000F0000 - 0x000FFFFF - System BIOS

; Set the Origin Address to which the BIOS Loads this file
; Default it is 0x7C00
;org 0x7C00

; Set the BIT Mode to 16-BIT
; This is the default in the first booted mode
bits 16

; Define the FAT32 File VBR Mode to this file
jmp short start
nop

; FirstDataSector = ReservedSectors + (TotalFATs * BigSectorsPerFAT) + MaxRootEntries
;                 = 0x20 + (0x02 * 0x580) + 0x0
;                 = 0xB20
; FirstSectorOfCluster = ((RootDirectoryStart - 2) * SectorsPerCluster) + FirstDataSector
;                      = ((0x2 - 2) * 8) + 0xB20
;                      = 0xB20
; RootDirectoryStart = FirstSectorOfCluster * BytesPerSector
;                    = 0xB20 * 0x200
;                    = 164000
; NEU: 163000
; CHS:
; C = LBA / (HP)

; OEM_ID			    db "DANI-OS "
; BytesPerSector		dw 0x0200
; SectorsPerCluster	db 0x0080
; ReservedSectors		dw 0x0020
; TotalFATs		    db 0x02
; MaxRootEntries		dw 0x0000
; NumberOfSectors		dw 0x0000
; MediaDescriptor		db 0xF8
; SectorsPerFAT		dw 0x0000
; SectorsPerTrack		dw 0x003F
; SectorsPerHead		dw 0x0020
; HiddenSectors		dd 0x00000000
; TotalSectors		dd 0x0015F8F7
; BigSectorsPerFAT	dd 0x00000580
; Flags			    dw 0x0000
; FSVersion		    dw 0x0000
; RootDirectoryStart	dd 0x00000002
; FSInfoSector		dw 0x0001
; BackupBootSector	dw 0x0006

; Somehow this Table has changed without any reason.
; So here the new one.

OEM_ID                  db "DANI-OS "
BytesPerSector          dw 0x0200
SectorsPerCluster       db 0x08
ReservedSectors         dw 0x0020
TotalFATs               db 0x02
MaxRootEntries          dw 0x0000
NumberOfSectors         dw 0x0000
MediaDescriptor         db 0xF8
SectorsPerFAT           dw 0x0000
SectorsPerTrack         dw 0x003F
SectorsPerHead          dw 0x0020
HiddenSectors           dd 0x00000000
TotalSectors            dd 0x0015F900
BigSectorsPerFAT        dd 0x0000057C
Flags                   dw 0x0000
FSVersion               dw 0x0000
RootDirectoryStart      dd 0x00000002
FSInfoSector            dw 0x0001
BackupBootSector        dw 0x0006

; FAT32 end should be:
; 000001B0  00 00 00 00  00 00 00 00   00 00 00 00  00 00 | 80 00             ................
; 000001C0  01 00 0C 1F  BF CA 00 00   00 00 A0 FE  15 00 | 00 00             ................
; 000001D0  00 00 00 00  00 00 00 00   00 00 00 00  00 00 | 00 00             ................
; 000001E0  00 00 00 00  00 00 00 00   00 00 00 00  00 00 | 00 00             ................
; 000001F0  00 00 00 00  00 00 00 00   00 00 00 00  00 00 55 AA             ..............U.
; 
; Partition 1:
; 80 00 01 00  0C 1F BF CA  00 00 00 00  A0 FE 15 00
; Boot Flag: 80
; CHS Begin: 00 01 00
; Type Code: 0C
; CHS End: 1F BF CA
; LBA Begin: 00 00 00 00
; Number of Sectors: A0 FE 15 00
; Partition 2:
; 00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
; Partition 3:
; 00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
; Partition 4:
; 00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00


; jumping to next offset
TIMES 12 DB 0

DriveNumber		    TIMES 1 DB 0x80
LBA_ADDR            TIMES 2 DB 0x00

; Reserved Bytes
TIMES 1 DB 0

Signature		    TIMES 1 DB 0x29
VolumeID		    dd 0x08029A05
VolumeLabel	    	TIMES 1 DB "DANIPROG OS"
SystemID		    db "FAT32   "

; Define Variable ENDL as New Line (\r\n)
%define ENDL 0x0D, 0x0A

start:
    ; save BIOS boot drive (DL) immediately
    mov [DriveNumber], dl

    ; setup data segments
    mov ax, 0               ; cant set ds and es register directly, so use ax instead
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00          ; stack grows downwards from where we are loaded in memory

    ; Some bios make wrong origin
    ; So make sure we are at right location
    push es
    push word .after
    retf

; We begin the main process of loading the second stage of the bootloader
.after:
    ; mov si, msg_loading
    ; call print_string
    call read_fat32

check_file_names:
    ; Move the Filename which we search, to si
    ; The Registers si and di will be compared together through cmpsb
    ; We limit the loop of iterations for checking each byte to 11,
    ; cause this filename is only 11 characters equal and not a long filename.
    mov si, file_stage2_bin
    mov cx, 11
    ; Push di to stack, so that the next functions access it through the stack
    push di
    repe cmpsb
    ; Now get the register back from stack, and save it to the register di
    pop di
    ; If the last check was successfully, then jump to the label found stage2
    ; where we will process the main loading of this file.
    je found_stage2
    ; We know from analyzing an fat32 partition, that every 0x20 bytes the filenames
    ; are stored at the root table.
    add di, 020h
    ; we will check if this entry is full of zeroes (means there is no more entry
    ; and this one is also invalid)
    mov si, file_32_zeros
    ; Also here 11 charackters only to check
    mov cx, 11
    push di
    repe cmpsb
    pop di
    ; If this check was successfull, this means, there is no bootloader stage2 file
    ; on this disk. So we could print out not found.
    je stage2_not_found
    ; Else jump to the loop and check this filename.
    jmp check_file_names

; This label is mainly the load this file.
; But also we found this file. So there are 2 labels for the same code.
found_stage2:
load_stage2:
    ; First empty all ax, eax, ebx, ecx registers, cause they will be used to store
    ; the values for the interrupt to read this file.
    xor ax, ax
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    ; We store first the Number of Sectors Per Cluster to the register ecx.
    mov ecx, 0x8                        ; => 0x0008
    ; Then we use from the description of FAT32 the 0x14. Byte to eax (there stood the cluster High Byte)
    mov eax, [di + 0x14]
    ; This will be moved than 8 bits to left, cause this was the High Byte
    shl eax, 8
    ; And we take the Low Byte, which was the 0x1A Byte from the Offset to the eax Low Bytes.
    add eax, [di + 0x1A]
    ; Now we need to decrement the eax Value, cause the counter of the first file starts at 3
    dec eax
    dec eax                             ; => 0x0000 0001
    ; Now multiply the Both Values (Sectors Per Cluster and the Cluster Value) to get the amount
    ; of Sectors we should jump over.
    mul ecx                             ; => 0x0000 2008
    ; Then we add the Offset where the Root Directory starts (0x0B18, found by analyzing)
    add ax, 0x0B18
    ; We store this value at the register address of LBA1, so that the next Function could read it
    ; from this variable address.
    mov [LBA1], ax
    ; And call the Function. This is a call, cause we want to get a return.
    ; A JMP will bring here some Stack Errors and will never return. (Hanging System)
    call read_fat32
    ; Now jump to the Code we loaded from the Function.
    jmp dword [Buff_Off]

; Break the System. This will stop to listen to all interrupts and Stop the CPU from doing anything else now.
; It will do nothing after that.
hlt:
    cli
    hlt

; We now need to read the file to the offset.
; Or better setup the Interrupt Values and Parse them back, to check if successfull or not.
read_fat32:
    push si
    push eax
    push ebx
    push ecx
    push edx
    ; We first check the Disk, if we are on the right disk and can get values.
    ; Interrupt 13h with Parameter ah = 0x41 and bx = 0x55AA and dl = [The Drive Number]
    mov ah, 041h
    mov bx, 055AAh
    mov dl, [DriveNumber]
    ; This will trigger the BIOS Loading Function / Interrupt.
    ; This is already implemented on every BIOS, so we dont need to worry about anything there.
    int 13h
    ; If there happened any error, we will jump to output the message of an floppy error
    jc floppy_error
    ; But if success, we will parse the values to read the disk part / sectors.
    ; Parameter 0x42 = Read
    mov ah, 042h
    ; We parse the Address of the Table where all other parameters stood, like where to save,
    ; how many sectors to read, ...
    lea si, DAP
    mov dl, [DriveNumber]
    int 13h
    jc floppy_error
    ; Then we save the Offset at Register DI and Return to the previous caller Function.
    mov di, [Buff_Off]
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop si
    ret

    mov di, [Buff_Off]
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop si
    ret

; (SIZE) (RESERVED) (SECTORS LSB) (SECTORS MSB)
; (BUFF OFF LSB) (BUFF OFF MSB) (BUFF SEG LSB) (BUFF SEG MSB)
; (LBA1 LSB) (LBA1 MSB) ...
DAP:
Size:       db 0x10
Reserved:   db 0x00
Sectors:    dw 0x04
Buff_Off:   dw 0x8400
Buff_Seg:   dw 0x0000
LBA1:       dd 0x0B18
LBA2:       dd 0x0000
LBA3:       dd 0x0000
LBA4:       dd 0x0000

; Print functions.
; The SI Register stores the Message / String we want to print out.
; And we call the global Function print string
; This will parse the String / Address from si and print character by character to the TTY Console.
stage2_not_found:
   mov si, msg_stage2_not_found
   call print_string
   jmp wait_for_key_and_reboot_system

floppy_error:
    mov si, msg_read_failed
    call print_string

wait_for_key_and_reboot_system:
    mov si, msg_print_any_key_to_reboot
    call print_string
    mov ah, 0               ; AH Set to 0 makes an read keystroke Interrupt by called Int 16h
                            ; Read Key Press
    int 16h                 ; Keystroke on the Keyboard Interrupt
    jmp 0FFFFh:0            ; jump back to the BIOS Call. This is also before this script is loaded

; This needs the String which should be printed stored in
; the Register si
print_string:
    ; save registers before modifying them
    push si
    push ax
    push bx

; Now loop through all Characters of the String which was pushed to Register si
.loop:
    lodsb	    	; loads the next character from Register si to Register al
    or al, al		; now verify that the Character is not the 0 / null Character
    jz .done		    ; but if so, jump to done, else print this Character

.print_char:
    mov ah, 0x0E	; This will set the Mode to Write Character in TTY Mode
    mov bh, 0		; Set the page to actual running Screen 0
        			; We could use Screens from 0-7 in mode 0 and 1, 0-3 in mode 2 and 3
	        		; This is usefull to print to a screen, and when ready, change the actual
		        	; screens to the new screen. (for this use case use int 0x10 and ah 0x05
			        ; while set al to the screen number)
    int 0x10		; Interrupt the Print Function of the Actual Character set in the Register al
    jmp .loop		; Now jump back to the printing Function

; Reset all saved Register which are back uped at the beginning of the print_string function
.done:
    ; It is also important to know that the order of restoring the registers is reversed
    ; The Stack Pointer will be moved backwards while pop a register
    pop bx
    pop ax
    pop si
    ret			    ; Returning to the called Function

; This will print the Register Values: AX, DX, CX, BX, ES:SI
;    push ax
;    call print_hex_word
;    pop ax
;    mov si, msg_new_line
;    call print_string
;    push dx
;    call print_hex_word
;    pop dx
;    mov si, msg_new_line
;    call print_string
;    push cx
;    call print_hex_word
;    pop cx
;    mov si, msg_new_line
;    call print_string
;    push bx
;    call print_hex_word
;    pop bx
;    mov si, msg_new_line
;    call print_string
;    mov si, [es:di]
;    push si
;    call print_hex_word
;    pop si
;    mov si, msg_new_line
;    call print_string
;

; print_array_values:
;     push si
;     push ax
;     push bx
;     push cx
;     push dx
;     xor ax, ax
;     xor bx, bx
;     xor cx, cx
;     xor dx, dx
;     mov cx, 128
;     mov bx, [Buff_Off]
;     ;mov bx, 0x7c00
; .loop_print_arrays:
;     mov ax, [bx]
;     ; push bx
;     ; call print_hex_word
;     ; pop bx
;     ; mov si, msg_double_point
;     ; call print_string
;     mov dx, ax
;     mov ah, dl
;     mov al, dh
;     push ax
;     call print_hex_word
;     pop ax
;     ; mov si, msg_space
;     ; call print_string
;     inc bx
;     inc bx
;     dec cx
;     test cx, cx
;     jnz .loop_print_arrays
; .end_loop_print_arrays:
;     mov si, msg_new_line
;     call print_string
;     pop dx
;     pop cx
;     pop bx
;     pop ax
;     pop si
;     ret
; 
; msg_new_line:       db ENDL, 0
; 
; print_hex_word:
;     pusha
;     mov bp, sp
;     mov cx, 0x0404
;     mov dx, [bp+18]
;     mov bx, [bp+20]
; .loop:
;     rol dx, cl
;     mov ax, 0x0e0f
;     and al, dl
;     add al, 0x90
;     daa
;     adc al, 0x40
;     daa
;     int 0x10
;     dec ch
;     jnz .loop
;     popa
;     ret

; Create a Stored Variable msg_loading with the Value: 'Loading' + New Line + 0
; It is needed a 0 at the End to set the End of the String
; otherwise the Printer wont know where the end is.
; msg_loading:            db 'Loading', ENDL, 0
msg_read_failed:        db 'Failed to read from Partition', ENDL, 0
msg_stage2_not_found:   db 'Second Stage File not found', ENDL, 0
msg_print_any_key_to_reboot: db 'PRESS ANY KEY TO REBOOT...', 0
file_stage2_bin:        db 'STAGE2  BIN'

jmp hlt

; Fill the Rest of this 512 Byte large file with 0
; But the last 2 Bytes need to be 0xAA55, so that the BIOS knows this is a
; bootable File
;
; ($-$$) tells, from start of the File ($$) till this line ($) (is the size in Bytes)
times 446-($-$$) db 0

; This is also needed, so the FAT32 Format knows where the Root Table Starts, ...
; This needs to be Hardcoded and not modified by the bootloader.
P1_BootFlag db 0x80
P1_CHSBegin db 0x00, 0x01, 0x00
P1_TypeCode db 0x0C
P1_CHSEnd   db 0x1F, 0xBF, 0xCA
P1_LBABegin dd 0x00000000
P1_NumberOfSectors dd 0x0015FEA0

; Partition 2 + 3
file_32_zeros:          times 32 db 0

; Partition 4
partition4:             times 16 db 0

dw 0AA55h
