.section ".crt0","ax"
.global _start

_start:
    b startup
    .word __nx_mod0 - _start
    .ascii "HOMEBREW"

.org _start+0x80
startup:
    // save lr
    mov  r7, lr

    // get aslr base
    bl   +4
next:
    sub  r6, lr, #0x88

    // context ptr and main thread handle
    mov  r5, r0
    mov  r4, r1

    // Handle the exception if needed.
    // if (ctx != NULL && main_thread != -1)__libnx_exception_entry(<inargs>);
    // cmp r5, #0
    // ccmn r4, #1, #4, ne // 4 = Z
    // beq bssclr_start
    // b __libnx_exception_entry

bssclr_start:
    mov r12, r7
    mov r10, r5
    mov r11, r4

    // clear .bss
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub  r1, r1, r0  // calculate size
    add  r1, r1, #7  // round up to 8
    bic  r1, r1, #7

    mov r9, #0
bss_loop:
    str  r9, [r0], #8
    subs r1, r1, #8
    bne  bss_loop

    // store stack pointer
    //mov  r1, sp
    //ldr r0, =__stack_top
    //str  r1, [r0]

    // process .dynamic section
    mov  r0, r6
    ldr r1, =_DYNAMIC
    bl   __nx_dynamic

    // initialize system
    //mov  r0, r10
    //mov  r1, r11
    //mov  r2, r12
    //bl   __libnx_init

    // call entrypoint
    //adrp r0, __system_argc // argc
    //ldr  r0, [r0, #:lo12:__system_argc]
    //adrp r1, __system_argv // argv
    //ldr  r1, [r1, #:lo12:__system_argv]

    ldr lr, =svcExitProcess
    b    main

// .global __nx_exit
// .type   __nx_exit, %function
// __nx_exit:
//     // restore stack pointer
//     // adrp x8, __stack_top
//     // ldr  x8, [x8, #:lo12:__stack_top]
//     // mov  sp, x8
//
//     // jump back to loader
//     //b    svcExitProcess

.global __nx_mod0
__nx_mod0:
    .ascii "MOD0"
    .word  _DYNAMIC             - __nx_mod0
    .word  __bss_start__        - __nx_mod0
    .word  __bss_end__          - __nx_mod0
    .word  __eh_frame_hdr_start - __nx_mod0
    .word  __eh_frame_hdr_end   - __nx_mod0
    .word  0 // "offset to runtime-generated module object" (neither needed, used nor supported in homebrew)

    // MOD0 extensions for homebrew
    .ascii "LNY0"
    .word  __got_start__        - __nx_mod0
    .word  __got_end__          - __nx_mod0
