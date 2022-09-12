#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "vkliste.h"
#include "spieler.h"

void spiel_readfile(vkliste** head) {
	FILE* fp = NULL;
	char buffer[50];
	fp = fopen("szavak.txt", "r");
	if (fp == NULL) {
		puts("Cant open szavak.txt");
		exit(-1);
	}

	while (fgets(buffer, 50, fp) != NULL) {
		buffer[strlen(buffer)-1] = '\0';
		vkliste_add(head, buffer);
	}
	fclose(fp);
}

void spiel_print_galgen(int schritte) {
	FILE* fp = NULL;
	char filename[30];
	char buffer[50];

	snprintf(filename, 30, "akasztofa%d.txt", schritte);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Cannot open %s", filename);
		exit(-1);
	}

	while (fgets(buffer, 50, fp) != NULL) {
		puts(buffer);
	}

	fclose(fp);
}

void spiel_print_buchstaben(char* bs, int i) {
	int j = 0;
	for (j = 0; j < i; j++) {
		printf("%c  ", bs[j]);
	}
}

int spiel_str_contains_char(char* bs, int last, char c) {
	int i = 0;

	for (i = 0; i < last; i++) {
		if (bs[i] == c) {
			return 1;
		}
	}
	return 0;
}

void spiel_save() {
	char buffer[100];
	char name[30];
	FILE* fp = fopen(DB_FILE, "r");
	FILE* tmp_fp = fopen("savetmp.csv", "w");

	if (fp == NULL || tmp_fp == NULL) {
		printf("Problem mit %s", DB_FILE);
		exit(-1);
	}

	while (fgets(buffer, 100, fp)) {
		sscanf(buffer, "%[^;];%*s\n", name);
		name[29] = '\0';
		if (strcmp(name, sp.name) == 0) {
			fprintf(tmp_fp, "%s;%s;%d\n", sp.name, sp.password, sp.rekord);
		}
		else {
			fputs(buffer, tmp_fp);
		}
	}

	fclose(fp);
	fclose(tmp_fp);

	remove(DB_FILE);

	rename("savetmp.csv", DB_FILE);
}

int spiel_read_valid_char(char* bs, int i) {
	do {
		printf("\n\nGeben Sie ein andere Character an (oder 9 zu austreten): ");
		scanf("%1s", &bs[i]);
		if (bs[i] == '9') {
			return 0;
		}
	} while (((bs[i] < 'a') || (bs[i] > 'z')) && spiel_str_contains_char(bs, i, bs[i]));

	return 1;
}

void spiel_start() {
	int i = 0;
	int anzahl = 0;
	char buchstaben[11];
	vkliste* woerter = NULL;

	for (i = 0; i < 10; i++) {
		buchstaben[i] = '\0';
	}

	spiel_readfile(&woerter);
	
	for (i = 0; i < 10; i++) {
		spiel_print_galgen(i);
		spiel_print_buchstaben(buchstaben, i);
		anzahl = vkliste_anzahl(woerter);

		if (anzahl <= 1) {
			printf("\nSie haben gewonnen! Der gefundene Wort: %s", woerter->str);
			break;
		}

		//printf("\nAktuelle Ergebnis: %d", anzahl);
		if (!spiel_read_valid_char(buchstaben, i)) {
			vkliste_free(woerter);
			return;
		}
		vkliste_del(&woerter, buchstaben[i]);
		system("cls");
	}

	sp.rekord = anzahl;
	if (anzahl > 1) {
		spiel_print_galgen(9);
		spiel_print_buchstaben(buchstaben, 9);
		puts("");
		printf("\nDeine Ergebnis: %d", sp.rekord);
	}

	puts("\n9. Zueruck zu Menu");

	do {
		scanf("%c", &buchstaben[0]);
	} while (buchstaben[0] != '9');
	vkliste_free(woerter);
	spiel_save();
}