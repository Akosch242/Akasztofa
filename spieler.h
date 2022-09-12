#ifndef _SPIELER_H_
#define _SPIELER_H_
#define DB_FILE "spieler.csv"
#define KEIN_REKORD_NOCH (1 << 30)

typedef struct spieler {
	char name[30];
	char password[30];
	int rekord;
} spieler;

extern spieler sp;

extern int logged;

void spieler_ranks();
void spieler_login();
void spieler_reg();

#endif