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
 *      Version: v0.2
 *         Date: 16/01/19
 * 
 *      Historique:
 *      -----------------------------------------------------------------------------------------
 *      v0.2WIP - BUGS !!
 *  =============================================================================================
 */ 

//#define VERSION_STRING  "v0.1 - retroric - 15/01/19"
#define VERSION_STRING  "v0.2WIP -retroric-16/01/19"

#include <stdio.h>
#include <sys/graphics.h>

#include "types.h"
#include "sysutils.h"

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
void  play_soundfx(const char *psg_data);

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
extern void  play_soundfx_entrypoint();
extern void  play_soundfx2(const char *psg_data);

/* ================== VARIABLES GLOBALES ================== */

/* adresse contenant la dernière touche pressée du clavier */
uchar *keyb_norm_key_press_addr = (uchar *) 0x208;

// tableau de pointeurs vers des fonctions de type void sans arguments
#define FXCOUNT 10
void (*funcptrs[FXCOUNT])() = { 
    play_metalcrush_fx,         play_electricity_fx,
    play_helicopter_fx,         play_helicopter2_fx, 
    play_distant_helicopter_fx, play_engine_fx, 
    play_planecockpit_fx,       play_turboprop_fx,
    play_pong_fx,               play_pchh_fx 
};

// tableau de chaines de caractères des noms des effets sonores
const char *soundfx_names[] = { 
    "metalcrush",         "electricity", 
    "helicopter",         "helicopter 2", 
    "distant helicopter", "engine", 
    "plane cockpit",      "turboprop",
    "pong",               "pchh" 
};
// données sonores


extern const char   *metalcrush_fxtable,            *electricity_fxtable,
                    *helicopter_fxtable,            *helicopter2_fxtable,
                    *distant_helicopter_fxtable,    *engine_fxtable,
                    *planecockpit_fxtable,          *turboprop_fxtable,
                    *pong_fxtable,                  *pchh_fxtable;


// COMMENTED OUT: doesn't compile in C ("initializer must be constant" error)
// ==> REPLACED by _fx_data_vector_table in effects.s
/*
const char *soundfx_data[] = {   
    metalcrush_fxtable,         electricity_fxtable, 
    helicopter_fxtable,         helicopter2_fxtable, 
    distant_helicopter_fxtable, engine_fxtable, 
    planecockpit_fxtable,      turboprop_fxtable,
    pong_fxtable,               pchh_fxtable 
};
*/

extern const char *fx_data_vector_table[];


/* ================== IMPLEMENTATION DES FONCTIONS ================== */

void play_soundfx(const char *psg_data) {
    // NB: utilisation de "STATIC" pour éviter que les variables locales hh et ll
    // ne soient créées sur la pile, car cela interdiarait leur référencement dans
    // l'inline asm (les variables locales "auto"  étant créées sur la pile,
    // cela conduirait à une traduction en ASM avec un adressage du type "ldx #(fp),6",
    // qui n'est pas un adressage légal en 6502 car le framepointer "fp" n'est pas en page zéro... )
    // NB: ne fonctionne toujours pas avec "static" ça se traduit par:
    // "ldx #Lmain86;  ldy #Lmain85"
    // BIZARREMENT, en redéclarant simplement "uchar hh, ll;" sans les initialiser au début,
    // c'est "retombé en marche" pour l'asm "ldx #%ll; ldy #%hh;"" ??!!

    //static uchar hh, ll;
    uchar hh, ll;
    /*
    uchar i = 2;
    printf("uchar i=%x\n", i); // Bizarrement avec %x un uchar s'affiche sur 4 digits au lieu de 2 !!??
    */
    hh = (uchar) ( (((unsigned int)  psg_data) >> 8)  & 0xff);
    ll = (uchar) (  ((unsigned int)  psg_data)        & 0xff);
    printf("playsoundfx: hh=%x, ll=%x\n", hh, ll);
    
    // OPTION 1: YIELDS following error:
    //asm("ldx #%ll; ldy #%hh; jsr _play_soundfx_entrypoint");
    /*  Assembling
        ldx #(fp),6
        V:\EMULATION\ORIC\OSDK\PROJECTS\SoundFXGallery\main.s(102):  093c:Syntax error
        ldy #(fp),7
        V:\EMULATION\ORIC\OSDK\PROJECTS\SoundFXGallery\main.s(103):  093c:Syntax error
    */

   /* See OSDK/documentation/doc_assembler.htm:

    *       Before an expression you can use these unitary operators:
    *           <  Gives the low byte of the expression
    *           >  Gives the high byte
    */
    
    // OPTION 2: used to yeild another error, but now ok ??!!
    asm(    "php; pha; txa; pha; tya; pha;" // push registers (car la routine modifie A, X et Y)
                                            // et il est préférable de sauver P également
            //"ldx #<%psg_data; ldy #>%psg_data; "
            //"ldx #$f7; ldy #$13;" // sound data addr for metalcrush
            "ldx #%ll; ldy #%hh;" // finalement... c'est tombé en marche, avec variables locales ??!!
            "jsr $FA86;" //"jsr _play_soundfx_entrypoint;"
            "pla; tay; pla; tax; pla; plp;" // pop saved registers

        );
    /*
       <copy error message>
    */
}

/**
 * main(): Point d'entrée du programme
 */  
void main() {
    uchar i;
	//test_keys();
    cls(); paper(4); ink(2);
    hide_cursor();
    disable_keyboard_sound();
    display_title_screen();

    printf("----- STAGE 1: press SPACE -----\n");
    wait_spacekey();
    for(i = 0; i < FXCOUNT; i++) {
        printf("Now playing: %s\n", soundfx_names[i]);
        printf("routine addr = $%x\n", funcptrs[i]);
        printf("Press SPACE ");
        if(i < (FXCOUNT-1)) printf("for next sound FX\n\n");
        else                printf("to end\n\n");
        (*funcptrs[i])(); // appel de la fonction d'index i
        wait_spacekey();
    }

/********* STAGE 2: fonction play_soundfx() buggée, sons bizarres !!! ***********/
/**
    printf("----- STAGE 2: press SPACE -----\n");
    wait_spacekey();
    printf("playsoundfx() addr = $%x\n\n", play_soundfx);
    for(i = 0; i < FXCOUNT; i++) {
        printf("Now playing: %s\n", soundfx_names[i]);
        printf("fx data addr = $%x\n", fx_data_vector_table[i]);
        play_soundfx(fx_data_vector_table[i]);
        printf("Press SPACE ");
        if(i < (FXCOUNT-1)) printf("for next sound FX\n\n");
        else                printf("to end\n\n");
        wait_spacekey();
    }
**/

/********** STAGE 3: la fonction play_soundfx2() FONCTIONNE DESORMAIS !!! ************/
    printf("----- STAGE 3: press SPACE -----\n");
    printf("playsoundfx2() addr = $%x\n\n", play_soundfx2);
    wait_spacekey();
    for(i = 0; i < FXCOUNT; i++) {
        printf("Now playing: %s\n", soundfx_names[i]);
        printf("Press SPACE ");
        play_soundfx2(fx_data_vector_table[i]);
        if(i < (FXCOUNT-1)) printf("for next sound FX\n\n");
        else                printf("to end\n\n");
        wait_spacekey();
    }

	// End: show the cursor and quit
    show_cursor();
    enable_keyboard_sound();
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
