#ifndef PRINTER_H
#define PRINTER_H
#include "bc_model.h"
#include "lk_model.h"


void printBits(size_t const size, void const * const ptr);

//Wrath of the Lich King
void print_anims_lk(LKM2 model);
void print_skin(Skin skin);
void print_transparency_lk(LKM2 model);
void print_events_lk(LKM2 model);
void print_bones_lk(LKM2 model);
void print_vertices_lk(LKM2 model);
void print_texnames_lk(LKM2 model);
void print_boneheaders_lk(LKM2 model);

//The Burning Crusade
void print_anims_bc(BCM2 model);
void print_views(BCM2 model);
void print_transparency_bc(BCM2 model);
void print_events_bc(BCM2 model);
void print_bones_bc(BCM2 model, char flags);
void print_vertices_bc(BCM2 model);
void print_texnames_bc(BCM2 model);
void print_boneheaders_bc(BCM2 model);
void print_cameras_bc(BCM2 model);
void print_animlookup_bc(BCM2 model);
void print_playanimlookup_bc(BCM2 model);
void print_attachments_bc(BCM2 model);
void print_attachlookup_bc(BCM2 model);

#endif
