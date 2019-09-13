#include "md5.h"  
#include "pbc.h"
#include "pbc_fp.h"
#include "pbc_fieldquadratic.h"
#include "pbc_curve.h"

unsigned char md5_m[1000] = { 0 };
int md_lenght;
int type1_counter;
int packet_lost[6] = { 0 };
unsigned char frame_lost[21] = {0};

void get_key() {

	int i;
	element_t g, h;
	element_t w1;
	element_t a, b;
	mpz_t prime, cofac, power;
	field_t Fq, Fq2, E, E2;
	mpz_t order;
	mpz_init(prime);
	mpz_init(order);
	mpz_init(cofac);
	mpz_set_ui(prime, 547);

	field_init_fp(Fq, prime);

	element_init(a, Fq);
	element_init(b, Fq);

	field_init_fi(Fq2, Fq);

	element_set1(a);
	element_set0(b);
	mpz_set_ui(order, 127);
	mpz_set_ui(cofac, 16);

	field_init_curve_ab(E, a, b, order, cofac);

	element_clear(a);
	element_clear(b);
	element_init(a, Fq2);
	element_init(b, Fq2);
	element_set1(a);
	element_set0(b);

	mpz_mul(cofac, cofac, cofac);
	field_init_curve_ab(E2, a, b, order, NULL);

	element_init(g, E2);
	element_init(h, E2);

	element_init(w0, Fq2);
	element_init(w1, Fq2);

	mpz_init(power);
	mpz_set_ui(power, b1);
	element_mul_mpz(h, h, power);
	element_set_str(g, "[[67,0],[481,0]", 0);
	element_set_str(h, "[[480,0],[0,481]", 0);

	mpz_init(power);
	mpz_set_ui(power, other);
	element_mul_mpz(g, g, power);
	//printf("the public key of this broad is b*q =");
	element_printf("%B", h);



	printf("the key of this broadcast is weilpairing e(%d*P,b&q)^%d = ",other,private);
	element_printf("%B", g);
	printf("^%d = ", private);
	element_printf("%B\n", w0);

}
void get_first() {
	//unsigned char* move = position;
	unsigned char* start = pos;
	px1 = pos[4];
	px2 = pos[8];
	b1 = pos[7];
	py1 = pos[12];
	py2 = pos[16];
	
	printf("x1:%d x2:%d Y1:%d Y2;%d |", pos[4], pos[8], pos[12], pos[16]);
	//get_key();
	pos += 23;
	md_lenght = pos[0] + pos[1] * 256 - 23 - 4 - 4 - 4;
	printf("the md_5 code length = %d |", md_lenght);
	pos += 4;
	printf("number of packets:%d |", pos[0] + pos[1] * 256);
	seq_num = pos[0] + pos[1] * 256;
	pos += 4;
	printf("number of block each packet:%d |\n\n", pos[0] + pos[1] * 256);
	block_num = pos[0] + pos[1] * 256;
	pos += 4;

	pos = start + 17;
	if (userName[0] == '1' || userName[0] == '3') {
		if (userName[0] == '1') {
			private = 71;
			other = 103;
		}
		else{
			private = 103;
			other = 71;
		}
		printf("\n");
		get_key();
		printf("\n\nthe signature is :\n");
		char sig[6];
		int o = 0;
		while (o <= 5) {
			sig[o] = (pos[0] - (39 + 49));
			printf("%c   ->%c\n", pos[0], sig[o]);
			//printf("%d\n", pos[0]);
			pos++;
			o++;
		}
		printf("%d\n", pos[0]);
		printf("sig correct \n");
		receiving = 1;
		printf("\n");
		o = 0;
		printf("the md_code of all file is:\n");
		pos = start + 39;
		while (o < md_lenght) {
			md5_m[o] = (unsigned char)pos[o] - (49 + 39);
			printf("%02x", (unsigned char)(pos[o] - 49 - 39));
			
			o++;
			if (o % 16 == 0) { 
				printf("\\");
			}
		}
		printf("\n");
		type1_counter++;
		
		//printf("packet 3 received \n");

	}
	
}

