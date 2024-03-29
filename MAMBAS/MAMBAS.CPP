// string.h  conio.h  stdio.h  stdlib.h  stdarg.h
#include <dos.h>
#include <time.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "\tc\stefan\mambas\stdste.h"

#define LEEG_78 "                                                                              "

#define FALSE 0
#define TRUE 1

#define BOVEN 1
#define LINKS 2
#define RECHTS 3
#define BENEDEN 4

#define MUUR -2
#define BLAUWVOEDSEL -3
#define GROENVOEDSEL -4
#define BONUS -5

#define BEGINSLANG_LENGTE 6
#define MAXSLANG_LENGTE 36
// 179:������������������������������������������

// De kleuren zoals ze gedefinieerd zijn in <conio.h>:
/*
BLACK
BLUE
GREEN
CYAN
RED
MAGENTA
BROWN
LIGHTGRAY
DARKGRAY
LIGHTBLUE
LIGHTGREEN
LIGHTCYAN
LIGHTRED
LIGHTMAGENTA
YELLOW
WHITE
*/

int Richting, TabelXY[40][22];
int PosXvanSlangdeel[MAXSLANG_LENGTE], PosYvanSlangdeel[MAXSLANG_LENGTE];
int SlangLengte, Score, Level, Energie, Bonus, AantalVoedsel, BonusStap;
int BonusAfstand, BonusX, BonusY, AlVerstrekenTijd, EindeSpel, Sound = TRUE;
char *screen;

void Einde(int eindcode)
{  _setcursortype(_NORMALCURSOR);
   kleurprintf(BLACK, LIGHTGRAY, "");
   clrscr();
   exit(eindcode);
}

void Geluid(int frequentie)
{  if (Sound == TRUE)
	 sound(frequentie);
}

void DrukAfEenB(void)
{  gotoxy(16, 1);
   kleurprintf(BLUE, BLUE, "                ");
   gotoxy(40, 1);
   kleurprintf(BLUE, BLUE, "                ");
   for (int ETeller = 1; ETeller <= Energie; ETeller++)
   {  gotoxy(12 + (3 * ETeller), 1);
	 kleurprintf(BLUE, LIGHTGREEN, " ��");
   }
   for (int BTeller = 1; BTeller <= Bonus; BTeller++)
   {  gotoxy(36 + (3 * BTeller), 1);
	 kleurprintf(BLUE, YELLOW, " ��");
   }
}

void BeginVoedsel(void)
{  int voedselX, voedselY;
   AantalVoedsel = (rand() % 5) + 3;
   for (int VTeller = 1; VTeller <= AantalVoedsel; VTeller++)
   {  voedselX = rand() % 39; voedselY = rand() % 22;
	 if (TabelXY[voedselX][voedselY] == 0)
	 {  TabelXY[voedselX][voedselY] = BLAUWVOEDSEL;
	    gotoxy((voedselX * 2) + 2, voedselY + 3);
	    kleurprintf(BLACK, CYAN, "@@");
	 }
	 else VTeller--;
   }
}

void DrukAfVoedsel()
{  int GekozenX, GekozenY;
   do { GekozenX = rand() % 39; GekozenY = rand() % 22;}
   while (TabelXY[GekozenX][GekozenY] > 0 || TabelXY[GekozenX][GekozenY] == BLAUWVOEDSEL) ;
   if (TabelXY[GekozenX][GekozenY] == 0)
   {  TabelXY[GekozenX][GekozenY] = BLAUWVOEDSEL;
	 gotoxy((GekozenX * 2) + 2, GekozenY + 3);
	 kleurprintf(BLACK, CYAN, "@@");
   }
   if (TabelXY[GekozenX][GekozenY] == MUUR)
   {  TabelXY[GekozenX][GekozenY] = GROENVOEDSEL;
	 gotoxy((GekozenX * 2) + 2, GekozenY + 3);
	 kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
	 int Kans, Extra = 1;
	 Kans = 10 - Level;
	 if (Kans < 5) Kans = 5;
	 if ((rand() % Kans) == 1)
	 {  if ((rand() % 2) == 1)
	    {  while (TabelXY[GekozenX + Extra][GekozenY] == MUUR)
		  {  TabelXY[GekozenX + Extra][GekozenY] = GROENVOEDSEL;
			gotoxy(((GekozenX + Extra) * 2) + 2, GekozenY + 3);
			kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
			Extra++;
		  }
	    }
	    else
	    {  while (TabelXY[GekozenX][GekozenY + Extra] == MUUR)
		  {  TabelXY[GekozenX][GekozenY + Extra] = GROENVOEDSEL;
			gotoxy((GekozenX * 2) + 2, (GekozenY + Extra) + 3);
			kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
			Extra++;
		  }
	    }
	 }
   }
}

