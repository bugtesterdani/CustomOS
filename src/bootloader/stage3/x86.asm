bits 32

global x86_PM_Video_WriteCharTeletype:
x86_PM_Video_WriteCharTeletype:
    ; make new call frame
    push ebp                ; save old call frame
    mov ebp, esp            ; initialize new call frame

    ; save eXx, si
    push eax
    push ebx
    push ecx
    push edx
    push esi

    ; Reset values
    xor eax, eax
    xor ebx, ebx

    ; [ebp - 0] - Caller Position (ret)
    ; [ebp - 4] - Old Call Frame (32 byte)
    ; [ebp - 12] - EAX (Row Number) ???
    ; [ebp - 16] - EBX (Foreground color)
    ; [ebp - 20] - ECX (Background color)
    ; [ebp - 24] - EDX (Line Number)
    ; [ebp - 28] - ESI (Position of String to print)
    call .calc_values

    ; Print the Character
    push si
    call .print_char
    pop si
    
    ; Restore the Register Values
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    ; Restore old call frame
    mov esp, ebp
    pop ebp
    ret
    
.calc_values:
    ; Set the Colors
    ; Save the Background
    mov ax, [ebp - 20]
    and ax, 0x0F
    mov bl, al
    shl bl, 4
    ; Save the Foreground
    mov ax, [ebp - 16]
    and ax, 0x0F
    add bl, al

    ; Set the Position
    mov edx, 0xB8000
    ; Save the Line
    mov ax, [ebp - 24]
    mov ecx, 0x50
    mul ecx
    mov ecx, 2
    mul ecx
    add edx, eax
    ; Save the Row
    xor eax, eax
    mov ax, [ebp - 12]
    and eax, 0xFF
    mov ecx, 2
    mul ecx
    add eax, edx
    mov edx, eax
    
    ; Set the Character to print
    mov si, [ebp - 28]
    ret

.print_char:
    lodsb
    mov byte [ds:edx], al
    add edx, 1
    mov byte [ds:edx], bl
    ret