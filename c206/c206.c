	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/

	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/

	while(L->First != NULL) {

		tDLElemPtr temp = L->First; // pomocny prvek

		L->First = L->First->rptr;	// posunuti na dalsi prvek

		free(temp);		// uvolneni prvku	 	
	}

	L->Act = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

	tDLElemPtr newElem;

	if((newElem = malloc(sizeof(struct tDLElem))) != NULL) {

		newElem->data = val;		// zapsani hodnot do noveho prvku
		newElem->rptr = L->First;
		newElem->lptr = NULL;

		if(L->First == NULL) {	// pokud zadavame uplne prvni polozku v seznamu, musime upravit i ukazatel na polozku Last
			
			L->Last = newElem;
			L->First = newElem;	
		}
		else { 
			
			L->First->lptr = newElem;
			L->First = newElem;
		}
				// nastaveni prvku jako prvniho
	}	
	else 
		DLError();
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr newElem;

	if((newElem = malloc(sizeof(struct tDLElem))) != NULL) {

		newElem->data = val;		// zapsani hodnot do noveho prvku
		newElem->rptr = NULL;
		newElem->lptr = L->Last;

		if(L->First == NULL) {	// pokud zadavame uplne prvni polozku v seznamu, musime upravit i ukazatel na First
			
			L->First = newElem;
			L->Last = newElem;
		}
		else {
			
			L->Last->rptr = newElem;
			L->Last = newElem;			// nastaveni prvku jako prvniho
		}	
	}	
	else 
		DLError();
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL || L->Last == NULL) 
		DLError();
	else {

		*val = L->First->data;
	}
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL || L->Last == NULL) 
		DLError();
	else {

		*val = L->Last->data;
	}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	
	if(L->First != NULL) {		// jestli je seznam prazdny, nema smysl pokracovat

		tDLElemPtr temp = L->First;		//pomocny prvek

		if(L->First == L->Act)			// kontrola aktivity prvku
			L->Act = NULL;

		if(L->First == L->Last) {		// kontrola, jestli neni v seznamu jen 1 prvek, pripadne vynulovani ukazatelu

			L->First = NULL;
			L->Last = NULL;
		}
		else {							// spojeni seznamu

			L->First = L->First->rptr;
			L->First->lptr = NULL;
		}

		free(temp);		// uvolneni prvku
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
	
	if(L->First != NULL) { 		// jestli je seznam prazdny, nema smysl pokracovat

		tDLElemPtr temp = L->Last;		// pomocny prvek

		if(L->Last == L->Act)			// kontrola aktivity prvku
			L->Act = NULL;

		if(L->Last == L->First) {		// kontrola, jestli neni v seznamu jen 1 prvek, pripadne vynulovani ukazatelu

			L->Last = NULL;
			L->First = NULL;
		}
		else {							// spojeni seznamu
			
			L->Last = L->Last->lptr;
			L->Last->rptr = NULL;
		}

		free(temp);		// uvolneni prvku
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
	
	if(L->Act != NULL && L->Act != L->Last) {

		tDLElemPtr temp = L->Act->rptr;

		L->Act->rptr = temp->rptr;

		if(temp == L->Last)		// pokud, za aktivnim prvkem je posledni prvek, tk se z aktivniho prvku stane posledni
			L->Last = L->Act;
		else					// zaruceni navaznosti, pokud neni poslednim prvkem
			temp->rptr->lptr = L->Act;

		free(temp);		// zruseni prvku
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/

	if(L->Act != NULL && L->Act != L->First) {

		tDLElemPtr temp = L->Act->lptr;  // pomocny prvek, ukazatal na prvek, ktery chceme zrusit

		if(temp == L->First) {	// kontrola zda, pred aktivni prvkem neni prvni prvek

			L->First = L->Act;
		}
		else {					// svazani seznamu

			temp->lptr->rptr = L->Act;
		}

		L->Act->lptr = temp->lptr;	//svazani seznamu

		free(temp);	// uvolneni pozadovaneho prvku
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act != NULL) {

		tDLElemPtr newElem;

		if((newElem = malloc(sizeof(struct tDLElem))) == NULL)
			DLError();
		
		else {

			newElem->data = val;				// nacteni dat do noveho prvku a svazani se seznamem
			newElem->rptr = L->Act->rptr;
			L->Act->rptr = newElem;
			newElem->lptr = L->Act;

			if(L->Act == L->Last)				// pokud aktivni prvek byl zaroven i poslednim prvkem, nyni bude novy prvek poslednim prvkem
				L->Last = newElem;

			else								// opacny pripad
				newElem->rptr->lptr = newElem;
		}
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act != NULL) {

		tDLElemPtr newElem;

		if((newElem = malloc(sizeof(struct tDLElem))) == NULL)
			DLError();
		else {

			newElem->data = val;				// nacteni dat a svazani seznamu s novym prvkem
			newElem->lptr = L->Act->lptr;			
			newElem->rptr = L->Act;
			L->Act->lptr = newElem;	

			if(L->Act == L->First) {			// pokud byl aktivni prvek zaroven i prvnim, nyni bude novy prvek prvnim
				
				L->First = newElem;
				newElem->lptr = NULL;
			}
			else								// opacny pripad
				newElem->lptr->rptr = newElem;
		}
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

	if(L->Act != NULL)
		*val = L->Act->data;
	else
		DLError();
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/

	if(L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/

	if(L->Act != NULL) {

		L->Act = L->Act->rptr;
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL) {		// kontrola, zda je seznam aktivni

		L->Act = L->Act->lptr;		// posunuti aktivity na predchozi prvek, v pripace Act = First budeme mit po prirazeni Act = NULL
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/

	return L->Act != NULL ? TRUE : FALSE;
}

/* Konec c206.c*/