void DrukAfStaart(int AchtergrondKleur, int VoorgrondKleur)
{  gotoxy((PosXvanSlangdeel[SlangLengte] * 2) + 2, PosYvanSlangdeel[SlangLengte] + 3);
   if (PosXvanSlangdeel[SlangLengte - 1] == PosXvanSlangdeel[SlangLengte] + 1)
	 kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 17, 17);
   if (PosXvanSlangdeel[SlangLengte - 1] == PosXvanSlangdeel[SlangLengte] -1)
	 kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 16, 16);
   if (PosYvanSlangdeel[SlangLengte - 1] == PosYvanSlangdeel[SlangLengte] + 1)
	 kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 30, 30);
   if (PosYvanSlangdeel[SlangLengte - 1] == PosYvanSlangdeel[SlangLengte] - 1)
	 kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 31, 31);
}

void DrukAfSlang(int AchtergrondKleur, int VoorgrondKleur)
{  gotoxy((PosXvanSlangdeel[1] * 2) + 2, PosYvanSlangdeel[1] + 3);
   kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 1, 1);
   for (int Teller = 2; Teller <= (SlangLengte - 1); Teller++)
   {  gotoxy((PosXvanSlangdeel[Teller] * 2) + 2, PosYvanSlangdeel[Teller] + 3);
	 kleurprintf(AchtergrondKleur, VoorgrondKleur, "%c%c", 254, 254);
   }
   DrukAfStaart(AchtergrondKleur, VoorgrondKleur);
}

void Botsing(void)
{  for (int ATeller = 1; ATeller <= 3; ATeller++)
   {  DrukAfSlang(BLACK, YELLOW);
	 for (int STeller = 1000; STeller <= 4000; STeller += 250)
	 {  Geluid(STeller);
	    if (STeller == 2000) DrukAfSlang(BLUE, WHITE);
	    delay (30);
	 }
	 nosound();
   }
   DrukAfSlang(BLACK, YELLOW);
   EindeSpel = TRUE;
}

void RandomGebeurtenis(void)
{  int gekozenX = rand() % 39, gekozenY = rand() % 22;
   if (TabelXY[gekozenX][gekozenY] == BLAUWVOEDSEL && AantalVoedsel > 3)
   {  gotoxy((gekozenX * 2) + 2, gekozenY + 3);
	 kleurprintf(BLACK, BLACK, "  ");
	 TabelXY[gekozenX][gekozenY] = 0;
   }
   if (gekozenX == gekozenY) DrukAfVoedsel();
   if (TabelXY[gekozenX][gekozenY] == MUUR)
   {  TabelXY[gekozenX][gekozenY] = GROENVOEDSEL;
	 gotoxy((gekozenX * 2) + 2, gekozenY + 3);
	 kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
	 int Kans, Extra = 1;
	 Kans = 10 - Level;
	 if (Kans < 5) Kans = 5;
	 if ((rand() % Kans) == 1)
	 {  if ((rand() % 2) == 1)
	    {  while (TabelXY[gekozenX + Extra][gekozenY] == MUUR)
		  {  TabelXY[gekozenX + Extra][gekozenY] = GROENVOEDSEL;
			gotoxy(((gekozenX + Extra) * 2) + 2, gekozenY + 3);
			kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
			Extra++;
		  }
	    }
	    else
	    {  while (TabelXY[gekozenX][gekozenY + Extra] == MUUR)
		  {  TabelXY[gekozenX][gekozenY + Extra] = GROENVOEDSEL;
			gotoxy((gekozenX * 2) + 2, (gekozenY + Extra) + 3);
			kleurprintf(BLACK, LIGHTGREEN, "%c%c", 15, 15);
			Extra++;
		  }
	    }
	 }
   }
}

