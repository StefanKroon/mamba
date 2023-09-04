// string.h  conio.h  stdio.h  stdlib.h  stdarg.h
#include <errno.h>
#include <process.h>
#include <dos.h>
#include <time.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <graphics.h>


#include "\tc\stefan\mambas\stdste.h"

#define LEEG_78 "                                                                              "

#define FALSE 0
#define TRUE 1

#define X_AS 1
#define Y_AS 2

#define AUTOSEARCH 1
#define GODMODE 2

#define BOVEN 1
#define LINKS 2
#define RECHTS 3
#define BENEDEN 4

#define MUUR -2
#define BLAUWVOEDSEL -3
#define GROENVOEDSEL -4
#define BONUS -5
#define SCHIJNMUUR -6

#define NAAM 1
#define LEVEL 2
#define SCORE 3

#define BEGINSLANG_LENGTE 6
#define MAXSLANG_LENGTE 36
// 179:≥¥µ∂∑∏π∫ªºΩæø¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹

// De kleuren zoals ze gedefinieerd zijn in <conio.h>:
/*
*/

int Richting, NRichting, TabelXY[40][22];
int PosXvanSlangdeel[MAXSLANG_LENGTE], PosYvanSlangdeel[MAXSLANG_LENGTE];
int SlangLengte, Level, Energie, Bonus, AantalVoedsel, BonusStap;
int BonusAfstand, BonusX, BonusY, AlVerstrekenTijd, EindeSpel, Sound = TRUE;
int CheatMode = FALSE, AantalKeerS = 0, GodMode = FALSE, Stap2, AutoSearch = FALSE;
int LoopNietDood = FALSE, CheatPlayer, ComputerDemo;
unsigned long Score;
char *screen;

void KleurNaarVGA(int *kleur)
{	switch(*kleur)
	{	case BLACK: *kleur = EGA_BLACK; break;
		case BLUE: *kleur = EGA_BLUE; break;
		case GREEN: *kleur = EGA_GREEN; break;
		case CYAN: *kleur = EGA_CYAN; break;
		case RED: *kleur = EGA_RED; break;
		case MAGENTA: *kleur = EGA_MAGENTA; break;
		case BROWN: *kleur = EGA_BROWN; break;
		case LIGHTGRAY: *kleur = EGA_LIGHTGRAY; break;
		case DARKGRAY: *kleur = EGA_DARKGRAY; break;
		case LIGHTBLUE: *kleur = EGA_LIGHTBLUE; break;
		case LIGHTGREEN: *kleur = EGA_LIGHTGREEN; break;
		case LIGHTCYAN: *kleur = EGA_LIGHTCYAN; break;
		case LIGHTRED: *kleur = EGA_LIGHTRED; break;
		case LIGHTMAGENTA: *kleur = EGA_LIGHTMAGENTA; break;
		case YELLOW: *kleur = EGA_YELLOW; break;
		case WHITE: *kleur = EGA_WHITE; break;
		default: Einde(3, "Fout in KleurInVGA."); break;
	}
}

int Gkleurprintf(int achtergrond, int kleur, char *formatstring, ...)
{    va_list argp;
	char str[100];
	int X_Begin = getx(), Y_Begin = gety();
	char TekenString[1];
	KleurNaarVGA(&kleur);
	setcolor(kleur);
	KleurNaarVGA(&achtergrond);
//	setbkcolor(achtergrond);
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
	settextstyle(1, HORIZ_DIR, 1);
	setusercharsize(6, 15, 1, 2);
	for (int Teller = 0; Teller < 100; Teller++)
	{    if (str[Teller] == '\x0') break;
		moveto(X_Begin + Teller * 8, Y_Begin);
		TekenString[0] = str[Teller];
		TekenString[1] = '\x0';
		outtext(TekenString);
	}
	return 0;
}

void Ggotoxy(int X, int Y)
{	moveto(X * 8, Y * 19);
}

void Geluid(int frequentie, int duur)
{	if (Sound == TRUE)
		sound(frequentie);
	delay(duur);
	nosound();
}

int WatIsErRichting(int ZoekRichting)
{    int WatErIs;
	switch (ZoekRichting)
	{    case (RECHTS):      if (PosXvanSlangdeel[1] + 1 > 38) WatErIs = MUUR;
						else WatErIs = TabelXY[PosXvanSlangdeel[1] + 1][PosYvanSlangdeel[1]];
						break;
		case (LINKS):		if (PosXvanSlangdeel[1] - 1 < 0) WatErIs = MUUR;
						else WatErIs = TabelXY[PosXvanSlangdeel[1] - 1][PosYvanSlangdeel[1]];
						break;
		case (BENEDEN):	if (PosYvanSlangdeel[1] + 1 > 21) WatErIs = MUUR;
						else WatErIs = TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] + 1];
						break;
		case (BOVEN):		if (PosYvanSlangdeel[1] - 1 < 0) WatErIs = MUUR;
						else WatErIs = TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] - 1];
						break;
		default:			Einde(1, "Fout in WatIsErRichting.\n");
	}
	if (WatErIs > 0) WatErIs = MUUR;
	return WatErIs;
}

//*************************************************************************
//************************* Alle SchermPrintFuncties **********************
//*************************************************************************
void Verwijder(int X_Weg, int Y_Weg)
{	TabelXY[X_Weg][Y_Weg] = 0;
	Ggotoxy((X_Weg * 2) + 2, Y_Weg + 3);
	Gkleurprintf(BLACK, BLACK, "  ");
}

void DrukAfLenS(void)
{    if (Score > 99999l) Score = 99999l;
	if (Level > 99) Level = 99;
	Ggotoxy(62, 1);Gkleurprintf(RED, YELLOW, "%02d", Level);
	Ggotoxy(75, 1);Gkleurprintf(RED, YELLOW, "%05ld", Score);
}

void DrukAfEenB(void)
{  Ggotoxy(16, 1);
   Gkleurprintf(BLUE, BLUE, "                ");
   Ggotoxy(40, 1);
   Gkleurprintf(BLUE, BLUE, "                ");
   for (int ETeller = 1; ETeller <= Energie; ETeller++)
   {  Ggotoxy(12 + (3 * ETeller), 1);
	 Gkleurprintf(BLUE, LIGHTGREEN, " €€");
   }
   for (int BTeller = 1; BTeller <= Bonus; BTeller++)
   {  Ggotoxy(36 + (3 * BTeller), 1);
	 Gkleurprintf(BLUE, YELLOW, " €€");
   }
}

void DrukAfVenster(int TopX, int TopY, int BottomX, int BottomY, int achtergrond, int voorgrond, int binnenkleur)
{    textcolor(voorgrond); textbackground(achtergrond);
	Ggotoxy(TopX, TopY);	cprintf("…");
	for (int t = 0; t < (BottomX - TopX - 1); t++) cprintf("Õ");
	cprintf("ª");
	for (t = TopY + 1; t < BottomY; t++)
	{    Ggotoxy(TopX, t);
		cprintf("∫");
		textbackground(binnenkleur);
		for (int T = 0; T < (BottomX - TopX - 1); T++) cprintf(" ");
		textcolor(voorgrond); textbackground(achtergrond);
		cprintf("∫");
	}
	Ggotoxy(TopX, BottomY); cprintf("»");
	for (t = 0; t < (BottomX - TopX- 1); t++) cprintf("Õ");
	if (BottomX == 80 && BottomY == 25)
	{  screen = (char *) MK_FP(0xb800, 0x0000);
	   screen[3998] = 'º';
	   screen[3999] = (achtergrond << 4) | voorgrond;
	}
	else cprintf("º");
}

