/*
 * TI81XX Clock Domain data.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/io.h>

#include "prcm44xx.h"
#include "clockdomain.h"
#include "cm.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"

/*
 * Common TI81XX
 */

static struct clockdomain alwon_l3s_81xx_clkdm = {
	.name		= "alwon_l3_slow_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_SLOW_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain alwon_ethernet_81xx_clkdm = {
	.name		= "alwon_ethernet_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ETHERNET_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};


/* OCMC clock domain */
static struct clockdomain alwon_ocmc_814x_clkdm = {
	.name		= "alwon_ocmc_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI814X_CM_OCMC_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain mmu_81xx_clkdm = {
	.name		= "mmu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_MMU_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain mmu_cfg_81xx_clkdm = {
	.name		= "mmu_cfg_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_MMUCFG_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/*
 * TI814X only
 */

static struct clockdomain gem_814x_clkdm = {
	.name		= "gem_clkdm",
	.pwrdm		= { .name = "gem_pwrdm" },
	.cm_inst	= TI814X_CM_DSP_MOD,
	.clkdm_offs	= TI814X_CM_DSP_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd_814x_clkdm = {
	.name		= "ivahd_clkdm",
	.pwrdm		= { .name = "ivahd_pwrdm" },
	.cm_inst	= TI814X_CM_HDVICP_MOD,
	.clkdm_offs	= TI814X_CM_HDVICP_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* ISS is inside ISP */
static struct clockdomain alwon2_mc_814x_clkdm = {
	.name		= "alwon2_mc_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI814X_CM_ALWON2_MOD,
	.clkdm_offs	= TI814X_CM_ALWON2_MC_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* GFX clock domain */
static struct clockdomain sgx_814x_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI814X_CM_GFX_MOD,
	.clkdm_offs	= TI814X_CM_GFX_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* DSS clock domain */
static struct clockdomain hdvpss_814x_clkdm = {
	.name		= "hdvpss_clkdm",
	.pwrdm		= { .name = "hdvpss_pwrdm" },
	.cm_inst	= TI814X_CM_HDVPSS_MOD,
	.clkdm_offs	= TI814X_CM_HDVPSS_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* ISP clock domain */
static struct clockdomain isp_814x_clkdm = {
	.name		= "isp_clkdm",
	.pwrdm		= { .name = "isp_pwrdm" },
	.cm_inst	= TI814X_CM_ISP_MOD,
	.clkdm_offs	= TI814X_CM_ISP_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};


/* L3 med clock domain */
static struct clockdomain alwon2_l3_med_814x_clkdm = {
	.name		= "alwon2_l3_med_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI814X_CM_ALWON2_MOD,
	.clkdm_offs	= TI814X_CM_ALWON2_L3_MED_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* PCIe clock domain */
static struct clockdomain alwon2_pcie_814x_clkdm = {
	.name		= "alwon2_pcie_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI814X_CM_ALWON2_MOD,
	.clkdm_offs	= TI814X_CM_ALWON2_PCI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};


static struct clockdomain alwon2_sata1_dm385_clkdm = {
	.name		= "alwon2_sata1_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI814X_CM_ALWON2_MOD,
	.clkdm_offs	= DM385_CM_ALWON2_SATA1_CKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* USB clock domain */
static struct clockdomain alwon2_usb_814x_clkdm = {
	.name		= "alwon2_usb_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI814X_CM_ALWON2_MOD,
	.clkdm_offs	= TI814X_CM_ALWON2_L3_SLOW_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/*
 * TI816X only
 */

static struct clockdomain alwon_mpu_816x_clkdm = {
	.name		= "alwon_mpu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_MPU_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain active_gem_816x_clkdm = {
	.name		= "active_gem_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI816X_CM_ACTIVE_MOD,
	.clkdm_offs	= TI816X_CM_ACTIVE_GEM_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd0_816x_clkdm = {
	.name		= "ivahd0_clkdm",
	.pwrdm		= { .name = "ivahd0_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD0_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD0_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd1_816x_clkdm = {
	.name		= "ivahd1_clkdm",
	.pwrdm		= { .name = "ivahd1_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD1_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD1_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd2_816x_clkdm = {
	.name		= "ivahd2_clkdm",
	.pwrdm		= { .name = "ivahd2_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD2_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD2_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain sgx_816x_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI816X_CM_SGX_MOD,
	.clkdm_offs	= TI816X_CM_SGX_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_l3_med_816x_clkdm = {
	.name		= "default_l3_med_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_MED_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_ducati_816x_clkdm = {
	.name		= "default_ducati_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_DUCATI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_pcie_816x_clkdm = {
	.name		= "default_pcie_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_PCI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_usb_816x_clkdm = {
	.name		= "default_usb_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_SLOW_CLKDM, /* FIXME */
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain *clockdomains_ti81xx[] __initdata = {
	&alwon_mpu_816x_clkdm,
	&alwon_l3_slow_81xx_clkdm,
	&alwon_ethernet_81xx_clkdm,
	&mmu_81xx_clkdm,
	&mmu_cfg_81xx_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_ti814x[] __initdata = {
	&alwon_ocmc_814x_clkdm,
	&gem_814x_clkdm,
	&ivahd_814x_clkdm,
	&alwon2_mc_814x_clkdm,
	&sgx_814x_clkdm,
	&hdvpss_814x_clkdm,
	&isp_814x_clkdm,
	&alwon2_l3_med_814x_clkdm,
	&alwon2_pcie_814x_clkdm,
	&alwon2_usb_814x_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_dm385[] __initdata = {
	&alwon2_sata1_dm385_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_ti816x[] __initdata = {
	&active_gem_816x_clkdm,
	&ivahd0_816x_clkdm,
	&ivahd1_816x_clkdm,
	&ivahd2_816x_clkdm,
	&sgx_816x_clkdm,
	&default_l3_med_816x_clkdm,
	&default_ducati_816x_clkdm,
	&default_pcie_816x_clkdm,
	&default_usb_816x_clkdm,
	NULL,
};

void __init ti81xx_clockdomains_init(void)
{
	clkdm_register_platform_funcs(&am33xx_clkdm_operations);
	clkdm_register_clkdms(clockdomains_ti81xx);
	if (cpu_is_ti814x())clockdomains_ti814x
		clkdm_register_clkdms(clockdomains_ti814x);
	if (cpu_is_dm385())
		clkdm_register_clkdms(clockdomains_dm385);
	if (cpu_is_ti816x())
		clkdm_register_clkdms(clockdomains_ti816x);
	clkdm_complete_init();
}
