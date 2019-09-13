#include <stdlib.h>
#include "pbc.h"
#include "pbc_fp.h"
#include "pbc_fieldquadratic.h"
#include "pbc_curve.h"
#include "md5.h"  

static field_t Fq, Fq2, E, E2;
static mpz_t order;
int public_x1 = 80, public_x2 = 0, public_y1 = 0, public_y2 = 77;
element_t w0;
int file_length = 0;
unsigned char md5_m[1000] = { 0 };
int md5_length = 0;
int packet_lost[6] = { 0 };
int frame_lost[21] = { 0 };

void do_vert(element_ptr z, element_ptr V, element_ptr Q)
{
	element_ptr Vx = curve_x_coord(V);
	element_ptr Qx = curve_x_coord(Q);
	element_ptr Qy = curve_y_coord(Q);

	element_t a, b, c;
	element_init_same_as(a, Vx);
	element_init_same_as(b, Vx);
	element_init_same_as(c, Vx);

	//a = 1
	//b = 0;
	//c = -Vx
	element_set1(a);
	element_set0(b);
	element_neg(c, Vx);

	// element_printf("vert at %B: %B %B %B\n", Vx, a, b, c);
	element_mul(a, a, Qx);
	element_mul(b, b, Qy);
	element_add(c, c, a);
	element_add(z, c, b);
	// element_printf("vert eval = %B\n", z);
	element_clear(a);
	element_clear(b);
	element_clear(c);
}

void do_tangent(element_ptr z, element_ptr V, element_ptr Q)
{
	element_ptr Vx = curve_x_coord(V);
	element_ptr Vy = curve_y_coord(V);
	element_ptr Qx = curve_x_coord(Q);
	element_ptr Qy = curve_y_coord(Q);

	element_t a, b, c;
	element_init_same_as(a, Vx);
	element_init_same_as(b, Vx);
	element_init_same_as(c, Vx);

	//a = -slope_tangent(V.x, V.y);
	//b = 1;
	//c = -(V.y + aV.x);
	/*
	//we could multiply by -2*V.y to avoid division so:
	//a = -(3 Vx^2 + cc->a)
	//b = 2 * Vy
	//c = -(2 Vy^2 + a Vx);
	//
	//actually no, since fasterweil won't work if we do this
	*/
	element_square(a, Vx);
	//element_mul_si(a, a, 3);
	element_add(b, a, a);
	element_add(a, b, a);
	element_set1(b);
	element_add(a, a, b);
	element_neg(a, a);
	element_double(b, Vy);
	element_div(a, a, b);
	element_set1(b);
	element_mul(c, a, Vx);
	element_add(c, c, Vy);
	element_neg(c, c);

	// element_printf("tan at %B: %B %B %B\n", V, a, b, c);

	element_mul(a, a, Qx);
	element_mul(b, b, Qy);
	element_add(c, c, a);
	element_add(z, c, b);
	//  element_printf("tan eval = %B\n", z);
	element_clear(a);
	element_clear(b);
	element_clear(c);
}

void do_line(element_ptr z, element_ptr V, element_ptr P, element_ptr Q)
{
	element_ptr Vx = curve_x_coord(V);
	element_ptr Vy = curve_y_coord(V);
	element_ptr Px = curve_x_coord(P);
	element_ptr Py = curve_y_coord(P);
	element_ptr Qx = curve_x_coord(Q);
	element_ptr Qy = curve_y_coord(Q);

	element_t a, b, c, e0;
	element_init_same_as(a, Vx);
	element_init_same_as(b, Vx);
	element_init_same_as(c, Vx);
	element_init_same_as(e0, Vx);

	//a = -(B.y - A.y) / (B.x - A.x);
	//b = 1;
	//c = -(A.y + a * A.x);

	element_sub(a, Py, Vy);
	element_sub(b, Vx, Px);
	element_div(a, a, b);
	element_set1(b);
	element_mul(c, a, Vx);
	element_add(c, c, Vy);
	element_neg(c, c);

	/*
	//but we could multiply by B.x - A.x to avoid division, so
	//a = -(By - Ay)
	//b = Bx - Ax
	//c = -(Ay b + a Ax);
	element_sub(a, Vy, Py);
	element_sub(b, Px, Vx);
	element_mul(c, Vx, Py);
	element_mul(e0, Vy, Px);
	element_sub(c, c, e0);
	//
	//actually no, since fasterweil won't work if we do this
	*/

	// element_printf("line at %B: %B %B %B\n", V, a, b, c);
	element_mul(a, a, Qx);
	element_mul(b, b, Qy);
	element_add(c, c, a);
	element_add(z, c, b);
	// element_printf(" = %B\n", z);

	element_clear(a);
	element_clear(b);
	element_clear(c);
	element_clear(e0);
}

