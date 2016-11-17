/**
 * @file
 * Main file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printer.h"
#include "reading.h"
#include "conversion.h"
#include "writing.h"
#include "common.h"
#include "fcaseopen.h"
#include "colors.h"

/** [Option] Turns the converter in a tool to analyze events in a BC model and write unknown IDs.
 * File id_list.txt will be written if new unknown IDs have been discovered.
 * 1 = ON, 0 = OFF.
 */
#define ANALYZER_MODE 0
/** Skin file extension */
#define SKIN_SUFF ".skin"
/** M2 file extension */
#define M2_EXT ".m2"

/**
 * @return The name of a skin file
 * Example : skin_name("Test.m2", 2) returns "Test02.skin"
 * @param m2_name A string usually given as an argument to the program. Example : "Test.m2"
 * @param number The skin file number.
 * @author ghuysmans
 */
char* skin_name(char *m2_name, int number) {
	size_t m2_name_length = strlen(m2_name);
	char skin_number[2];
	sprintf(skin_number, "%d", number);
	char *s = malloc(
			m2_name_length - strlen(M2_EXT) + 2 + strlen(SKIN_SUFF) + 1);
	char *p = s + m2_name_length - 3;
	strcpy(s, m2_name);
	strcpy(p, "0");
	p++;
	strcpy(p, skin_number);
	p++;
	strcpy(p, SKIN_SUFF);
	return s;
}

char* final_name(char *name) {
	size_t name_length = strlen(name);
	char *s = malloc(name_length + strlen("_BC.m2") + 1 + 16);
	strcpy(s, name);
	if (classic > 0) {
		strcat(s, "_CL.m2");
	} else {
		strcat(s, "_BC.m2");
	}
	return s;
}

void show_help() {
	fprintf(stderr,
			"Usage : lkbc_converter [OPTIONS] <model name> <target name>\n");
	fprintf(stderr, " -t\tShow the textures your model need to work.\n");
	fprintf(stderr,
			" -c\tSet the converter target to Classic instead of The Burning Crusade.\n");
	fprintf(stderr, " Developer options :\n");
	fprintf(stderr, " -a\tShow animations data.\n");
	fprintf(stderr, " -b\tShow bones data.\n");
	fprintf(stderr, " -e\tShow events data.\n");
}
/**
 * Main function
 */
int main(int argc, char *argv[]) {
	printf("[[ LKBC_Converter by Koward v0.5-alpha ]]\n");
	char *m2_name = 0;
	char *target_name = 0;
	short show_anims = 0;
	short show_bones = 0;
	short show_events = 0;
	short show_textures = 0;
	classic = 0;
	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 't':
				show_textures = 1;
				break;
			case 'c':
				classic = 1;
				break;
			case 'a':
				show_anims = 1;
				break;
			case 'b':
				show_bones = 1;
				break;
			case 'e':
				show_events = 1;
				break;
			default:
				fprintf(stderr, KYEL "[Warning] " RESET "Unknown option: %s\n",
						argv[i]);
			}
		} else { //M2 name or target name
			if (m2_name == 0) {
				m2_name = argv[i];
			} else {
				target_name = argv[i];
			}
		}
	}

	if (m2_name == NULL) {
		fprintf(stderr, KRED "[Error] " RESET "No M2 file specified.\n");
		show_help();
		exit(EXIT_FAILURE);
	}
	//Storing M2 name without .m2 extension
	size_t m2_name_length = strlen(m2_name);
	model_name = malloc(m2_name_length - 3 + 1);
	name_length = m2_name_length - 3;
	strncpy(model_name, m2_name, m2_name_length - 3);
	model_name[m2_name_length - 3] = 0;

	//ANALYZER MODE
	if (ANALYZER_MODE) {
		printf(KGRN "[Opening M2/BC file for analysis]\n" RESET);
		printf("\t%s\n", m2_name);
		FILE *m2_file = fcaseopen(m2_name, "r+b");
		if (m2_file == NULL) {
			fprintf(stderr, KRED "[Error] " RESET "M2 file %s not found.\n",
					m2_name);
			exit(EXIT_FAILURE);
		}
		BCM2 analyzed_model;
		read_model_bc(m2_file, &analyzed_model);
		analyze_events(analyzed_model);
		exit(EXIT_SUCCESS);
	}

	//Reading files
	printf(KCYN "[Opening M2/WotLK file]\n" RESET);
	printf("\t%s\n", m2_name);
	FILE *lk_m2_file = fcaseopen(m2_name, "r+b");
	if (lk_m2_file == NULL) {
		fprintf(stderr, KRED "[Error] " RESET "M2 file not found.\n");
		show_help();
		exit(EXIT_FAILURE);
	}

	LKM2 lk_model;
	read_model(lk_m2_file, &lk_model);

	FILE **skin_files;
	skin_files = malloc(lk_model.header.nViews * sizeof(FILE *));
	for (i = 0; i < lk_model.header.nViews; i++) {
		printf("\t%s\n", skin_name(m2_name, i));
		skin_files[i] = fcaseopen(skin_name(m2_name, i), "r+b");
		if (skin_files[i] == NULL) {
			fprintf(stderr,
			KRED "[Error] " RESET "SKIN/LK number %d not found.\n", i);
			exit(EXIT_FAILURE);
		}
	}

	printf("==> ");
	Skin *skins;
	read_skins(skin_files, &skins, lk_model.header.nViews);
	printf("Model successfully read.\n");
	for (i = 0; i < lk_model.header.nViews; i++) {
		fclose(skin_files[i]);
	}
	fclose(lk_m2_file);

	//Converting
	BCM2 bc_model;
	lk_to_bc(lk_model, skins, &bc_model);
	printf("Conversion complete.\n");

	//Printing
	if (show_anims > 0) {
		print_anims_bc(bc_model);
	}
	if (show_bones > 0) {
		print_bones_bc(bc_model, 7);
	}
	if (show_textures > 0) {
		print_texnames_bc(bc_model);
	}

	if (show_events > 0) {
		print_events_bc(bc_model);
	}

	/*
	 //Reads the genuine TBC file. Useful to compare the models.
	 FILE *genuine_m2_file = fcaseopen("UI_DwarfBC.m2", "r+b");
	 if (genuine_m2_file == NULL) {
	 fprintf(stderr,
	 "Debug file opening error.\nIf you have this error using a release version, please report issue on Github.\n");
	 exit(EXIT_FAILURE);
	 }
	 BCM2 genuine_model;
	 read_model_bc(genuine_m2_file, &genuine_model);
	 print_events_bc(genuine_model);
	 */

	//Writing
	char *new_name;
	if (target_name != 0) {
		new_name = target_name;
	} else {
		new_name = final_name(model_name);
	}
	if (classic > 0) {
		printf(KBLU "[Creating M2/CL file]\n" RESET);
	} else {
		printf(KGRN "[Creating M2/BC file]\n" RESET);
	}
	printf("\t%s\n", new_name);
	printf("==> ");
	FILE *bc_m2_file = fcaseopen(new_name, "w+b");
	if (bc_m2_file == NULL) {
		fprintf(stderr,
				KRED "[Error] " RESET "Can't write M2 file. Check system permissions or target name if specified.\n");
		exit(EXIT_FAILURE);
	}
	write_model(bc_m2_file, &bc_model);
	printf("Model successfully written.\n");
	fclose(bc_m2_file);
	return EXIT_SUCCESS;
}
