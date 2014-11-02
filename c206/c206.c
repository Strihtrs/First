	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/

	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
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
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

	tDLElemPtr newElem;

	if((newElem = malloc(sizeof(struct tDLElem))) != NULL) {

		newElem->data = val;		// zapsani hodnot do noveho prvku
		newElem->rptr = L->First;
		newElem->lptr = NULL;

		if(L->First == NULL)		// pokud zadavame uplne prvni polozku v seznamu, musime upravit i ukazatel na polozku Last
			L->Last = newElem;
		else 
			L->First->lptr = newElem;

		L->First = newElem;			// nastaveni prvku jako prvniho
	}	
	else 
		DLError();
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	
	
	tDLElemPtr newElem;

	if((newElem = malloc(sizeof(struct tDLElem))) != NULL) {

		newElem->data = val;		// zapsani hodnot do noveho prvku
		newElem->rptr = NULL;
		newElem->lptr = L->Last;

		if(L->First == NULL)		// pokud zadavame uplne prvni polozku v seznamu, musime upravit i ukazatel na First
			L->First = newElem;
		else
			L->Last->rptr = newElem;

		L->Last = newElem;			// nastaveni prvku jako prvniho
	}	
	else 
		DLError();
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/

	if(L->First == NULL || L->Last == NULL) 
		DLError();
	else {

		*val = L->First->data;
	}
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/

	if(L->First == NULL || L->Last == NULL) 
		DLError();
	else {

		*val = L->Last->data;
	}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
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
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
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
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/

	if(L->Act != NULL)
		*val = L->Act->data;
	else
		DLError();
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/

	if(L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/

	if(L->Act != NULL) {

		L->Act = L->Act->rptr;
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if(L->Act != NULL) {		// kontrola, zda je seznam aktivni

		L->Act = L->Act->lptr;		// posunuti aktivity na predchozi prvek, v pripace Act = First budeme mit po prirazeni Act = NULL
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/

	return L->Act != NULL ? TRUE : FALSE;
}

/* Konec c206.c*/
