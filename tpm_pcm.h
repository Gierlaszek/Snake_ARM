/******************************************************************************
 * This file is a part of the SM2 Tutorial (C).                               *                                                 *
 ******************************************************************************/

/**
 * @file tpm_pcm.h
 * @author Koryciak
 * @date Nov 2020
 * @brief File containing enums, structures and declarations for TPM.
 * @ver 0.1
 */

#ifndef TPM_PCM_H
#define TPM_PCM_H

#include "MKL05Z4.h"

/**
 * @brief TPM0 initialization. PCM.
 */
void TPM0_Init_PCM(void);
/**
 * @brief Play wave once.
 *
 */
void TPM0_PCM_Play(void);

#endif /* TPM_PCM_H */
