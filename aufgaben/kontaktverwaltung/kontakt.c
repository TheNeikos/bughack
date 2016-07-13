#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kontakt.h"
#include "menu.h"
#include "functions.h"
#include "readStr.h"
#include "replaceStr.h"
#include "dbLaden.h"
#include "dbSpeichern.h"
#include "main.h"

Adresse* AdresseHinzufuegen(Adresse* pCurrent, Adresse* pPrev, AdressenTyp pTyp, char* pStrasse, char* pHausnummer, char* pPlz, char* pStadt, char* pLand) {
	pCurrent = malloc(sizeof(Adresse));
	pCurrent->next = NULL;

	if (pPrev != NULL) {
		pCurrent->prev = pPrev;
		pCurrent->prev->next = pCurrent;
		pCurrent->id = pCurrent->prev->id +1;
	} else {
		pCurrent->prev = NULL;
		pCurrent->id = 1;
	}

	pCurrent->typ = pTyp;

	pCurrent->strasse = pStrasse;
	pCurrent->hausnummer = pHausnummer;
	pCurrent->plz = pPlz;
	pCurrent->stadt = pStadt;
	pCurrent->land = pLand;

    return pCurrent;
}

void AdresseAnzeigen(Adresse* pCurrent) {
	if(pCurrent != NULL) {
		if (pCurrent->typ == privateAdresse) {
			printf("%d. Adresse (privat): %s %s, %s %s, %s\n",pCurrent->id, pCurrent->strasse, pCurrent->hausnummer, pCurrent->plz, pCurrent->stadt, pCurrent->land);
		} else if (pCurrent->typ == geschaeftlicheAdresse) {
			printf("%d. Adresse (geschaeftlich): %s %s, %s %s, %s\n",pCurrent->id, pCurrent->strasse, pCurrent->hausnummer, pCurrent->plz, pCurrent->stadt, pCurrent->land);
		}

		fflush(stdout);
	}
	return;
}

void AlleAdressenAnzeigen(Adresse* pCurrent) {
	/* Wenn nicht Anfang der Liste, springe an Anfang der Liste
	 * Gebe dann alle Eintr�ge aus
	 * Andernfalls gebe an, dass keine Eintr�ge vorhanden sind */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			AdresseAnzeigen(pCurrent);
			pCurrent = pCurrent->next;
		}
	} else {
		printf("Es wurde keine Adresse gefunden\n");
		fflush(stdout);
	}

	return;
}

Adresse* AdresseLoeschen(Adresse* pCurrent) {
	Adresse* buffer1 = pCurrent;
	Adresse* buffer2 = pCurrent;

	/*
	 * Wenn Nachfolger vorhanden
	 *  - Wenn Nachfolger vorhanden, zeigt die zur�ckgegebene Liste auf den Nachfolger des zu l�schenden Eintrags
	 *  - Nachfolgende Eintr�ge passen Ihre ID um 1 verringert an
	 *  - Fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt der Nachfolger auf den Vorg�nger und umgehert
	 *		- Wenn nein, dann zeigt der Nachfolger auf kein vorheriges Element (da er dann zum ersten Element der Liste wird)
	 *  - Wenn Nachfolger nicht vorhanden, dann fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt die Liste auf den Vorg�nger und der Vorg�nger hat keinen Nachfolger
	 *		- Wenn nein, dann ist die Liste nach L�schen dieses Elements leer (siehe buffer1 = NULL)
	 */
	if(pCurrent->next != NULL) {
		buffer1 = buffer1->next;

		while(buffer2->next != NULL) {
			buffer2->next->id = buffer2->next->id -1;
			buffer2 = buffer2->next;
		}

		if(pCurrent->prev != NULL) {
			pCurrent->next->prev = pCurrent->prev;
			pCurrent->prev->next = pCurrent->next;
		} else {
			pCurrent->next->prev = NULL;
		}
	} else {
		if(pCurrent->prev != NULL) {
			buffer1 = buffer1->prev;
			pCurrent->prev->next = NULL;
		} else {
			buffer1 = NULL;
		}
	}

    free(pCurrent);
    return buffer1;
}

Adresse* AlleAdressenLoeschen(Adresse* pCurrent) {
	/*
	 * �berpr�ft, ob Kontakte �berhaupt vorhanden sind
	 * 	- Wenn ja, dann springe an den Anfang der Liste
	 * 	- L�sche dann diesen Eintrag und speichere den Nachfolger in pCurrent
	 * 	- Wiederhole dies, bis pCurrent = Null ist (alle Kontakte geloescht)
	 *
	 */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = AdresseLoeschen(pCurrent);
		}
	} else {
		printf("Es wurden keine Adresse gefunden\n");
		fflush(stdout);
	}

	return NULL;
}

