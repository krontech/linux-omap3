/*
 * OMAP2/3 Power/Reset Management (PRM) register definitions
 *
 * Copyright (C) 2007-2009, 2011 Texas Instruments, Inc.
 * Copyright (C) 2008-2010 Nokia Corporation
 * Paul Walmsley
 *
 * Added TI86X PRM module offsets as most of the operations fit with OMAP3.
 * Hemant Pedanekar (hemantp@ti.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * The PRM hardware modules on the OMAP2/3 are quite similar to each
 * other.  The PRM on OMAP4 has a new register layout, and is handled
 * in a separate file.
 */
#ifndef __ARCH_ARM_MACH_OMAP2_PRM81XX_H
#define __ARCH_ARM_MACH_OMAP2_PRM81XX_H

#include "prcm-common.h"
#include "prm.h"

/* TI81XX PRM DEVICE offsets */
#define TI81XX_PRM_DEVICE_RSTCTRL			0x00A0

/* TI81XX specific register offsets from each PRM module base */
#define TI81XX_PM_PWSTCTRL_OFFSET		0x0000
#define TI81XX_PM_PWSTST_OFFSET 		0x0004
#define TI81XX_RM_RSTCTRL				0x0010
#define TI81XX_RM_RSTST					0x0014

/* TI81XX Graphcis register offsets. */
#define TI81XX_PM_SGX_PWSTCTRL_OFFSET   0x0000
#define TI81XX_RM_SGX_RSTCTRL_OFFSET    0x0004
#define TI81XX_PM_SGX_PWSTST_OFFSET     0x0010
#define TI81XX_RM_SGX_RSTST_OFFSET      0x0014

/* DDR power management control register */
#define TI814X_PMCR					0x0038
#define TI814X_DMM_LISA_MAP_0				0x0040
#define TI814X_DMM_LISA_MAP_1				0x0044
#define TI814X_DMM_LISA_MAP_2				0x0048
#define TI814X_DMM_LISA_MAP_3				0x004C
#define TI814X_DDR_PHY_CTRL				0x00EC
#define TI814X_EMIF_CLK_GATE				0x030C

/*
 * TI81XX common PRM module offsets
 */

#define TI81XX_PRM_DEVICE_MOD			0x0000	/* 256B */
#define TI81XX_PRM_ACTIVE_MOD			0x0a00	/* 256B */
#define TI81XX_PRM_GFX_MOD              0x0f00  /* 256B */
#define TI81XX_PRM_ALWON_MOD			0x1800	/* 1KB */

/* FIXME: TI814X: Same as ALWON2, older doc called this DEFAULT */
#define TI81XX_PRM_DEFAULT_MOD			0x0b00	/* 256B */

/*
 * TI816X PRM module offsets
 */

#define TI816X_PRM_OCP_SOCKET_MOD		0x0200	/* 256B */
#define TI816X_PRM_IVAHD0_MOD			0x0c00	/* 256B */
#define TI816X_PRM_IVAHD1_MOD			0x0d00	/* 256B */
#define TI816X_PRM_IVAHD2_MOD			0x0e00	/* 256B */

/*
 * TI814X PRM module offsets
 */

#define TI814X_REVISION_PRM_MOD			0x0200  /* 256B */
#define TI814X_PRM_ALWON2_MOD			0x0b00  /* 256B */
#define TI814X_PRM_HDVICP_MOD			0x0c00  /* 256B */
#define TI814X_PRM_ISP_MOD			    0x0d00  /* 256B */
#define TI814X_PRM_HDVPSS_MOD			0x0e00  /* 256B */

#endif