void BotsControle(void)
{  int NieuwePositieX, NieuwePositieY;
   if (Richting == RECHTS)
   {  NieuwePositieX = PosXvanSlangdeel[1] + 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (Richting == LINKS)
   {  NieuwePositieX = PosXvanSlangdeel[1] - 1;
	 NieuwePositieY = PosYvanSlangdeel[1];
   }
   if (Richting == BOVEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] - 1;
   }
   if (Richting == BENEDEN)
   {  NieuwePositieX = PosXvanSlangdeel[1];
	 NieuwePositieY = PosYvanSlangdeel[1] + 1;
   }
   if (NieuwePositieX < 0 || NieuwePositieX > 38) Botsing();
   if (NieuwePositieY < 0 || NieuwePositieY > 21) Botsing();
   if (TabelXY[NieuwePositieX][NieuwePositieY] > 0) Botsing();
   if (TabelXY[NieuwePositieX][NieuwePositieY] == MUUR) Botsing();
   if (TabelXY[NieuwePositieX][NieuwePositieY] == BONUS)
   {  RandomGebeurtenis();
	 BonusAfstand = 0;
	 if (Bonus < 4)
	 {  Bonus += 1;
	    Score += (Level * 75) + rand() % (150 * Level);
	 }
	 else
	 {  Bonus = 0;
	    Energie++;
	    Score += (Level * 500) + rand() % (100 * Level);
	 }
	 if (Energie == 6)
	 {  Energie = 5;
	    Score += Level * 500;
	    for (int t = 250; t <= 750; t += 100)
	    {  Geluid(t);
		  delay(100);
	    }
	    AlVerstrekenTijd = 100;
	 }
	 else
	 {  for (int t = 250; t <= 750; t += 500)
	    {  Geluid(t);
		  delay(100);
	    }
	    AlVerstrekenTijd = 100;
	 }
	 nosound();
	 DrukAfEenB();
	 gotoxy(75, 1);
	 kleurprintf(RED, YELLOW, "%05d", Score);
   }
   if (TabelXY[NieuwePositieX][NieuwePositieY] == BLAUWVOEDSEL || TabelXY[NieuwePositieX][NieuwePositieY] == GROENVOEDSEL)
   {  RandomGebeurtenis();
	 if (TabelXY[NieuwePositieX][NieuwePositieY] == BLAUWVOEDSEL)
	 {  for (int t = 750; t <= 1000; t += 250)
	    {  Geluid(t);
		  delay(50);
	    }
	    AlVerstrekenTijd = 50;
	 }
	 else
	 {  for (int t = 1250; t <= 1500; t += 250)
	    {  Geluid(t);
		  delay(50);
	    }
	    AlVerstrekenTijd = 50;
	 }
	 nosound();
	 if (TabelXY[NieuwePositieX][NieuwePositieY] == BLAUWVOEDSEL) Score += Level;
	 else Score += Level * 10;
	 TabelXY[NieuwePositieX][NieuwePositieY] = 0;
	 gotoxy(75, 1);
	 kleurprintf(RED, YELLOW, "%05d", Score);
	 DrukAfVoedsel();
	 if (((SlangLengte > 20 - (int)(0.5 * Level)) && (SlangLengte == 35 - rand() % 25)) || SlangLengte == 35)
	 {  Level++;
	    gotoxy(62, 1); kleurprintf(RED, YELLOW, "%02d", Level);
	    for (int SATeller = 7; SATeller <= SlangLengte; SATeller++)
	    {  gotoxy((PosXvanSlangdeel[SATeller] * 2) + 2, PosYvanSlangdeel[SATeller] + 3);
		  kleurprintf(WHITE, BROWN, "��");
		  TabelXY[PosXvanSlangdeel[SATeller]][PosYvanSlangdeel[SATeller]] = MUUR;
	    }
	    SlangLengte = 6;
	    int RandomX, RandomY;
	    do
	    { RandomX = rand() % 39; RandomY = rand() % 22; }
	    while (TabelXY[RandomX][RandomY] != 0) ;
	    TabelXY[RandomX][RandomY] = BONUS;
	    BonusX = RandomX;
	    BonusY = RandomY;
	    gotoxy((RandomX * 2) + 2, RandomY + 3);
	    kleurprintf(BLACK, LIGHTGREEN, "��");
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
   }
}

void HerdrukSlang(void)
{  int OnthoudPositieX = PosXvanSlangdeel[SlangLengte];
   int OnthoudPositieY = PosYvanSlangdeel[SlangLengte];
   BotsControle();
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
	 gotoxy((OnthoudPositieX * 2) + 2, OnthoudPositieY + 3);
	 kleurprintf(BLACK, BLACK, "  ");
	 TabelXY[OnthoudPositieX][OnthoudPositieY] = 0;
	 gotoxy((PosXvanSlangdeel[1] * 2) + 2, PosYvanSlangdeel[1] + 3);
	 kleurprintf(BLACK, YELLOW, "%c%c", 1, 1);
	 gotoxy((PosXvanSlangdeel[2] * 2) + 2, PosYvanSlangdeel[2] + 3);
	 kleurprintf(BLACK, YELLOW, "%c%c", 254, 254);
	 DrukAfStaart(BLACK, YELLOW);
	 for (int Slangdeel = 1; Slangdeel <= SlangLengte; Slangdeel++)
	 {  TabelXY[PosXvanSlangdeel[Slangdeel]][PosYvanSlangdeel[Slangdeel]] = Slangdeel;
	 }
   }
}