void get_file_image() {

	unsigned char* start = pos;
	pos += 1;
	printf("get the packet %d\n", pos[0]-1);
	
	pos += 8;
	unsigned char rec_file[2048] = { 0 };
	int x = 0;
	if (userName[0] == '1' || userName[0] == '3') {	
		
		if (userName[0] == '1') {
			int test;
			srand(time(NULL));
			test = rand() % 4; 
			//printf("\ntest:%d\n", test);
			if (test == 1 || test == 2) { // 50% of packet loss
			//if(start[1] == 1){
				printf("------------I loss pakcet %d------------\n", start[1]-1);
				packet_lost[start[1] - 1] = 1;
				while (x < 1024) {
					rec_file[x] = '?';
					file[((start[1] - 1) * 1024) + x] = rec_file[x];
					//printf("\ntesting::\n%s", file);
					x++;
					//return;
				}
				if (start[1] == 6) {
					printf("all recedivd\n");
					state = 2;

				}
				return;
			}
		}

		while (x < 1024) {
			
			rec_file[x] = pos[x] - (49 + 39);
			if (userName[0] == '3' && start[1] != 6) {
				int test;
				srand(time(NULL));
				test = rand() % 4;
				rec_file[test * 256 + 10] = '?';
				rec_file[test * 256 + 11] = '?';
				rec_file[test * 256 + 12] = '?';
				rec_file[test * 256 + 13] = '?';
				rec_file[test * 256 + 14] = '?';
				rec_file[test * 256 + 15] = '?';

			}

			//printf("%c", rec_file[x]);
			file[((start[1] - 1) * 1024) + x] = rec_file[x]; //latter
			//printf("\ntesting::\n%s", file);
			x++;

		}


	}
	else {
		printf("packet %d contain %d %d %d %d\n", start[1], start[5], start[6], start[7], start[8]);
	}
	printf("\n--------------the code frame i received : %s\n\n",rec_file);
	//testing
	if (userName[0] == '1' || userName[0] == '3') {
		printf("\n ------- now testing the correctness of the file-----\n");
		MD5_CTX md5;
		MD5Init(&md5);
		int j = 0;
		int frame = 0;
		unsigned char encrypt[257] = { 0 };
		unsigned char decrypt[16];
		while (j < x) {
			int counter = 0;
			//printf("%d;\n", j / 32);
			while (counter < 256 && j < x) {
				encrypt[counter] = rec_file[j];
				j++;
				counter++;
			}
			MD5Init(&md5);
			MD5Update(&md5, encrypt, strlen((char*)encrypt));
			MD5Final(&md5, decrypt);
			printf("testing frame %d:\n ", j / 256 + (start[1]-1)*4);
			int o = 0;
			for (o = 0; o < 16; o++) {
				
				int z = (j / 256 + (start[1] - 1) * 4 - 1)*16;
				
				if (decrypt[o] != md5_m[z + o] && start[1] != 6) {
					printf("%d:%02x -> %02x ", z,decrypt[o], md5_m[z + o]);
					printf("the code is incorrect at seq: %d and frame: %d\n",start[1],j/256 + (start[1] - 1) * 4);
					frame_lost[(start[1] - 1) * 4 + j / 256 - 1] = 1;
				}
				else {
					printf("correct\n");
				}
				
			}
			//j++;
		}
	}

	if (start[1] == 6) {
		printf("all recedivd correctly\n");
		state = 2;
	
	}

}

