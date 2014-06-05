	.arch armv6
	.fpu softvfp
	.code	16
	.file	"fib.c"
	.text
	.align	2
	.global	main
	.code	16
	.thumb_func
	.type	main, %function
main:
   push{r4, r5, r6, r7, lr}
   @ sub sp minus immediate
   sub sp, sp, #16
   @ sub immediate
   subs r5, r4, #4
   subs r5, #64
   @sub reg
   subs r5, r3, r4
   subs r5, r4

   @ add sp minus immediate
   add sp, sp, #16
   @ add immediate
   adds r5, r4, #4
   adds r5, #64
   @sub reg
   adds r5, r3, r4 
   add  r5, r4  

   @cmp reg
   cmp r5, r4
   @cmp r5, #64

   @mov imm
   mov r5, #64 
   @mov reg
   mov  r5, r4
   movs r5, r4

   @str
   str r5, [r5, #8]
   str r5, [sp, #8]
   str r5, [r6, r7]
  
   @strb
   strb r5, [r6, r7]
   
   @stmia 
   stmia r5!, {r5, r6}
   
   @ldr
   ldr r5, [r5, #8]
   ldr r5, [sp, #8]
   ldr r5, [r5, r6]
   
   @ldrb 
   ldrb r5 [r5, r6]
   
   @ldr literal
   ldr r5, #8

   @ldmia
   ldmia r5!, {r5, r6}
   
   @NEED TO DO BRANCHES
  
   @lsl
   lsls r5, r5 , #4

   @neg
   neg r5, r5
   
   @adc
   adcs r5, r6

   @orr
   orr r5, r5

   @tst
   tst r5, r6



  
    

   pop{r4, r5, r6, r7, pc}