Mail* MailAdresseHinzufuegen(Mail* pCurrent, Mail* pPrev, char* pMail, MailAdressenTyp pTyp) {
	pCurrent = malloc(sizeof(Mail));
	pCurrent->next = NULL;

	if (pPrev != NULL) {
		pCurrent->prev = pPrev;
		pCurrent->prev->next = pCurrent;
		pCurrent->id = pCurrent->prev->id +1;
	} else {
		pCurrent->prev = NULL;
		pCurrent->id = 1;
	}

	pCurrent->typ = pTyp;
	pCurrent->mailadresse = pMail;

    return pCurrent;
}

void MailAdresseAnzeigen(Mail* pCurrent) {
	if(pCurrent != NULL) {
		if(pCurrent->typ == privateMail) {
			printf("%d. Mail (privat): %s\n",pCurrent->id, pCurrent->mailadresse);
		} else if (pCurrent->typ == geschaeftlicheMail){
			printf("%d. Mail (geschaeftlich): %s\n",pCurrent->id, pCurrent->mailadresse);
		}

		fflush(stdout);
	}
	return;
}

void AlleMailAdressenAnzeigen(Mail* pCurrent) {
	/* Wenn nicht Anfang der Liste, springe an Anfang der Liste
	 * Gebe dann alle Eintr�ge aus
	 * Andernfalls gebe an, dass keine Eintr�ge vorhanden sind */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteMailAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			MailAdresseAnzeigen(pCurrent);
			pCurrent = pCurrent->next;
		}
	} else {
		printf("Es wurden keine Mail-Adressen gefunden\n");
		fflush(stdout);
	}

	return;
}

Mail* MailAdresseLoeschen(Mail* pCurrent) {
	Mail* buffer1 = pCurrent;
	Mail* buffer2 = pCurrent;

	/*
	 * Wenn Nachfolger vorhanden
	 *  - Wenn Nachfolger vorhanden, zeigt die zur�ckgegebene Liste auf den Nachfolger des zu l�schenden Eintrags
	 *  - Nachfolgende Eintr�ge passen Ihre ID um 1 verringert an
	 *  - Fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt der Nachfolger auf den Vorg�nger und umgehert
	 *		- Wenn nein, dann zeigt der Nachfolger auf kein vorheriges Element (da er dann zum ersten Element der Liste wird)
	 *  - Wenn Nachfolger nicht vorhanden, dann fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt die Liste auf den Vorg�nger und der Vorg�nger hat keinen Nachfolger
	 *		- Wenn nein, dann ist die Liste nach L�schen dieses Elements leer (siehe buffer1 = NULL)
	 */
	if(pCurrent->next != NULL) {
		buffer1 = buffer1->next;

		while(buffer2->next != NULL) {
			buffer2->next->id = buffer2->next->id -1;
			buffer2 = buffer2->next;
		}

		if(pCurrent->prev != NULL) {
			pCurrent->next->prev = pCurrent->prev;
			pCurrent->prev->next = pCurrent->next;
		} else {
			pCurrent->next->prev = NULL;
		}
	} else {
		if(pCurrent->prev != NULL) {
			buffer1 = buffer1->prev;
			pCurrent->prev->next = NULL;
		} else {
			buffer1 = NULL;
		}
	}

    free(pCurrent);
    return buffer1;
}

Mail* AlleMailAdressenLoeschen(Mail* pCurrent) {
	/*
	 * �berpr�ft, ob Kontakte �berhaupt vorhanden sind
	 * 	- Wenn ja, dann springe an den Anfang der Liste
	 * 	- L�sche dann diesen Eintrag und speichere den Nachfolger in pCurrent
	 * 	- Wiederhole dies, bis pCurrent = Null ist (alle Kontakte geloescht)
	 *
	 */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteMailAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = MailAdresseLoeschen(pCurrent);
		}
	} else {
		printf("Es wurden keine Mail-Adresse gefunden\n");
		fflush(stdout);
	}

	return NULL;
}

Telefon* TelefonnummerHinzufuegen(Telefon* pCurrent, Telefon* pPrev, char* pTelefonnummer, TelefonNummerTyp pTyp) {
	pCurrent = malloc(sizeof(Telefon));
	pCurrent->next = NULL;

	if (pPrev != NULL) {
		pCurrent->prev = pPrev;
		pCurrent->prev->next = pCurrent;
		pCurrent->id = pCurrent->prev->id +1;
	} else {
		pCurrent->prev = NULL;
		pCurrent->id = 1;
	}

	pCurrent->typ = pTyp;
	pCurrent->telefonnummer = pTelefonnummer;

    return pCurrent;
}