void BeginInstellingen(void)
{  int Gekozen;
   for (int x = 0; x <= 39; x++) for (int y = 0; y <= 21; y++) TabelXY[x][y] = 0;
   for (x = 0; x < MAXSLANG_LENGTE; x++) PosXvanSlangdeel[x] = 0;
   for (x = 0; x < MAXSLANG_LENGTE; x++) PosYvanSlangdeel[x] = 0;
   Richting = RECHTS;
   EindeSpel = FALSE;
   Score = 0; Level = 1; Energie = 5; Bonus = 0;
   BonusStap = 0; BonusAfstand = 0;
   DrukAfEenB();
   gotoxy(62, 1); kleurprintf(RED, YELLOW, "%02d", Level);
   gotoxy(75, 1); kleurprintf(RED, YELLOW, "%05d", Score);
   Gekozen = rand() % 22;
   SlangLengte = BEGINSLANG_LENGTE;
   for (int Slangdeel = 1; Slangdeel <= SlangLengte; Slangdeel++)
   {  PosXvanSlangdeel[Slangdeel] = 20 - Slangdeel;
	 PosYvanSlangdeel[Slangdeel] = Gekozen;
	 TabelXY[20 - Slangdeel][Gekozen] = Slangdeel;
   }
   DrukAfSlang(BLACK, YELLOW);
}


void ToetsVerwerking(void)
{  int Toets;
   Toets = getch();
//   if (Toets == 27) Einde(1);
   if (Toets == '\x0')
   {  Toets = getch();
	 if (Toets == 72 && Richting != BENEDEN) Richting = BOVEN;
	 if (Toets == 75 && Richting != RECHTS) Richting = LINKS;
	 if (Toets == 77 && Richting != LINKS) Richting = RECHTS;
	 if (Toets == 80 && Richting != BOVEN) Richting = BENEDEN;
   }
   if (Toets == 's' || Toets == 'S' || Toets == 'q' || Toets == 'Q')
	   Sound = Sound?FALSE:TRUE;
   if (Toets == ' ')
	 while(!bioskey(1)) ;
}

