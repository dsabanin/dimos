#include "drivers/screen.h"
#include "kernel/ports.h"
#include "kernel/util.h"

/*  Print a char on the  screen  at col , row , or at  cursor  position  */
void print_char(char character, int col, int row, char attribute_byte) {
	/*  Create a byte (char) pointer  to the  start  of  video  memory  */
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	/* If  attribute  byte is zero , assume  the  default  style. */
	if (! attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}
	/* Get  the  video  memory  offset  for the  screen  location  */
	int offset;
	/* If col  and  row  are non -negative , use  them  for  offset. */
	if (col >= 0 && row >= 0) {
		offset = get_screen_offset(col , row);
	/* Otherwise , use  the  current  cursor  position. */
	} else {
		offset = get_cursor();
	}

	// If we see a newline  character , set  offset  to the  end of
	//  current row , so it will be  advanced  to the  first  col
	// of the  next  row.
	if (character == '\n') {
		int rows = offset / (2 * MAX_COLS);
		offset = get_screen_offset(0, rows+1);
	// Otherwise , write  the  character  and  its  attribute  byte to
	//  video  memory  at our  calculated  offset.
	} else {
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
		offset += 2;
	}
	//  Update  the  offset  to the  next  character  cell , which  is
	// two  bytes  ahead  of the  current  cell.
	// Make  scrolling  adjustment , for  when we  reach  the  bottom
	// of the  screen.
	//  Update  the  cursor  position  on the  screen  device.
	offset = handle_scrolling(offset);
	set_cursor(offset);
}

int handle_scrolling(int offset) {
	if(offset < MAX_ROWS*MAX_COLS*2) {
		return offset;
	}

	for(int i=1; i < MAX_ROWS; i++) {
		memory_copy((char *) get_screen_offset(0, i) + VIDEO_ADDRESS,
					(char *) get_screen_offset(0, i-1) + VIDEO_ADDRESS,
					MAX_COLS*2);
	}

	char *last_line = (char *) get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;

	for(int i=0; i < MAX_COLS*2; i++) {
		last_line[i] = 0;
	}

	offset -= MAX_COLS*2;

	return offset;
}

int get_screen_offset(int col, int row) {
	return 2*(row*MAX_COLS+col);
}

int  get_cursor() {
	// The  device  uses  its  control  register  as an  index
	// to  select  its  internal  registers , of  which  we are
	//  interested  in:
	//    reg  14:  which  is the  high  byte of the  cursor 's offset
	//    reg  15:  which  is the  low  byte of the  cursor 's offset
	// Once  the  internal  register  has  been  selected , we may  read or
	//  write a byte on the  data  register.
	port_write_byte(REG_SCREEN_CTRL , 14);
	int offset = port_read_byte(REG_SCREEN_DATA) << 8;
	port_write_byte(REG_SCREEN_CTRL , 15);
	offset += port_read_byte(REG_SCREEN_DATA );
	//  Since  the  cursor  offset  reported  by the VGA  hardware  is the
	//  number  of  characters , we  multiply  by two to  convert  it to
	// a character  cell  offset.
	return offset*2;
}

void set_cursor(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_write_byte(REG_SCREEN_CTRL, 14);
    port_write_byte(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_write_byte(REG_SCREEN_CTRL, 15);
	port_write_byte(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void kprint_at(char *str, int col, int row, char attribute_byte) {
	char* cur = str;
	if(col < 0 && row < 0) {
		col = get_cursor_col();
		row = get_cursor_row();
	}
	while(*cur != '\0') {
		print_char(*cur, col, row, attribute_byte);
		col = get_cursor_col();
		row = get_cursor_row();
		cur++;
	}
}

void kprint(char *message) {
	kprint_at(message, -1, -1, 0);
}

int get_cursor_row() {
	return get_cursor() / 2 / MAX_COLS;
}

int get_cursor_col() {
	return (get_cursor() / 2) - (get_cursor_row() * MAX_COLS);
}

void clear_screen() {
	for(int row = 0; row <= MAX_ROWS; row++) {
		for(int col = 0; col <= MAX_COLS; col++) {
			kprint_at(" ", col, row, 0);
		}
	}
	set_cursor(0);
}