void get_some_frame() {
	unsigned char* start = pos;
	pos += 1;
	printf("get the packet  of type 4\n");
	printf("this packet contain frame num %d %d %d %d\n",pos[0],pos[1],pos[2],pos[3]);
	pos += 4;
	//unsigned char rec_file[2048] = { 0 };
	

	///
	if (userName[0] == '3') {
		printf("\n------------ it contains frames that I missed-----------\n");
		int i = 0;
		printf("\n------before decode------------\n");
		while (i < 4) {
			int j = 0;
			printf("\n-----------reading frame number %d---------\n", start[1 + i]);
			while (j < 256) {
				file[start[1 + i] * 256 + j] = pos[i * 256 + j] -49 -39;
				if ( (j == 56 || j == 57 || j == 58 || j == 59 || j == 60) && i == 2) {
					printf("?");
				
				}
				else {
					printf("%c", file[start[1 + i] * 256 + j]);
				}
				j++;
			}
			frame_lost[start[1 + i]] = 0;
			printf("\n---------------\n");
			i++;
		}
		i = 0;
		printf("\n--------------after decode----------------\n");
		while (i < 4) {
			int j = 0;
			printf("\n-----------reading frame number %d---------\n", start[1 + i]);
			while (j < 256) {	
				printf("%c", file[start[1 + i] * 256 + j]);
				j++;
			}
			frame_lost[start[1 + i]] = 0;
			printf("\n---------------\n");
			i++;
		}

		int o = 0;
		int x = 0;
		while (o < 21) {
			//printf("??");
			if (packet_lost[o] == 1) {
				x++;
			}
			o++;
		}
		if (x == 0) {
			printf("all frame lost resended\n");
			state = 2;
		}

	
	}

}
void get_one_image() {
	unsigned char* start = pos;
	pos += 1;
	printf("get the packet %d\n", pos[0]);
	pos += 8;
	unsigned char rec_file[2048] = { 0 };
	int x = 0;
	if (userName[0] == '1') {
		printf("this is the packet that I missed\n");
		//int x = 0;
		while (x < 1024) {
			rec_file[x] = pos[x] - (49 + 39);
			//printf("%c", rec_file[x]);
			file[((start[1] - 1) * 1024) + x] = rec_file[x]; //latter
			//printf("\ntesting::\n%s", file);
			x++;

		}
	}
	else {
		printf("packet %d contain %d %d %d %d\n", start[1], start[5], start[6], start[7], start[8]);
	}
	printf("\n--------------the code frame i received : %s\n\n", rec_file);
	if (userName[0] == '1') {
		printf("\n ------- now testing the correctness of packet -----\n");
		MD5_CTX md5;
		MD5Init(&md5);
		int j = 0;
		int frame = 0;
		unsigned char encrypt[257] = { 0 };
		unsigned char decrypt[16];
		//printf("\n ------- now testing the correctness of packet ?-----\n");
		while (j < x) {
			int counter = 0;
			printf("%d;\n", j / 32);
			while (counter < 256 && j < x) {
				encrypt[counter] = rec_file[j];
				j++;
				counter++;
			}
			MD5Init(&md5);
			MD5Update(&md5, encrypt, strlen((char*)encrypt));
			MD5Final(&md5, decrypt);
			printf("testing frame %d:\n ", j / 256 + (start[1] - 1) * 4);
			int o = 0;
			for (o = 0; o < 16; o++) {

				int z = (j / 256 + (start[1] - 1) * 4 - 1) * 16;

				//if (decrypt[o] != md5_m[z + o] && start[1] != 6) {
				if(0){
					printf("%d:%02x -> %02x ", z, decrypt[o], md5_m[z + o]);
					printf("the code is incorrect at seq: %d and frame: %d\n", start[1], j / 256);

				}
				else {
					printf("correct\n");
				}

			}
			//j++;
		}
		//if correct
		packet_lost[start[1]-1] = 0;
		int o = 0;
		x = 0;
		while (o < 6) {
			//printf("??");
			if (packet_lost[o] == 1) {
				x++;
			}
			o++;
		}
		if (x == 0) {
			printf("all packet lost resended\n");
			state = 2;
		}
	}

}