void TelefonnummerAnzeigen(Telefon* pCurrent) {
	if(pCurrent != NULL) {
		if(pCurrent->typ == festnetzPrivat) {
			printf("%d. Festnetz (Privat): %s\n",pCurrent->id, pCurrent->telefonnummer);
		} else if (pCurrent->typ == festnetzGeschaeftlich) {
			printf("%d. Festnetz (Geschaeftlich): %s\n",pCurrent->id, pCurrent->telefonnummer);
		} else if (pCurrent->typ == mobilfunkPrivat) {
			printf("%d. Mobilfunk (Privat): %s\n",pCurrent->id, pCurrent->telefonnummer);
		} else if (pCurrent->typ == mobilfunkGeschaeftlich) {
			printf("%d. Mobilfunk (Geschaeftlich): %s\n",pCurrent->id, pCurrent->telefonnummer);
		} else if (pCurrent->typ == faxPrivat) {
			printf("%d. Fax (privat): %s\n",pCurrent->id, pCurrent->telefonnummer);
		} else if (pCurrent->typ == faxGeschaeftlich) {
			printf("%d. Fax (Geschaeftlich): %s\n",pCurrent->id, pCurrent->telefonnummer);
		}

		fflush(stdout);
	}
	return;
}

void AlleTelefonnummernAnzeigen(Telefon* pCurrent) {
	/* Wenn nicht Anfang der Liste, springe an Anfang der Liste
	 * Gebe dann alle Eintr�ge aus
	 * Andernfalls gebe an, dass keine Eintr�ge vorhanden sind */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteTelefonnummer(pCurrent);
		}

		while(pCurrent != NULL) {
			TelefonnummerAnzeigen(pCurrent);
			pCurrent = pCurrent->next;
		}
	} else {
		printf("Es wurde keine Telefon-/Faxnummer gefunden\n");
		fflush(stdout);
	}

	return;
}

Telefon* TelefonnummerLoeschen(Telefon* pCurrent) {
	Telefon* buffer1 = pCurrent;
	Telefon* buffer2 = pCurrent;

	/*
	 * Wenn Nachfolger vorhanden
	 *  - Wenn Nachfolger vorhanden, zeigt die zur�ckgegebene Liste auf den Nachfolger des zu l�schenden Eintrags
	 *  - Nachfolgende Eintr�ge passen Ihre ID um 1 verringert an
	 *  - Fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt der Nachfolger auf den Vorg�nger und umgehert
	 *		- Wenn nein, dann zeigt der Nachfolger auf kein vorheriges Element (da er dann zum ersten Element der Liste wird)
	 *  - Wenn Nachfolger nicht vorhanden, dann fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt die Liste auf den Vorg�nger und der Vorg�nger hat keinen Nachfolger
	 *		- Wenn nein, dann ist die Liste nach L�schen dieses Elements leer (siehe buffer1 = NULL)
	 */
	if(pCurrent->next != NULL) {
		buffer1 = buffer1->next;

		while(buffer2->next != NULL) {
			buffer2->next->id = buffer2->next->id -1;
			buffer2 = buffer2->next;
		}

		if(pCurrent->prev != NULL) {
			pCurrent->next->prev = pCurrent->prev;
			pCurrent->prev->next = pCurrent->next;
		} else {
			pCurrent->next->prev = NULL;
		}
	} else {
		if(pCurrent->prev != NULL) {
			buffer1 = buffer1->prev;
			pCurrent->prev->next = NULL;
		} else {
			buffer1 = NULL;
		}
	}

    free(pCurrent);
    return buffer1;
}

Telefon* AlleTelefonnummernLoeschen(Telefon* pCurrent) {
	/*
	 * �berpr�ft, ob Kontakte �berhaupt vorhanden sind
	 * 	- Wenn ja, dann springe an den Anfang der Liste
	 * 	- L�sche dann diesen Eintrag und speichere den Nachfolger in pCurrent
	 * 	- Wiederhole dies, bis pCurrent = Null ist (alle Kontakte geloescht)
	 *
	 */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteTelefonnummer(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = TelefonnummerLoeschen(pCurrent);
		}
	} else {
		printf("Es wurden keine Telefon-/Faxnummern gefunden\n");
		fflush(stdout);
	}

	return NULL;
}

