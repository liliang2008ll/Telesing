
#include <stdio.h>
#include <string.h>
#include "bch.h"
#include<sys/time.h>
/*




 * 测试函数

 * */


/******************************************************************* 
 *                             	
 *	ECE 476 - Final Project - 5/5/04	
 *
 *	(31,16) BCH Triple Eroor Correcting Code.
 *			Version 9.0 - Final
 *
 *	Alexander Krol & Cezar Serban
 *
 */

#include <pthread.h>	//线程
#include <stdio.h>
#include <stdlib.h>
#include <math.h>		// For use of power function


void Bits2Bytes_int(unsigned long num, int *p);
void GF32Init_int(int *p);
int  GF32Evaluate_int(int a, int *p);
int  GF32FindDegree_int(int *p);
int  GF32add2alpha_int(int a, int b) ;
void GF32Multiply_int(int *a, int *b, struct Poly32_int *mul);
void GF32Add_int(int *a, int *b, struct Poly32_int *powers);
void multiplyX_int(int x_power, int *p, struct Poly32_int *ret);
void multiplyConstant_int(int c, int *p, struct Poly32_int *ret);
void CorrectErrors_int(int *p);
unsigned long Bytes2Bits_int(int *p);
 int Mistake(int bch) ;
 
// Note: Must be compiled with "char is unsigned"//

// Variable Definitions//

//UserStateMachine states


//enum {start, original, EncodeDecode, adderror, DisplayToggle, DetailToggle, help};

#define start 1
#define original  2
#define EncodeDecode 3
#define adderror  4
#define DisplayToggle  5
#define DetailToggle   6
#define help  7



// Various vars for user input and command control with HyperTerm
int char_ready;
int char_count;
int cmd_ready;
char cmd_str[8];

//User State Machine state
//unsigned char UserState;
char UserState;

// Message to be encoded/decoded (stored in flash due to its large size)
// Note - could easily be a 'stored' file that is to be transmitted
// Note the '\r' terminators at the end of the message
char Message[] = "This is the Message to be Encoded and Decoded with a (31,16) BCH Triple-Error Correcting Code.\r\r\r";

//Global Vars
unsigned long EncodedMessage=0;	// Encoded Message as a long
char EncMsgArray[32];			// Encoded Message in Array format
int EncMsgArray_int[32];		
char n=31, k=16, t=3;			// BCH Code definitions
unsigned long GeneratorPoly = 36783;	// Generator Polynomial: 107657 in octal
char seed=0;				// Random seed for Error module
//Counters to keep track of errors for BER calculations
int trans_error_count=0,rec_error_count=0;
int enc_message_bits;


// Flags for Display and Error Module
//bit detail_toggle=0, display_toggle=0, Add_Error=0;
int detail_toggle=0, display_toggle=0, Add_Error=0;

// Struct for use in Berlekamp Decoding, allows for arrays of Poly32's
// Note that all GF32 arrays contain the POWERS of alpha as their data,
// not the actual GF32 element - hence multiplication is actually addition
struct Poly32
{
	char p[32];
	//int p[32];
};
struct Poly32_int
{
	int p[32];
};

// Define GF32 - ZERO as negative one since a zero would
// zero out various multiplications
#define ZERO -1

// Efficient lookup tables for GF32
// Alpha exponents - computed as powers of 2
// Where alpha is from the Galois Field(32)
char lookup[] = {
                1,  // a^0  = 0
                2,  // a^1  = 1
                4,  // a^2  = 2
                8,  // a^3  = 3
                16, // a^4  = 4
                5,  // a^5  = 2 0
                10, // a^6  = 3 1
                20, // a^7  = 4 2
                13, // a^8  = 3 2 0
                26, // a^9  = 4 3 1
                17, // a^10 = 4 0
                7,  // a^11 = 2 1 0
                14, // a^12 = 3 2 1
                28, // a^13 = 4  3 2
                29, // a^14 = 4 3 2 0
                31, // a^15 = 4 3 2 1 0
                27, // a^16 = 4 3 1 0
                19, // a^17 = 4  1 0
                3,  // a^18 = 1 0
                6,  // a^19 = 2 1
                12, // a^20 = 3 2
                24, // a^21 = 4 3
                21, // a^22 = 4 2 0
                15, // a^23 = 3 2 1 0
                30, // a^24 = 4 3 2 1
                25, // a^25 = 4 3 0
                23, // a^26 = 4 2 1 0
                11, // a^27 = 3 1 0
                22, // a^28 = 4 2 1
                9,  // a^29 = 3 0
                18, // a^30 = 4 1
                1   // a^31 = 0
            };

char reverseLookup[] = {
                ZERO, // a^0
                0,  // a^1
                1,  // a^2
                18, // a^3
                2,  // a^4
                5,  // a^5
                19, // a^6
                11, // a^7
                3,  // a^8
                29, // a^9
                6,  // a^10
                27, // a^11
                20, // a^12
                8,  // a^13
                12, // a^14
                23, // a^15
                4,  // a^16
                10, // a^17
                30, // a^18
                17, // a^19
                7,  // a^20
                22, // a^21
                28, // a^22
                26, // a^23
                21, // a^24
                25, // a^25
                9,  // a^26
                16, // a^27
                13, // a^28
                14, // a^29
                24, // a^30
                15  // a^31
            };

int reverseLookup_int[] = {
                ZERO, // a^0
                0,  // a^1
                1,  // a^2
                18, // a^3
                2,  // a^4
                5,  // a^5
                19, // a^6
                11, // a^7
                3,  // a^8
                29, // a^9
                6,  // a^10
                27, // a^11
                20, // a^12
                8,  // a^13
                12, // a^14
                23, // a^15
                4,  // a^16
                10, // a^17
                30, // a^18
                17, // a^19
                7,  // a^20
                22, // a^21
                28, // a^22
                26, // a^23
                21, // a^24
                25, // a^25
                9,  // a^26
                16, // a^27
                13, // a^28
                14, // a^29
                24, // a^30
                15  // a^31
            };
