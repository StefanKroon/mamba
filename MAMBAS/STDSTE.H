#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
void OpenGrafischScherm(void);
void SluitGrafischScherm(void);
int kleurprintf(int achtergrond, int kleur, char *formatstring, ...);
void Einde(int eindcode, char *FoutMelding, ...);
void centreer(char *Invoerstring, ...);
void EffectHaalRegelWeg(int VanafRegel, int AantalRegels);
void EffectDrukRegelAf(int regel, char *Invoerstring, ...);
void Invoer(char *Uitvoer ,char *StandaardTekst);