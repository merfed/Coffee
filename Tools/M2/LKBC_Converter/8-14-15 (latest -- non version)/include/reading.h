/**
 * @file
 * Reading functions prototypes
 */

#ifndef READING_H
#define READING_H

#include <stdio.h>
#include "lk_model.h"
#include "bc_model.h"

int read_skins(FILE **skin_files, Skin **ptr, int n);
int read_model(FILE *lk_m2_file, LKM2 *ptr);
int read_model_bc(FILE *bc_m2_file, BCM2 *ptr);

#endif
