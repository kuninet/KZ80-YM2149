/*
	KZ80-PSG JoyStick Port Test Program
	KUNI-NET 2019/8/17
 */

#include <stdio.h>

/* PSG I/O PORT DEFINE */
#define PSG_REG   0x40
#define PSG_WRITE 0x41
#define PSG_READ  0x42

/* PSG REGISTER DEFINE */
#define IO_MIX_SET 0x07
#define IO_PORT_A 0x0E
#define IO_PORT_B 0x0F

#define JOY_A "JoyStick-A"
#define JOY_B "JoyStick-B"


/* OUTPUT MESSGAE */
char *msg_up = "UP";
char *msg_down = "DOWN";
char *msg_left = "LEFT";
char *msg_right = "RIGHT";
char *msg_trig_a = "TRIG-A";
char *msg_trig_b = "TRIG-B";
char *msg_tbl[6];


/* JoyStick etc DATA */
typedef struct joy_stick {
	char *joy_Stick_Name;
	unsigned char psg_data;
	unsigned char psg_sel_joy;
} ;

struct joy_stick joy_a = {JOY_A,0xff,0x00};
struct joy_stick joy_b = {JOY_B,0xff,0x40};

/* 
	Register No. set to PSG
 */
void psg_reg_set(unsigned char reg){
	outp(PSG_REG,reg);
}

/* 
	Data READ from PSG
 */
unsigned char psg_read(){
	return (unsigned char)inp(PSG_READ);
}

/* 
	Data WRITE to PSG
 */
void psg_write(unsigned char reg,unsigned char data){
	psg_reg_set(reg);
	outp(PSG_WRITE,data);
}

/* 
	PSG Initialize
 */
void init_psg(){
	psg_write(IO_MIX_SET,0x80);  /* PORT-A input,PORT-B output */
	psg_write(IO_PORT_B,0x00);   /* Select JoyStick-A & common = L */
}

/* 
	JoyStick Status Print
 */
void print_status(char *joy,unsigned char data){
	unsigned char chk = 0x01; /* check bit */
	int i;
	
	for(i = 0; i < 6; i++){
		if(!(data & chk)){
			printf("%s : %s\n",joy,msg_tbl[i]);
		}
		chk = chk << 1;
	}
}

/* 
	JoyStick Status READ
 */
void joy_read(struct joy_stick *j){
	unsigned char data;
	
	psg_write(IO_PORT_B,j->psg_sel_joy);
	psg_reg_set(IO_PORT_A);

	data = psg_read();
	if (j->psg_data != data){		
		print_status(j->joy_Stick_Name,data);
	}
	j->psg_data = data;
}

void msg_tabl_init(){
	msg_tbl[0] = msg_up;
	msg_tbl[1] = msg_down;
	msg_tbl[2] = msg_left;
	msg_tbl[3] = msg_right;
	msg_tbl[4] = msg_trig_a;
	msg_tbl[5] = msg_trig_b;
}

/*
	MAIN ROUTINE
 */
main(int argc,char *argv[]){
	msg_tabl_init();
	init_psg();
	while(1){
		joy_read(&joy_a);
		joy_read(&joy_b);
	}
}