void Letter_M(int LetterPositie)
{  Geluid(1000); delay(1); nosound(); delay (50);
   gotoxy(LetterPositie,  9); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 10); kleurprintf(BLACK, YELLOW, "��      ��");
   gotoxy(LetterPositie, 11); kleurprintf(BLACK, YELLOW, "� �    � �");
   gotoxy(LetterPositie, 12); kleurprintf(BLACK, YELLOW, "�  �  �  �");
   gotoxy(LetterPositie, 13); kleurprintf(BLACK, YELLOW, "�   ��   �");
   gotoxy(LetterPositie, 14); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 15); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 16); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 17); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 18); kleurprintf(BLACK, YELLOW, "%c        %c", 31, 1);
}
void Letter_A(int LetterPositie)
{  Geluid(1000); delay(1); nosound(); delay (50);
   gotoxy(LetterPositie,  9); kleurprintf(BLACK, YELLOW, "    ��    ");
   gotoxy(LetterPositie, 10); kleurprintf(BLACK, YELLOW, "   �  �   ");
   gotoxy(LetterPositie, 11); kleurprintf(BLACK, YELLOW, "  �    �  ");
   gotoxy(LetterPositie, 12); kleurprintf(BLACK, YELLOW, " �      � ");
   gotoxy(LetterPositie, 13); kleurprintf(BLACK, YELLOW, "����������");
   gotoxy(LetterPositie, 14); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 15); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 16); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 17); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 18); kleurprintf(BLACK, YELLOW, "%c        %c", 31, 1);
}
void Letter_B(int LetterPositie)
{  Geluid(1000); delay(1); nosound(); delay(50);
   gotoxy(LetterPositie,  9); kleurprintf(BLACK, YELLOW, "��������  ");
   gotoxy(LetterPositie, 10); kleurprintf(BLACK, YELLOW, "�       � ");
   gotoxy(LetterPositie, 11); kleurprintf(BLACK, YELLOW, "�       � ");
   gotoxy(LetterPositie, 12); kleurprintf(BLACK, YELLOW, "�       � ");
   gotoxy(LetterPositie, 13); kleurprintf(BLACK, YELLOW, "��������� ");
   gotoxy(LetterPositie, 14); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 15); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 16); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 17); kleurprintf(BLACK, YELLOW, "�        �");
   gotoxy(LetterPositie, 18); kleurprintf(BLACK, YELLOW, "%c%c������� ", 31, 1);
}

void Proloog(void)
{  clrscr();
   kleurprintf(BLUE, WHITE, "����������������������������");
   kleurprintf(BLACK, GREEN, " Bert Uffen presents: ");
   kleurprintf(BLUE, WHITE, "����������������������������ͻ");
   for (int TekenTeller = 1; TekenTeller <= 23; TekenTeller++)
   {  kleurprintf(BLUE, WHITE, "�");
	 kleurprintf(BLACK, BLACK, LEEG_78);
	 kleurprintf(BLUE, WHITE, "�");
   }

   kleurprintf(BLUE, WHITE, "�������������������������������������������������������������������������������");
   screen = (char *) MK_FP(0xb800, 0x0000);
   screen[3998] = '�';
   screen[3999] = 31;
   Letter_M(6);
   Letter_A(21);
   Letter_M(36);
   Letter_B(51);
   Letter_A(66);
   screen[2908] = '�'; screen[2909] = 140;
   screen[3068] = '^'; screen[3069] = 140;
   screen[2938] = '�'; screen[2939] = 140;
   screen[3098] = '^'; screen[3099] = 140;
   screen[2968] = '�'; screen[2969] = 140;
   screen[3128] = '^'; screen[3129] = 140;
   screen[2982] = '�'; screen[2983] = 140;
   screen[3142] = '^'; screen[3143] = 140;
   screen[3028] = '�'; screen[3029] = 140;
   screen[3188] = '^'; screen[3189] = 140;
   gotoxy(27 ,21); kleurprintf(BLACK, YELLOW, "MAMBA V2.0 (November 11th, 1989)");
   while(!bioskey(1)) ;
}