/** Function Prototypes **/
// Note: For GF2 unisgned longs can be used since only two elements
// in the field.  For GF32, byte arrays needed to be used since there
// are 32 elements.

// State machine definition for the user menu options
void UserStateMachine();

// Initialization function
void Initialize();

// Displays a helpmenu for command definitions
void HelpMenu();

// Initializes the UART interrupt and flags for reading commands
void get_cmd_init();

// Main Encoding/Decoding System
void System();

// All encoding for BCH ECC is done here - accepts 2 char message
void EncoderBCH(unsigned char a, unsigned char b);

// All encoding for BCH ECC is done here
void DecoderBCH();

/** Helper Functions **/

// Concatenates 2 chars into a long
unsigned long Concate(unsigned char num1, unsigned char num2);

// Finds the degree of a polynomial encoded as a long in GaloisField2
unsigned char GF2FindDegree(unsigned long a);

// Adds 2 polynomials encoded as a long - GF2
unsigned long GF2Add(unsigned long a, unsigned long b);

// Polynomial Multiplication for longs in GF2
unsigned long GF2Multiply(unsigned long a, unsigned long b);

// Polynomial Division for longs in GF2
void GF2Divide(unsigned long a, unsigned long b, unsigned long *qr);

// Polynomial Modulo for longs in GF2
unsigned long GF2Mod(unsigned long a, unsigned long b);

// Retrieves a specified bit from a long
unsigned char getBit(unsigned long r, char i);

// Converts a long into a 32 length byte array
void Bits2Bytes(unsigned long num, char *p);

// Initializes GF32 arrays to ZERO
void GF32Init(char *p);

// Prints an Array byte by byte
void PrintArray(char *p);

// Prints an array in GF32 polynomial format
void GF32PrintArray(char *p);

// Adds two alpha coefficients in GF32
char GF32add2alpha(char a, char b);

// Finds the degree of a GF32 polynomial
char GF32FindDegree(char *p);

// Evaluates the result of a GF32 Polynomial for some x
char GF32Evaluate(char a, char *p);

// Adds all alpha coefficients pairwise in 2 Arrays in GF32
void GF32Add(char *a, char *b, struct Poly32 *powers);

// Polynomial Multiplication for longs in GF32
void GF32Multiply(char *a, char *b, struct Poly32 *mul);

// Multiplies a GF32 polynomial with some power of x
void multiplyX(char x_power, char *p, struct Poly32 *ret);

// Multiplies a GF32 polynomial by a constant
void multiplyConstant(char c, char *p, struct Poly32 *ret);

// Corrects the detected errors in an encoded message
void CorrectErrors(char *p);

// Converts an array of bytes into a long
unsigned long Bytes2Bits(char *p);

// Parses the lower 16bits of a long into 2 chars
void deConcate(unsigned long a, unsigned char *ret);

// Prints a long as 4 chars
void PrintLong(unsigned long a);

// A random error module that corrupts an encoded message
void ErrorModule(char *p, char numerrors);



// The user interface: allows the user to select what it
// is he wants to do.
void UserStateMachine()
{
	char i, input;

	switch (UserState)
  	{   
		case start: 		// Inital State
			if (cmd_ready == 1)	// Check if a command has been entered
			{
				input = cmd_str[0];	// if so, grab it
				
				if ((input == 'O') || (input == 'o')) // Print out the original message
					UserState = original;
				else 
					get_cmd_init();			// Else, re-init for more user input
			    
				if ((input == 'N') || (input == 'n')) // Toggle Noise Module On/Off
				{
					UserState = adderror;
				 	get_cmd_init();			// Check for user-inputted random seed
				 	if (!Add_Error)			// only if module is being turned on
						printf("Enter Random Seed for Noise Module (1-9): ");
				}
				else 
					get_cmd_init();

				if ((input == 'R') || (input == 'r'))	// Start Encoding/Decoding
					UserState = EncodeDecode;
				else
					get_cmd_init();

				if ((input == 'G') || (input == 'g'))	// Main Display Toggle
					UserState = DisplayToggle;		// Displays only partial info
				else
					get_cmd_init();

				if ((input == 'D') || (input == 'd'))	// Detailed Display Toggle
					UserState = DetailToggle;		// Can only be activated if Main
				else							// Display is on
					get_cmd_init();

				if ((input == '?'))				// User Help Menu
					UserState = help;
				else
					get_cmd_init();					
			}
		break;

		case original:	// Print original message
			i=0;
			printf("\r\n--Original Message: ");
			while (Message[i] != '\r')	// Simply run trhough message until terminator
			{					// is reached
				printf("%c",Message[i]);
				i++;			
			}
			printf("\r\n");
			get_cmd_init();			// Re-init command input
			UserState = start;		// and go back to initial state
		break;
			
		case EncodeDecode:			// Run Encoder/Decoder System
			printf("\r\n--Encoding/Decoding of Message: ");
			System();				// Run the system
	  		get_cmd_init();
	  		UserState = help;  		// Display Help Menu before
		break;					// going back to initial state
			
		case adderror:		// Error Module Toggle
			if (Add_Error)	// If it's on, turn it off
			{
				 Add_Error=0;
				 printf("--Noise Module is OFF\r\n");
				 get_cmd_init();
				 UserState = start;
			}
			
			if (cmd_ready == 1)	// Otherwise, turn it on and grab
			{				// the user-inputted seed
				seed = cmd_str[0]-48;
				Add_Error = 1;
				printf("--Noise Module is ON: Seed %d\r\n", seed);
				get_cmd_init(); 				
				UserState = start;
			}
		break;			

		// Note that display toggles are used, since so much is displayed
		// to HyperTerm, that the speed of encoding/decoding is comprimised
		// and it is difficult to interpret the whole message

		case DisplayToggle:		// Main Display toggle
			display_toggle ^= 1;	// Switch it on and off with XOR
			if (display_toggle)
				printf("--Display is ON\r\n");
			else
				printf("--Display is OFF\r\n");
			get_cmd_init(); 				
			UserState = start;
		break;
			
		case DetailToggle:		// Detailed Display Toggle
			if (display_toggle)	// Can only be activated if
			{				// Main Display is ON
				detail_toggle ^= 1;	// Switch it On/Off
				if (detail_toggle)
					printf("--Detailed Display is ON\r\n");
				else
					printf("--Detailed Display is OFF\r\n");
			}
			else
				printf("--Display is NOT on\r\n");
			get_cmd_init(); 				
			UserState = start;
		break;
			
		case help:	// Simply displays the help menu
		    HelpMenu();
		    get_cmd_init();   
		    UserState = start;
		break;
  	}
}

