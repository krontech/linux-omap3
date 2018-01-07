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
 * Note that 814x seems to have HWSUP_SWSUP for many clockdomains
 * while 816x does not. According to the TRM, 816x only has HWSUP
 * for ALWON_L3_FAST. Also note that the TI tree clockdomains81xx.h
 * seems to have the related ifdef the wrong way around claiming
 * 816x supports HWSUP while 814x does not. For now, we only set
 * HWSUP for ALWON_L3_FAST as that seems to be supported for both
 * dm814x and dm816x.
 */

/* Common for 81xx */

static struct clockdomain alwon_l3_slow_81xx_clkdm = {
	.name		= "alwon_l3s_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs = TI81XX_CM_ALWON_L3_SLOW_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain alwon_l3_med_81xx_clkdm = {
	.name		= "alwon_l3m_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_MED_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain alwon_l3_fast_81xx_clkdm = {
	.name		= "alwon_l3f_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_FAST_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain alwon_ethernet_81xx_clkdm = {
	.name		= "alwon_ethernet_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ETHERNET_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain alwon_mpu_81xx_clkdm = {
	.name		= "alwon_mpu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ALWON_MPU_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain mmu_81xx_clkdm = {
	.name		= "mmu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_MMU_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain mmu_cfg_81xx_clkdm = {
	.name		= "mmu_cfg_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_MMUCFG_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* Active/DSP clock domain */
static struct clockdomain active_gem_81xx_clkdm = {
	.name		= "active_gem_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI81XX_CM_ACTIVE_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ACTIVE_GEM_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

/* SGX Graphics clock domain */
static struct clockdomain sgx_81xx_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI81XX_CM_SGX_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_SGX_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_pcie_81xx_clkdm = {
	.name		= "default_pcie_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_DEFAULT_PCI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* Ducati/Media Controller clock domain */
static struct clockdomain default_ducati_81xx_clkdm = {
	.name		= "default_ducati_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_DEFAULT_DUCATI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

/* USB clock domain */
static struct clockdomain default_usb_81xx_clkdm = {
	.name		= "default_usb_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_DEFAULT_USB_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/*
 * Default L3 clock registers aren't documented in the 81xx
 * TRM, so it's not entirely clear where these should go
 */
static struct clockdomain default_l3_slow_81xx_clkdm = {
	.name		= "default_l3s_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_SLOW_CLKDM,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain default_l3_med_81xx_clkdm = {
	.name		= "default_l3m_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_DEFAULT_L3_MED_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

/*
 * TI814X only
 */

/* OCMC clock domain */
static struct clockdomain alwon_ocmc_814x_clkdm = {
	.name		= "alwon_ocmc_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI81XX_CM_ALWON_OCMC_0_CLKSTCTRL_OFFSET,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd_814x_clkdm = {
	.name		= "ivahd_clkdm",
	.pwrdm		= { .name = "ivahd_pwrdm" },
	.cm_inst	= TI814X_CM_HDVICP_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI814X_CM_HDVICP_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* DSS clock domain */
static struct clockdomain hdvpss_814x_clkdm = {
	.name		= "hdvpss_clkdm",
	.pwrdm		= { .name = "hdvpss_pwrdm" },
	.cm_inst	= TI814X_CM_HDVPSS_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI814X_CM_HDVPSS_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* ISP clock domain */
static struct clockdomain isp_814x_clkdm = {
	.name		= "isp_clkdm",
	.pwrdm		= { .name = "isp_pwrdm" },
	.cm_inst	= TI814X_CM_ISP_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI814X_CM_ISP_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/* SATA clock domains */
static struct clockdomain default_sata1_dm385_clkdm = {
	.name		= "default_sata1_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= DM385_CM_DEFAULT_SATA1_CKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_SWSUP | CLKDM_NO_AUTODEPS),
};

/*
 * TI816X only
 */

static struct clockdomain ivahd0_816x_clkdm = {
	.name		= "ivahd0_clkdm",
	.pwrdm		= { .name = "ivahd0_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD0_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI816X_CM_IVAHD0_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd1_816x_clkdm = {
	.name		= "ivahd1_clkdm",
	.pwrdm		= { .name = "ivahd1_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD1_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI816X_CM_IVAHD1_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain ivahd2_816x_clkdm = {
	.name		= "ivahd2_clkdm",
	.pwrdm		= { .name = "ivahd2_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD2_MOD,
	.prcm_partition = TI81XX_PRM_PARTITION,
	.clkdm_offs	= TI816X_CM_IVAHD2_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= (CLKDM_CAN_HWSUP_SWSUP | CLKDM_NO_AUTODEPS),
};

static struct clockdomain *clockdomains_ti81xx[] __initdata = {
	&alwon_mpu_81xx_clkdm,
	&alwon_l3_slow_81xx_clkdm,
	&alwon_l3_med_81xx_clkdm,
	&alwon_l3_fast_81xx_clkdm,
	&alwon_ethernet_81xx_clkdm,
	&mmu_81xx_clkdm,
	&mmu_cfg_81xx_clkdm,
	&default_l3_slow_81xx_clkdm,
	&default_l3_med_81xx_clkdm,
	&default_pcie_81xx_clkdm,
	&default_usb_81xx_clkdm,
	&default_ducati_81xx_clkdm,
	&sgx_81xx_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_ti814x[] __initdata = {
	&alwon_ocmc_814x_clkdm,
	&active_gem_81xx_clkdm,
	&ivahd_814x_clkdm,
	&hdvpss_814x_clkdm,
	&isp_814x_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_dm385[] __initdata = {
	&default_sata1_dm385_clkdm,
	NULL,
};

static struct clockdomain *clockdomains_ti816x[] __initdata = {
	&active_gem_81xx_clkdm,
	&ivahd0_816x_clkdm,
	&ivahd1_816x_clkdm,
	&ivahd2_816x_clkdm,
	NULL,
};

void __init ti81xx_clockdomains_init(void)
{
	clkdm_register_platform_funcs(&ti81xx_clkdm_operations);
	clkdm_register_clkdms(clockdomains_ti81xx);
	if (cpu_is_ti814x())
		clkdm_register_clkdms(clockdomains_ti814x);
	if (cpu_is_dm385())
		clkdm_register_clkdms(clockdomains_dm385);
	if (cpu_is_ti816x())
		clkdm_register_clkdms(clockdomains_ti816x);
	clkdm_complete_init();
}
