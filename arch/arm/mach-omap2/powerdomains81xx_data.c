/*
 * TI81xx Power domains framework
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include "powerdomain.h"
#include "prcm-common.h"
#include "prm81xx.h"
#include "prcm44xx.h"

/*
 * TI81XX common
 */
/*  Always ON logic that sits in VDD_CORE voltage domain */
static struct powerdomain alwon_81xx_pwrdm = {
	.name			= "alwon_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_ALWON_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_ON,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
};

/*
 * A vaguely undocumented powerdomain that appears as PRM_ALWON2
 * in the 816x docs, and PRM_DEFAULT in the 814x. Seems to manage
 * the video, PCI and USB subsystems.
 */
static struct powerdomain default_81xx_pwrdm = {
	.name			= "default_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_DEFAULT_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
};

static struct powerdomain active_81xx_pwrdm = {
	.name			= "active_pwrdm",
	.voltdm			= { .name = "active" },
	.prcm_offs		= TI81XX_PRM_ACTIVE_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.pwrsts_mem_ret		= {
		[0]	= PWRSTS_OFF_RET,	/* active_mem */
	},
	.pwrsts_mem_on		= {
		[0]	= PWRSTS_ON,		/* active_mem */
	},
};

static struct powerdomain sgx_81xx_pwrdm = {
	.name			= "sgx_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_GFX_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF_RET,
	.pwrstctrl_offs		= TI81XX_PM_SGX_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_SGX_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.pwrsts_mem_ret		= {
		[0]	= PWRSTS_OFF_RET,	/* sgx_mem */
	},
	.pwrsts_mem_on		= {
		[0]	= PWRSTS_ON,		/* sgx_mem */
	},
};

/*
 * TI814X/DM385 only
 */
static struct powerdomain ivahd_814x_pwrdm = {
	.name			= "ivahd_pwrdm",
	.voltdm			= { .name = "hdvicp" },
	.prcm_offs		= TI814X_PRM_HDVICP_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.pwrsts_mem_ret		= {
		[0]	= PWRSTS_OFF_RET,	/* hdvicp_mem */
	},
	.pwrsts_mem_on		= {
		[0]	= PWRSTS_ON,		/* hdvicp_mem */
	},
};

/* Also referred to as PRM_DSS in the manual. */
static struct powerdomain hdvpss_814x_pwrdm = {
	.name			= "hdvpss_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI814X_PRM_HDVPSS_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.pwrsts_mem_ret		= {
		[0]	= PWRSTS_OFF_RET,	/* hdvpss_mem */
	},
	.pwrsts_mem_on		= {
		[0]	= PWRSTS_ON,		/* hdvpss_mem */
	},
};

static struct powerdomain isp_814x_pwrdm = {
	.name			= "isp_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI814X_PRM_ISP_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.banks			= 1,
	.pwrsts_mem_ret		= {
		[0]	= PWRSTS_OFF_RET,	/* isp_mem */
	},
	.pwrsts_mem_on		= {
		[0]	= PWRSTS_ON,		/* isp_mem */
	},
};

/*
 * TI816X only
 */
static struct powerdomain ivahd0_816x_pwrdm = {
	.name			= "ivahd0_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI816X_PRM_IVAHD0_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
};

static struct powerdomain ivahd1_816x_pwrdm = {
	.name			= "ivahd1_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI816X_PRM_IVAHD1_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
};

static struct powerdomain ivahd2_816x_pwrdm = {
	.name			= "ivahd2_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI816X_PRM_IVAHD2_MOD,
	.prcm_partition		= TI81XX_PRM_PARTITION,
	.pwrsts			= PWRSTS_OFF_ON,
	.pwrsts_logic_ret	= PWRSTS_OFF,
	.pwrstctrl_offs		= TI81XX_PM_PWSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_PWSTST_OFFSET,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
};

static struct powerdomain *powerdomains_ti81xx[] __initdata = {
	&alwon_81xx_pwrdm,
	&default_81xx_pwrdm,
	NULL
};

static struct powerdomain *powerdomains_dm385[] __initdata = {
	&ivahd_814x_pwrdm,
	&hdvpss_814x_pwrdm,
	&isp_814x_pwrdm,
	NULL
};

static struct powerdomain *powerdomains_ti814x[] __initdata = {
	&active_81xx_pwrdm,
	&sgx_81xx_pwrdm,
	&ivahd_814x_pwrdm,
	&hdvpss_814x_pwrdm,
	&isp_814x_pwrdm,
	NULL
};

static struct powerdomain *powerdomains_ti816x[] __initdata = {
	&active_81xx_pwrdm,
	&sgx_81xx_pwrdm,
	&ivahd0_816x_pwrdm,
	&ivahd1_816x_pwrdm,
	&ivahd2_816x_pwrdm,
	NULL
};

void __init ti81xx_powerdomains_init(void)
{
	if (!cpu_is_ti81xx())
		return;
	pwrdm_register_platform_funcs(&omap4_pwrdm_operations);
	pwrdm_register_pwrdms(powerdomains_ti81xx);

	if (cpu_is_ti816x())
		pwrdm_register_pwrdms(powerdomains_ti816x);
	else if (cpu_is_dm385())
		pwrdm_register_pwrdms(powerdomains_dm385);
	else if (cpu_is_ti814x())
		pwrdm_register_pwrdms(powerdomains_ti814x);
	else
		WARN(1, "TI81XX powerdomain init: unknown chip type\n");

	pwrdm_complete_init();
}