//**Initialize**
//Sets up vars, timers, and Mega32 registers
void Initialize()
{
	//Initialize the USART registers
	//UCSRB = 0x18; // enable RX/TX interrupts
	//UBRRL = 103;  // baud rate = 9600

	//Initialize the state variables
	UserState = start;

	n=31; k=16; t=3;	// Initialize BCH Code parameters to (31,16)
	EncodedMessage=0;
	GeneratorPoly = 36783; // Init the Generator polynomial
	
	// Initialize the toggles and the random seed to No Errors
	display_toggle=0;
	detail_toggle=0;
	Add_Error=0;
	seed=0;

	// Initialize the Error Counters for BER
	trans_error_count=0;
	rec_error_count=0;		
	enc_message_bits=0;		// The # of bits in the Full encoded msg
	
	get_cmd_init(); //start the RXC interrupt for keypad input.
	HelpMenu(); // Run the help menu for User cmds
	
	// Fire up the ISR's
	/*
	#asm
		sei
	#endasm
	*/
}



int bch_test_func()
{
	unsigned long EncodedMessage_v; 
	unsigned char a, b;
	a=11,b=22;
	/*
	EncodedMessage = Concate(a, b);	// Concatenates the two chars to a 16bit message
	printf("[EncodedMessage] %d\n",(int)EncodedMessage);		
	EncodedMessage = EncodedMessage << (n-k);	// Multiply by x^(n-k)
	printf("[EncodedMessage] %d\n",(int)EncodedMessage);		
	EncodedMessage = GF2Add(GF2Mod(EncodedMessage, GeneratorPoly), EncodedMessage);
	printf("[EncodedMessage] %d\n",(int)EncodedMessage);		
	*/
	char out_str[100];
	//编码
	bch_encode(a,b,&EncodedMessage_v);
	sprintf(out_str,"[bch 编码] %d\n",(int)EncodedMessage_v);
	printf("%s\n",out_str);
//	LOGW(out_str);
	//设置错误位
	int var2=Mistake((int)EncodedMessage_v);
	printf("var1 %d var2:%d\n",(int)EncodedMessage_v,var2);

	unsigned char out[2];
	//解码
	bch_decoder(EncodedMessage_v,out);
	sprintf(out_str,"[bch 解码] %d %d\n",out[0],out[1]);
	printf("%s\n",out_str);
//	LOGW(out_str);
	
	return 0;	
}

int bch_encode(unsigned char a, unsigned char b,unsigned long *EncodedMessage_v)
{
	unsigned long EncodedMessage_r=0;
	//编码
	EncodedMessage_r = Concate(a, b);	// Concatenates the two chars to a 16bit message
	//printf("[EncodedMessage] %d\n",(int)EncodedMessage_r);		
	EncodedMessage_r = EncodedMessage_r << (n-k);	// Multiply by x^(n-k)
	//printf("[EncodedMessage] %d\n",(int)EncodedMessage_r);		
	EncodedMessage_r = GF2Add(GF2Mod(EncodedMessage_r, GeneratorPoly), EncodedMessage_r);
	//printf("[bch_encode] %d\n",(int)EncodedMessage_r);		
	memcpy(EncodedMessage_v,&EncodedMessage_r,sizeof(EncodedMessage_r));
	return 0;
}


//**Encode/Decode System**
// High level system for the encoding and decoding operations for
// the entire message to be transmitted
void System()
{ 	
	bch_test_func();
	return ;
	
	
	unsigned char out[2], a, b;
	unsigned int CurrIndex = 0;	// Keeps track of where we are in the Message
	unsigned long decoded_bits;	// Final decoded message of 2 chars

	a = Message[CurrIndex];	// Initialize the first 2 chars to be encoded
	b = Message[CurrIndex+1];
	
	while ((a != '\r') && (b != '\r'))	// Loop throught the message until terminator chars are reached
	{
		if (display_toggle)	// In general, only disply if display toggles are ON
		{
			printf("\r\n========================================================================");
			printf("\r\nOriginal Message: %c %c\r\n", a, b);
		}
		
		a=11;
		b=22;
		printf("here----%d--%d\n",a,b);
		EncoderBCH(a,b);	// Run the encoder on the first 2 chars of Message
		DecoderBCH();	// and Decoded the resulting encoded message
		decoded_bits = Bytes2Bits(EncMsgArray);
		deConcate(decoded_bits, out);	// Parse out the 2 chars


		if (display_toggle)
		{
			printf("\nCorrected Message Array:\r\n");
			PrintArray(EncMsgArray);
			printf("\r\nMessage: %c %c\r\n\n", out[0], out[1]);
		}
		else
			printf("%c %c ", out[0], out[1]);	// if no display toggle, just print decoded
		
		// message
	 	CurrIndex += 2;		// Increment to grab the next 2 chars to be Encoded/Decoded
		a = Message[CurrIndex];
		b = Message[CurrIndex+1];
		enc_message_bits += 31;		// Every iteration, 31 bits are encoded
	}
	printf("\r\n\nBER of Transmitted Message: %f", (float)trans_error_count/enc_message_bits);
	printf("\r\nBER of Recieved Message: %f\r\n", (float)rec_error_count/enc_message_bits);
}