miller(element_t z, element_t PR, element_t R, element_t P, element_t Q)
{
	int m = mpz_sizeinbase(order, 2) - 2;
	//order->
	//printf("m=%d order = %d\n", m,*order->_mp_d);
	element_t Z;
	element_t z1;
	element_t x1;
	element_init_same_as(Z, PR);

	element_set(Z, P);
	element_set1(z);
	element_init_same_as(z1, z);
	element_init_same_as(x1, z);

	do_vert(x1, PR, Q);
	 element_printf("vert(P+R) %B\n", x1);
	do_line(z1, P, R, Q);
	 element_printf("line(P,R) %B\n", z1);
	element_div(x1, x1, z1);
	 element_printf("x1 %B\n", x1);
	element_set(z, x1);

	for (;;) {
		//printf("iteration %d: %d\n", m, mpz_tstbit(order,m));
		element_square(z, z);
		// element_printf("squared: %B\n", z);
		do_tangent(z1, Z, Q);
		element_mul(z, z, z1);

		element_double(Z, Z);
		do_vert(z1, Z, Q);
		element_div(z, z, z1);
		// element_printf("pre-if: %B\n", z);

		if (mpz_tstbit(order, m)) {
			element_mul(z, z, x1);
			do_vert(z1, P, Q);
			element_mul(z, z, z1);
			
		}
		if (!m) break;
		m--;
	}

	element_clear(x1);
	element_clear(z1);
}

void weil(element_t w, element_t g, element_t h)


