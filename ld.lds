SECTIONS {
	. = 0xffff880000010000;
	.text : {
		*(.text)
	}
	
	.rodata : {
		*(.rodata)
	}

	.eh_frame : {
		*(.eh_frame)
	}

	.bss 0xffff880000020000: {
	 	*(.bss)
	 }
	 
	.data 0xffff880000050000: {
		*(.data)
	}
	.got : {
		*(.got)
	}

	.got.plt : {
		*(.got.plt)
	}
}
