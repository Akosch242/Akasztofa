#ifndef _VKLISTE_H_
#define _VKLISTE_H_

typedef struct vkliste {
	char* str;
	struct vkliste* next;
} vkliste;

void vkliste_add(vkliste** head, char* str);

int vkliste_anzahl(vkliste* head);

void vkliste_del(vkliste**, char);

void vkliste_free(vkliste* head);

#endif