Kontakt* KontaktHinzufuegen(Kontakt* pCurrent, Kontakt* pPrev, char* pVorname, char* pNachname) {
	pCurrent = malloc(sizeof(Kontakt)); 	//Weisst einen Speicherplatz zu
	pCurrent->next = NULL;					//Zeigt standardmässig auf NULL, Ende der Liste

	pCurrent->adressen = NULL;
	pCurrent->mailadressen = NULL;
	pCurrent->telefonnummern = NULL;

	if (pPrev != NULL) {
		pCurrent->prev = pPrev;
		pCurrent->prev->next = pCurrent;
		pCurrent->id = pCurrent->prev->id +1;
	} else {
		pCurrent->prev = NULL;
		pCurrent->id = 1;
	}

	pCurrent->vorname = pVorname;
	pCurrent->nachname = pNachname;

    return pCurrent;
}

void KontaktAnzeigen(Kontakt* pCurrent) {
	if(pCurrent != NULL) {
		/*
		 * Gibt Vorname und Nachname aus
		 */
		printf("\n%d. Kontakt:\nVorname: %s Nachname: %s\n", pCurrent->id, pCurrent->vorname, pCurrent->nachname);
		fflush(stdout);

		/*
		 * Gibt Liste mit Adressen, Mail-Adressen und Telefonnummern aus
		 */
		AlleAdressenAnzeigen(pCurrent->adressen);
		AlleMailAdressenAnzeigen(pCurrent->mailadressen);
		AlleTelefonnummernAnzeigen(pCurrent->telefonnummern);
	} else {
		printf("\n Kontakt nicht gefunden");
		fflush(stdout);
	}

	return;
}

void AlleKontakteAnzeigen(Kontakt* pCurrent) {
	/* Wenn nicht Anfang der Liste, springe an Anfang der Liste
	 * Gebe dann alle Eintr�ge aus
	 * Andernfalls gebe an, dass keine Eintr�ge vorhanden sind */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersterKontakt(pCurrent);
		}

		while(pCurrent != NULL) {
			KontaktAnzeigen(pCurrent);
			pCurrent = pCurrent->next;
		}
	} else {
		printf("Es wurden keine Kontakte gefunden\n");
		fflush(stdout);
	}

	return;
}

Kontakt* KontaktLoeschen(Kontakt* pCurrent) {
	Kontakt* buffer1 = pCurrent;
	Kontakt* buffer2 = pCurrent;

	/*
	 * Wenn Nachfolger vorhanden
	 *  - Wenn Nachfolger vorhanden, zeigt die zur�ckgegebene Liste auf den Nachfolger des zu l�schenden Eintrags
	 *  - Nachfolgende Eintr�ge passen Ihre ID um 1 verringert an
	 *  - Fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt der Nachfolger auf den Vorg�nger und umgehert
	 *		- Wenn nein, dann zeigt der Nachfolger auf kein vorheriges Element (da er dann zum ersten Element der Liste wird)
	 *  - Wenn Nachfolger nicht vorhanden, dann fragen ob Vorg�nger vorhanden
	 *		- Wenn ja, dann zeigt die Liste auf den Vorg�nger und der Vorg�nger hat keinen Nachfolger
	 *		- Wenn nein, dann ist die Liste nach L�schen dieses Elements leer (siehe buffer1 = NULL)
	 */
	if(pCurrent->next != NULL) {
		buffer1 = buffer1->next;

		while(buffer2->next != NULL) {
			buffer2->next->id = buffer2->next->id -1;
			buffer2 = buffer2->next;
		}

		if(pCurrent->prev != NULL) {
			pCurrent->next->prev = pCurrent->prev;
			pCurrent->prev->next = pCurrent->next;
		} else {
			pCurrent->next->prev = NULL;
		}
	} else {
		if(pCurrent->prev != NULL) {
			buffer1 = buffer1->prev;
			pCurrent->prev->next = NULL;
		} else {
			buffer1 = NULL;
		}
	}

	/*
	 * Leert zuerst alle Listen und dann den Kontakt (sicheres L�schen)
	 */
	pCurrent->adressen = AlleAdressenLoeschen(pCurrent->adressen);
	pCurrent->mailadressen = AlleMailAdressenLoeschen(pCurrent->mailadressen);
	pCurrent->telefonnummern = AlleTelefonnummernLoeschen(pCurrent->telefonnummern);
    free(pCurrent);
    return buffer1;
}

Kontakt* AlleKontakteLoeschen(Kontakt* pCurrent) {
	/*
	 * �berpr�ft, ob Kontakte �berhaupt vorhanden sind
	 * 	- Wenn ja, dann springe an den Anfang der Liste
	 * 	- L�sche dann diesen Eintrag und speichere den Nachfolger in pCurrent
	 * 	- Wiederhole dies, bis pCurrent = Null ist (alle Kontakte geloescht)
	 *
	 */
	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersterKontakt(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = KontaktLoeschen(pCurrent);
		}
	}

	return pCurrent;
}

