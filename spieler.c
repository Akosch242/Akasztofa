#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <conio.h>	// getch
#define DB_FILE "spieler.csv"
#define KEIN_REKORD_NOCH (1 << 30)

typedef struct spieler {
	char name[30];
	char password[30];
	int rekord;
} spieler;

spieler sp;
int logged = 0;

int is_str_valid(char* str) {
	int index = 0;
	while (str[index] != '\0') {
		if (index >= 30) {
			puts("\nZu lange, probieren Sie noch einmal!");
			return 0;
		}

		if (str[index] == ';') {
			puts("\nVerwenden Sie keine \';\' Karakter, probieren Sie noch einmal!");
			return 0;
		}
		
		index++;
	}
	return 1;
}

void spieler_reg() {
	FILE* fp = NULL;
	char name[30];
	char passwort[30];

	char nm_from_file[30];
	int gueltig = 1;

	fp = fopen(DB_FILE, "r");
	if (fp == NULL) {
		fp = fopen(DB_FILE, "w");
	}
	fclose(fp);

	do {
		fp = fopen(DB_FILE, "r");
		if (fp == NULL) {
			printf("Problem mit %s", DB_FILE);
			exit(-1);
		}

		printf("Geben Sie ihren Spielername an (max: 30char, ohne \';\'): ");
		scanf("%s", name);

		gueltig = is_str_valid(name);

		while (fscanf(fp, "%[^;];%*s\n", nm_from_file) == 1) { // ';'-t nem tartalmazó stringet olvas be, ';' utána részt figyelmen kívül hagyja és sortörésig olvas
			nm_from_file[29] = '\0'; // biztos hogy van benne lezáró nulla
			if (strcmp(nm_from_file, name) == 0) {
				puts("\nDieser Name ist bereits im Einsatz");
				gueltig = 0;
				break;
			}
		}
		fclose(fp);
	} while (!gueltig);



	do {
		printf("Geben Sie ihr Passwort an (max: 30char, ohne \';\'): ");
		scanf("%s", passwort);
	} while (!is_str_valid(passwort));

	fp = fopen(DB_FILE, "a");
	if (fp == NULL) {
		printf("Problem mit %s", DB_FILE);
		exit(-1);
	}

	fprintf(fp, "%s;%s;%d\n", name, passwort, KEIN_REKORD_NOCH);
	fclose(fp);

	return;
}

void spieler_login() {
	FILE* fp = NULL;
	char nm_from_file[30];
	char pw_from_file[30];

	char name[30];
	char passwort[30];
	int rekord = 0;

	int zueruck = 0;
	int index = 0;
	int gueltig = 0;

	do {
		fp = fopen(DB_FILE, "r");
		if (fp == NULL) {
			printf("Problem mit %s", DB_FILE);
			exit(-1);
		}

		printf("Geben Sie ihren Spielername an: ");
		scanf("%s", name);

		gueltig = is_str_valid(name);

		if (gueltig) {
			while (fscanf(fp, "%[^;];%[^;];%d\n", nm_from_file, pw_from_file, &rekord) == 3) {
				nm_from_file[29] = '\0';
				if (strcmp(name, nm_from_file) != 0) {
					gueltig = 0;
				}
				else {
					gueltig = 1;
					break;
				}
			}
		}

		if (!gueltig) {
			printf("Falscher Spielername, probieren Sie noch einmal (0) oder treten Sie zueruck? (1)");
			scanf("%d", &zueruck);

			if (zueruck) {
				return;
			}
		}

		fclose(fp);
	} while (!gueltig);

	do {
		gueltig = 0;
		index = 0;
		printf("Geben Sie ihre Passwort an: ");
		do {
			passwort[index] = _getch();
			if (passwort[index] == '\r' || passwort[index] == '\n') {
				break;
			}
			
			if (passwort[index] == '\b') {
				if (index > 0) {
					int i = 0;
					index--;

					system("cls");
					printf("Login\n\nGeben Sie ihren Spielername an: %s\n", name);
					printf("Geben Sie ihre Passwort an: ");
					for (i = 0; i < index; i++) {
						printf("*");
					}

					continue;
				}
			}

			printf("*");

			index++;
		} while (!gueltig);
		passwort[index] = '\0';

		gueltig = is_str_valid(passwort);

		if (gueltig) {
			pw_from_file[29] = '\0';
			if (strcmp(passwort, pw_from_file) != 0) {
				printf("\nFalsches Passwort, probieren Sie noch einmal (0) oder treten Sie zueruck? (1)");
				scanf("%d", &zueruck);

				if (zueruck) {
					return;
				}
				else {
					gueltig = 0;
				}
			}
		}
	} while (!gueltig);

	strcpy(sp.name, name);
	strcpy(sp.password, passwort);
	sp.rekord = rekord;
	logged = 1;

	return;
}

int spieler_rekordcmp(const void* p1, const void* p2) {
	const spieler* sp1 = (spieler*)p1;
	const spieler* sp2 = (spieler*)p2;

	return (sp1->rekord) - (sp2->rekord);
}

void spieler_print_ranks(spieler* splist, int lines) {
	int zueruck = 0;
	int index = 0;
	printf(" _____________________________________________\n");
	printf("|______________Name_____________|____Rekord___|\n");
	for (index = 0; index < lines; index++) {
		if (logged) {
			if (strcmp(splist[index].name, sp.name) == 0) {
				printf("| \x1B[32m%-30s\033[0m", splist[index].name);
				if (splist[index].rekord == KEIN_REKORD_NOCH) printf("| \x1B[32mkein rekord\033[0m |\n");
				else printf("| \x1B[32m%11d\033[0m |\n", splist[index].rekord);
				continue;
			}
		}
		printf("| %-30s", splist[index].name);
		if (splist[index].rekord == KEIN_REKORD_NOCH) printf("| kein rekord |\n");
		else printf("| %11d |\n", splist[index].rekord);
	}
	printf("|_______________________________|_____________|");

	printf("\n\n9. Zueruck\n");


	do {
		scanf("%d", &zueruck);
	} while (zueruck != 9);

}

void spieler_ranks() {
	FILE* fp = NULL;
	spieler* splist = NULL;
	char name[30];
	char pw[30];
	int rekord;

	unsigned lines = 0;
	unsigned index = 0;
	char c = '\0';


	fp = fopen(DB_FILE, "r");
	if (fp == NULL) {
		return;
	}

	while ((c = getc(fp)) != EOF) {
		if (c == '\n')
			lines++;
	}
	fclose(fp);

	splist = (spieler*)malloc(lines * sizeof(spieler));
	if (splist == NULL) {
		puts("Problem mit malloc()");
		exit(-2);
	}

	fp = fopen(DB_FILE, "r");
	if (fp == NULL) {
		return;
	}

	while (fscanf(fp, "%[^;];%[^;];%d\n", name, pw, &rekord) == 3 && index < lines) {
		name[29] = '\0';
		pw[29] = '\0';

		strcpy(splist[index].name, name);
		strcpy(splist[index].password, pw);
		splist[index].rekord = rekord;

		index++;
	}

	qsort(splist, lines, sizeof(spieler), spieler_rekordcmp);

	spieler_print_ranks(splist, lines);

	free(splist);

	fclose(fp);
	return;
}