void sendFirstP() {
	//printf("-----------------all sensor connectted press to go on------------------------------------\n");
	//getch();
	printf("-----------------------now generating public key and private key of this boradcast------------------------------\n");

	//printf("hi\n");
	int a1 = 71, a2 = 126, a3 = 103;
	printf("the private key of each node is a1 = %d, a2 = %d, a3 = %d the target is a1 a2 a4\n", a1, a2, a3);
	printf("the sig of basestation is 'ABCDEFG'\n");
	printf("chosen curve : y^2 = x^3 + x over finit field F_547\n");
	printf("chosen P(67,481) Q(-67,481i) on the curve \n");
	printf("---------------------------------------------------------------------------------------------\n");
	int b1;
	srand(time(NULL));
	b1 = rand()%4 + 10; //later
	//b1 = 11;
	printf("the scaling factor b of this boradcast is %d\n", b1);
	////////build the curv
	int i;
	element_t g, h;
	element_t w1;
	element_t a, b;
	mpz_t prime, cofac, power;

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

	element_set_str(g, "[[67,0],[481,0]", 0);
	element_set_str(h, "[[480,0],[0,481]", 0);

	mpz_init(power);
	mpz_set_ui(power, b1);
	element_mul_mpz(h, h, power);
	printf("the public key of this broad is b*q =");
	element_printf("%B", h);

	//weil(w0, g, h);

	printf("the key of this broadcast is weilpairing e(a1*a3*P,b&q) = ");
	get_key(b1);
	

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("now sending the first packet which contains the sig and basic information of the file\n");
	getch();
	
	printf("\n");
	int o;
	for (o = 0; o < md5_length; o++) {
		//md5_m[k] = decrypt[o];
		printf("%02x", md5_m[o]);

	}
	printf("\n");
	int ret = 0;
	unsigned char first_packet[2048];
	//add seqnumber
	int seq = 1;
	//first_packet[0] = 1;
	unsigned char* pos = first_packet;
	pos[0] = seq;
	//pos += 4;
	//add public key
	public_x1 = 80, public_x2 = 0, public_y1 = 0, public_y2 = 77;
	pos[4] = public_x1;
	pos[7] = b1;
	pos[8] = public_x2;
	pos[12] = public_y1;
	pos[16] = public_y2;
	//add sig
	pos[17] = 'a';
	pos[18] = 'b';
	pos[19] = 'c';
	pos[20] = 'd';
	pos[21] = 'e';
	pos[22] = 'f';
	//entry the sig
	o = 17;
	while (o <= 22) {
		char x = pos[o] + (49 + 39);
		pos[o] = x;
		o++;
	}
	//size of this packet
	pos += 23;
	int size = 23 + 4 + 4 + 4 + md5_length;
	memcpy(pos, &size, sizeof(int));
	//number of pakcet 
	pos += 4;
	int each = file_length/1024;
	printf("number of packet is %d\n", each);
	memcpy(pos, &each, sizeof(int));
	//number of block of each packet
	pos += 4;
	int block_num = 4;
	memcpy(pos, &block_num, sizeof(int));
	
	pos += 4;
	memcpy(pos, &file_length, sizeof(int));
	//int t = pos[0]+pos[1]*256 + pos[2] * 256 *256;
	//printf("--------%d\n", t);

	pos += 4;
	o = 0;
	while (o < md5_length) {
		pos[o] = md5_m[o] + (49 + 39);
		o++;
	}
	//pos[1] = '*';
	//send first packet
	printf("the first packet structure;\n");
	pos = first_packet;
	printf("%d |", pos[0]);
	printf("x1:%d x2:%d Y1:%d Y2;%d |", pos[4], pos[8], pos[12], pos[16]);
	printf("sig:%c%c%c%c%c%c(encrted) |", pos[17] - 49 - 39, pos[18] - 49 - 39, pos[19] - 49 - 39, pos[20] - 49 - 39, pos[21] - 49 - 39, pos[22] - 49 - 39);
	pos += 23;
	printf("size of this packet:%d |", pos[0]+pos[1]*256);
	pos += 4;
	printf("number of packet going to send:%d |", pos[0] + pos[1] * 256);
	pos += 4;
	printf("number of block of each packet:%d |", pos[0] + pos[1] * 256);
	pos += 4;
	printf("size of file:%d |\n\n", pos[0] + pos[1] * 256);
	pos += 4; 
	printf("the hash value of each code block(entryted)\n");
	o = 0;
	while (o < md5_length) {
		printf("%02x", (unsigned char)(pos[o]-49-39));
		o++;
	}
	memcpy(g_Client[0].buf, first_packet, sizeof(g_Client[0].buf));
	memcpy(g_Client[1].buf, first_packet, sizeof(g_Client[1].buf));
	memcpy(g_Client[2].buf, first_packet, sizeof(g_Client[2].buf));
	//memcpy(g_Client[3].buf, first_packet, sizeof(g_Client[3].buf));
	//memcpy(g_Client[1].buf, message, sizeof(g_Client[1].buf));

	ret = send(g_Client[0].sClient, g_Client[0].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[1].sClient, g_Client[1].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[2].sClient, g_Client[2].buf, sizeof(g_Client[0].buf), 0);
	//ret = send(g_Client[3].sClient, g_Client[3].buf, sizeof(g_Client[0].buf), 0);
	//Sleep(1000);

}
//Phase
void sendSecond() {

	//add seqnumbe
	//add public key
	int o = 0;
	while (o < file_length) {
		printf("now sending pakcet start at %d --------------\n", o/1024);
		unsigned char packet[2048] = {0};
		int type = 2;	
		unsigned char* pos = packet;
		pos[0] = type;
		pos += 1;
		//packet number
		pos[0] = o/1024 + 1;
		pos += 4;
		//frame number
		pos[0] = (o / 1024) * 4;
		pos[1] = (o / 1024) * 4 + 1;
		pos[2] = (o / 1024) * 4 + 2;
		pos[3] = (o / 1024) * 4 + 3;
		//file size
		int size = 1 + 4 + 4 + 4 + 1024;
		memcpy(pos, &size, sizeof(int));
		pos += 4;
		//file contain
		int x = 0;
		while (x < 1024) {
			pos[x] = file[o + x] + (49 + 39);
			//printf("%c", pos[x]-49-39);
			x++;
		}
		printf("\n\n");
		//printf("\n\n%s\n\n", packet);
		//memcpy(g_Client[0].buf, packet, sizeof(g_Client[0].buf));
		//ret = send(g_Client[0].sClient, g_Client[0].buf, sizeof(g_Client[0].buf), 0);
		memcpy(g_Client[0].buf, packet, sizeof(g_Client[0].buf));
		memcpy(g_Client[1].buf, packet, sizeof(g_Client[1].buf));
		memcpy(g_Client[2].buf, packet, sizeof(g_Client[2].buf));
		//memcpy(g_Client[3].buf, packet, sizeof(g_Client[3].buf));
		//memcpy(g_Client[1].buf, message, sizeof(g_Client[1].buf));
		int ret;
		ret = send(g_Client[0].sClient, g_Client[0].buf, sizeof(g_Client[0].buf), 0);
		ret = send(g_Client[1].sClient, g_Client[1].buf, sizeof(g_Client[0].buf), 0);
		ret = send(g_Client[2].sClient, g_Client[2].buf, sizeof(g_Client[0].buf), 0);
		//ret = send(g_Client[3].sClient, g_Client[3].buf, sizeof(g_Client[0].buf), 0);
		//seq++;
		o += 1024;
		if (o > file_length) {
			break;
		}
		
		Sleep(2000);
		//getch();
	}
	all_state++;
	
}