void SchermOpbouw(void)
{  clrscr();
   kleurprintf(RED, YELLOW, " MAMBA ");
   kleurprintf(BLUE, WHITE, " Energy                   Bonus                                           ");
   gotoxy(56, 1);
   kleurprintf(BLUE, WHITE, "Level        Score ");
   kleurprintf(BLUE, WHITE, "\r\n������������������������������������������������������������������������������ͻ");
   for (int TekenTeller = 1; TekenTeller <=22; TekenTeller++)
   {  kleurprintf(BLUE, WHITE, "�");
	 kleurprintf(BLACK, BLACK, LEEG_78);
	 kleurprintf(BLUE, WHITE, "�");
   }
   kleurprintf(BLUE, WHITE, "�������������������������������������������������������������������������������");
   screen = (char *) MK_FP(0xb800, 0x0000);
   screen[3998] = '�';
   screen[3999] = 31;
}

void VraagNaam(char *Naam)
{  char karakter;
   int positie = 0, Insert = FALSE, pTeller;
   strcpy(Naam, "            ");
   gotoxy(28, 12);
   kleurprintf(WHITE, BLACK, "��������������������ͻ");
   gotoxy(28, 13);
   kleurprintf(WHITE, BLACK, "� Name:              �");
   gotoxy(28, 14);
   kleurprintf(WHITE, BLACK, "��������������������ͼ");
   gotoxy(36, 13);
   kleurprintf(LIGHTGREEN, BLACK, "            ");
   gotoxy(36, 13);
   _setcursortype(_NORMALCURSOR);
   karakter = getch();
   while (karakter != '\r')
   {  karakter = toupper(karakter);
	 if (isprint(karakter))
	 {  if (positie < 12)
	    {  if (Insert == FALSE)
		  {  kleurprintf(LIGHTGREEN, BLACK, "%c", karakter);
			Naam[positie] = karakter;
			positie++;
		  }
		  else
		  {  for (pTeller = strlen(Naam); pTeller > positie; pTeller--)
			Naam[pTeller] = Naam[pTeller - 1];
			Naam[12] = '\x0';
			Naam[positie] = karakter;
			positie++;
			gotoxy(36, 13);
			kleurprintf(LIGHTGREEN, BLACK, "%s", Naam);
			gotoxy(36 + positie, 13);
		  }
	    }
	 }
	 else
	 {  if (karakter == '\b')
	    {  kleurprintf(LIGHTGREEN, BLACK, "\b \b");
		  positie--;
		  Naam[positie] = 0;
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
			gotoxy(36, 13);
			kleurprintf(LIGHTGREEN, BLACK, "%s ", Naam);
			gotoxy(36 + positie, 13);
		  }
		  if (karakter == 71)
		  {  positie = 0;
			gotoxy(36, 13);
		  }
		  if (karakter == 79)
		  {  positie = 11;
			gotoxy(47, 13);
		  }
		  if (karakter == 75)
		  {  positie--;
			gotoxy(36 + positie, 13);
		  }
		  if (karakter == 77)
		  {  positie++;
			gotoxy(36 + positie, 13);
		  }
	    }
	 }
	 karakter = getch();
   }
   _setcursortype(_NOCURSOR);
}