void DrukAfGroenVoedsel(int GroeneX, int GroeneY)
{	int Kans, Extra = 1;
	TabelXY[GroeneX][GroeneY] = GROENVOEDSEL;
	Ggotoxy((GroeneX * 2) + 2, GroeneY + 3);
	Gkleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
	Kans = 10 - Level;
	if (Kans < 5) Kans = 5;
	if ((rand() % Kans) == 1)
	{	if ((rand() % 2) == 1)
		{	while (TabelXY[GroeneX + Extra][GroeneY] == MUUR)
			{	TabelXY[GroeneX + Extra][GroeneY] = GROENVOEDSEL;
				Ggotoxy(((GroeneX + Extra) * 2) + 2, GroeneY + 3);
				Gkleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
				Extra++;
			}
		}
		else
		{	while (TabelXY[GroeneX][GroeneY + Extra] == MUUR)
			{	TabelXY[GroeneX][GroeneY + Extra] = GROENVOEDSEL;
				Ggotoxy((GroeneX * 2) + 2, (GroeneY + Extra) + 3);
				Gkleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
				Extra++;
			}
		}
	}
}

void DrukAfBlauwVoedsel(void)
{	int GekozenX, GekozenY;
	do { GekozenX = rand() % 39; GekozenY = rand() % 22;}
	while (TabelXY[GekozenX][GekozenY] > 0 || TabelXY[GekozenX][GekozenY] == BLAUWVOEDSEL) ;
	if (TabelXY[GekozenX][GekozenY] == 0)
	{	TabelXY[GekozenX][GekozenY] = BLAUWVOEDSEL;
		Ggotoxy((GekozenX * 2) + 2, GekozenY + 3);
		Gkleurprintf(BLACK, CYAN, "@@");
	}
	if (TabelXY[GekozenX][GekozenY] == MUUR)
		DrukAfGroenVoedsel(GekozenX, GekozenY);
}

void DrukAfStaart(int AchtergrondKleur, int VoorgrondKleur)
{  Ggotoxy((PosXvanSlangdeel[SlangLengte] * 2) + 2, PosYvanSlangdeel[SlangLengte] + 3);
   if (PosXvanSlangdeel[SlangLengte - 1] == PosXvanSlangdeel[SlangLengte] + 1)
	 Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 17, 17);
   if (PosXvanSlangdeel[SlangLengte - 1] == PosXvanSlangdeel[SlangLengte] -1)
	 Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 16, 16);
   if (PosYvanSlangdeel[SlangLengte - 1] == PosYvanSlangdeel[SlangLengte] + 1)
	 Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 30, 30);
   if (PosYvanSlangdeel[SlangLengte - 1] == PosYvanSlangdeel[SlangLengte] - 1)
	 Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 31, 31);
}

void DrukAfSlang(int AchtergrondKleur, int VoorgrondKleur)
{  Ggotoxy((PosXvanSlangdeel[1] * 2) + 2, PosYvanSlangdeel[1] + 3);
   Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 1, 1);
   for (int Teller = 2; Teller <= (SlangLengte - 1); Teller++)
   {  Ggotoxy((PosXvanSlangdeel[Teller] * 2) + 2, PosYvanSlangdeel[Teller] + 3);
	 Gkleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 254, 254);
   }
   DrukAfStaart(AchtergrondKleur, VoorgrondKleur);
}

void DrukAfMamba(void)
{	Ggotoxy(1, 1);
	Gkleurprintf(RED, YELLOW, " MAMBA ");
	Ggotoxy(8, 1);
	Gkleurprintf(BLUE, WHITE, " ");
	Stap2 = 0;
}

void DrukAfLetter_M(int LetterPositie)
{	Ggotoxy(LetterPositie,  9); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 10); Gkleurprintf(BLACK, YELLOW, "˛˛      ˛˛");
	Ggotoxy(LetterPositie, 11); Gkleurprintf(BLACK, YELLOW, "˛ ˛    ˛ ˛");
	Ggotoxy(LetterPositie, 12); Gkleurprintf(BLACK, YELLOW, "˛  ˛  ˛  ˛");
	Ggotoxy(LetterPositie, 13); Gkleurprintf(BLACK, YELLOW, "˛   ˛˛   ˛");
	Ggotoxy(LetterPositie, 14); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 15); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 16); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 17); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 18); Gkleurprintf(BLACK, YELLOW, "%c        %c", 31, 1);
}
void DrukAfLetter_A(int LetterPositie)
{	Ggotoxy(LetterPositie,  9); Gkleurprintf(BLACK, YELLOW, "    ˛˛    ");
	Ggotoxy(LetterPositie, 10); Gkleurprintf(BLACK, YELLOW, "   ˛  ˛   ");
	Ggotoxy(LetterPositie, 11); Gkleurprintf(BLACK, YELLOW, "  ˛    ˛  ");
	Ggotoxy(LetterPositie, 12); Gkleurprintf(BLACK, YELLOW, " ˛      ˛ ");
	Ggotoxy(LetterPositie, 13); Gkleurprintf(BLACK, YELLOW, "˛˛˛˛˛˛˛˛˛˛");
	Ggotoxy(LetterPositie, 14); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 15); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 16); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 17); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 18); Gkleurprintf(BLACK, YELLOW, "%c        %c", 31, 1);
}
void DrukAfLetter_B(int LetterPositie)
{	Ggotoxy(LetterPositie,  9); Gkleurprintf(BLACK, YELLOW, "˛˛˛˛˛˛˛˛  ");
	Ggotoxy(LetterPositie, 10); Gkleurprintf(BLACK, YELLOW, "˛       ˛ ");
	Ggotoxy(LetterPositie, 11); Gkleurprintf(BLACK, YELLOW, "˛       ˛ ");
	Ggotoxy(LetterPositie, 12); Gkleurprintf(BLACK, YELLOW, "˛       ˛ ");
	Ggotoxy(LetterPositie, 13); Gkleurprintf(BLACK, YELLOW, "˛˛˛˛˛˛˛˛˛ ");
	Ggotoxy(LetterPositie, 14); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 15); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 16); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 17); Gkleurprintf(BLACK, YELLOW, "˛        ˛");
	Ggotoxy(LetterPositie, 18); Gkleurprintf(BLACK, YELLOW, "%c%c˛˛˛˛˛˛˛ ", 31, 1);
}

void DrukAfDemoBalk(int DemoStap)
{	char DemoTekst[50] = "µDit Is Een Demo... Druk 'D' Om Te Stoppen...∆Õ";
	int Xpositie;
	for (int T = 0; T < 2; T++)
	{	for (int t = 0; t <= 46; t++)
		{    if (DemoStap + t > 79) Xpositie = DemoStap + t - 78;
			else Xpositie = DemoStap + t;
			Ggotoxy(Xpositie, 25);
			Gkleurprintf(BLUE, WHITE, "%c", DemoTekst[t]);
		}
//		delay(50);
	}
	AlVerstrekenTijd = 100;
}
//************************ Einde SchermPrintFuncties **********************