//**16 bit Message Encoder**
// Grabs a 16 bit message, and encodes it according to the BCH algorithm
// with a specified generator polynomial
void EncoderBCH(unsigned char a, unsigned char b)
{
	char numerrors;	// Random number of errors to introduce into the message
				// from 0 - user selected seed
	// Calculates the number of errors
	numerrors = (char)(rand() * ((float)(seed+1)) / (RAND_MAX + 1.0));

	trans_error_count += numerrors;	// Internal counter to keep track of errors
	if (numerrors > 3)			// Errors in decoded message only for 4 or more
		rec_error_count += numerrors;

	// Systematic encoding as follows:
	// (m(x) * x^(n-k)) mod g(x) + (m(x) * x^(n-k))
	
	// Shift by x^(n-k) = x*15
	EncodedMessage = Concate(a, b);	// Concatenates the two chars to a 16bit message
	EncodedMessage = EncodedMessage << (n-k);	// Multiply by x^(n-k)

	EncodedMessage = GF2Add(GF2Mod(EncodedMessage, GeneratorPoly), EncodedMessage);
//	EncodedMessage ^= 0x20010008;	// Errors can be manually added to message for debugging
	
	GF32Init(EncMsgArray); // Initialize to ZERO's
	Bits2Bytes(EncodedMessage, EncMsgArray);	// Convert to a GF32 polynomial

	if (display_toggle)
	{
		printf("\nEncoded Message Array:\r\n");
		PrintArray(EncMsgArray);
	}

//	printf(" NumErrors: %d\r\n",numerrors);

	// Execute the Error Module only if it is activated	
	if (Add_Error)
	{
		if (display_toggle)
			printf("\nRandom Index Errors:  ");
		ErrorModule(EncMsgArray, numerrors);	// Execute it
		if (display_toggle)
		{
			printf("\r\n\nEncoded Message Array with %d Errors:\r\n", numerrors);
			PrintArray(EncMsgArray);
		}
	}
}

//将编码的数据设置一个错误作为解码参数用来纠正
 int Mistake(int bch) 
 {
        srand((unsigned) time(NULL));
       // int value = rand() % (MAX + 1 - MIN) + MIN; 
        int mAmount = rand() % (2 + 1 - 1) + 1;
        for (int i = 0; i < mAmount; i++) 
        {
        		int idx =rand() % (31 + 1 - 1) + 1;
        		bch = (int)(bch ^ (int) pow((float )2,(float )idx)); 
        		//printf("[Mistake] mAmount:%d idx:%d pow:%d bch:%d\n",mAmount,idx,(int) pow((float )2,(float )idx),bch);
        }
        return bch;
    }

void bch_decoder(unsigned long EncodedMessage_v,unsigned char *out)
{
	int EncMsgArray_int_v[32];
	Bits2Bytes_int(EncodedMessage_v,EncMsgArray_int_v);
	for(int i=0;i<32;i++)
	{
		//printf(" %d",EncMsgArray_int_v[i]);
	}
	
	char i, j;
	char k=0;
	int delta[7];
	int Syndromes[32];
 	//Lambda is an array of 7 GF32 polynomials structs
	//Note: Lambda/T arrays can be small (i.e length 4) if rewrite multiply/divide, etc.
	struct Poly32_int lambda[7];
	struct Poly32_int T[7];
	struct Poly32_int temp;

	GF32Init_int(Syndromes);
	//GF32Init(Syndromes);
	// Create Syndrome Polynomial
	for (i=0; i<2*t; i++)
	{
		Syndromes[i+1] = (int)GF32Evaluate_int(i+1, EncMsgArray_int_v);
		//Syndromes[i+1] = (char)GF32Evaluate(i+1, EncMsgArray);
		//printf("[DecoderBCH] i:%d Syndromes:%d \n",i,Syndromes[i+1]);
	}
	
	
  	/* 1  - Initialization */
   	// add 1 to S(x) and initialize Berlekamp's Algorithm
	Syndromes[0] = 0;

	//Init Lambda[i] polynomials
	for (i=0; i<7; i++)
		for (j=0; j<32; j++)
		{
			lambda[i].p[j]=ZERO;
			T[i].p[j]=ZERO;
		}

	// lambda_0 (x) = 1
	lambda[0].p[0] = 0;
    	// T_0 (x) = 1
    	T[0].p[0] = 0;
	
	while( k < t )
	{
      /* Berlekamp Algorithm */

	  /* 2 */    // Delta[2k] = coeff. of x^(2k+1) in the product Lambda[2k](x) * [1 + Syn(x)]
			 GF32Multiply_int(lambda[2*k].p, Syndromes, &temp);
			 delta[2*k]  = (int)temp.p[2*k+1];
			//printf("[temp]1 %d,%d,%d,%d,%d,%d\n",temp.p[0],temp.p[1],temp.p[2],temp.p[3],temp.p[4],temp.p[5]);

	  /* 3  	 // Lambda[2k+2](x) = Lambda[2k](x) + Delta[2k]*(x*T[2k](x)) */    
			multiplyX_int(1, T[2*k].p, &temp);
			 //multiplyX(1, T[2*k].p, &temp);
			 multiplyConstant_int(delta[2*k], temp.p, &temp);
			 //multiplyConstant(delta[2*k], temp.p, &temp);
			 GF32Add_int(lambda[2*k].p, temp.p, &lambda[2*k+2]);
     		//printf("[temp]2 %d,%d,%d,%d,%d,%d\n",temp.p[0],temp.p[1],temp.p[2],temp.p[3],temp.p[4],temp.p[5]);

	  /* 4  */    	 
			 if (delta[2*k] == ZERO || (int)GF32FindDegree_int(lambda[2*k].p) > k)
				multiplyX_int(2, T[2*k].p, &T[2*k+2]);
			 else
			 {
				multiplyX_int(1, lambda[2*k].p, &temp);
				multiplyConstant_int((int)(31-delta[2*k]), temp.p, &T[2*k+2]);
			 }
			 //printf("[temp]3 %d,%d,%d,%d,%d,%d\n",temp.p[0],temp.p[1],temp.p[2],temp.p[3],temp.p[4],temp.p[5]);

	
	  /* 5 * */   k++; // Increment for next iteration/ 
        }
	
	  CorrectErrors_int(lambda[2*k].p);	// Correct the errors as determined by the locator

	unsigned long decoded_bits;	
	decoded_bits = Bytes2Bits_int(EncMsgArray_int_v);
	//unsigned char out[2];
	deConcate(decoded_bits, out);	// Parse out the 2 chars
	//printf("[out]%d %d\n",out[0],out[1]);
	
}