void DrukAfScorelijst(void)
{  int LijstLevel[26] = {{0}}, EindeRegel;
   int achtergrondkleur, voorgrondkleur, NaamGevraagd = FALSE, SpelersNummer = 0;
   long int LijstScore[26] = {0};
   char LijstNaam[26][14] = {{0}}, ch;
   FILE *fp;
   if ((fp = fopen("mambas.hi", "rt")) == NULL)
   {  if ((fp = fopen("mambas.hi", "w+t")) == NULL)
	 {  gotoxy(1, 10);
	    kleurprintf(BLACK, RED, "Fout bij aanmaken van bestand");
	    Einde(1);
	 }
   }
   for (int NaamTeller = 1; NaamTeller <= 25; NaamTeller++)
   {  EindeRegel = FALSE;
	 for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
	 {  if (EindeRegel == FALSE) ch = getc(fp);
	    else ch = ' ';
	    switch (ch)
	    {  case -1:   LijstNaam[NaamTeller][LetterTeller] = ' '; break;
		  case '\r': LijstNaam[NaamTeller][LetterTeller] = ' '; break;
		  case '\n': LijstNaam[NaamTeller][LetterTeller] = ' '; EindeRegel = TRUE; break;
		  default:   LijstNaam[NaamTeller][LetterTeller] = ch;
	    }
	 }
	 for (int LevelTeller = 1; LevelTeller <= 2; LevelTeller++)
	 {  if (EindeRegel == FALSE) ch = getc(fp);
	    else ch = -1;
	    if ((ch < '0' || ch > '9') && ch != EOF && ch != '\r' && ch != ' ' && ch != '\n') ch = '\r';
	    switch (ch)
	    {  case -1:   break;
		  case '\r': ch = -1; break;
		  case '\n': ch = -1; EindeRegel = TRUE; break;
		  case ' ':  if (LijstLevel[NaamTeller] > 0) ch = -1; else LevelTeller--; ch = -1; break;
		  default:   ch -= 48;
	    }
	    if (ch != -1) LijstLevel[NaamTeller] = (LijstLevel[NaamTeller] * 10) + ch;
	 }
	 for (int ScoreTeller = 1; ScoreTeller <= 5; ScoreTeller++)
	 {  if (EindeRegel == FALSE) ch = getc(fp);
	    else ch = -1;
	    if ((ch < '0' || ch > '9') && ch != EOF && ch != '\r' && ch != ' ' && ch != '\n') ch = '\r';
	    switch (ch)
	    {  case -1:   break;
		  case '\r': ch = -1; break;
		  case '\n': ch = -1; EindeRegel = TRUE; break;
		  case ' ':  ScoreTeller--; ch = -1; break;
		  default:   ch -= 48;
	    }
	    if (ch != -1) LijstScore[NaamTeller] = (LijstScore[NaamTeller] * 10) + ch;
	 }
	 if (LijstScore[NaamTeller] <= Score && NaamGevraagd == FALSE)
	 {  NaamGevraagd = TRUE;
	    strcpy(LijstNaam[NaamTeller + 1], LijstNaam[NaamTeller]);
	    LijstLevel[NaamTeller + 1] = LijstLevel[NaamTeller];
	    LijstScore[NaamTeller + 1] = LijstScore[NaamTeller];
	    VraagNaam(LijstNaam[NaamTeller]);
	    LijstLevel[NaamTeller] = Level;
	    LijstScore[NaamTeller] = Score;
	    SpelersNummer = NaamTeller;
	    NaamTeller++;
	 }
	 while (ch != 10 && ch != EOF) ch = getc(fp);
   }
   fclose(fp);
//Opslaan ScoreLijst
   if ((fp = fopen("mambas.hi", "wt")) == NULL)
   {  gotoxy(1, 10);
	 kleurprintf(BLACK, RED, "Fout bij openen bestand.");
	 Einde(1);
   }
   for (NaamTeller = 1; NaamTeller <= 25; NaamTeller++)
   {  for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
	    putc(LijstNaam[NaamTeller][LetterTeller], fp);
	 putc(' ', fp); putc(' ', fp); putc(' ', fp);
	 fprintf(fp, "%2d", LijstLevel[NaamTeller]);
	 putc(' ', fp); putc(' ', fp); putc(' ', fp);
	 fprintf(fp, "%5ld", LijstScore[NaamTeller]);
	 putc('\n', fp);
   }
   fclose(fp);
// VensterAfdruk
   gotoxy(7, 5);
   kleurprintf(CYAN, BLUE, "����������������������������");
   kleurprintf(RED, WHITE, " High Score ");
   kleurprintf(CYAN, BLUE, "��������������������������ͻ");
   for (int RTeller = 1; RTeller <= 16; RTeller++)
   {  gotoxy(7, 5 + RTeller);
	 kleurprintf(CYAN, BLUE, "�                                                                  �");
   }
   gotoxy(7, 22);
   kleurprintf(CYAN, BLUE, "��������������");
   kleurprintf(RED, WHITE, " <Esc> = quit      <Enter> = continue ");
   kleurprintf(CYAN, BLUE, "��������������ͼ");
/* EerstePlaatsAfdruk*/
   if (SpelersNummer == 1)
   {  achtergrondkleur = BLUE;
	 voorgrondkleur = WHITE;
   }
   else
   {  achtergrondkleur = CYAN;
	 voorgrondkleur = RED;
   }
   gotoxy(27, 6); kleurprintf(achtergrondkleur, voorgrondkleur, "1. ");
   for (int LetterTeller = 0; LetterTeller < 12; LetterTeller++)
	   kleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[1][LetterTeller]);
   kleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[1], LijstScore[1]);