//*************************************************************************
//************************* ScoreLijstBewerkingen *************************
//*************************************************************************
void VraagNaam(char *Naam)
{  char karakter;
   int positie = 0, Insert = FALSE, pTeller;
   strcpy(Naam, "            ");
   DrukAfVenster(28, 12, 50, 14, WHITE, BLACK, WHITE);
   Ggotoxy(30, 13);
   Gkleurprintf(WHITE, BLACK, "Name: ");
   Ggotoxy(36, 13);
   Gkleurprintf(LIGHTGREEN, BLACK, "            ");
   Ggotoxy(36, 13);
   _setcursortype(_NORMALCURSOR);
   while (bioskey(1)) bioskey(0); //Uitlezen van nog aanwezige karakters
   karakter = getch();
   while (karakter != '\r')
   {  karakter = toupper(karakter);
	 if (isprint(karakter))
	 {  if (positie < 12)
	    {  if (Insert == FALSE)
		  {  Ggotoxy(30 + positie, 13);
			Gkleurprintf(LIGHTGREEN, BLACK, "%c", karakter);
			Naam[positie] = karakter;
			positie++;
		  }
		  else
		  {  for (pTeller = strlen(Naam); pTeller > positie; pTeller--)
			Naam[pTeller] = Naam[pTeller - 1];
			Naam[12] = '\x0';
			Naam[positie] = karakter;
			positie++;
			Ggotoxy(36, 13);
			Gkleurprintf(LIGHTGREEN, BLACK, "%s", Naam);
			Ggotoxy(36 + positie, 13);
		  }
	    }
	 }
	 else
	 {  if (karakter == '\b' && positie > 0)
	    {  positie--;
		  pTeller = positie;
		  while (Naam[pTeller] != 0)
		  {  Naam[pTeller] = Naam[pTeller + 1];
			pTeller++;
		  }
		  Naam[11] = ' ';
		  Ggotoxy(36, 13);
		  Gkleurprintf(LIGHTGREEN, BLACK, "%s", Naam);
		  Ggotoxy(36 + positie, 13);
	    }
	    if (karakter == 0)
	    {  karakter = getch();
		  if (karakter == 82)
		  {  if (Insert == FALSE)
			{  Insert = TRUE;
			   _setcursortype(_SOLIDCURSOR);
			}
			else
			{  Insert = FALSE;
			   _setcursortype(_NORMALCURSOR);
			}
		  }
		  if (karakter == 83 && positie < 11)
		  {  pTeller = positie;
			while (Naam[pTeller] != 0)
			{  Naam[pTeller] = Naam[pTeller + 1];
			   pTeller++;
			}
			Naam[11] = ' ';
			Ggotoxy(36, 13);
			Gkleurprintf(LIGHTGREEN, BLACK, "%s", Naam);
			Ggotoxy(36 + positie, 13);
		  }
		  if (karakter == 71)
		  {  positie = 0;
			Ggotoxy(36, 13);
		  }
		  if (karakter == 79)
		  {  positie = 11;
			Ggotoxy(47, 13);
		  }
		  if (karakter == 75 && positie != 0)
		  {  positie--;
			Ggotoxy(36 + positie, 13);
		  }
		  if (karakter == 77 && positie != 11)
		  {  positie++;
			Ggotoxy(36 + positie, 13);
		  }
	    }
	 }
	 karakter = getch();
   }
   _setcursortype(_NOCURSOR);
}

char FileKarakter(FILE *FP, int Doel)
{	char karakter = getc(FP);
	if (karakter == EOF || karakter == '\n')
	{	ungetc(karakter, FP);
		switch (Doel)
		{	case NAAM:	karakter = ' '; break;
			case LEVEL:
			case SCORE:	karakter = -1; break;
			default:		Einde(1, "Fout in FileKarakter.\n");
		}
	}
	return karakter;
}

void DrukAfScorelijst(void)
{	int LijstLevel[26] = {{0}};
	int achtergrondkleur, voorgrondkleur, NaamGevraagd = FALSE, SpelersNummer = 0;
	long int LijstScore[26] = {0};
	char LijstNaam[26][14] = {{0}}, ch, bestand[15];
	FILE *fp;
	if (CheatPlayer == TRUE) strcpy(bestand, "mambac.chi");
	else
	{	if (ComputerDemo == TRUE) strcpy(bestand, "mambac.dhi");
		else strcpy(bestand, "mambac.chi");
	}
	if ((fp = fopen(bestand, "r+t")) == NULL) if ((fp = fopen(bestand, "w+t")) == NULL) Einde(1, "Fout bij aanmaken van bestand.\n");
	for (int NaamTeller = 1; NaamTeller <= 25; NaamTeller++)
	{	for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
			LijstNaam[NaamTeller][LetterTeller] = FileKarakter(fp, NAAM);
		for (int LevelTeller = 1; LevelTeller <= 2; LevelTeller++)
		{	int Karakter = FileKarakter(fp, LEVEL);
			switch (Karakter)
			{	case ' ':	if (LijstLevel[NaamTeller] > 0) LevelTeller = 10;
						else LevelTeller--;
						break;
				case -1:	LevelTeller = 10; break;
				default:	if (Karakter < '0' || Karakter > '9') Einde(1, "Fout in de scorelijst.\n");
						else LijstLevel[NaamTeller] = (LijstLevel[NaamTeller] * 10) + (Karakter - 48);
			}
		}
		for (int ScoreTeller = 1; ScoreTeller <= 5; ScoreTeller++)
		{	int Karakter = FileKarakter(fp, SCORE);
			switch (Karakter)
			{	case ' ':	if (LijstScore[NaamTeller] > 0) ScoreTeller = 10;
						else ScoreTeller--;
						break;
				case -1:	ScoreTeller = 10; break;
				default:	if (Karakter < '0' || Karakter > '9') Einde(1, "Fout in de scorelijst.\n");
						else LijstScore[NaamTeller] = (LijstScore[NaamTeller] * 10) + (Karakter - 48);
			}
		}
		if (LijstScore[NaamTeller] <= Score && NaamGevraagd == FALSE)
		{	NaamGevraagd = TRUE;
			strcpy(LijstNaam[NaamTeller + 1], LijstNaam[NaamTeller]);
			LijstLevel[NaamTeller + 1] = LijstLevel[NaamTeller];
			LijstScore[NaamTeller + 1] = LijstScore[NaamTeller];
			VraagNaam(LijstNaam[NaamTeller]);
			LijstLevel[NaamTeller] = Level;
			LijstScore[NaamTeller] = Score;
			SpelersNummer = NaamTeller;
			NaamTeller++;
		}
		while (FileKarakter(fp, SCORE) != -1);
		getc(fp);
	}
	rewind(fp);
	for (NaamTeller = 1; NaamTeller <= 25; NaamTeller++)
	{	for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
		putc(LijstNaam[NaamTeller][LetterTeller], fp);
		putc(' ', fp); putc(' ', fp); putc(' ', fp);
		fprintf(fp, "%2d", LijstLevel[NaamTeller]);
		putc(' ', fp); putc(' ', fp); putc(' ', fp);
		fprintf(fp, "%5ld", LijstScore[NaamTeller]);
		putc('\n', fp);
	}
	fclose(fp);
// VensterAfdruk
	DrukAfVenster(7, 5, 73, 22, CYAN, BLUE, CYAN);
	Ggotoxy(35, 5);
	if (CheatPlayer == TRUE) Gkleurprintf (RED, WHITE, "Top Cheaters");
	else
	{	if (ComputerDemo == TRUE) Gkleurprintf(RED, WHITE, " Best Demo ");
		else Gkleurprintf(RED, WHITE, " High Score ");
	}
	Ggotoxy(21, 22);
	Gkleurprintf(RED, WHITE, " <Esc> = quit      <Enter> = continue ");
/* EerstePlaatsAfdruk*/
	if (SpelersNummer == 1)
	{	achtergrondkleur = BLUE;
		voorgrondkleur = WHITE;
	}
	else
	{	achtergrondkleur = CYAN;
		voorgrondkleur = RED;
	}
	Ggotoxy(27, 6); Gkleurprintf(achtergrondkleur, voorgrondkleur, "1. ");
	for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
	{	Ggotoxy(30 + LetterTeller, 6);
		Gkleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[1][LetterTeller]);
	}
	Ggotoxy(42, 6);
	Gkleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[1], LijstScore[1]);