//** 32 bit BerleKamp Decoder for BCH Codes **
// Decodes an encoded 32 bit message, and corrects for any detected errors
// according to the Berlekamp algorithm. See the writeup for details.
void DecoderBCH()
{

	printf("\n[DecoderBCH]\n");


	char i, j;
	char k=0;
	char delta[7];
	char Syndromes[32];
 	//Lambda is an array of 7 GF32 polynomials structs
	//Note: Lambda/T arrays can be small (i.e length 4) if rewrite multiply/divide, etc.
	struct Poly32 lambda[7];
	struct Poly32 T[7];
	struct Poly32 temp;

	GF32Init(Syndromes);
	// Create Syndrome Polynomial
	for (i=0; i<2*t; i++)
	{
		Syndromes[i+1] = (char)GF32Evaluate(i+1, EncMsgArray);
		printf("[DecoderBCH] i:%d Syndromes:%s \n",i,Syndromes+i+1);
	}




	if (display_toggle & detail_toggle)
	{	
		printf("\nSyndromes:\r\n");
		GF32PrintArray(Syndromes);
	}
		
  	/* 1  - Initialization */
   	// add 1 to S(x) and initialize Berlekamp's Algorithm
	Syndromes[0] = 0;

	//Init Lambda[i] polynomials
	for (i=0; i<7; i++)
		for (j=0; j<32; j++)
		{
			lambda[i].p[j]=ZERO;
			T[i].p[j]=ZERO;
		}

	// lambda_0 (x) = 1
	lambda[0].p[0] = 0;
    	// T_0 (x) = 1
    	T[0].p[0] = 0;
	
	while( k < t )
	{
      /* Berlekamp Algorithm */

	  /* 2 */    // Delta[2k] = coeff. of x^(2k+1) in the product Lambda[2k](x) * [1 + Syn(x)]
			 GF32Multiply(lambda[2*k].p, Syndromes, &temp);
			 delta[2*k]  = (char)temp.p[2*k+1];
			   
	  /* 3	 // Lambda[2k+2](x) = Lambda[2k](x) + Delta[2k]*(x*T[2k](x))
			 multiplyX(1, T[2*k].p, &temp);
			 multiplyConstant(delta[2*k], temp.p, &temp);
			 GF32Add(lambda[2*k].p, temp.p, &lambda[2*k+2]);
          */  
	  /* 4   
			 if (delta[2*k] == ZERO || (char)GF32FindDegree(lambda[2*k].p) > k)
				multiplyX(2, T[2*k].p, &T[2*k+2]);
			 else
			 {
				multiplyX(1, lambda[2*k].p, &temp);
				multiplyConstant((char)(31-delta[2*k]), temp.p, &T[2*k+2]);
			 }

			 if (display_toggle & detail_toggle)
			 {
		 	 	printf("\nDelta2k: %d\r\n",delta[2*k]);
			 	printf("\nLambda2k+2:\r\n");
			 	GF32PrintArray(lambda[2*k+2].p);
			 	printf("\nT2k+2:\r\n");
			 	GF32PrintArray(T[2*k+2].p);
			  }
		*/   	 
	  /* 5     k++; // Increment for next iteration */
        }
	
	//  CorrectErrors(lambda[2*k].p);	// Correct the errors as determined by the locator
	/*
	unsigned long decoded_bits;	
	decoded_bits = Bytes2Bits(EncMsgArray);
	deConcate(decoded_bits, out);	// Parse out the 2 chars
	*/

}							// Lambda polynomial



//** Concatenate **//
// Concatenates two 8bit numbers into a 16bit message
// Since it's a (31,16) code
unsigned long Concate(unsigned char num1, unsigned char num2)
{
	unsigned long temp=0;
	
	temp = temp | num1;	// Or in the num1 and shift it up
	temp = temp << 8;
 	temp = temp | num2;	// Or in num2
	
	return temp;
}

//** Find Degree of a Polynomial in GF2 **//
// Simply finds the first index of a 32bit number that is not zero
// And that is the degree+1 of the polynomial
unsigned char GF2FindDegree(unsigned long num)
{
	char str_out[20];
	char i=0;
	
	num = num << 1;	// Shift left since top bit is ignored in algorithm
	for(i=0; i<30; i++)
	{
		if (num & 0x80000000)	// Mask the current top bit, to see if it's a one
		{
			//又是需要输出梳理内存
			sprintf(str_out,"%ld\n",num);
			//printf("[GF2FindDegree]num:%ld\n",num);
			return (30-i);	// if so, that's the degree
		}
		num = num << 1;		// otherwise, keep shifting
	}
}