/* OverigePlaatsAfdruk*/
   for (int rteller = 2; rteller <= 13; rteller++)
   {  if (SpelersNummer == rteller)
	 {  achtergrondkleur = BLUE;
	    voorgrondkleur = WHITE;
	 }
	 else
	 {  achtergrondkleur = CYAN;
	    voorgrondkleur = BLUE;
	 }
	 gotoxy(9, rteller + 7);
	 kleurprintf(achtergrondkleur, voorgrondkleur, "%2d. ", rteller);
	 for (LetterTeller = 0; LetterTeller < 12; LetterTeller++)
		 kleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[rteller][LetterTeller]);
	 kleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[rteller], LijstScore[rteller]);
	 kleurprintf(CYAN, BLUE, " � ");
	 if (SpelersNummer == rteller + 12)
	 {  achtergrondkleur = BLUE;
	    voorgrondkleur = WHITE;
	 }
	 else
	 {  achtergrondkleur = CYAN;
	    voorgrondkleur = BLUE;
	 }
	 kleurprintf(achtergrondkleur, voorgrondkleur, "%2d. ", rteller + 12);
	 for (LetterTeller = 0; LetterTeller < 12; LetterTeller++)
		 kleurprintf(achtergrondkleur, voorgrondkleur, "%c", LijstNaam[rteller + 12][LetterTeller]);
	 kleurprintf(achtergrondkleur, voorgrondkleur, " ( %2d)   %5ld", LijstLevel[rteller + 12], LijstScore[rteller + 12]);
   }
   do
   {  ch = getch();
   }
   while (ch != 27 && ch != 13) ;
   if (ch == 27) Einde(0);
}


void AlleGroenenWeg(void)
{  for (int XasTeller = 0; XasTeller <= 38; XasTeller++)
   {  for (int YasTeller = 0; YasTeller <= 21; YasTeller++)
	 {  if (TabelXY[XasTeller][YasTeller] == GROENVOEDSEL)
	    {  TabelXY[XasTeller][YasTeller] = 0;
		  gotoxy((XasTeller * 2) + 2, YasTeller + 3);
		  kleurprintf(BLACK, BLACK, "  ");
	    }
	 }
   }
}

void BonusCheck(void)
{     if (BonusStap >= BonusAfstand)
	 {  TabelXY[BonusX][BonusY] = 0;
	    gotoxy((BonusX * 2) + 2, BonusY + 3);
	    kleurprintf(BLACK, BLACK, "  ");
	    BonusAfstand = 0;
	    Bonus = 0;
	    if (Energie > 1) Energie--;
	    else AlleGroenenWeg();
	    DrukAfEenB();
	 }
}

void main(void)
{  int Stap = 0;
   _setcursortype(_NOCURSOR);
   randomize();
   Proloog();
   while(1)
   {  SchermOpbouw();
	 BeginInstellingen();
	 BeginVoedsel();
	 while (EindeSpel == FALSE)
	 {  delay(100 - AlVerstrekenTijd); Geluid(3000); delay(1); nosound();
	    AlVerstrekenTijd = 0;
	    if (bioskey(1)) ToetsVerwerking();
	    HerdrukSlang();
	    if (EindeSpel == FALSE)
	    {  if (Stap % Level == 0 && Stap > 3)
		  {  RandomGebeurtenis();
			Stap = 0;
		  }
		  Stap ++;
		  BonusStap++;
		  if (BonusAfstand > 0) BonusCheck();
	    }
	 }
	 DrukAfScorelijst();
   }
}