/* OverigePlaatsAfdruk*/
	for (int rteller = 2; rteller <= 13; rteller++)
	{	if (SpelersNummer == rteller)
		{	achtergrondkleur = BLUE;
			voorgrondkleur = WHITE;
		}
		else
		{	achtergrondkleur = CYAN;
			voorgrondkleur = BLUE;
		}
		Ggotoxy(9, rteller + 7);
		Gkleurprintf(achtergrondkleur, voorgrondkleur, "%2d. ", rteller);
		for (LetterTeller = 0; LetterTeller < 12; LetterTeller++)
		{    Ggotoxy(13 + LetterTeller, rteller + 7);
			Gkleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[rteller][LetterTeller]);
		}
		Ggotoxy(15, rteller + 7);
		Gkleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[rteller], LijstScore[rteller]);
		Gkleurprintf(CYAN, BLUE, " ≥ ");
		if (SpelersNummer == rteller + 12)
		{	achtergrondkleur = BLUE;
			voorgrondkleur = WHITE;
		}
		else
		{	achtergrondkleur = CYAN;
			voorgrondkleur = BLUE;
		}
		Gkleurprintf(achtergrondkleur, voorgrondkleur, "%2d. ", rteller + 12);
		for (LetterTeller = 0; LetterTeller < 12; LetterTeller++)
			Gkleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[rteller + 12][LetterTeller]);
		Gkleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[rteller + 12], LijstScore[rteller + 12]);
	}
// Wacht op een toets
	do
	{	ch = getch();
	}
	while (ch != 27 && ch != 13) ;
	if (ch == 27) Einde(0, "");
}
//************************ Einde ScorelijstBewerkingen **********************

void Proloog(void)
{	clrscr();
//	spawnl(P_WAIT, "mbac_gra.exe", NULL);
	clrscr();
	_setcursortype(_NOCURSOR);
	randomize();
//	DrukAfVenster(1, 1, 80, 25, BLUE, WHITE, BLACK);
	Ggotoxy(28, 1);
	Gkleurprintf(BLACK, GREEN, " Stefan Kroon Presents: ");
	Geluid(1000, 1); delay (50); DrukAfLetter_M(6);
	Geluid(1000, 1); delay (50); DrukAfLetter_A(21);
	Geluid(1000, 1); delay (50); DrukAfLetter_M(36);
	Geluid(1000, 1); delay (50); DrukAfLetter_B(51);
	Geluid(1000, 1); delay (50); DrukAfLetter_A(66);
	Ggotoxy(6, 19);
	Gkleurprintf(BLACK, LIGHTRED, "         ≥              ≥              ≥      ≥                      ≥");
	Ggotoxy(6, 20);
	Gkleurprintf(BLACK, LIGHTRED, "         ^              ^              ^      ^                      ^");
	Ggotoxy(27 ,21); Gkleurprintf(BLACK, YELLOW, "MAMBA V3.0 (July 1st, 1997)");
	Ggotoxy(18, 22); Gkleurprintf(BLACK, YELLOW, "The New Version With Cheats And A Demo (Press D)");

	while (!bioskey(1));
	char toets = bioskey(1);
	if (toets == 'd' || toets == 'D')
	{	getch();
		ComputerDemo = TRUE;
		CheatMode = FALSE;
		GodMode = TRUE;
		AutoSearch = TRUE;
		LoopNietDood = TRUE;
	}
	else
	ComputerDemo = FALSE;
	SluitGrafischScherm();
}

void SchermOpbouw(void)
{	clrscr();
	Ggotoxy(1, 1);
	Gkleurprintf(RED, YELLOW, " MAMBA ");
	Ggotoxy(8, 1);
	Gkleurprintf(BLUE, WHITE, " Energy                   Bonus                                           ");
	Ggotoxy(56, 1);
	Gkleurprintf(BLUE, WHITE, "Level        Score ");
	DrukAfVenster(1, 2, 80, 25, BLUE, WHITE, BLACK);
}

void BeginInstellingen(void)
{	int Gekozen;
	for (int x = 0; x <= 39; x++) for (int y = 0; y <= 21; y++) TabelXY[x][y] = 0;
	for (x = 0; x < MAXSLANG_LENGTE; x++) PosXvanSlangdeel[x] = 0;
	for (x = 0; x < MAXSLANG_LENGTE; x++) PosYvanSlangdeel[x] = 0;
	Richting = RECHTS; NRichting = RECHTS;
	EindeSpel = FALSE;
	Score = 0; Level = 1; Energie = 5; Bonus = 0;
	BonusStap = 0; BonusAfstand = 0; Stap2 = 0;
	if (CheatMode == TRUE) CheatPlayer = TRUE;
	else CheatPlayer = FALSE;
	DrukAfLenS();
	DrukAfEenB();
	Gekozen = rand() % 22;
	SlangLengte = BEGINSLANG_LENGTE;
	for (int Slangdeel = 1; Slangdeel <= SlangLengte; Slangdeel++)
	{	PosXvanSlangdeel[Slangdeel] = 20 - Slangdeel;
		PosYvanSlangdeel[Slangdeel] = Gekozen;
		TabelXY[20 - Slangdeel][Gekozen] = Slangdeel;
	}
	DrukAfSlang(BLACK, YELLOW);
}

void BeginVoedsel(void)
{	AantalVoedsel = (rand() % 5) + 3;
	for (int VTeller = 1; VTeller <= AantalVoedsel; VTeller++)
		DrukAfBlauwVoedsel();
}

//*************************************************************************
//**************************** Overige Funcites ***************************
//*************************************************************************
void RandomGebeurtenis(void)
{	int gekozenX = rand() % 39, gekozenY = rand() % 22;
	if (TabelXY[gekozenX][gekozenY] == BLAUWVOEDSEL && AantalVoedsel > 3)
		Verwijder(gekozenX, gekozenY);
	if (gekozenX == gekozenY) DrukAfBlauwVoedsel();
	if (TabelXY[gekozenX][gekozenY] == MUUR)
		DrukAfGroenVoedsel(gekozenX, gekozenY);
}

void AlleGroenenWeg(void)
{	for (int XasTeller = 0; XasTeller <= 38; XasTeller++)
	{	for (int YasTeller = 0; YasTeller <= 21; YasTeller++)
		{	if (TabelXY[XasTeller][YasTeller] == GROENVOEDSEL)
				Verwijder(XasTeller, YasTeller);
		}
	}
}

void BonusWeg(void)
{	Verwijder(BonusX, BonusY);
	BonusAfstand = 0; Bonus = 0;
	if (Energie > 1) Energie--;
	else AlleGroenenWeg();
	DrukAfEenB();
}
//************************ Einde OverigeFuncties **************************

//*************************************************************************
//************************ CheatFuncties **********************************
//*************************************************************************
int AantalRichtingenMuur(int X_Positie, int Y_Positie)
{	int AantalR = 0;
	if (TabelXY[X_Positie + 1][Y_Positie] == MUUR || TabelXY[X_Positie + 1][Y_Positie] <= SCHIJNMUUR || TabelXY[X_Positie + 1][Y_Positie] > 0 || (X_Positie + 1) > 38)
		AantalR++;
	if (TabelXY[X_Positie - 1][Y_Positie] == MUUR || TabelXY[X_Positie - 1][Y_Positie] <= SCHIJNMUUR || TabelXY[X_Positie - 1][Y_Positie] > 0 || (X_Positie - 1) < 0)
		AantalR++;
	if (TabelXY[X_Positie][Y_Positie + 1] == MUUR || TabelXY[X_Positie][Y_Positie + 1] <= SCHIJNMUUR || TabelXY[X_Positie][Y_Positie + 1] > 0 || (Y_Positie + 1) > 21)
		AantalR++;
	if (TabelXY[X_Positie][Y_Positie - 1] == MUUR || TabelXY[X_Positie][Y_Positie - 1] <= SCHIJNMUUR || TabelXY[X_Positie][Y_Positie - 1] > 0 || (Y_Positie - 1) < 0)
		AantalR++;
	return AantalR;
}

