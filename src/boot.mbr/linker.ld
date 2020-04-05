/* The bootloader will start execution at the symbol designated as the entry point. In this case, that's 'start' (defined in start.s) */
ENTRY(_start)
 
/* Tell the linker part of the compiler where the various sections of the kernel will be put in the final kernel executable. */
SECTIONS
{
	/* Begin putting sections at 1 Megabyte (1M), a good place for kernels to be loaded at by the bootloader. */
	/* This is because memory below 1 Megabyte is reserved for other x86-related things, so we can't use it */
	. = 1M;
 
	.boot BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}
 
	/* Executable code */
	.text : ALIGN(4K)
	{
		*(.text)
	}

    .bss BLOCK(32K) : ALIGN(4K)
    {
        *(.bss)
    }

    .eh_frame :
    {
        __eh_frame_start = .;
        KEEP(*(.eh_frame))
        __eh_frame_end = .;
   }

   .eh_frame_hdr :
   {
       KEEP(*(.eh_frame_hdr))
   }

   __eh_frame_hdr_start = SIZEOF(.eh_frame_hdr) > 0 ? ADDR(.eh_frame_hdr) : 0;
   __eh_frame_hdr_end = SIZEOF(.eh_frame_hdr) > 0 ? . : 0;
}