//sending lost packet
void sendOnePacket(int num) {
	//int o = 0;
	
	printf("now sending pakcet start at %d --------------\n", num);
	unsigned char packet[2048] = { 0 };
	int type = 3;
	unsigned char* pos = packet;
	pos[0] = type;
	pos += 1;
	//packet number
	pos[0] = num + 1;
	pos += 4;
	//frame number
	pos[0] = num * 4;
	pos[1] = num * 4 + 1;
	pos[2] = num * 4 + 2;
	pos[3] = num * 4 + 3;
	//file size
	int size = 1 + 4 + 4 + 4 + 1024;
	memcpy(pos, &size, sizeof(int));
	pos += 4;
	//file contain
	int x = 0;
	while (x < 1024) {
		pos[x] = file[num*1024 + x] + (49 + 39);
		printf("%c", pos[x]-49-39);
		x++;
	}
	printf("\n\n");
	getch();
	memcpy(g_Client[0].buf, packet, sizeof(g_Client[0].buf));
	memcpy(g_Client[1].buf, packet, sizeof(g_Client[1].buf));
	memcpy(g_Client[2].buf, packet, sizeof(g_Client[2].buf));
	//memcpy(g_Client[3].buf, packet, sizeof(g_Client[3].buf));
	//memcpy(g_Client[1].buf, message, sizeof(g_Client[1].buf));
	int ret;
	ret = send(g_Client[0].sClient, g_Client[0].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[1].sClient, g_Client[1].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[2].sClient, g_Client[2].buf, sizeof(g_Client[0].buf), 0);
	
	packet_lost[num] = 0;
}
void sendSomeFrame() {

	int fram_num = 0;
	int i = 0;
	unsigned char packet[2048] = { 0 };
	int type = 4;
	unsigned char* pos = packet;
	pos[0] = type;
	pos += 1;

	while (i < 21) {
		if (frame_lost[i] == 1) {
			pos[fram_num] = i;
			fram_num++;
			frame_lost[i] = 0;
			if (fram_num == 4) {
				break;
			}
		}
		i++;
	}
	while (fram_num < 4) {
		//fram_num++;
		pos[fram_num] = 1;
		fram_num++;
		
	}
	printf("%d %d %d %d\n", packet[1], packet[2], packet[3], packet[4]);
	getch();
	pos += 4;
	int x = 0;
	int t = 0;
	while (x < 1024) {
		int j = 0;
		while (j < 256) {
			pos[x+j] = file[packet[1 + t] * 256 + j] + (49 + 39);
			//printf("fram %d ;%d:\n", packet[1 + t],packet[1 + t] * 256 + j);
			printf("%c", pos[x + j] - 49 - 39);
			j++;
		}
		printf("\n------\n");
		x+= 256;
		t++;
	}
	x = 0;
	printf("\n---esures code:---\n");
	while (x < 256) {
		int test;
		srand(time(NULL));
		test = rand() % 256;
		printf("%d ", test);

	}
	printf("\n------\n");
	memcpy(g_Client[0].buf, packet, sizeof(g_Client[0].buf));
	memcpy(g_Client[1].buf, packet, sizeof(g_Client[1].buf));
	memcpy(g_Client[2].buf, packet, sizeof(g_Client[2].buf));
	//memcpy(g_Client[3].buf, packet, sizeof(g_Client[3].buf));
	//memcpy(g_Client[1].buf, message, sizeof(g_Client[1].buf));
	int ret;
	ret = send(g_Client[0].sClient, g_Client[0].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[1].sClient, g_Client[1].buf, sizeof(g_Client[0].buf), 0);
	ret = send(g_Client[2].sClient, g_Client[2].buf, sizeof(g_Client[0].buf), 0);

}