int AfstandNRechts(int RedenVanZoeken)
{	int AfstandNaarRechts = 1, ZPositie, Gevonden = FALSE;
	if (Richting == RECHTS || Richting == BENEDEN || Richting == BOVEN)
	{	ZPositie = PosXvanSlangdeel[1] + 1;
		while (TabelXY[ZPositie][PosYvanSlangdeel[1]] < 1 && TabelXY[ZPositie][PosYvanSlangdeel[1]] != MUUR && ZPositie <= 38)
		{	if (RedenVanZoeken == AUTOSEARCH)
			{    if (AantalRichtingenMuur(ZPositie, PosYvanSlangdeel[1]) < 3)
				{	if (TabelXY[ZPositie][PosYvanSlangdeel[1]] == BLAUWVOEDSEL || TabelXY[ZPositie][PosYvanSlangdeel[1]] == GROENVOEDSEL)
					{	Gevonden = TRUE;
						break;
					}
				}
			}
			AfstandNaarRechts++; ZPositie++;
		}
	}
	if (Gevonden == FALSE) AfstandNaarRechts *= -1;
	return AfstandNaarRechts;
}

int AfstandNLinks(int RedenVanZoeken)
{	int AfstandNaarLinks = 1, ZPositie, Gevonden = FALSE;
	if (Richting == LINKS || Richting == BENEDEN || Richting == BOVEN)
	{	ZPositie = PosXvanSlangdeel[1] - 1;
		while (TabelXY[ZPositie][PosYvanSlangdeel[1]] < 1 && TabelXY[ZPositie][PosYvanSlangdeel[1]] != MUUR && ZPositie >= 0)
		{	if (RedenVanZoeken == AUTOSEARCH)
			{	if (AantalRichtingenMuur(ZPositie, PosYvanSlangdeel[1]) < 3)
				{	if (TabelXY[ZPositie][PosYvanSlangdeel[1]] == BLAUWVOEDSEL || TabelXY[ZPositie][PosYvanSlangdeel[1]] == GROENVOEDSEL)
					{	Gevonden = TRUE;
						break;
					}
				}
			}
			AfstandNaarLinks++; ZPositie--;
		}
	}
	if (Gevonden == FALSE) AfstandNaarLinks *= -1;
	return AfstandNaarLinks;
}

int AfstandNBoven(int RedenVanZoeken)
{	int AfstandNaarBoven = 1, ZPositie, Gevonden = FALSE;
	if (Richting == LINKS || Richting == RECHTS || Richting == BOVEN)
	{	ZPositie = PosYvanSlangdeel[1] - 1;
		while (TabelXY[PosXvanSlangdeel[1]][ZPositie] < 1 && TabelXY[PosXvanSlangdeel[1]][ZPositie] != MUUR && ZPositie >= 0)
		{	if (RedenVanZoeken == AUTOSEARCH)
			{	if (AantalRichtingenMuur(PosXvanSlangdeel[1], ZPositie) < 3)
				{	if (TabelXY[PosXvanSlangdeel[1]][ZPositie] == BLAUWVOEDSEL || TabelXY[PosXvanSlangdeel[1]][ZPositie] == GROENVOEDSEL)
					{	Gevonden = TRUE;
						break;
					}
				}
			}
			AfstandNaarBoven++; ZPositie--;
		}
	}
	if (Gevonden == FALSE) AfstandNaarBoven *= -1;
	return AfstandNaarBoven;
}

int AfstandNBeneden(int RedenVanZoeken)
{	int AfstandNaarBeneden = 1, ZPositie, Gevonden = FALSE;
	if (Richting == LINKS || Richting == RECHTS || Richting == BENEDEN)
	{	ZPositie = PosYvanSlangdeel[1] + 1;
		while (TabelXY[PosXvanSlangdeel[1]][ZPositie] < 1 && TabelXY[PosXvanSlangdeel[1]][ZPositie] != MUUR && ZPositie <= 21)
		{	if (RedenVanZoeken == AUTOSEARCH)
			{	if (AantalRichtingenMuur(PosXvanSlangdeel[1], ZPositie) < 3)
				{	if (TabelXY[PosXvanSlangdeel[1]][ZPositie] == BLAUWVOEDSEL || TabelXY[PosXvanSlangdeel[1]][ZPositie] == GROENVOEDSEL)
					{	Gevonden = TRUE;
						break;
					}
				}
			}
			AfstandNaarBeneden++; ZPositie++;
		}
	}
	if (Gevonden == FALSE) AfstandNaarBeneden *= -1;
	return AfstandNaarBeneden;
}

void GaNaarBonus(void)
{	if (BonusX > PosXvanSlangdeel[1] && WatIsErRichting(RECHTS) != MUUR) NRichting = RECHTS;
	if (BonusX < PosXvanSlangdeel[1] && WatIsErRichting(LINKS) != MUUR) NRichting = LINKS;
	if (BonusY > PosYvanSlangdeel[1] && WatIsErRichting(BENEDEN) != MUUR) NRichting = BENEDEN;
	if (BonusY < PosYvanSlangdeel[1] && WatIsErRichting(BOVEN) != MUUR) NRichting = BOVEN;
}

void AutoS(void)
{	if (BonusAfstand == 0)
	{	int AfstandNaarBoven = AfstandNBoven(AUTOSEARCH);
		int AfstandNaarBeneden = AfstandNBeneden(AUTOSEARCH);
		int AfstandNaarLinks = AfstandNLinks(AUTOSEARCH);
		int AfstandNaarRechts = AfstandNRechts(AUTOSEARCH);
		for (int aTeller = 0; aTeller <= 39; aTeller++)
		{  	if (aTeller == AfstandNaarLinks)
			{	NRichting = LINKS;
				break;
			}
			if (aTeller == AfstandNaarRechts)
			{	NRichting = RECHTS;
				break;
			}
			if (aTeller == AfstandNaarBoven)
			{	NRichting = BOVEN;
				break;
			}
			if (aTeller == AfstandNaarBeneden)
			{	NRichting = BENEDEN;
				break;
			}
		}
	}
	else GaNaarBonus();
}

int NPositie(int As, int SchijnRichting)
{  int NieuwePositieX, NieuwePositieY;
   if (SchijnRichting == RECHTS)
   {  NieuwePositieX = PosXvanSlangdeel[1] + 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (SchijnRichting == LINKS)
   {  NieuwePositieX = PosXvanSlangdeel[1] - 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (SchijnRichting == BOVEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] - 1;
   }
   if (SchijnRichting == BENEDEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] + 1;
   }
   if (As == X_AS) return NieuwePositieX;
   if (As == Y_AS) return NieuwePositieY;
   Einde(1, "Fout in NPositie.\n");
   return -1;
}

int ProbeerAndereRichting()
{    int Gelukt = -1;
	switch (Richting)
	{	case (BOVEN):
		case (BENEDEN):	int AfstandNaarLinks = AfstandNLinks(GODMODE);
						int AfstandNaarRechts = AfstandNRechts(GODMODE);
						if (AfstandNaarRechts == -1 && AfstandNaarLinks == -1)
						{	Gelukt = FALSE;
							break;
						}
						if (AfstandNaarLinks < AfstandNaarRechts)
							NRichting = LINKS;
						else	NRichting = RECHTS;
						Gelukt = TRUE;
						break;
		case (LINKS):
		case (RECHTS):		int AfstandNaarBoven = AfstandNBoven(GODMODE);
						int AfstandNaarBeneden = AfstandNBeneden(GODMODE);
						if (AfstandNaarBoven == -1 && AfstandNaarBeneden == -1)
						{	Gelukt = FALSE;
							break;
						}
						if (AfstandNaarBoven < AfstandNaarBeneden)
							NRichting = BOVEN;
						else	NRichting = BENEDEN;
						Gelukt = TRUE;
						break;
	}
	if (Gelukt == -1) Einde(1, "Fout in ProbeerAndereRichting"); // Hopelijk een overbodige melding
	return Gelukt;
}

