/*
 * DriverLCD16x2.h
 *
 * Created: 25/11/2017 20:41:15
 *  Author: LEE
 */ 

#define loop_until_bit_is_set(sfr,bit) \
do { } while (bit_is_clear(sfr, bit))
#define bit_is_clear(sfr,bit) \
(!(_SFR_BYTE(sfr) & _BV(bit)))
#define bit_is_set(sfr,bit) \
(_SFR_BYTE(sfr) & _BV(bit))
#define sbi(sfr,bit) sfr|=_BV(bit)
#define cbi(sfr,bit) sfr&=~(_BV(bit))
#define EN 2
#define RW 1
#define RS 0
#define CTRL PORTB
#define DDR_CTRL DDRB
#define DATA_O PORTB
#define DATA_I PINB
#define DDR_DATA DDRB

char Read2Nip(){
	char HNib, LNib;
	DATA_O|=0xF0; //tim hieu them
	sbi(CTRL,EN);
	DDR_DATA &=0x0F; //set 4 bits cao PORT DATA lam input
	HNib=DATA_I & 0xF0;
	cbi(CTRL,EN);
	
	sbi(CTRL,EN);
	LNib=DATA_I & 0xF0;
	cbi(CTRL,EN);
	LNib>>=4;
	return (HNib|LNib);
}

void Write2Nip (uint8_t ch) {
	uint8_t HNib, LNib, temp_data;
	temp_data = DATA_O & 0x0F;
	HNib=ch & 0xF0;
	LNib=(ch<<4) & 0xF0;
	DATA_O = (HNib|temp_data);
	sbi (CTRL,EN);
	cbi	(CTRL,EN);
	
	DATA_O = (LNib|temp_data);
	sbi (CTRL,EN);
	cbi	(CTRL,EN);
}

void Write8Bit(uint8_t ch) {
	DATA_O=ch;
	sbi (CTRL,EN);
	cbi (CTRL,EN);
}

void wait_LCD() {
	//#ifdef LCD8BIT
	//while (1) {
		//cbi(CTRL,EN); //disable
		//cbi(CTRL,RS); //IR command
		//sbi(CTRL,RW); //doc LCD, LCD->chip
		//
		//DDR_DATA=0xFF;
		//DATA_O=0xFF;	//gui lenh read BF, D7=1 cac D# tuy chon
		//sbi(CTRL,EN);
		//DDR_DATA=0x00; // doi huong de nhan du lieu
		//if(bit_is_clear(DATA_I,7)) break;
	//}
	//cbi(CTRL,EN);
	//cbi(CTRL,RW);
	//DDR_DATA=0xFF;
	//#else
	//char temp_val;
	//while(1){
		//cbi(CTRL,RS); //IR
		//sbi(CTRL,RW); //LCD->chip
		//temp_val=Read2Nip();
		//if(bit_is_clear(temp_val,7)) break;
	//}
	//cbi(CTRL,RW); //san sang nhan lenh tiep theo
	//DDR_DATA=0xFF; // ready to out
	//#endif
	_delay_ms(3);
}

void init_LCD() {
	DDR_CTRL=0xFF;
	DDR_DATA=0xFF;
	// function set=========================
	cbi(CTRL,RS); // IR
	cbi(CTRL,RW);	//avr->lcd
	cbi(CTRL,EN);
	#ifdef LCD8BIT
	Write8Bit(0x38);
	wait_LCD();
	#else
	sbi(CTRL,EN);
	sbi(DATA_O,5); // 0010xxxx bao cho lcd biet mode 4 bit
	cbi(CTRL,EN);
	wait_LCD();
	Write2Nip(0x28); //4 bit mode, 2 lines, 5x8 font
	wait_LCD();
	#endif
	// Display control ===============================
	cbi(CTRL,RS); // Instruction
	#ifdef LCD8BIT
	Write8Bit(0x0C);
	wait_LCD();
	#else
	Write2Nip(0x0C);
	wait_LCD();
	#endif
	// entry mode set ======================
	cbi(CTRL,RS);
	#ifdef LCD8BIT
	Write8Bit(0x06);
	wait_LCD();
	#else
	Write2Nip(0x06);
	wait_LCD();
	#endif
}

void MvHome(){
	cbi(CTRL,RS);
	#ifdef LCD8BIT
	Write8Bit(0x02);
	wait_LCD();
	#else
	Write2Nip(0x02);
	wait_LCD();
	#endif
}

void MvCur(uint8_t x, uint8_t y) {
	uint8_t code=-1;
	code=64*(y-1)+(x-1)+0x80;
	cbi(CTRL,RS);
	#ifdef LCD8BIT
	Write8Bit(code);
	wait_LCD();
	#else
	Write2Nip(code);
	wait_LCD();
	#endif
}

void clr_LCD() {
	cbi(CTRL,RS);
	#ifdef LCD8BIT
	Write8Bit(0x01);
	wait_LCD();
	#else
	Write2Nip(0x01);
	wait_LCD();
	#endif
}
void put_char (char ch) {
	switch (ch) {
		case '%' : ch=0x25; break;
		case '!' : ch=0x21; break;
	}
	sbi (CTRL,RS);
	#ifdef LCD8BIT
	Write8Bit((unsigned char)ch);
	wait_LCD();
	#else
	Write2Nip((unsigned char)ch);
	wait_LCD();
	#endif
}
void print_string(char* str, unsigned char len) {
	for (unsigned char i=0; i<len; i++) {
		if(str[i]>0) {
			put_char(str[i]);
		}
		else put_char(' ');
	}
}

void clear_row(char y, char x, uint8_t len) {
	MvCur(y,x);
	for (uint8_t i=0; i<len; i++) put_char(' ');
}