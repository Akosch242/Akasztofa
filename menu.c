#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "spieler.h"
#include "spiel.h"

enum menu {
	REGISTER = 1,
	LOGIN,
	RANK,
	PLAY,
	EXIT
};

void print_menu() {
	if (logged) {
		printf("Hauptmenu\t\t\x1B[32mName: %s | Rekord: ", sp.name);
		if (sp.rekord == KEIN_REKORD_NOCH)
			printf("-");
		else
			printf("%d", sp.rekord);
		puts("\033[0m\n");
	}
	else {
		printf("Hauptmenu\n\n");
	}
	printf("1. Registration\n");
	printf("2. Anmeldung\n");
	printf("3. Rangliste\n");
	printf("%s", logged ? "4. Spielen\n" : "\n");

	printf("5. Verlasse das Spiel\n");
}

void menu() {
	char gewaehlt[2];
	while (1) {
		system("cls");

		print_menu();
		scanf("%1s", gewaehlt);

		system("cls");

		switch (gewaehlt[0]-'0')
		{
		case REGISTER:
			printf("Registration\n\n");
			spieler_reg();
			break;
		case LOGIN:
			printf("Login\n\n");
			spieler_login();
			break;
		case RANK:
			printf("Rangliste\n\n");
			spieler_ranks();
			break;
		case PLAY:
			if (logged) {
				spiel_start();
			}
			break;
		case EXIT:
			return;
		default:
			break;
		}
	}

	return;
}