
#ifndef __SYSUTILS_H
#define __SYSUTILS_H

#include "types.h"

// ========== Registre d'état en $26A: [OricANu p. 59]
// bit 0 : afficher/cacher le curseur
// bit 3 : désactiver/activer le son des touches
extern uchar *status_reg; // defined in sysutils.c

// NB: utilisation  de #defines au lieu de fonctions, plus efficace
// (la 'fonction "inline"' du pauvre, en quelque sorte...)

#define show_cursor()              (*status_reg |= 1)       // show cursor: set   bit 0 @ 0x26A
#define hide_cursor()              (*status_reg &= 0xFE)    // hide cursor: clear bit 0 @ 0x26A
#define enable_keyboard_sound()    (*status_reg &= 247)     // enable  keyboard sound: clear bit 3 @ 0x26A
#define disable_keyboard_sound()   (*status_reg |= 8)       // disable keyboard sound: set   bit 3 @ 0x26A

#endif // __SYSUTILS_H