Kontakt* findeKontaktDurchNamen(Kontakt* pCurrent, char* pVorname, char* pNachname) {
	while(pCurrent != NULL) {
		if((strcmp(pCurrent->vorname,pVorname) == 0) && (strcmp(pCurrent->nachname,pNachname) == 0)) {
			return pCurrent;
		} else {
			pCurrent = pCurrent->next;
		}
	}

	return NULL;
}

Kontakt* findeKontaktDurchID(Kontakt* pCurrent, int pID) {
	while(pCurrent != NULL) {
		if(pCurrent->id == pID) {
			return pCurrent;
		} else {
			pCurrent = pCurrent->next;
		}
	}

	return NULL;
}

Adresse* findeAdresseDurchID(Adresse* pCurrent, int pID) {
	while(pCurrent != NULL) {
		if(pCurrent->id == pID) {
			return pCurrent;
		} else {
			pCurrent = pCurrent->next;
		}
	}

	return NULL;
}

Mail* findeMailDurchID(Mail* pCurrent, int pID) {
	while(pCurrent != NULL) {
		if(pCurrent->id == pID) {
			return pCurrent;
		} else {
			pCurrent = pCurrent->next;
		}
	}

	return NULL;
}

Telefon* findeTelefonDurchID(Telefon* pCurrent, int pID) {
	while(pCurrent != NULL) {
		if(pCurrent->id == pID) {
			return pCurrent;
		} else {
			pCurrent = pCurrent->next;
		}
	}

	return NULL;
}

Adresse* ersteAdresse(Adresse* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->prev != NULL) {
			pCurrent = pCurrent->prev;
		}
	}

	return pCurrent;
}

Adresse* letzteAdresse(Adresse* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->next != NULL) {
			pCurrent = pCurrent->next;
		}
	}

	return pCurrent;
}

int anzahlAdressen(Adresse* pCurrent) {
	/*
	 * Z�hlvariable auf 0 setzen
	 * Dann an die Anfang der liste Springen
	 * Bis ans Ende der Liste gehen und Z�hlervariable hochz�hlen
	 */
	int index = 0;

	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = pCurrent->next;
			index++;
		}
	}

	return index;
}

Mail* ersteMailAdresse(Mail* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->prev != NULL) {
			pCurrent = pCurrent->prev;
		}
	}

	return pCurrent;
}

Mail* letzteMailAdresse(Mail* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->next != NULL) {
			pCurrent = pCurrent->next;
		}
	}

	return pCurrent;
}

int anzahlMailAdressen(Mail* pCurrent) {
	/*
	 * Z�hlvariable auf 0 setzen
	 * Dann an die Anfang der liste Springen
	 * Bis ans Ende der Liste gehen und Z�hlervariable hochz�hlen
	 */
	int index = 0;

	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteMailAdresse(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = pCurrent->next;
			index++;
		}
	}

	return index;
}

Telefon* ersteTelefonnummer(Telefon* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->prev != NULL) {
			pCurrent = pCurrent->prev;
		}
	}

	return pCurrent;
}

Telefon* letzteTelefonnummer(Telefon* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->next != NULL) {
			pCurrent = pCurrent->next;
		}
	}

	return pCurrent;
}

int anzahlTelefonnummern(Telefon* pCurrent) {
	/*
	 * Z�hlvariable auf 0 setzen
	 * Dann an die Anfang der liste Springen
	 * Bis ans Ende der Liste gehen und Z�hlervariable hochz�hlen
	 */
	int index = 0;

	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersteTelefonnummer(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = pCurrent->next;
			index++;
		}
	}

	return index;
}

Kontakt* ersterKontakt(Kontakt* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->prev != NULL) {
			pCurrent = pCurrent->prev;
		}
	}

	return pCurrent;
}

Kontakt* letzterKontakt(Kontakt* pCurrent) {
	if(pCurrent != NULL) {
		while(pCurrent->next != NULL) {
			pCurrent = pCurrent->next;
		}
	}

	return pCurrent;
}

int anzahlKontakte(Kontakt* pCurrent) {
	/*
	 * Z�hlvariable auf 0 setzen
	 * Dann an die Anfang der liste Springen
	 * Bis ans Ende der Liste gehen und Z�hlervariable hochz�hlen
	 */
	int index = 0;

	if(pCurrent != NULL) {
		if(pCurrent->prev != NULL) {
			pCurrent = ersterKontakt(pCurrent);
		}

		while(pCurrent != NULL) {
			pCurrent = pCurrent->next;
			index++;
		}
	}

	return index;
}

