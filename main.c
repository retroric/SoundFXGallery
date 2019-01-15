/**
 *      SoundFXGallery
 *      ==============
 * 
 *      Galerie d'effets sonores sur Oric, en C et ASM
 *      par laurentd75, Jan. 2019
 * 
 *      Effets sonores et routine/définitions ASM par LadyWasky
 *      cf. http://forums.oric.org/t638-musique-en-c#8085
 *
 *      Version: v0.1
 *         Date: 15/01/19
 * 
 *      Historique:
 *      -----------------------------------------------------------------------------------------
 *      v0.1    - première version
 *  =============================================================================================
 */ 

#define VERSION_STRING  "v0.1 - retroric - 15/01/19"

#include <stdio.h>
#include <sys/graphics.h>


/* ================== TYPES ================== */
typedef unsigned char uchar;
typedef unsigned char bool; // boolean


/* ================== CONSTANTES ================== */

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef FALSE
#define FALSE ((bool) 0)
#define TRUE  (!FALSE)
#endif

// Adresse de début de l'écran TEXT
#define TEXT_SCREEN 0xBB80

// Dimensions de l'écran
#define SCREEN_WIDTH  40
#define SCREEN_HEIGHT 28


// Scan codes des touches du clavier
#define KEY_SPACE 132
#define KEY_ESC   169

#define NO_KEY 0x38 // NO KEY PRESSED


/* ================== DECLARATION DES FONCTIONS (PROTOTYPES) ================== */
void  display_title_screen();
void  wait_spacekey();
void  test_keys();
void  hide_cursor();
void  show_cursor();

/* ================== DECLARATION DES FONCTIONS ASSEMBLEUR (PROTOTYPES) ================== */
extern void  play_metalcrush_fx();
extern void  play_electricity_fx();
extern void  play_helicopter_fx();
extern void  play_helicopter2_fx();
extern void  play_distant_helicopter_fx();
extern void  play_pchh_fx();
extern void  play_engine_fx();
extern void  play_planecockpit_fx();
extern void  play_turboprop_fx();
extern void  play_pong_fx();
extern void  play_pchh_fx();

/* ================== VARIABLES GLOBALES ================== */

/* adresse contenant la dernière touche pressée du clavier */
uchar *keyb_norm_key_press_addr = (uchar *) 0x208;

// tableau de pointeurs vers des fonctions de type void sans arguments
#define FXCOUNT 10
void (*funcptrs[FXCOUNT])() = { play_metalcrush_fx,         play_electricity_fx,
                                play_helicopter_fx,         play_helicopter2_fx, 
                                play_distant_helicopter_fx, play_engine_fx, 
                                play_planecockpit_fx,       play_turboprop_fx,
                                play_pong_fx,               play_pchh_fx };

// tableau de chaines de caractères des noms des effets sonores
char *soundfx_names[]       = { "metalcrush",         "electricity", 
                                "helicopter",         "helicopter 2", 
                                "distant helicopter", "engine", 
                                "plane cockpit",      "turboprop",
                                "pong",               "pchh" };

/* ================== IMPLEMENTATION DES FONCTIONS ================== */

/**
 * main(): Point d'entrée du programme
 */  
void main() {
    uchar i;
	//test_keys();

    cls(); paper(4); ink(2);

    hide_cursor();

    display_title_screen();

    for(i = 0; i < FXCOUNT; i++) {
        printf("Now playing: %s\n", soundfx_names[i]);
        printf("Press SPACE ");
        if(i < (FXCOUNT-1)) printf("for next sound FX\n\n");
        else                printf("to end\n\n");
        (*funcptrs[i])(); // appel de la fonction d'index i
        wait_spacekey();
    }

	// End: show the cursor and quit
    show_cursor();
}



/** 
 * display_title_screen(): affichage d'un écran titre
 */
void display_title_screen() {
	text();
	cls(); paper(4); ink(3);

	printf("\n\n");    
    printf("------------------------------\n");
    printf("-       SoundFXGallery       -\n");
    printf("------------------------------\n");
	printf("-  Galerie d'effets sonores  -\n");
	printf("-    (C + assembleur 6502)   -\n");
    printf("- "); printf(VERSION_STRING); printf(" -\n");
	//printf("- v0.1 - retroric - 14/01/19 -\n");
    printf("------------------------------\n");
    printf("\n");
    printf("Press SPACE to begin\n\n");
    wait_spacekey();
}

/** 
 * Attend une pression sur la touche ESPACE 
 */
void wait_spacekey() {
    uchar key = NO_KEY;
    // wait for space key pressed
	for(; key != KEY_SPACE;) {
		key = *keyb_norm_key_press_addr;
	}
    // wait for key release
	for(; key != NO_KEY;) {
		key = *keyb_norm_key_press_addr;
	}
}



// ======================================================================
// Test Keys: display scan code of pressed key (press SPACE to exit test)
// ======================================================================
void test_keys() {
    unsigned char *keyb_norm_key_press_addr = (unsigned char *) 0x208;
    unsigned char key      = NO_KEY; // 0x38 = NO KEY PRESSED
    unsigned char prev_key = NO_KEY; // 0x38 = NO KEY PRESSED
    
    printf("Keypress test -- press SPACE to end\n");

    for(; key != KEY_SPACE;) { // loop until SPACE is pressed to quit
        key = *keyb_norm_key_press_addr;
        if(key != NO_KEY) {	// 0x38 = NO KEY PRESSED
            if(key != prev_key) {
                // Do not log repeat key presses
                printf("Key pressed: dec=%d, hex=0x%x\n", key, key);
                prev_key = key;
            }
        }
    }  
}

/**
 * hide_cursor(): cache le curseur 
 */ 
void hide_cursor() {
	// Hide the cursor: clear bit 0 @ 0x26A
	uchar *addr = (uchar *) 0x26A;
	*addr &= 0xFE;
}

/**
 * hide_cursor(): montre le curseur 
 */ 
void show_cursor() {
	// End game: show cursor: set bit 0 @ 0x26A
	uchar *addr = (uchar *) 0x26A;
	*addr |= 1;
}