int MogelijkeRichtingVanaf(int posX, int posY)
{    int MogelijkeRichting, AantalKeer = 0;
	if (TabelXY[posX + 1][posY] != MUUR && TabelXY[posX + 1][posY] > SCHIJNMUUR && TabelXY[posX + 1][posY] < 1 && posX + 1 < 39)
	{	MogelijkeRichting = RECHTS;
		AantalKeer++;
	}
	if (TabelXY[posX - 1][posY] != MUUR && TabelXY[posX - 1][posY] > SCHIJNMUUR && TabelXY[posX - 1][posY] < 1 && posX - 1 > -1)
	{	MogelijkeRichting = LINKS;
		AantalKeer++;
	}
	if (TabelXY[posX][posY + 1] != MUUR && TabelXY[posX][posY + 1] > SCHIJNMUUR && TabelXY[posX][posY + 1] < 1 && posY + 1 < 22)
	{	MogelijkeRichting = BENEDEN;
		AantalKeer++;
	}
	if (TabelXY[posX][posY - 1] != MUUR && TabelXY[posX][posY - 1] > SCHIJNMUUR && TabelXY[posX][posY - 1] < 1 && posY - 1 > -1)
	{	MogelijkeRichting = BOVEN;
		AantalKeer++;
	}
	if (AantalKeer != 1) Einde(1, "Fout in MogelijkeRichtingVanaf(x, y).\n");
	return MogelijkeRichting;
}



void DoodLoopControle(void)
{    int SchijnX, SchijnY, SchijnRichting;
	int NieuwePosX = NPositie(X_AS, NRichting);
	int NieuwePosY = NPositie(Y_AS, NRichting);
	if (AantalRichtingenMuur(NieuwePosX, NieuwePosY) == 4)
		ProbeerAndereRichting();
	if (AantalRichtingenMuur(NieuwePosX, NieuwePosY) == 3)
	{    SchijnX = NieuwePosX; SchijnY = NieuwePosY;
		do
		{	SchijnRichting = MogelijkeRichtingVanaf(SchijnX, SchijnY);
			TabelXY[SchijnX][SchijnY] += SCHIJNMUUR;
			switch (SchijnRichting)
			{	case RECHTS:	SchijnX++; break;
				case LINKS:	SchijnX--; break;
				case BENEDEN:	SchijnY++; break;
				case BOVEN:	SchijnY--; break;
				default:		Einde(1, "Geen SchijnRichting meegekregen.\r\nFout in DoodLoopControle.\n");
			}
			if (AantalRichtingenMuur(SchijnX, SchijnY) == 4)
			{	ProbeerAndereRichting();
				break;
			}
			if (AantalRichtingenMuur(SchijnX, SchijnY) < 3)
				break;
		}
		while(1);
		for (int x = 0; x <= 38; x++)
		{	for (int y = 0; y <= 21; y++)
				if (TabelXY[x][y] <= SCHIJNMUUR) TabelXY[x][y] -= SCHIJNMUUR;
		}
	}
}
//************************** Einde CheatFuncties **************************

//*************************************************************************
//*********************** Controle Functies *******************************
//*************************************************************************
void BotsGeluidEnKnipperen(void)
{	for (int ATeller = 1; ATeller <= 3; ATeller++)
	{	DrukAfSlang(BLACK, YELLOW);
		for (int STeller = 1000; STeller <= 4000; STeller += 250)
		{	Geluid(STeller, 30);
			if (STeller == 2000) DrukAfSlang(BLUE, WHITE);
		}
	}
	DrukAfSlang(BLACK, YELLOW);
	EindeSpel = TRUE;
}

void Botsing(void)
{   	if (GodMode == FALSE) BotsGeluidEnKnipperen();
	else
	{	if (ProbeerAndereRichting() == FALSE) BotsGeluidEnKnipperen();
	}
}

void BotsControle(void)
{  if (WatIsErRichting(NRichting) == MUUR) Botsing();
}

void AantalBonusControle(void)
{    unsigned long BeginScore = Score;
	if (Bonus < 5)
	{	Score += (Level * 75) + rand() % (150 * Level);
		for (int t = 250; t <= 750; t += 500) Geluid(t, 100);
	}
	else
	{	Bonus = 0;
		Energie++;
		Score += (Level * 500);
		Score += rand() % (100 * Level);
		if (Energie == 6)
		{	Energie = 5;
			Score += Level * 500;
			for (int t = 250; t <= 750; t += 100) Geluid(t, 100);
		}
		else for (int t = 250; t <= 750; t += 500) Geluid(t, 100);
	}
	if (BeginScore > Score) Score = 99999l;
	DrukAfEenB();
	DrukAfLenS();
	AlVerstrekenTijd = 100;
}

void VoedselControle(void)
{	if (WatIsErRichting(NRichting) == BONUS)
	{	RandomGebeurtenis();
		BonusAfstand = 0;
		Bonus++;
		AantalBonusControle();
	}
	if (WatIsErRichting(NRichting) == BLAUWVOEDSEL || WatIsErRichting(NRichting) == GROENVOEDSEL)
	{	RandomGebeurtenis();
		if (WatIsErRichting(NRichting) == BLAUWVOEDSEL)
		{	for (int t = 750; t <= 1000; t += 250) Geluid(t, 50);
			Score += Level;
		}
		else
		{	for (int t = 1250; t <= 1500; t += 250) Geluid(t, 50);
			Score += Level * 10;
		}
		AlVerstrekenTijd = 50;
		DrukAfBlauwVoedsel();
		if (((SlangLengte > (int)(MAXSLANG_LENGTE * 0.5) - (int)(0.5 * Level)) && (SlangLengte == (MAXSLANG_LENGTE - 1) - rand() % 25)) || SlangLengte == MAXSLANG_LENGTE - 1)
		{	Level++;
			for (int SATeller = BEGINSLANG_LENGTE + 1; SATeller <= SlangLengte; SATeller++)
			{	Ggotoxy((PosXvanSlangdeel[SATeller] * 2) + 2, PosYvanSlangdeel[SATeller] + 3);
				Gkleurprintf(WHITE, BROWN, "±±");
				TabelXY[PosXvanSlangdeel[SATeller]][PosYvanSlangdeel[SATeller]] = MUUR;
			}
			SlangLengte = BEGINSLANG_LENGTE;
			int RandomX, RandomY;
			do
			{	RandomX = rand() % 39;
				RandomY = rand() % 22;
			}
			while (TabelXY[RandomX][RandomY] != 0) ;
			TabelXY[RandomX][RandomY] = BONUS;
			BonusX = RandomX;
			BonusY = RandomY;
			Ggotoxy((RandomX * 2) + 2, RandomY + 3);
			Gkleurprintf(BLACK, LIGHTGREEN, "€€");
			BonusStap = 0;
			int AfstandX, AfstandY;
			AfstandX = BonusX - PosXvanSlangdeel[1];
			AfstandY = BonusY - PosYvanSlangdeel[1];
			if (AfstandX < 0) AfstandX = AfstandX * -1;
			if (AfstandY < 0) AfstandY = AfstandY * -1;
			BonusAfstand = AfstandY + AfstandX + ((Level % 10) * 3) + (rand() % Level) + 5;
			if (BonusAfstand < 15) BonusAfstand = 15;
		}
		SlangLengte++;
		PosXvanSlangdeel[SlangLengte] = PosXvanSlangdeel[SlangLengte - 1];
		PosYvanSlangdeel[SlangLengte] = PosYvanSlangdeel[SlangLengte - 1];
		DrukAfLenS();
	}
}
//*********************** Einde ControleFucties ***************************


