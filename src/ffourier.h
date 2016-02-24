/*
 * Program:     Period98
 *
 * File:        ffourier.h
 * Purpose:     definition of labels 
 *              for the fourier part 
 *              of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "fgeneral.h"

// the protocol part

// save fourier

#define PROTOCOL_SAVE_FOURIER "Saved fourier-data with the title %s\n"\
                              "to file: %s"

// fourier-calculations

#define PROTOCOL_FOURIER_CALC_CUSTOMQUALITY "Fourier calculation\n"\
                                            "Input:\n"\
                                            "Title:          %s\n"\
                                            "From frequency: "FORMAT_FREQUENCY"\n"\
                                            "To frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_FOURIER_CALC_PRESETQUALITY "Fourier calculation\n"\
                                            "Input:\n"\
                                            "Title:          %s\n"\
                                            "From frequency: "FORMAT_FREQUENCY"\n"\
                                            "To frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Step quality:   %s\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_FOURIER_WEIGHTED " weighted"

#define PROTOCOL_FOURIER_CALC_USE_ORIG     "Using the %i%s original measurements"
#define PROTOCOL_FOURIER_CALC_USE_ADJUSTED "Using the %i%s original measurements"
#define PROTOCOL_FOURIER_CALC_USE_WINDOW   "Using the %i%s points for the spectral window"
#define PROTOCOL_FOURIER_CALC_USE_RESORIG  "Using the %i%s residuals at the original\n"\
                                           "measurements with %i frequencies prewithened"
#define PROTOCOL_FOURIER_CALC_USE_RESADJU  "Using the %i%s residuals at the adjusted\n"\
                                           "measurements with %i frequencies prewithened"

#define PROTOCOL_FOURIER_SUBTRACT_ZERO     "A zeropoint of "FORMAT_AMPLITUDE \
                                           " has ben subtracted."

#define PROTOCOL_FOURIER_CALC_PEAK_POINTS  "The peak-only spectra contains %i points."
#define PROTOCOL_FOURIER_CALC_POINTS       "The spectra contains %i points."

#define PROTOCOL_FOURIER_HIGHEST_AMPLITUDE "The following amplitude is the highest:\n"\
                                           "Frequency: "FORMAT_FREQUENCY"\n"\
                                           "Amplitude: "FORMAT_AMPLITUDE"\n"

#define PROTOCOL_FOURIER_INCLUDE           "and has been included as frequency %i"

// the noise spectra calculations
#define PROTOCOL_NOISESPEC_CALC_CUSTOMQUALITY "Noise spectrum calculation\n"\
                                            "Input:\n"\
                                            "From frequency: "FORMAT_FREQUENCY"\n"\
                                            "To frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Steps:          "FORMAT_FREQUENCY"\n"\
                                            "Boxsize:        "FORMAT_FREQUENCY"\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_NOISESPEC_CALC_PRESETQUALITY "Noise spectrum calculation\n"\
                                            "Input:\n"\
                                            "From frequency: "FORMAT_FREQUENCY"\n"\
                                            "To frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Steps:          "FORMAT_FREQUENCY"\n"\
                                            "Boxsize:        "FORMAT_FREQUENCY"\n"\
                                            "Step quality:   %s\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_NOISESPEC_HEAD   "Frequency        Noise"
#define PROTOCOL_NOISESPEC_FORMAT FIXEDFORMAT_FREQUENCY"  "FIXEDFORMAT_AMPLITUDE

// the noise spectra calculations
#define PROTOCOL_NOISE_CALC_CUSTOMQUALITY "Noise calculation\n"\
                                            "Input:\n"\
                                            "At frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Boxsize:        "FORMAT_FREQUENCY"\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_NOISE_CALC_PRESETQUALITY "Noise calculation\n"\
                                            "Input:\n"\
                                            "At frequency:   "FORMAT_FREQUENCY"\n"\
                                            "Boxsize:        "FORMAT_FREQUENCY"\n"\
                                            "Step quality:   %s\n"\
                                            "Step rate:      "FORMAT_FREQUENCY"\n"\
                                            "With the calculations started on: %s\n"

#define PROTOCOL_NOISE_RESULT "The noise is: "FORMAT_AMPLITUDE

//
// the dialog part
//

// Load fourier
#define DIALOG_LOAD_FOURIER_TITLE "Load fourier"

// Save fourier
#define DIALOG_SAVE_FOURIER_TITLE "Save fourier"

// Shall we subtract a zeropoint

#define DIALOG_GENERAL_SHALL_SUBTRACT_ZERO \
                    "requires that the data\n"\
                    "does not contain a zero point shift.\n"\
                    "Otherwise additional features centered\n"\
                    "at frequency 0.0 will appear.\n"\
                    "These features may even dominate\n"\
                    "the whole frequency spectrum.\n\n"\
                    "Do you want to subtract the\n"\
                    "average zero point of "FORMAT_AMPLITUDE" ?"

#define DIALOG_FOURIER_SHALL_SUBTRACT_ZERO "Fourier analysis "\
                                           DIALOG_GENERAL_SHALL_SUBTRACT_ZERO

#define DIALOG_NOISE_SHALL_SUBTRACT_ZERO   "Noise calculation "\
                                           DIALOG_GENERAL_SHALL_SUBTRACT_ZERO

#define DIALOG_FOURIER_INCLUDE_FREQUENCY   "%s\nDo you want to include this frequency?"

// file extentions

#define DIALOG_FOURIER_EXTENTION  "fou"
#ifdef wx_msw
#define DIALOG_FOURIER_PATTERN    "Fourier files(*.fou)|*.fou|"ALL_FILE_PATTERN
#else
#define DIALOG_FOURIER_PATTERN    "*.fou"
#endif

// Quality definitions

#define QUALITY_FOURIER_LOW    "low"
#define QUALITY_FOURIER_MEDIUM "medium"
#define QUALITY_FOURIER_HIGH   "high"
#define QUALITY_FOURIER_CUSTOM "custom"
