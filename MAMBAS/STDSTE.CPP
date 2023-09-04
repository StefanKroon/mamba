#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>

#define DELETE 83
#define BACKSPACE '\b'
#define ENTER '\r'
#define ESC 27
#define INSERT 82
#define PIJLOMHOOG 72
#define PIJLOMLAAG 80

#define AFSLUITTEKST "Vragen of opmerkingen zijn altijd welkom op E-mail adres S.Kroon@st.hhs.nl"
#define DELAYTIJD 30
#define INVOERBREEDTE 10

void SluitGrafischScherm(void)
{	closegraph();
}

void Einde(int eindcode, char *AfsluitTekst, ...)
{    // Deze functie moet gezien worden als een uitbreiding van de 'exit'-
	// functie.
	//	Opm. Deze functie voegt de AfsluitTekst samen met argumenten.
	// Benodigde bibliotheken: stdio.h conio.h stdarg.h stdlib.h

	// Declaratie
	va_list ArgumentenPointer;

	#ifdef __GRAPHICS_H
	SluitGrafischScherm();
	#endif

	// Stel Standaardopmaak van DOS in
	_setcursortype(_NORMALCURSOR);
	textcolor(LIGHTGRAY); textbackground(BLACK);

	// Maak het scherm schoon
	clrscr();

	// Druk de AfsluitTekst af
	va_start(ArgumentenPointer, AfsluitTekst);
	vprintf(AfsluitTekst, ArgumentenPointer);
	va_end(ArgumentenPointer);

	// Ga naar een nieuwe regel
	printf("\n");
	exit(eindcode);
}
void OpenGrafischScherm(void)
{	int gdriver = VGA, gmode = VGAHI, errorcode;
	initgraph(&gdriver, &gmode, "d:\\tc\\bgi");
	errorcode = graphresult();
	if (errorcode != grOk)
	{	Einde(8, "Grafische fout: %s\n", grapherrormsg(errorcode));
	}
	setcolor(getmaxcolor());
}
void centreer(char *Invoerstring, ...)
{    // Deze functie centreert af te drukken tekst op een regel.
	// 	Opm1. De af te drukken tekst wordt WEL samengevoegd met de argumenten.
	// 	Opm2. De functie is niet berekend op strings langer dan 80 tekens.

	// Hier wordt de af te drukken tekst samengevoegd met de argumenten (inclusief foutcontrole)
	char Uitvoerstring[100];
	va_list argp; va_start(argp, Invoerstring);
	if (vsprintf(Uitvoerstring, Invoerstring, argp) < 0)
		Einde(1, "Fout bij het samenvoegen van de string en zijn argumenten in 'centreer'.");
	va_end(argp);
	// Foutcontrole op strings langer dan 80 tekens.
	if (strlen(Uitvoerstring) > 80)
		Einde(1, "De lengte van de Uitvoerstring is groter dan 80 karakters. Fout in 'centreer'.");

	gotoxy((int)(40 - (strlen(Uitvoerstring) /2)), wherey());
	cprintf(Uitvoerstring);
}

int kleurprintf(int achtergrond, int kleur, char *formatstring, ...)
{    va_list argp;
	char str[100];
	textcolor(kleur);
	textbackground(achtergrond);
	va_start(argp, formatstring);
	if (vsprintf(str, formatstring, argp)<0)
	{	printf("FOUT in PROCEDURE kleurprintf\n");
		printf("Onjuiste aanroep\n");
		return EOF;
	}
	va_end(argp);
	if (strlen(str)>99)
	{	printf("Fout in PROCEDURE kleurprintf\n");
		printf("Te lange string\n");
		return EOF;
	}
	return cprintf(str);
}

void EffectHaalRegelWeg(int VanafRegel, int AantalRegels)
{    // Deze functie leegt het scherm vanaf 'VanafRegel'.
	// Er wordt 'AantalRegels' keer een regel opgetrokken.
	// Voor een mooi visueel effect is er een wachttijd ingebouwd, deze is
	// in te stellen d.m.v. DELAYTIJD.
	gotoxy(1, VanafRegel); // De cursor wordt aan het begin van de opgegeven regel geplaatst.
	// Er wordt AantalRegels keer een regel gewist.
	for(int Teller = 0; Teller < AantalRegels; Teller++)
	{	delline();
		delay(DELAYTIJD);
	}
}

void EffectDrukRegelAf(int regel, char *Invoerstring, ...)
{	// Deze functie is het tegengestelde effect van EffectHaalRegelWeg.
	// De af te drukken string wordt ingevoegd op regel 'regel' en de
	// onderliggende regels worden een regel lager geplaatst.
	// Voor een mooi visueel effect is er een wachttijd ingebouwd, deze is
	// in te stellen d.m.v. DELAYTIJD.

	// Hier wordt de af te drukken tekst samengevoegd met de argumenten (inclusief foutcontrole)
	char Uitvoerstring[100];
	va_list argp; va_start(argp, Invoerstring);
	if (vsprintf(Uitvoerstring, Invoerstring, argp) < 0)
		Einde(1, "Fout bij het samenvoegen van de string en zijn argumenten in 'centreer'.");
	va_end(argp);
	// Foutcontrole op strings langer dan 80 tekens.
	if (strlen(Uitvoerstring) > 80)
		Einde(1, "De lengte van de Uitvoerstring is groter dan 80 karakters. Fout in 'centreer'.");

	gotoxy(1, regel); // De cursor wordt aan het begin van de opgegeven regel geplaatst.
	insline(); // Er wordt een lege regel ingevoegd.
	printf(Uitvoerstring); // De nieuwe lege regel wordt beschreven
	delay(DELAYTIJD); // Er wordt een DELAYTIJD gewacht.
}

void Invoer(char *Uitvoer ,char *StandaardTekst)
{    char karakter;
	int teller = 0;
	_setcursortype(_NORMALCURSOR);
	strcpy(Uitvoer, StandaardTekst);
	gotoxy(20, wherey());
	textcolor(BLACK); textbackground(GREEN);
	for (int t = 0; t < INVOERBREEDTE; t++) cprintf(" ");
	gotoxy(20, wherey());
	cprintf(Uitvoer);
	karakter = getch();
	if (karakter == '\n' || karakter == '\r')
	{	textcolor(WHITE); textbackground(BLUE);
		_setcursortype(_NOCURSOR);
		return;
	}

	strcpy(Uitvoer, "");
	gotoxy(20, wherey());
	for (t = 0; t < INVOERBREEDTE; t++) cprintf(" ");
	gotoxy(20, wherey());

	while(1)
	{    if (karakter >= ' ' && karakter <= 126 && teller != INVOERBREEDTE)
		{	cprintf("%c", karakter);
			Uitvoer[teller] = karakter;
			teller++;
			Uitvoer[teller] = '\x0';
		}
		switch(karakter)
		{	case '\r':
			case '\n':
				textcolor(WHITE); textbackground(BLUE);
				_setcursortype(_NOCURSOR);
				return;
			case '\b':
				if (teller > 0)
				{	teller--;
					gotoxy(wherex() - 1, wherey());
					cprintf(" ");
					gotoxy(wherex() -1, wherey());
					Uitvoer[teller] = '\x0';
				}
		}
		karakter = getch();
	}
}