void HerdrukSlang(void)
{  int OnthoudPositieX = PosXvanSlangdeel[SlangLengte];
   int OnthoudPositieY = PosYvanSlangdeel[SlangLengte];
   if (EindeSpel == FALSE)
   {   for (int Teller = SlangLengte; Teller >= 1; Teller--)
	 {  PosXvanSlangdeel[Teller] = PosXvanSlangdeel[Teller - 1];
	    PosYvanSlangdeel[Teller] = PosYvanSlangdeel[Teller - 1];
	 }
	 if (Richting == RECHTS)
	 {  PosXvanSlangdeel[1] = PosXvanSlangdeel[2] + 1;
	    PosYvanSlangdeel[1] = PosYvanSlangdeel[2];
	 }
	 if (Richting == BOVEN)
	 {  PosXvanSlangdeel[1] = PosXvanSlangdeel[2];
	    PosYvanSlangdeel[1] = PosYvanSlangdeel[2] - 1;
	 }
	 if (Richting == BENEDEN)
	 {  PosXvanSlangdeel[1] = PosXvanSlangdeel[2];
	    PosYvanSlangdeel[1] = PosYvanSlangdeel[2] + 1;
	 }
	 if (Richting == LINKS)
	 {  PosXvanSlangdeel[1] = PosXvanSlangdeel[2] - 1;
	    PosYvanSlangdeel[1] = PosYvanSlangdeel[2];
	 }
	 Ggotoxy((OnthoudPositieX * 2) + 2, OnthoudPositieY + 3);
	 Gkleurprintf(BLACK, BLACK, "  ");
	 TabelXY[OnthoudPositieX][OnthoudPositieY] = 0;
	 Ggotoxy((PosXvanSlangdeel[1] * 2) + 2, PosYvanSlangdeel[1] + 3);
	 Gkleurprintf(BLACK, YELLOW, "%c%c", 1, 1);
	 Ggotoxy((PosXvanSlangdeel[2] * 2) + 2, PosYvanSlangdeel[2] + 3);
	 Gkleurprintf(BLACK, YELLOW, "%c%c", 254, 254);
	 DrukAfStaart(BLACK, YELLOW);
	 for (int Slangdeel = 1; Slangdeel <= SlangLengte; Slangdeel++)
	 {  TabelXY[PosXvanSlangdeel[Slangdeel]][PosYvanSlangdeel[Slangdeel]] = Slangdeel;
	 }
   }
}

//************************************************************************
//************************* ToetsVerwerking ******************************
//************************************************************************
void DemoToetsVerwerking(void)
{  int Toets = getch();
   switch (Toets)
   {  case 'd':
	 case 'D': ComputerDemo = FALSE;
			 EindeSpel = TRUE;
			 CheatMode = FALSE;
			 GodMode = FALSE;
			 AutoSearch = FALSE;
			 LoopNietDood = FALSE;
			 break;
	 case 27:  Einde(0, ""); break;
   }
}

void ToetsVerwerking(void)
{	int Toets = getch();
	switch (Toets)
	{	case 27:		Einde(0, ""); break;
		case '\x0':	Toets = getch();
					if (Toets == 72 && Richting != BENEDEN) NRichting = BOVEN;
					if (Toets == 75 && Richting != RECHTS) NRichting = LINKS;
					if (Toets == 77 && Richting != LINKS) NRichting = RECHTS;
					if (Toets == 80 && Richting != BOVEN) NRichting = BENEDEN;
					break;
		case 'q':
		case 'Q':		Sound = Sound?FALSE:TRUE; AantalKeerS = 0; break;
		case ' ':		while(!bioskey(1)); AantalKeerS = 0; break;
		case 's':
		case 'S':		AantalKeerS++; break;
		case 'k':
		case 'K':		if (AantalKeerS >= 3)
					{	CheatMode = CheatMode?FALSE:TRUE;
						Ggotoxy(1, 1);
						if (CheatMode == FALSE)
						{	Gkleurprintf(RED, YELLOW, "CheatOff");
							GodMode = FALSE;
							AutoSearch = FALSE;
							LoopNietDood = FALSE;
						}
						else Gkleurprintf(RED, YELLOW, "CheatOn");
						AantalKeerS = 0; Stap2 = 0; CheatPlayer = TRUE;
					}
					else AantalKeerS = 0;
					break;
		default: AantalKeerS = 0;
	}
	if (CheatMode == TRUE)
	{	switch (Toets)
		{	case '+':		Level++; break;
			case 'g':
			case 'G':		GodMode = GodMode?FALSE:TRUE;
						Ggotoxy(1, 1);
						if (GodMode == TRUE) Gkleurprintf(RED, YELLOW, "GodMode");
						else Gkleurprintf(RED, YELLOW, "Normal ");
						Stap2 = 0;
						break;
			case '>':
			case '.':		Score += 100; break;
			case ',':
			case '<':		if (Score > 100) Score -= 100; break;
			case 'b':
			case 'B':		Bonus++; AantalBonusControle();
						Ggotoxy(1, 1);
						Gkleurprintf(RED, YELLOW, "Bonus+ ");
						Stap2 = 0;
						break;
			case 'a':
			case 'A': 	AutoSearch = AutoSearch?FALSE:TRUE;
						Ggotoxy(1, 1);
						if (AutoSearch == TRUE) Gkleurprintf(RED, YELLOW, "AutoOn ");
						else Gkleurprintf(RED, YELLOW, "AutoOff");
						Stap2 = 0;
						break;
			case 'd':
			case 'D':		LoopNietDood = LoopNietDood?FALSE:TRUE;
						Ggotoxy(1, 1);
						if (LoopNietDood == TRUE) Gkleurprintf(RED, YELLOW, "DoodOn ");
						else Gkleurprintf(RED, YELLOW, "DoodOff");
						Stap2 = 0;
						break;
		}
		DrukAfLenS();
	}
}
//************************ Einde ToetsVerwerkingen ***********************

//*************************************************************************
//************************ HoofdFunctie ************************************
//*************************************************************************
void main(void)
{  int Stap = 0, DemoStap = 50;
   OpenGrafischScherm();
   Proloog();
   while(1)
   {  SchermOpbouw();
	 BeginInstellingen();
	 BeginVoedsel();
	 while (EindeSpel == FALSE)
	 {  delay(100 - AlVerstrekenTijd);
	    Geluid(3000, 1);
	    AlVerstrekenTijd = 0;
	    if (AutoSearch == TRUE) AutoS();
	    if (bioskey(1))
	    {  if (ComputerDemo == FALSE) ToetsVerwerking();
		  else DemoToetsVerwerking();
	    }
	    BotsControle();
	    if (LoopNietDood == TRUE) DoodLoopControle();
	    VoedselControle();
	    Richting = NRichting;
	    HerdrukSlang();
	    if (EindeSpel == FALSE)
	    {  if (Stap % Level == 0 && Stap > 3)
		  {  RandomGebeurtenis();
			Stap = 0;
		  }
		  if (Stap2 >= 15) DrukAfMamba();
		  if (DemoStap == 2) DemoStap = 79;
		  Stap++; Stap2++; BonusStap++; DemoStap--;
		  if (BonusAfstand > 0 && BonusStap >= BonusAfstand) BonusWeg();
	    }
	    if (ComputerDemo == TRUE) DrukAfDemoBalk(DemoStap);
	 }
	 DrukAfScorelijst();
   }
}
//************************ Einde HoofdFunctie *****************************