//** Polynomial Addition in GF2 **//
// Simply executes Modulo 2 addition
unsigned long GF2Add(unsigned long a, unsigned long b)
{
        return (a^b);	// simply xor the bits (GF2 addition for polynomials)
}

//** Polynomial Multiplication in GF2 **//
// Executes Multiplication in GF2 for polynomials
unsigned long GF2Multiply(unsigned long a, unsigned long b)
{
    unsigned long mul = 0;
    unsigned long add;
        
	char i;
	
	add = b;
	//printf("2 [GF2Multiply]--GF2FindDegree:%d a:%ld\n",GF2FindDegree(a),a);	
 	for(i=0; i <= GF2FindDegree(a); i++) // loop while not to the end of the poly
	{
		if(getBit(a, i) == 1)		// If coeff. is a one, then add multiplicand
			mul ^= add;
		add = add<<1;			// and shift the multiplicand up one
  	}

    return mul;
}

//** Polynomial Long Division in GF2 **//
// Executes Long Division in GF2 for polynomials
// The remainder (qr[1]) is equal to the final dividend (qr[0])
// Degree of qr[1] should be smaller than degree of divisor (the break condition in the loop)
void GF2Divide(unsigned long a, unsigned long b, unsigned long *qr)
{
	unsigned long dividend;
	unsigned long divisor;

	unsigned long q;
 	int deg = 0;

	dividend = a;
	divisor = b;
	qr[0] = 0;
		
	while(1)	// Keep doing this until break is activated
	{
		// Subtract degrees to find what the degree of each term in the quotient
		deg = (int)(GF2FindDegree(dividend) - (int)GF2FindDegree(divisor));

		if (deg < 0) 	// If negative, then you are done
		{
			qr[1] = dividend;		// return the dividend as the remainder
			return ;
		}

		if (deg > 0)	// otherwise find the appropriate degree for the term
			q = (unsigned long)pow((float)2,(float)deg)+1;
		else
			q = 1;
		qr[0] = GF2Add(qr[0], q);	// and add the term to the quotient
		// finally, reduce (i.e add mod 2) the divided by (term*divisor)
		
		//printf("[GF2Add]---start %ld %ld\n",q,divisor);
		unsigned long var1 =(GF2Multiply(q, divisor));
		//printf("[GF2Add]---%d %ld %ld\n",times,dividend,var1);
		dividend = GF2Add(dividend,var1);
		//printf("[GF2Divide]%d %ld \n",deg,dividend);
	}
	qr[1] = dividend;		// Return the remainder
}

//** Polynomial Modulo in GF2 **//
// Simply executes GF2Divide to find remainder of two polynomials
unsigned long GF2Mod(unsigned long a, unsigned long b)
{
	unsigned long qr[2] = {0,0};
	GF2Divide(a, b, &qr[0]);
	return qr[1];
}

//** Get a bit from a Long **//
// Returns the bit i of the long r
unsigned char getBit(unsigned long r, char i)
{
        unsigned char ret;
	  
	  // Shifts and Masks to get the appropriate bit
        ret = ((r<<(32-i-1))>>31)& 0x00000001;
        return ret;
}

//** Long to Array convertor **//
// Takes a polynomial in GF2 (long) and coverts it into a polynomial in
// GF32 (a byte array)
void Bits2Bytes(unsigned long num, char *p)
{
	char i=0, temp=0;
	
 	for(i=0; i<32; i++)
	{
		temp = num % 2;
		if (temp == 0)
			p[i] = ZERO;	// -1 is ZERO, i.e. coeff = 0
		else
			p[i] = 0;		// alpha**0, i.e. coeff = 1
		num = num >> 1;		// shift for next iteration
	}
}
 
void Bits2Bytes_int(unsigned long num, int *p)
{
	int i=0, temp=0;
	
 	for(i=0; i<32; i++)
	{
		temp = num % 2;
		if (temp == 0)
			p[i] = ZERO;	// -1 is ZERO, i.e. coeff = 0
		else
			p[i] = 0;		// alpha**0, i.e. coeff = 1
		num = num >> 1;		// shift for next iteration
	}
} 
 void GF32Init_int(int *p)
{
	 char i=0;
	 
	 for (i=0; i<32; i++)
		 p[i]=ZERO;
}
//** GF32 Initialize **//
// Simply initializes a GF32 array to all ZERO's
void GF32Init(char *p)
{
	 char i=0;
	 
	 for (i=0; i<32; i++)
		 p[i]=ZERO;
}

//** GF32 Initialize **//
// Prints a 32-element array
void PrintArray(char *p)
{
	char i=0;
	
	for(i=0; i<32; i++)
	{
		if (i%8 == 0) printf("  ");	// Space every 8 elements for clarity
		if (p[31-i] == ZERO)	// i.e. if ZERO, print 0
			printf("%d ", 0);
		else
			printf("%d ", 1);	// otherwise it's a one
	}
	printf("\r\n");
}

//** GF32 Polynomial Print **//
// Prints a 32-element array as a formatted GF32 polynomial
void GF32PrintArray(char *p)
{
	char i=0;
	char alpha=224;	// Ascii definition for alpha
	
	for(i=0; i<32; i++)
	{
		if (p[31-i] != ZERO)	// I.e. print as polynomial with coeff. in GF32
			printf("%c^%d*x^%d + ", alpha, p[31-i], 31-i);	// 0 is alpha**0 = 1
	}										// -1 is coeff = ZERO
	printf("0\r\n"); // to make the last '+' term be sensical
}


int GF32add2alpha_int(int a, int b) 
{
        if ((a == ZERO) && (b == ZERO))	// ZERO+ZERO=ZERO
            return ZERO;
        if (a == ZERO)				// ZERO is additive identity
            return b;
        else if (b == ZERO)
            return a;
        else					// Simply XOR and use lookup
            return reverseLookup_int[lookup[a]^lookup[b]];
}
 
