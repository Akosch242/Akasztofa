#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct vkliste {
	char* str;
	struct vkliste* next;
} vkliste;

void vkliste_add(vkliste** head, char* str) {
	vkliste* iter = NULL;
	vkliste* neu = (vkliste*)malloc(sizeof(vkliste));
	if (neu == NULL) {
		puts("malloc error");
		exit(-1);
	}

	neu->str = (char*)malloc((strlen(str)+1)*sizeof(char));
	if (neu->str == NULL) {
		puts("malloc error");
		exit(-1);
	}
	strcpy(neu->str, str);
	neu->next = NULL;

	if (*head == NULL) {
		*head = neu;
		return;
	}

	for (iter = *head; iter->next != NULL; iter = iter->next);

	iter->next = neu;
	return;
}

int vkliste_anzahl(vkliste* head) {
	int anzahl = 0;
	vkliste* iter = head;

	for (iter = head; iter->next != NULL; iter = iter->next) {
		anzahl++;
	}

	return anzahl;
}

int contains_char(char* str, char c) {
	int index = 0;

	if (!isalpha(c)) {
		return 0;
	}

	while (str[index] != '\0') {
		if (tolower(str[index]) == tolower(c))
			return 1;

		index++;
	}

	return 0;
}

void vkliste_del(vkliste** head, char c) {
	vkliste* temp = *head;
	vkliste* prev = NULL;

	while (temp != NULL && contains_char(temp->str, c)) {
		*head = temp->next;
		free(temp->str);
		free(temp);
		temp = *head;
	}

	while (temp != NULL) {

		while (temp != NULL && !contains_char(temp->str, c)) {
			prev = temp;
			temp = temp->next;
		}

		if (temp == NULL) {
			return;
		}

		prev->next = temp->next;

		free(temp->str);
		free(temp);

		temp = prev->next;
	}
}

void vkliste_free(vkliste* head) {
	vkliste* next = head;

	while (head != NULL) {
		next = head->next;
		free(head->str);
		free(head);
		head = next;
	}
}