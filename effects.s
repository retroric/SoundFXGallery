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
;      Version: v0.2
;      Date: 16/01/19
:       
;      Historique:
;      -----------------------------------------------------------------------------------------
;      v0.2WIP - BUGS !!! 
;  =============================================================================================
 
; ---------------------- Données des effets sonores --------------------------------------------
; 
; Note: pour qu'un label soit exposé et accessible depuis un programme C linké avec le source ASM,
; le label doit être précédé d'un caractère de soulignement. Dans le code C, pour référencer ce label,
; on devra utiliser le nom du label SANS le caractère de soulignement initial

;;;.data	;;; <--- cause des erreurs, les sons ne fonctionnent plus ??!!

_metalcrush_fxtable
	.byt 32,134,250,96,0,0,0,0,0,0,31,7,16,16
_electricity_fxtable
    .byt 0,4,0,184,0,3,120,190,12,0,0,0,167,194
_helicopter_fxtable
    .byt 168,191,0,3,184,191,14,0,0,0,167,194,76,176
_helicopter2_fxtable
    .byt 206,108,231,36,137,112,70,182,170,239,83,246,12,165
_distant_helicopter_fxtable
    .byt 14,136,132,12,140,0,69,173,0,5,208,4,169,192
_engine_fxtable
    .byt 1,5,193,7,7,8,7,8,53,151,47,151,0,152
_planecockpit_fxtable
    .byt 150,0,152,46,4,255,255,112,7,80,0,102,5,65
_turboprop_fxtable
    .byt 32,2,140,0,5,132,14,136,132,12,140,0,69,173
_pong_fxtable
    .byt $EE, $02, $00, $00, $00, $00, $00, $3E, $10, $00, $00, $D0, $07, $00
_pchh_fxtable
    .byt $00, $00, $00, $00, $00, $00, $01, $37, $10, $00, $00, $D6, $0B, $00

;;;.align 2
_fx_data_vector_table:
	.word _metalcrush_fxtable, _electricity_fxtable
	.word _helicopter_fxtable, _helicopter2_fxtable
	.word _distant_helicopter_fxtable, _engine_fxtable
	.word _planecockpit_fxtable, _turboprop_fxtable
	.word _pong_fxtable, _pchh_fxtable

	
; ---------------------- Routines de lecture en ASM --------------------------------------------
;;;.text

_play_soundfx2
	; routine ASM déclarée en C comme: extern void  play_soundfx2(const char *psg_data);
	; NB: pour les appels de fonction le compilateur C passe les paramètres sur la pile
	; => il faut dépiler l'adresse "HHLL" passée sur la pile
	; normalement (sp) doit contenir HH et (sp+1) doit contenir LL.
	ldy #0
	lda (sp),y
	sta $14
	tax
	iny

	lda (sp),y
	sta $15

	php
	sei
	ldy #0 ; indiquer paramètre 0
	jmp load_psg_param_value


; routine de lecture générique
; Entrée: X: poids fort adresse données sonores (14 octets pour les 14 registres du PSG)
;         Y: poids faible adresse données sonores (14 octets pour les 14 registres du PSG)
; 	adaoté de la routine en $FA86 pour l'Atmos - cf. L'Oric A Nu pp. 374-375
_play_soundfx_entrypoint
	php	; sauver registre d'état
	sei	; et inhiber IRQ
	stx $14 ; sauver adresse données dans (XY) dans ($14-$15)
	sty $15 ;
	ldy #00 ; indiquer paramètre 0
load_psg_param_value:
	lda ($14),y 		; prendre la valeur du paramètre de n° y 
						; (l'adresse de départ des valeurs en en $14-$15, et on lui rajoute "y" pour 
						; accéder à l'octet contenant la valeur du registre n° "y" à positionner
	tax         		; sauve la valeur à positionner dans X
	tya         		; sauver n° du registre dans A
	pha 				; sauver A 'n° de registre)'
	jsr set_psg_param 	; positionner le paramètre sur le PSG
	pla 				; récupérer A (n° de registre)
	tay 				; et le mettre dans Y
	iny 				; et incrémenter y pour passer au n° de registre suivant
	cpy #$0E 			; si Y vaut 14, c'est qu'on a transféré les 14 registres
	bne load_psg_param_value
	plp 				; FiN: récupérer registre d'état sauvegardé
	rts 				; et sortir (NB: on a restauré le registre P, 
						; donc l'état des interruptions (flag I) avant l'appel...)

set_psg_param:
; 	adapté de la routine en $F590 pour l'Atmos - cf. L'Oric A Nu pp. 345-346
;   "ROUTINE D'E/S AVEC LE PSG 8912"
php 					; sauver P car on va toucher à I
sei 					; interdire IRQ (problèmes de timing) 
						; -- NB: DEJA fait lors de l'appel routine $FA86 / _play_soundfx
sta $030F 				; mettre le n° du registre à modifier sur le port
						; E/S avec le VIA 6522
tay 					; n° de registre dans Y
txa 					; et valeur à positionner pour ce registre dans A
cpy #$07 				; est-ce le registre d'autorisation ?
bne skip_psg_auth_reg 	; non, sauter
ora #$40 				; oui, forcer le port A en sortie
skip_psg_auth_reg:
pha 					; sauver la donnée sur la pile
lda $30c
ora #$EE 				; positionner CA2 et CB2 à 1
sta $030c 				; indiquer n° de reigstre
and #$11				; garder les sens de transition
ora #$cc 				; positionner CA2 et CB2 à 0
sta $030c 				; validation du n° de registre
tax 					; on sauve la valeur de PCR dans X
pla 					; on récupère la donnée
sta $030f 				; et on la met sur le port
txa
ora #$ec 				; positionner CA2 à 1 et CB2 à 0
sta $30c 				; indiquer donnée
and #$11
ora #$cc 				; positionner CA2 et CB2 à 0
sta $030c 				; valider la donnée
plp 					; récupérer registre d'état
rts 					; et sortir


; -- Routines de lecture individuelles (aucun intérêt)
; -- NOTE: routine utilisée pour Oric ATMOS $FA86: ENVOYER 14 PARAMETRES AU PSG AY-3-8912
;          (cf. L'Oric A Nu, pp. 374-375)
; -- NB:   pour l'Oric 1, l'adresse de cette routine est $FA6C

_play_metalcrush_fx	
	ldx #<_metalcrush_fxtable
	ldy #>_metalcrush_fxtable
	jmp $FA86

_play_electricity_fx
	ldx #<_electricity_fxtable
	ldy #>_electricity_fxtable
	jmp $FA86

_play_helicopter_fx
	ldx #<_helicopter_fxtable
	ldy #>_helicopter_fxtable
	jmp $FA86

_play_helicopter2_fx
	ldx #<_helicopter2_fxtable
	ldy #>_helicopter2_fxtable
	jmp $FA86

_play_distant_helicopter_fx
	ldx #<_distant_helicopter_fxtable
	ldy #>_distant_helicopter_fxtable
	jmp $FA86

_play_engine_fx
	ldx #<_engine_fxtable
	ldy #>_engine_fxtable
	jmp $FA86

_play_planecockpit_fx
	ldx #<_planecockpit_fxtable
	ldy #>_planecockpit_fxtable
	jmp $FA86

_play_turboprop_fx
	ldx #<_turboprop_fxtable
	ldy #>_turboprop_fxtable
	jmp $FA86

_play_pong_fx
	ldx #<_pong_fxtable
	ldy #>_pong_fxtable
	jmp $FA86

_play_pchh_fx
	ldx #<_pchh_fxtable
	ldy #>_pchh_fxtable
	jmp $FA86