//** Add Two Alpha Coeff. in GF32 **//
// Uses the precomputed lookup tables to add powers of alpha mod 32
char GF32add2alpha(char a, char b) 
{
        if ((a == ZERO) && (b == ZERO))	// ZERO+ZERO=ZERO
            return ZERO;
        if (a == ZERO)				// ZERO is additive identity
            return b;
        else if (b == ZERO)
            return a;
        else					// Simply XOR and use lookup
            return reverseLookup[lookup[a]^lookup[b]];
}
 
 int  GF32FindDegree_int(int *p)
{
        int i = 32;
        while(--i > 0)
            if (p[i] != ZERO) return i;
        return 0;
}

//** Find Degree of GF32 Polynomial **//
// Returns the index of the first non-ZERO element
char GF32FindDegree(char *p)
{
        char i = 32;
        while(--i > 0)
            if (p[i] != ZERO) return i;
        return 0;
}


int GF32Evaluate_int(int a, int *p)
{
	int ret = ZERO, i=0;
	int pow=0;
	
	 for(i=0; i <= GF32FindDegree_int(p); i++) 
     // for(i=0; i <= GF32FindDegree(p); i++) // evaluate over the length of the polynomial
	{
          if (p[i] != ZERO)
	    {
                pow = (int)((p[i]+a*i) % 31); // index is the degree, multiply exponents
                if (pow < 0)
                	pow = (int)(31+pow); // Evaluate mod 32
                 ret = GF32add2alpha_int(ret, pow);	
                //ret = GF32add2alpha(ret, pow);	// exponent multiplication = add
                
            }
        }
        return ret;
 }
//** GF32 Polynomial Evaluation **//
// Evaluates the result of a polynomial defined over GF32 evaluated
// with an element from GF32
char GF32Evaluate(char a, char *p)
{
	char ret = ZERO, i=0;
	char pow=0;

      for(i=0; i <= GF32FindDegree(p); i++) // evaluate over the length of the polynomial
	{
          if (p[i] != ZERO)
	    {
                pow = (char)((p[i]+a*i) % 31); // index is the degree, multiply exponents
                if (pow < 0)
                	pow = (char)(31+pow); // Evaluate mod 32
                ret = GF32add2alpha(ret, pow);	// exponent multiplication = add
            }
        }
        return ret;
 }
 
 
 void GF32Add_int(int *a, int *b, struct Poly32_int *powers)
{
	char i=0;

        for (i=0; i < 32; i++)
            powers->p[i] = GF32add2alpha_int(b[i], a[i]);
}
//** GF32 Add Two Polynomials **//
// Adds two GF32 polys using the lookup tables for each pairwise coeff.
void GF32Add(char *a, char *b, struct Poly32 *powers)
{
	char i=0;

        for (i=0; i < 32; i++)
            powers->p[i] = GF32add2alpha(b[i], a[i]);
}


void GF32Multiply_int(int *a, int *b, struct Poly32_int *mul)
{
	struct Poly32_int add;
	int i,j;
	
	for(i=0; i<32; i++)	// Initialize the arrays
	{
		mul->p[i]=ZERO;
		add.p[i]=ZERO;
	}

      for(i=0; i <= GF32FindDegree_int(a); i++)
	{
          if(a[i] != ZERO )	// multiply only non-zero terms
	    {
            for(j=0; j <= GF32FindDegree_int(b); j++) // add then shift
		{
                    if(b[j] != ZERO)
                        add.p[j+i] = (int)((a[i]+b[j]) % 31);
            }
            GF32Add_int(mul->p,add.p, mul);
            GF32Init_int(add.p);
          }
      }
}

//** GF32 Polynomial Multiplication **//
// Multiplies two GF32 polynomials and returns the result by reference.
void GF32Multiply(char *a, char *b, struct Poly32 *mul)
{
	struct Poly32 add;
	char i,j;
	
	for(i=0; i<32; i++)	// Initialize the arrays
	{
		mul->p[i]=ZERO;
		add.p[i]=ZERO;
	}

      for(i=0; i <= GF32FindDegree(a); i++)
	{
          if(a[i] != ZERO )	// multiply only non-zero terms
	    {
            for(j=0; j <= GF32FindDegree(b); j++) // add then shift
		{
                    if(b[j] != ZERO)
                        add.p[j+i] = (char)((a[i]+b[j]) % 31);
            }
            GF32Add(mul->p, add.p, mul);
            GF32Init(add.p);
          }
      }
}

void multiplyX_int(int x_power, int *p, struct Poly32_int *ret)
{
	int i;

        for(i=0; i<32; i++)
            ret->p[(i+x_power) % 32] = p[i];	// cyclic shift mod 32
}
//** Multiply GF32 Polynomial by x^power **//
// Simply executes a cyclic shift (mod 32) by the power of x
void multiplyX(char x_power, char *p, struct Poly32 *ret)
{
	char i;

        for(i=0; i<32; i++)
            ret->p[(i+x_power) % 32] = p[i];	// cyclic shift mod 32
}
 
void multiplyConstant_int(int c, int *p, struct Poly32_int *ret)
{
	 int i;
      // if multiplying by zero, return zero
      if(c == ZERO)
	{	
		GF32Init_int(ret->p);
		return ;
	}

      for (i = 0; i < 32; i++)
	{
            if(p[i] != ZERO )
                ret->p[i] = (char)((p[i]+c) % 31); // add the constant exponent, mod 32
            else
                ret->p[i] = ZERO;
      }
} 
//** Multiply GF32 Polynomial by Constant **//
// Simply multiplies each coeff. by an element from GF32, mod 32
void multiplyConstant(char c, char *p, struct Poly32 *ret)
{
	 char i;
	 
      // if multiplying by zero, return zero
      if(c == ZERO)
	{	
		GF32Init(ret->p);
		return ;
	}

      for (i = 0; i < 32; i++)
	{
            if(p[i] != ZERO )
                ret->p[i] = (char)((p[i]+c) % 31); // add the constant exponent, mod 32
            else
                ret->p[i] = ZERO;
      }
}