// Oude BotsControle
/*  if (NieuwePositieX < 0 || NieuwePositieX > 38)
   {  Botsing();
	 BotsingAlGeweest = TRUE;
   }
   if ((NieuwePositieY < 0 || NieuwePositieY > 21) && BotsingAlGeweest == FALSE)
   {  Botsing();
	 BotsingAlGeweest = TRUE;
   }
   if ((TabelXY[NieuwePositieX][NieuwePositieY] > 0) && BotsingAlGeweest == FALSE)
   {  Botsing();
	 BotsingAlGeweest = TRUE;
   }
   if ((TabelXY[NieuwePositieX][NieuwePositieY] == MUUR) && BotsingAlGeweest == FALSE)
   {  Botsing();
	 BotsingAlGeweest = TRUE;
   }
*/
// Overbodige Functie
/*
int NPositie(int As)
{  int NieuwePositieX, NieuwePositieY;
   if (NRichting == RECHTS)
   {  NieuwePositieX = PosXvanSlangdeel[1] + 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (NRichting == LINKS)
   {  NieuwePositieX = PosXvanSlangdeel[1] - 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (NRichting == BOVEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] - 1;
   }
   if (NRichting == BENEDEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] + 1;
   }
   if (As == X_AS) return NieuwePositieX;
   if (As == Y_AS) return NieuwePositieY;
   Einde(1, "Fout in NPositie.\n");
   return -1;
}
*/
// Oude Functie in Botsing() voor GodMode
/*   {  int NieuweRichting = Richting;
	 if (TabelXY[PosXvanSlangdeel[1] + 1][PosYvanSlangdeel[1]] != MUUR && (PosXvanSlangdeel[1] + 1) < 39 && TabelXY[PosXvanSlangdeel[1] + 1][PosYvanSlangdeel[1]] < 1)
	    NieuweRichting = RECHTS;
	 if (TabelXY[PosXvanSlangdeel[1] - 1][PosYvanSlangdeel[1]] != MUUR && (PosXvanSlangdeel[1] - 1) > -1 && TabelXY[PosXvanSlangdeel[1] - 1][PosYvanSlangdeel[1]] < 1)
	    NieuweRichting = LINKS;
	 if (TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] + 1] != MUUR && (PosYvanSlangdeel[1] + 1) < 22 && TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] + 1] < 1)
	    NieuweRichting = BENEDEN;
	 if (TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] - 1] != MUUR && (PosYvanSlangdeel[1] - 1) > -1 && TabelXY[PosXvanSlangdeel[1]][PosYvanSlangdeel[1] - 1] < 1)
	    NieuweRichting = BOVEN;
		if (NieuweRichting == Richting) BotsGeluidEnKnipperen();
		else
		Richting = NieuweRichting;
	}
}*/
\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00ªŒ®\00ùæ/‹%;à{ÿ°'!=£x˝F”…ÔºkàÒúF…ﬁÉW,˘≤ãâVÑ~yë…]oÊÙ#?dG¡hºè^®·⁄>i˘È˚If:èD-%≥•ñ‰,”Û…@¯√∫≤92q£µ¥EôﬂÃÇ(?iC˘…)/öl+0#É	k<É%xäÉx∆ è‰Z›_E5¡™ÁHeˆﬁ‘‘±à∞ıÖ©√˙Q3˚’%§≠–ÛbïÊíæı	)/ Â\00sw(H
⁄-ó1E≠Ïs|oƒå¬tﬂ=‘(Ã73Ù∫7ÕÑ˘#ﬁPf36EØ€éëd>09U3$ÖÚΩ≈9úb˜€HØî\Ø«`¥ÖëÛ=.£Î8å«À¶fFæ7â#I*ü˚Ä¶\2
€øJ«ÓÍîÌø2êhoÆeﬁÇ¬=≤&r≠j &!DÊbﬁ)RNjíêì∏2h#X
ª<Ÿº⁄Ä/∞[öTµ”Õ√»Ïxå8ô3ëØŸå≥Œ’πÎL˚%{Ç÷Æ7ı‘Iì8«co¡[≠°ò*z	°®Zæf…ï(≠Œ'áîåSZcxq/ú∂Ül_a¢›?w6Òí=KX(Ÿ—dÉ:YHgäSÈv ï8¿£ıÃ√K¸%–≤VÂù5…«GKz|må*]à7È4˘ÛìåbÜEﬁÆr¸ÓívùÛ!D=BO)ªÈ’ç“M¿v≈`˜Kır^+6}shˇD
]L£ÿé£MV¥∂dh≈«d‚{Ñ„(Äæc-.ä‰ø—¢\00,î$¥sÇﬂ'Q'aëﬁd;Ãµ¬˝M’.i•’‹™§& RÕ…4Ô…|Õ+8'Y≤Ω/pYMJ˘˝ÿó ’¢\Çsö„€•sµ:ÄmEØM—W∏•ÑÔ«P\00Ãú{‡´≈ñ∫ﬁ#óC∑
Zuﬂ!&‰§cÃó¥Åxì8≥„µ√Qk/T≤9Cp=Æúª€†Sm6ë‹êàåºà%rﬁqºCrHâ˝V’SZL9F‚%∫Ÿ˘ÆWç∏ÅV’lU°Óæ•∏H'_ßGöEü¬†C	î‹∏ù≠^gtto≤⁄˙N◊v\=qvó
ñÖÍø˝òÒ2lË´«a‹qÚWvêÔ˘é(/å#-+∂Ú¶ì§D◊ØL†ña“ÇD_§˚R,Ú€L&HmÊƒDö∫qÓRñSñã%ﬁh
u·8ıÆ?ﬁÄ§‘‰<:AEå¶$c“á)c¶Iô0N2IhÃ#h,à±9‹làR/æw≠rµÊÔì‘::÷ÀO∑˜oàÂkNÊß«¨-‰,ôæØÈ"'∫k
	ÂX2©„’ì5ÈÔ0|_Ï“ÈíÍ=‹}u£IÿFÃck4·$œ™KÎGV⁄»ZD/ﬁºøˆÑv ﬁÂ»"√ ˜≥Ÿ_Øù»ˆ≠◊£òm⁄ÅÏi€Ö1RkË@1ÉúiŒóãı∞k(7oC{OQ ^–Î∏fp…ñ·II7g%‚îá¨π›)µ+ëjvè∂@Ë˚LKÇL]∏\00YÊÈ†KEÔ„«ßóöÂQïZßlGßKì>}*ﬁêQÍPÛÎëDn√ı–˙RV¢/	m›å@[_ÌJ⁄9ƒ„ÿ´|#–ﬁCƒﬁ{P»;JŒä#C2~)+‹…Y	RV<Y4+
Y4É€≤ìÃ‚-˛hËıºëÒCmºÖ°o(äà*:7É≤’ ≤XÙŸºÛàå˛£√]ãL¶Ê,:v¡vd≈K-ﬁwœ›@«∞;VΩº§ùπ7Ö˝ã€;ëK£ﬁ∂wÑCﬂ¬PTÃH¿IºråÜÆÇ‡˝∆nöqßA∑6ƒØ8Rã(∑pio_R¯ˆ_°Œÿ6ÙYçÚ®;…?
˝îçNô∂ﬂ?4UÃˆª·õv?˝u`¡îû¨~ô∑;Ü¶ç£¥!o|>d~B>≈‰≥bÚÂQò-©b&ÊS(÷Q”\00)π˝Q ?y˚V˙N›˛ }ßow“7e˚„Ùùπ›Eﬂ9€ãøb∆™*·[ﬂÜˆ™ )Ëñfk6ç?
