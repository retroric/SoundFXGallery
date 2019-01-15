;
;      SoundFXGallery
;      ==============
;      Galerie d'effets sonores sur Oric, en C et ASM
;      par laurentd75, Jan. 2019
; 
;      Fichier: effects.s
;      Description:
;      Données des effets sonores + routine de lecture en ASM
;;     d'après un post sur le forum oric.org par LadyWasky
;      cf. http://forums.oric.org/t638-musique-en-c#8085
;
;      Version: v0.1
;      Date: 15/01/19
:       
;      Historique:
;      -----------------------------------------------------------------------------------------
;      v0.1    - première version
;  =============================================================================================
 
; ---------------------- Données des effets sonores --------------------------------------------
metalcrush_fxtable
	.byt 32,134,250,96,0,0,0,0,0,0,31,7,16,16
electricity_fxtable
    .byt 0,4,0,184,0,3,120,190,12,0,0,0,167,194
helicopter_fxtable
    .byt 168,191,0,3,184,191,14,0,0,0,167,194,76,176
helicopter2_fxtable
    .byt 206,108,231,36,137,112,70,182,170,239,83,246,12,165
distant_helicopter_fxtable
    .byt 14,136,132,12,140,0,69,173,0,5,208,4,169,192
engine_fxtable
    .byt 1,5,193,7,7,8,7,8,53,151,47,151,0,152
planecockpit_fxtable
    .byt 150,0,152,46,4,255,255,112,7,80,0,102,5,65
turboprop_fxtable
    .byt 32,2,140,0,5,132,14,136,132,12,140,0,69,173
pong_fxtable
    .byt $EE, $02, $00, $00, $00, $00, $00, $3E, $10, $00, $00, $D0, $07, $00
pchh_fxtable
    .byt $00, $00, $00, $00, $00, $00, $01, $37, $10, $00, $00, $D6, $0B, $00

; ---------------------- Routines de lecture en ASM --------------------------------------------
; -- NOTE: routine utilisée pour Oric ATMOS $FA86: ENVOYER 14 PARAMETRES AU PSG AY-3-8912
;          (cf. L'Oric A Nu, pp. 374-375)
; -- NB:   pour l'Oric 1, l'adresse de cette routine est $FA6C

_play_metalcrush_fx	
	ldx #<metalcrush_fxtable
	ldy #>metalcrush_fxtable
	jmp $FA86

_play_electricity_fx
	ldx #<electricity_fxtable
	ldy #>electricity_fxtable
	jmp $FA86

_play_helicopter_fx
	ldx #<helicopter_fxtable
	ldy #>helicopter_fxtable
	jmp $FA86

_play_helicopter2_fx
	ldx #<helicopter2_fxtable
	ldy #>helicopter2_fxtable
	jmp $FA86

_play_distant_helicopter_fx
	ldx #<distant_helicopter_fxtable
	ldy #>distant_helicopter_fxtable
	jmp $FA86

_play_engine_fx
	ldx #<engine_fxtable
	ldy #>engine_fxtable
	jmp $FA86

_play_planecockpit_fx
	ldx #<planecockpit_fxtable
	ldy #>planecockpit_fxtable
	jmp $FA86

_play_turboprop_fx
	ldx #<turboprop_fxtable
	ldy #>turboprop_fxtable
	jmp $FA86

_play_pong_fx
	ldx #<pong_fxtable
	ldy #>pong_fxtable
	jmp $FA86

_play_pchh_fx
	ldx #<pchh_fxtable
	ldy #>pchh_fxtable
	jmp $FA86