void CorrectErrors_int(int *p)
{
	int i;

	// evaluate roots of lambda[2*k] and flip the received code word bits accordingly
	for(i=0; i<32; i++)
	{
     		if (GF32Evaluate_int(i, p) == ZERO )	// Find the roots of Lambda
	  	{
       		if (display_toggle)
	       		printf("\r\nError at index: %d\r\n", (31-i));
         		if (EncMsgArray_int[31-i] == ZERO)	// Simply flip the bits
         			EncMsgArray_int[31-i] = (int)0;
         		else
         			EncMsgArray_int[31-i] = ZERO;
     		}
   	}
}
//** Correct the Errors in the Encoded Message **//
// Corrects the encoded message according to the errors detected by the locator
// polynomial, Lambda which is passed as p - i.e. the roots of Lambda
// are the locations of the errors
void CorrectErrors(char *p)
{
	char i;

	// evaluate roots of lambda[2*k] and flip the received code word bits accordingly
	for(i=0; i<32; i++)
	{
     		if (GF32Evaluate(i, p) == ZERO )	// Find the roots of Lambda
	  	{
       		if (display_toggle)
	       		printf("\r\nError at index: %d\r\n", (31-i));
         		if (EncMsgArray[31-i] == ZERO)	// Simply flip the bits
         			EncMsgArray[31-i] = (char)0;
         		else
         			EncMsgArray[31-i] = ZERO;
     		}
   	}
}


unsigned long Bytes2Bits_int(int *p)
{
	int i;
	unsigned long ret=0;
	
	for(i=0; i<31; i++)
	{
		ret = ret | (p[31-i] == 0); // if 0, or in a 1, if ZERO, or in a 0
		ret = ret << 1;	// and then shift it up
	}
	ret = ret | (p[0] == 0);	// shift up the last one - since only 31 elements
	
	return ret;
}
//** Convert GF32 polynomial (array) to a GF2 polynomial (long) **//
// Simply Or-in the appropriate bits, and shift up
unsigned long Bytes2Bits(char *p)
{
	char i;
	unsigned long ret=0;
	
	for(i=0; i<31; i++)
	{
		ret = ret | (p[31-i] == 0); // if 0, or in a 1, if ZERO, or in a 0
		ret = ret << 1;	// and then shift it up
	}
	ret = ret | (p[0] == 0);	// shift up the last one - since only 31 elements
	
	return ret;
}

//** De-Concatenate **//
// Returns the bottom 16 bits of a long as 2 chars
void deConcate(unsigned long a, unsigned char *ret)
{
	a = a << 1;
	ret[0] = (unsigned char)((a & 0xFF000000) >> 24);
	ret[1] = (unsigned char)((a & 0x00FF0000) >> 16);
}


//** User Help menu **//
// Displays help menu options for User, while taking into
// account the appropriate status toggles
void HelpMenu()
{
	printf("\r\n\n-=- Help Menu -=-\r\n\n");
	if (Add_Error)
		printf("--Noise Module is ON: Seed %d\r\n", seed);
	if (display_toggle & detail_toggle)
		printf("--Detailed Display is ON\r\n");
	if (display_toggle)
		printf("--Display is ON\r\n\n");
	else
		printf("\n");
	printf("(O)utput Original Message to the Screen.\r\n");
	printf("(R)un Encode/Decode and Display.\r\n");
	printf("(G)eneral Display - Toggle On/Off.\r\n");
	printf("(N)oise Module - Toggle On/Off.\r\n");	
	printf("(D)etailed Display of Intermediate Steps - Toggle On/Off.\r\n");
	printf("(?)Display Help Menu.\r\n\n");
}

//** Get Command Initialize **//
// Initializes the UART for recieving a command from the user

void get_cmd_init()
{
	//char_count = 0;
	char_ready = 0;
	//UCSRB.7 = 1;
	cmd_ready = 0;
}

//** Print a long as 4 chars **//
// For use with Hyperterm, since it cannot display longs
void PrintLong(unsigned long a)
{
	unsigned char b,c,d,e;

	b = (unsigned char)((a & 0xFF000000) >> 24)+50;
	c = (unsigned char)((a & 0x00FF0000) >> 16)+50;
	d = (unsigned char)((a & 0x0000FF00) >> 8)+50;	
	e = (unsigned char)((a & 0x000000FF))+50;
	printf("%c%c%c%c\r\n",b,c,d,e);
}

//** Random Noise Module **//
// Generates a random number of errors in random locations in the 
// encoded message. Essentially simulates a variably noisy channel over
// which the encoded message would be transmitted.
void ErrorModule(char *p, char numerrors)
{
	char i, val;
	for (i=1; i<=numerrors; i++)	// NumErrors is random and seeded by the user
	{
		val = (int)(rand() * (32.0) / (RAND_MAX + 1.0)) + 1; 	//random integer in 1-32 range
		if (display_toggle)
			printf("%d ", val-1);
		if (p[val-1] == ZERO)	// Flip the appropriate bits
			p[val-1] = 0;
		else
			p[val-1] = ZERO;
	}
}
/*
*测试函数
*/
int bch_init()
{
//	LOGW("[bch_init]");
	bch_test_func();
	return 0;
}



// The Entry point to the ECC unit
// Simply initializes and then runs the UserStateMachine forever
/*
int main()
{
	printf("[main] bch\n");
	bch_test_func();
	printf("over\n");
	return 0;
}*/
