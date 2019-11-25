/*
 * omap_hwmod_ti81xx_data.c - hardware modules data for TI81XX chips
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
 *
 */

#include <linux/gpio.h>

#include <plat/omap_hwmod.h>
#include <mach/irqs.h>
#include <plat/cpu.h>
#include <plat/dma.h>
#include <plat/mmc.h>
#include <plat/mcspi.h>
#include <plat/i2c.h>
#include <plat/serial.h>
#include <plat/l4_3xxx.h>
#include <plat/ti81xx.h>

#include "omap_hwmod_common_data.h"

#include "common.h"
#include "control.h"
#include "cm81xx.h"
#include "wd_timer.h"
#include "cm-regbits-81xx.h"


/*
 * DM816X hardware modules integration data
 *
 * Note: This is incomplete and at present, not generated from h/w database.
 */

/*
 * Common alwon .clkctrl_offs from dm814x TRM "Table 2-278. CM_ALWON REGISTERS"
 * also dm816x TRM 18.7.17 CM_ALWON device register values minus 0x1400.
 */
#define DM81XX_CM_ALWON_MCASP0_CLKCTRL		0x140
#define DM81XX_CM_ALWON_MCASP1_CLKCTRL		0x144
#define DM81XX_CM_ALWON_MCASP2_CLKCTRL		0x148
#define DM81XX_CM_ALWON_MCBSP_CLKCTRL		0x14c
#define DM81XX_CM_ALWON_UART_0_CLKCTRL		0x150
#define DM81XX_CM_ALWON_UART_1_CLKCTRL		0x154
#define DM81XX_CM_ALWON_UART_2_CLKCTRL		0x158
#define DM81XX_CM_ALWON_GPIO_0_CLKCTRL		0x15c
#define DM81XX_CM_ALWON_GPIO_1_CLKCTRL		0x160
#define DM81XX_CM_ALWON_I2C_0_CLKCTRL		0x164
#define DM81XX_CM_ALWON_I2C_1_CLKCTRL		0x168
#define DM81XX_CM_ALWON_WDTIMER_CLKCTRL		0x18c
#define DM81XX_CM_ALWON_SPI_CLKCTRL		0x190
#define DM81XX_CM_ALWON_MAILBOX_CLKCTRL		0x194
#define DM81XX_CM_ALWON_SPINBOX_CLKCTRL		0x198
#define DM81XX_CM_ALWON_MMUDATA_CLKCTRL		0x19c
#define DM81XX_CM_ALWON_MMUCFG_CLKCTRL		0x1a8
#define DM81XX_CM_ALWON_CONTROL_CLKCTRL		0x1c4
#define DM81XX_CM_ALWON_GPMC_CLKCTRL		0x1d0
#define DM81XX_CM_ALWON_ETHERNET_0_CLKCTRL	0x1d4
#define DM81XX_CM_ALWON_L3_CLKCTRL		0x1e4
#define DM81XX_CM_ALWON_L4HS_CLKCTRL		0x1e8
#define DM81XX_CM_ALWON_L4LS_CLKCTRL		0x1ec
#define DM81XX_CM_ALWON_RTC_CLKCTRL		0x1f0
#define DM81XX_CM_ALWON_TPCC_CLKCTRL		0x1f4
#define DM81XX_CM_ALWON_TPTC0_CLKCTRL		0x1f8
#define DM81XX_CM_ALWON_TPTC1_CLKCTRL		0x1fc
#define DM81XX_CM_ALWON_TPTC2_CLKCTRL		0x200
#define DM81XX_CM_ALWON_TPTC3_CLKCTRL		0x204

/* Registers specific to dm814x */
#define DM814X_CM_ALWON_MCASP_3_4_5_CLKCTRL	0x16c
#define DM814X_CM_ALWON_ATL_CLKCTRL		0x170
#define DM814X_CM_ALWON_MLB_CLKCTRL		0x174
#define DM814X_CM_ALWON_PATA_CLKCTRL		0x178
#define DM814X_CM_ALWON_UART_3_CLKCTRL		0x180
#define DM814X_CM_ALWON_UART_4_CLKCTRL		0x184
#define DM814X_CM_ALWON_UART_5_CLKCTRL		0x188
#define DM814X_CM_ALWON_OCM_0_CLKCTRL		0x1b4
#define DM814X_CM_ALWON_VCP_CLKCTRL		0x1b8
#define DM814X_CM_ALWON_MPU_CLKCTRL		0x1dc
#define DM814X_CM_ALWON_DEBUGSS_CLKCTRL		0x1e0
#define DM814X_CM_ALWON_DCAN_0_1_CLKCTRL	0x218
#define DM814X_CM_ALWON_MMCHS_0_CLKCTRL		0x21c
#define DM814X_CM_ALWON_MMCHS_1_CLKCTRL		0x220
#define DM814X_CM_ALWON_MMCHS_2_CLKCTRL		0x224
#define DM814X_CM_ALWON_CUST_EFUSE_CLKCTRL	0x228

/* Registers specific to dm816x */
#define DM816X_DM_ALWON_BASE		0x1400
#define DM816X_CM_ALWON_TIMER_1_CLKCTRL	(0x1570 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_2_CLKCTRL	(0x1574 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_3_CLKCTRL	(0x1578 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_4_CLKCTRL	(0x157c - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_5_CLKCTRL	(0x1580 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_6_CLKCTRL	(0x1584 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_TIMER_7_CLKCTRL	(0x1588 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_SDIO_CLKCTRL	(0x15b0 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_OCMC_0_CLKCTRL	(0x15b4 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_OCMC_1_CLKCTRL	(0x15b8 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_ETHERNET_1_CLKCTRL (0x15d8 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_MPU_CLKCTRL	(0x15dc - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_SR_0_CLKCTRL	(0x1608 - DM816X_DM_ALWON_BASE)
#define DM816X_CM_ALWON_SR_1_CLKCTRL	(0x160c - DM816X_DM_ALWON_BASE)

/*
 * The default .clkctrl_offs field is offset from CM_DEFAULT, that's
 * TRM 18.7.6 CM_DEFAULT device register values minus 0x500
 */
#define DM81XX_CM_DEFAULT_OFFSET	0x500
#define DM81XX_CM_DEFAULT_USB_CLKCTRL	(0x558 - DM81XX_CM_DEFAULT_OFFSET)

static struct omap_hwmod ti81xx_mpu_hwmod;
static struct omap_hwmod ti81xx_alwon_l3s_hwmod;
static struct omap_hwmod ti81xx_alwon_l3m_hwmod;
static struct omap_hwmod ti81xx_alwon_l3f_hwmod;
static struct omap_hwmod ti81xx_l4_slow_hwmod;
static struct omap_hwmod ti81xx_l4_fast_hwmod;

static struct omap_hwmod_ocp_if ti81xx_alwon_l3_slow__l4_slow;
static struct omap_hwmod_ocp_if ti81xx_alwon_l3_med__l4_fast;
static struct omap_hwmod_ocp_if ti81xx_mpu__alwon_l3_slow;
static struct omap_hwmod_ocp_if ti81xx_mpu__alwon_l3_med;

/* L3 main interconnect */
static struct omap_hwmod ti81xx_l3_main_hwmod = {
	.name		= "l3_main",
	.class		= &l3_hwmod_class,
	.clkdm_name	= "alwon_l3_clkdm",
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= DM81XX_CM_ALWON_L3_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
};

/* L3 Interconnect entries clocked at 125, 250 and 500MHz */
static struct omap_hwmod_ocp_if *ti81xx_l3_slow_slaves[] = {
	&ti81xx_mpu__alwon_l3_slow,
};
static struct omap_hwmod_ocp_if *ti81xx_l3_slow_masters[] = {
	&ti81xx_alwon_l3_slow__l4_slow,
};
static struct omap_hwmod ti81xx_alwon_l3s_hwmod = {
	.name		= "alwon_l3_slow",
	.clkdm_name	= "alwon_l3s_clkdm",
	.class		= &l3_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.masters	= ti81xx_l3_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_l3_slow_masters),
	.slaves		= ti81xx_l3_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_l3_slow_slaves),
};

static struct omap_hwmod ti81xx_default_l3s_hwmod = {
	.name		= "default_l3_slow",
	.clkdm_name	= "default_l3s_clkdm",
	.class		= &l3_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if *ti81xx_l3_med_slaves[] = {
	&ti81xx_mpu__alwon_l3_med,
};
static struct omap_hwmod_ocp_if *ti81xx_l3_med_masters[] = {
	&ti81xx_alwon_l3_med__l4_fast,
};
static struct omap_hwmod ti81xx_alwon_l3m_hwmod = {
	.name		= "alwon_l3_med",
	.clkdm_name	= "alwon_l3m_clkdm",
	.class		= &l3_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.masters	= ti81xx_l3_med_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_l3_med_masters),
	.slaves		= ti81xx_l3_med_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_l3_med_slaves),
};

static struct omap_hwmod ti81xx_alwon_l3f_hwmod = {
	.name		= "alwon_l3_fast",
	.clkdm_name	= "alwon_l3f_clkdm",
	.class		= &l3_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

/*
 * L4 standard peripherals, see TRM table 1-12 for devices using this.
 * See TRM table 1-73 for devices using the 125MHz SYSCLK6 clock.
 */
static struct omap_hwmod ti81xx_l4_slow_hwmod = {
	.name		= "l4_slow",
	.clkdm_name	= "alwon_l3s_clkdm",
	.class		= &l4_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/*
 * L4 high-speed peripherals. For devices using this, please see the TRM
 * table 1-13. On dm816x, only EMAC, MDIO and SATA use this. See also TRM
 * table 1-73 for devices using 250MHz SYSCLK5 clock.
 */
static struct omap_hwmod ti81xx_l4_fast_hwmod = {
	.name		= "l4_fast",
	.clkdm_name	= "alwon_l3m_clkdm",
	.class		= &l4_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* L3 slow -> L4 ls peripheral interface running at 125MHz */
static struct omap_hwmod_ocp_if ti81xx_alwon_l3_slow__l4_slow = {
	.master	= &ti81xx_alwon_l3s_hwmod,
	.slave	= &ti81xx_l4_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* L3 med -> L4 fast peripheral interface running at 250MHz */
static struct omap_hwmod_ocp_if ti81xx_alwon_l3_med__l4_fast = {
	.master	= &ti81xx_alwon_l3m_hwmod,
	.slave	= &ti81xx_l4_fast_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU */
static struct omap_hwmod_ocp_if *ti81xx_mpu_masters[] = {
	&ti81xx_mpu__alwon_l3_slow,
};
static struct omap_hwmod ti81xx_mpu_hwmod = {
	.name		= "mpu",
	.clkdm_name	= "alwon_mpu_clkdm",
	.class		= &mpu_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
	.main_clk	= "mpu_ck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_MPU_CLKCTRL_OFFSET,
			.modulemode = MODULEMODE_SWCTRL,
		},
	},
	.masters	= ti81xx_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_mpu_masters),
};

static struct omap_hwmod_ocp_if ti81xx_mpu__alwon_l3_slow = {
	.master	= &ti81xx_mpu_hwmod,
	.slave	= &ti81xx_alwon_l3s_hwmod,
	.user	= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if ti81xx_mpu__alwon_l3_med = {
	.master	= &ti81xx_mpu_hwmod,
	.slave	= &ti81xx_alwon_l3m_hwmod,
	.user	= OCP_USER_MPU,
};

static struct omap_hwmod ti814x_dcan0_hwmod;
static struct omap_hwmod ti814x_dcan1_hwmod;
static struct omap_hwmod ti81xx_uart1_hwmod;
static struct omap_hwmod ti81xx_uart2_hwmod;
static struct omap_hwmod ti81xx_uart3_hwmod;
static struct omap_hwmod ti814x_uart4_hwmod;
static struct omap_hwmod ti814x_uart5_hwmod;
static struct omap_hwmod ti814x_uart6_hwmod;
static struct omap_hwmod ti81xx_mcspi1_hwmod;
static struct omap_hwmod ti81xx_mcspi2_hwmod;
static struct omap_hwmod ti81xx_mcspi3_hwmod;
static struct omap_hwmod ti81xx_mcspi4_hwmod;
static struct omap_hwmod ti816x_wd_timer2_hwmod;
static struct omap_hwmod ti814x_wd_timer1_hwmod;
static struct omap_hwmod ti81xx_i2c1_hwmod;
static struct omap_hwmod ti81xx_i2c2_hwmod;
static struct omap_hwmod ti81xx_i2c3_hwmod;
static struct omap_hwmod ti81xx_i2c4_hwmod;
static struct omap_hwmod ti81xx_gpmc_hwmod;
static struct omap_hwmod ti81xx_gpio1_hwmod;
static struct omap_hwmod ti81xx_gpio2_hwmod;
static struct omap_hwmod ti814x_gpio3_hwmod;
static struct omap_hwmod ti814x_gpio4_hwmod;
static struct omap_hwmod ti81xx_usbss_hwmod;
static struct omap_hwmod ti81xx_mailbox_hwmod;
static struct omap_hwmod ti81xx_mcasp0_hwmod;
static struct omap_hwmod ti81xx_mcasp1_hwmod;
static struct omap_hwmod ti81xx_mcasp2_hwmod;
static struct omap_hwmod ti81xx_mcspi1_hwmod;
static struct omap_hwmod ti81xx_mcspi2_hwmod;
static struct omap_hwmod ti81xx_mcspi3_hwmod;
static struct omap_hwmod ti81xx_mcspi4_hwmod;
static struct omap_hwmod ti81xx_mmc0_hwmod;
static struct omap_hwmod ti81xx_mmc1_hwmod;
static struct omap_hwmod ti81xx_mmc2_hwmod;
static struct omap_hwmod ti81xx_elm_hwmod;
static struct omap_hwmod ti814x_cpgmac0_hwmod;
static struct omap_hwmod ti814x_mdio_hwmod;
static struct omap_hwmod ti81xx_tpcc_hwmod;
static struct omap_hwmod ti81xx_tptc0_hwmod;
static struct omap_hwmod ti81xx_tptc1_hwmod;
static struct omap_hwmod ti81xx_tptc2_hwmod;
static struct omap_hwmod ti81xx_tptc3_hwmod;
static struct omap_hwmod_ocp_if ti81xx_l4_slow__elm;

static struct omap_hwmod ti814x_timer1_hwmod;
static struct omap_hwmod ti814x_timer2_hwmod;
static struct omap_hwmod ti814x_timer3_hwmod;
static struct omap_hwmod ti814x_timer4_hwmod;
static struct omap_hwmod ti814x_timer5_hwmod;
static struct omap_hwmod ti814x_timer6_hwmod;
static struct omap_hwmod ti814x_timer7_hwmod;
static struct omap_hwmod ti814x_timer8_hwmod;

static struct omap_hwmod ti816x_timer1_hwmod;
static struct omap_hwmod ti816x_timer2_hwmod;
static struct omap_hwmod ti816x_timer3_hwmod;
static struct omap_hwmod ti816x_timer4_hwmod;
static struct omap_hwmod ti816x_timer5_hwmod;
static struct omap_hwmod ti816x_timer6_hwmod;
static struct omap_hwmod ti816x_timer7_hwmod;

/* L4 SLOW -> GPIO1 */
static struct omap_hwmod_addr_space ti81xx_gpio1_addrs[] = {
	{
		.pa_start	= 0x48032000,
		.pa_end		= 0x48032000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_gpio1_hwmod,
	.addr		= ti81xx_gpio1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO2 */
static struct omap_hwmod_addr_space ti81xx_gpio2_addrs[] = {
	{
		.pa_start	= 0x4804C000,
		.pa_end		= 0x4804C000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_gpio2_hwmod,
	.addr		= ti81xx_gpio2_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO3 */
static struct omap_hwmod_addr_space ti814x_gpio3_addrs[] = {
	{
		.pa_start	= 0x481AC000,
		.pa_end		= 0x481AC000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_gpio3_hwmod,
	.addr		= ti814x_gpio3_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO4 */
static struct omap_hwmod_addr_space ti814x_gpio4_addrs[] = {
	{
		.pa_start	= 0x481AE000,
		.pa_end		= 0x481AE000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_gpio4_hwmod,
	.addr		= ti814x_gpio4_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

#if 0
/* Slave interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l4_slow_slaves[] = {
	&ti81xx_l3_slow__l4_slow,
};

/* Master interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l4_slow_masters[] = {
	&ti81xx_l4_slow__gpio1,
	&ti81xx_l4_slow__gpio2,
	&ti814x_l4_slow__gpio3,
	&ti814x_l4_slow__gpio4,
};

/* L4 SLOW */
static struct omap_hwmod ti81xx_l4_slow_hwmod = {
	.name		= "l4_slow",
	.clkdm_name	= "alwon_l3s_clkdm",
	.class		= &l4_hwmod_class,
	.masters	= ti81xx_l4_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_l4_slow_masters),
	.slaves		= ti81xx_l4_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_l4_slow_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};
#endif

/* L4 SLOW -> Watchdog */
static struct omap_hwmod_addr_space ti816x_wd_timer2_addrs[] = {
	{
		.pa_start	= 0x480C2000,
		.pa_end		= 0x480C2FFF,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_addr_space ti814x_wd_timer1_addrs[] = {
	{
		.pa_start	= 0x481C7000,
		.pa_end		= 0x481C7FFF,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__wd_timer2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_wd_timer2_hwmod,
	.clk		= "wdt2_ick",
	.addr		= ti816x_wd_timer2_addrs,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__wd_timer1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_wd_timer1_hwmod,
	.clk		= "wdt1_ick",
	.addr		= ti814x_wd_timer1_addrs,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C1 */
static struct omap_hwmod_addr_space ti81xx_i2c1_addr_space[] = {
	{
		.pa_start	= 0x48028000,
		.pa_end		= 0x48028000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__i2c1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_i2c1_hwmod,
	.clk		= "i2c1_ick",
	.addr		= ti81xx_i2c1_addr_space,
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C2 */
static struct omap_hwmod_addr_space ti81xx_i2c2_addr_space[] = {
	{
		.pa_start	= 0x4802A000,
		.pa_end		= 0x4802A000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__i2c2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_i2c2_hwmod,
	.clk		= "i2c2_ick",
	.addr		= ti81xx_i2c2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti81xx_i2c3_addr_space[] = {
	{
		.pa_start	= 0x4819C000,
		.pa_end		= 0x4819C000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__i2c3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_i2c3_hwmod,
	.clk		= "i2c3_ick",
	.addr		= ti81xx_i2c3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti81xx_i2c4_addr_space[] = {
	{
		.pa_start       = 0x4819E000,
		.pa_end         = 0x4819E000 + SZ_4K - 1,
		.flags          = ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__i2c4 = {
	.master         = &ti81xx_l4_slow_hwmod,
	.slave          = &ti81xx_i2c4_hwmod,
	.clk            = "i2c4_ick",
	.addr           = ti81xx_i2c4_addr_space,
	.user           = OCP_USER_MPU,
};

/* cpgmac0 */
static struct omap_hwmod_class_sysconfig ti814x_cpgmac_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x8,
	.syss_offs	= 0x4,
	.rst_offs	= 0x8,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | MSTANDBY_FORCE |
			MSTANDBY_NO),
	.sysc_fields	= &omap_hwmod_sysc_type3,
};

static struct omap_hwmod_class ti814x_cpgmac0_hwmod_class = {
	.name		= "cpsw",
	.sysc		= &ti814x_cpgmac_sysc,
};

/* Used by driver */
struct omap_hwmod_addr_space ti814x_cpgmac0_addr_space[] = {
	/* cpsw ss */
	{
		.pa_start	= 0x4A100000,
		.pa_end		= 0x4A100024 - 1,
		.flags		= ADDR_MAP_ON_INIT,
	},
	/* cpsw wr */
	{
		.pa_start	= 0x4A100900,
		.pa_end		= 0x4A100900 + SZ_256 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti814x_l3_fast__cpgmac0 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti814x_cpgmac0_hwmod,
	.addr		= ti814x_cpgmac0_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti814x_cpsw_sl1_addr_space[] = {
	/* cpsw sl1 */
	{
		.pa_start	= 0x4A100700,
		.pa_end		= 0x4A100700 + SZ_64 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti814x_l3_fast__cpsw_sl1 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti814x_cpgmac0_hwmod,
	.addr		= ti814x_cpsw_sl1_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti814x_cpsw_sl2_addr_space[] = {
	/* cpsw sl2 */
	{
		.pa_start	= 0x4A100740,
		.pa_end		= 0x4A100740 + SZ_64 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti814x_l3_fast__cpsw_sl2 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti814x_cpgmac0_hwmod,
	.addr		= ti814x_cpsw_sl2_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti814x_cpsw_cpdma_addr_space[] = {
	/* cpsw cpdma */
	{
		.pa_start	= 0x4A100100,
		.pa_end		= 0x4A100100 + SZ_512 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti814x_l3_fast__cpsw_cpdma = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti814x_cpgmac0_hwmod,
	.addr		= ti814x_cpsw_cpdma_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti814x_cpgmac0_slaves[] = {
	&ti814x_l3_fast__cpgmac0,
	&ti814x_l3_fast__cpsw_sl1,
	&ti814x_l3_fast__cpsw_sl2,
	&ti814x_l3_fast__cpsw_cpdma,
};

static struct omap_hwmod_irq_info ti814x_cpgmac0_irqs[] = {
	{ .name = "c0_rx_thresh_pend", .irq = 40 },
	{ .name = "c0_rx_pend", .irq = 41 },
	{ .name = "c0_tx_pend", .irq = 42 },
	{ .name = "c0_misc_pend", .irq = 43 },
	{ .irq = -1 }
};

static struct omap_hwmod ti814x_cpgmac0_hwmod = {
	.name		= "cpgmac0",
	.class		= &ti814x_cpgmac0_hwmod_class,
	.clkdm_name	= "alwon_ethernet_clkdm",
	.mpu_irqs	= ti814x_cpgmac0_irqs,
	.main_clk	= "cpgmac0_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_ETHERNET_0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_cpgmac0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_cpgmac0_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY |
				HWMOD_SWSUP_RESET_BEFORE_IDLE),
};

/* mdio class */
static struct omap_hwmod_class ti814x_mdio_hwmod_class = {
	.name		= "davinci_mdio",
};

struct omap_hwmod_addr_space ti814x_mdio_addr_space[] = {
	{
		.pa_start	= 0x4A100800,
		.pa_end		= 0x4A100800 + SZ_256 - 1,
		.flags		= ADDR_MAP_ON_INIT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti814x_cpgmac0__mdio = {
	.master		= &ti814x_cpgmac0_hwmod,
	.slave		= &ti814x_mdio_hwmod,
	.addr		= ti814x_mdio_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti814x_mdio_slaves[] = {
	&ti814x_cpgmac0__mdio,
};

static struct omap_hwmod ti814x_mdio_hwmod = {
	.name		= "mdio",
	.class		= &ti814x_mdio_hwmod_class,
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.clkdm_name	= "alwon_ethernet_clkdm",
	.main_clk	= "cpgmac0_ick",
	.slaves		= ti814x_mdio_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_mdio_slaves),
};

/* 'dcan' class */
static struct omap_hwmod_class ti814x_dcan_hwmod_class = {
	.name		= "d_can",
};

/* dcan0 */
static struct omap_hwmod_addr_space ti814x_dcan0_addrs[] = {
	{
		.pa_start	= 0x481CC000,
		.pa_end		= 0x481CC000 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_irq_info ti814x_dcan0_irqs[] = {
	{ .name = "d_can_ms", .irq = 52 },
	{ .name = "d_can_mo", .irq = 53 },
	{ .name = "d_can_par", .irq = 54 },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__dcan0 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_dcan0_hwmod,
	.clk		= "dcan0_ick",
	.addr		= ti814x_dcan0_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *ti814x_dcan0_slaves[] = {
	&ti814x_l4_slow__dcan0,
};

static struct omap_hwmod ti814x_dcan0_hwmod = {
	.name		= "d_can0",
	.class		= &ti814x_dcan_hwmod_class,
	.clkdm_name	= "alwon_l4s_clkdm",
	.mpu_irqs	= ti814x_dcan0_irqs,
	.main_clk	= "dcan0_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_DCAN_0_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_dcan0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_dcan0_slaves),
};

/* dcan1 */
static struct omap_hwmod_addr_space ti814x_dcan1_addrs[] = {
	{
		.pa_start	= 0x481D0000,
		.pa_end		= 0x481D0000 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_irq_info ti814x_dcan1_irqs[] = {
	{ .name = "d_can_ms", .irq = 55 },
	{ .name = "d_can_mo", .irq = 56 },
	{ .name = "d_can_par", .irq = 57 },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__dcan1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_dcan1_hwmod,
	.clk		= "dcan1_ick",
	.addr		= ti814x_dcan1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *ti814x_dcan1_slaves[] = {
	&ti814x_l4_slow__dcan1,
};

static struct omap_hwmod ti814x_dcan1_hwmod = {
	.name		= "d_can1",
	.class		= &ti814x_dcan_hwmod_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti814x_dcan1_irqs,
	.main_clk	= "dcan1_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_DCAN_0_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_dcan1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_dcan1_slaves),
};

/* UART common */

static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_ENAWAKEUP |
			SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			SIDLE_SMART_WKUP),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
};


/* gpmc */
static struct omap_hwmod_class_sysconfig gpmc_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x14,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_SIDLEMODE |
			SYSC_HAS_SOFTRESET | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_gpmc_hwmod_class = {
	.name		= "gpmc",
	.sysc		= &gpmc_sysc,
};

struct omap_hwmod_addr_space ti81xx_gpmc_addr_space[] = {
	{
		.pa_start	= 0x50000000,
		.pa_end		= 0x50000000 + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__gpmc = {
	.master		= &ti81xx_l3_main_hwmod,
	.slave		= &ti81xx_gpmc_hwmod,
	.addr		= ti81xx_gpmc_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_gpmc_slaves[] = {
	&ti81xx_l3_main__gpmc,
};

static struct omap_hwmod_irq_info ti81xx_gpmc_irqs[] = {
	{ .irq = 100 },
	{ .irq = -1 }
};

static struct omap_hwmod ti81xx_gpmc_hwmod = {
	.name		= "gpmc",
	.class		= &ti81xx_gpmc_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_gpmc_irqs,
	.main_clk	= "gpmc_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_GPMC_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_gpmc_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpmc_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY |
				HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x90,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_CLOCKACTIVITY |
			SYSC_HAS_ENAWAKEUP | SYSC_HAS_SIDLEMODE |
			SYSC_HAS_SOFTRESET | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			SIDLE_SMART_WKUP),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_i2c_dev_attr i2c_dev_attr = {
	.flags		= OMAP_I2C_FLAG_BUS_SHIFT_NONE |
					OMAP_I2C_FLAG_RESET_REGS_POSTIDLE,
};

static struct omap_hwmod_class i2c_class = {
	.name 	= "i2c",
	.sysc 	= &i2c_sysc,
	.rev  	= OMAP_I2C_IP_VERSION_2,
	.reset	= &omap_i2c_reset,
};

/*
 * 'gpio' class
 * general purpose io module
 */
static struct omap_hwmod_class_sysconfig ti81xx_gpio_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0114,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_SOFTRESET | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			   SIDLE_SMART_WKUP),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_gpio_hwmod_class = {
	.name	= "gpio",
	.sysc	= &ti81xx_gpio_sysc,
	.rev	= 2,
};

/* gpio dev_attr */
static struct omap_gpio_dev_attr gpio_dev_attr = {
	.bank_width	= 32,
	.dbck_flag	= true,
};

/* UART1 */

static struct omap_hwmod_dma_info ti81xx_uart1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 26, },
	{ .name = "rx",	.dma_req = 27, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_uart1_addr_space[] = {
	{
		.pa_start	= 0x48020000,
		.pa_end		= 0x48020000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__uart1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_uart1_hwmod,
	.clk		= "uart1_ick",
	.addr		= ti81xx_uart1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti81xx_uart1_irqs[] = {
	{ .irq = 72 },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_uart1_slaves[] = {
	&ti81xx_l4_slow__uart1,
};

static struct omap_hwmod ti81xx_uart1_hwmod = {
	.name		= "uart1",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_uart1_irqs,
	.main_clk	= "uart1_fck",
	.sdma_reqs	= ti81xx_uart1_edma_reqs,
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_uart1_slaves),
};

/* UART2 */

static struct omap_hwmod_dma_info ti81xx_uart2_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 28, },
	{ .name = "rx",	.dma_req = 29, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_uart2_addr_space[] = {
	{
		.pa_start	= 0x48022000,
		.pa_end		= 0x48022000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__uart2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_uart2_hwmod,
	.clk		= "uart2_ick",
	.addr		= ti81xx_uart2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti81xx_uart2_irqs[] = {
	{ .irq = 73, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_uart2_slaves[] = {
	&ti81xx_l4_slow__uart2,
};

static struct omap_hwmod ti81xx_uart2_hwmod = {
	.name		= "uart2",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_uart2_irqs,
	.main_clk	= "uart2_fck",
	.sdma_reqs	= ti81xx_uart2_edma_reqs,
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_uart2_slaves),
};

/* UART3 */

static struct omap_hwmod_dma_info ti81xx_uart3_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 30, },
	{ .name = "rx",	.dma_req = 31, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_uart3_addr_space[] = {
	{
		.pa_start	= 0x48024000,
		.pa_end		= 0x48024000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__uart3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_uart3_hwmod,
	.clk		= "uart3_ick",
	.addr		= ti81xx_uart3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti81xx_uart3_irqs[] = {
	{ .irq = 74, },
	{ .irq = -1 }
};


static struct omap_hwmod_ocp_if *ti81xx_uart3_slaves[] = {
	&ti81xx_l4_slow__uart3,
};

static struct omap_hwmod ti81xx_uart3_hwmod = {
	.name		= "uart3",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_uart3_irqs,
	.main_clk	= "uart3_fck",
	.sdma_reqs	= ti81xx_uart3_edma_reqs,
	/* Mysterious voodoo: system deadlocks at boot when attempting to idle UART2. */
	.flags      = (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_2_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_uart3_slaves),
};

/* UART4 */

static struct omap_hwmod_addr_space ti814x_uart4_addr_space[] = {
	{
		.pa_start	= 0x481A6000,
		.pa_end		= 0x481A6000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart4_hwmod,
	.clk		= "uart4_ick",
	.addr		= ti814x_uart4_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti814x_uart4_irqs[] = {
	{ .irq = 44, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti814x_uart4_slaves[] = {
	&ti814x_l4_slow__uart4,
};

static struct omap_hwmod ti814x_uart4_hwmod = {
	.name		= "uart4",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_uart4_irqs,
	.main_clk	= "uart4_fck",
//	.sdma_reqs	= uart4_edma_reqs, /* TODO Figure out the EDMA Crossbar */
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI814X_CM_ALWON_UART_3_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_uart4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart4_slaves),
};

/* UART5 */

static struct omap_hwmod_addr_space ti814x_uart5_addr_space[] = {
	{
		.pa_start	= 0x481A8000,
		.pa_end		= 0x481A8000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart5 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart5_hwmod,
	.clk		= "uart5_ick",
	.addr		= ti814x_uart5_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti814x_uart5_irqs[] = {
	{ .irq = 45, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti814x_uart5_slaves[] = {
	&ti814x_l4_slow__uart5,
};

static struct omap_hwmod ti814x_uart5_hwmod = {
	.name		= "uart5",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_uart5_irqs,
//	.sdma_reqs	= uart5_edma_reqs, /* TODO Figure out the EDMA Crossbar */
	.main_clk	= "uart5_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI814X_CM_ALWON_UART_4_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_uart5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart5_slaves),
};

/* UART6 */

static struct omap_hwmod_addr_space ti814x_uart6_addr_space[] = {
	{
		.pa_start	= 0x481AA000,
		.pa_end		= 0x481AA000 + SZ_8K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart6 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart6_hwmod,
	.clk		= "uart6_ick",
	.addr		= ti814x_uart6_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info ti814x_uart6_irqs[] = {
	{ .irq = 46, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti814x_uart6_slaves[] = {
	&ti814x_l4_slow__uart6,
};

static struct omap_hwmod ti814x_uart6_hwmod = {
	.name		= "uart6",
	.class		= &uart_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_uart6_irqs,
//	.sdma_reqs	= uart6_edma_reqs, /* TODO Figure out the EDMA Crossbar */
	.main_clk	= "uart6_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI814X_CM_ALWON_UART_5_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_uart6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart6_slaves),
};

/*
 * 'wd_timer' class
 * 32-bit watchdog upward counter that generates a pulse on the reset pin on
 * overflow condition
 */

static struct omap_hwmod_class_sysconfig wd_timer_sysc = {
	.rev_offs       = 0x0000,
	.sysc_offs      = 0x0010,
	.syss_offs      = 0x0014,
	.sysc_flags     = (SYSC_HAS_SIDLEMODE | SYSC_HAS_EMUFREE |
				SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				SYSC_HAS_AUTOIDLE | SYSC_HAS_CLOCKACTIVITY),
	.idlemodes      = (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class wd_timer_class = {
	.name = "wd_timer",
	.sysc = &wd_timer_sysc,
	.pre_shutdown	= &omap2_wd_timer_disable,
};

static struct omap_hwmod_ocp_if *ti816x_wd_timer2_slaves[] = {
	&ti816x_l4_slow__wd_timer2,
};

static struct omap_hwmod_ocp_if *ti814x_wd_timer1_slaves[] = {
	&ti814x_l4_slow__wd_timer1,
};

static struct omap_hwmod ti816x_wd_timer2_hwmod = {
	.name		= "wd_timer2",
	.class		= &wd_timer_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.flags	  = HWMOD_INIT_NO_RESET,
	.main_clk	= "wdt2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_WDTIMER_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_wd_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_wd_timer2_slaves),
};


static struct omap_hwmod ti814x_wd_timer1_hwmod = {
	.name		= "wd_timer1",
	.class		= &wd_timer_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.main_clk	= "wdt1_fck",
	.prcm		= {
		.omap4		= {
			.clkctrl_offs = TI81XX_CM_ALWON_WDTIMER_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_wd_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_wd_timer1_slaves),
	.flags	  = HWMOD_INIT_NO_RESET,
};

/* I2C1 */
static struct omap_hwmod_irq_info ti81xx_i2c1_irqs[] = {
	{ .irq = 70, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_i2c1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 58, },
	{ .name = "rx",	.dma_req = 59, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_i2c1_slaves[] = {
	&ti81xx_l4_slow__i2c1,
};

static struct omap_hwmod ti81xx_i2c1_hwmod = {
	.name			= "i2c1",
	.class			= &i2c_class,
	.flags			= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.clkdm_name		= "alwon_i2c02_clkdm",
	.mpu_irqs		= ti81xx_i2c1_irqs,
	.sdma_reqs		= ti81xx_i2c1_edma_reqs,
	.main_clk		= "i2c1_fck",
	.dev_attr	    = &i2c_dev_attr,
	.slaves		    = ti81xx_i2c1_slaves,
	.slaves_cnt	    = ARRAY_SIZE(ti81xx_i2c1_slaves),
};

/* I2C2 */
static struct omap_hwmod_irq_info ti81xx_i2c2_irqs[] = {
	{ .irq = 71, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_i2c2_edma_reqs[] = {
	{ .name = "tx", .dma_req = 60, },
	{ .name = "rx", .dma_req = 61, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_i2c2_slaves[] = {
	&ti81xx_l4_slow__i2c2,
};

static struct omap_hwmod ti81xx_i2c2_hwmod = {
	.name           = "i2c2",
	.class			= &i2c_class,
	.flags			= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.clkdm_name		= "alwon_i2c13_clkdm",
	.mpu_irqs       = ti81xx_i2c2_irqs,
	.sdma_reqs      = ti81xx_i2c2_edma_reqs,
	.main_clk       = "i2c2_fck",
	.dev_attr	    = &i2c_dev_attr,
	.slaves         = ti81xx_i2c2_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti81xx_i2c2_slaves),
};

/* I2C3 */
static struct omap_hwmod_irq_info ti81xx_i2c3_irqs[] = {
	{ .irq = 30, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_i2c3_slaves[] = {
	&ti81xx_l4_slow__i2c3,
};

static struct omap_hwmod ti81xx_i2c3_hwmod = {
	.name           = "i2c3",
	.class			= &i2c_class,
	.flags			= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.clkdm_name		= "alwon_i2c02_clkdm",
	.mpu_irqs       = ti81xx_i2c3_irqs,
//	.sdma_reqs      = ti81xx_i2c3_edma_reqs, /* TODO: Need to figure out the EDMA crossbar */
	.main_clk       = "i2c3_fck",
	.dev_attr	    = &i2c_dev_attr,
	.slaves         = ti81xx_i2c3_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti81xx_i2c3_slaves),
};

/* I2C4 */
static struct omap_hwmod_irq_info ti81xx_i2c4_irqs[] = {
	{ .irq = 31, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if *ti81xx_i2c4_slaves[] = {
	&ti81xx_l4_slow__i2c4,
};

static struct omap_hwmod ti81xx_i2c4_hwmod = {
	.name			= "i2c4",
	.class			= &i2c_class,
	.flags			= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
	.clkdm_name		= "alwon_i2c13_clkdm",
	.mpu_irqs		= ti81xx_i2c4_irqs,
//	.sdma_reqs		= ti81xx_i2c4_edma_reqs, /* TODO: Need to figure out the EDMA crossbar */
	.main_clk		= "i2c4_fck",
	.slaves		= ti81xx_i2c4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_i2c4_slaves),
};

/* 'elm' class */
static struct omap_hwmod_class_sysconfig ti81xx_elm_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_SOFTRESET |
				SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_elm_hwmod_class = {
	.name = "elm",
	.sysc = &ti81xx_elm_sysc,
};

static struct omap_hwmod_irq_info ti81xx_elm_irqs[] = {
	{ .irq = 4 },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_elm_addr_space[] = {
	{
		.pa_start	= 0x48080000,
		.pa_end		= 0x48080000 + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__elm = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_elm_hwmod,
	.addr		= ti81xx_elm_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_elm_slaves[] = {
	&ti81xx_l4_slow__elm,
};

static struct omap_hwmod ti81xx_elm_hwmod = {
	.name		= "elm",
	.class		= &ti81xx_elm_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_elm_irqs,
	.main_clk	= "elm_fck",
	.slaves		= ti81xx_elm_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_elm_slaves),
};

/* GPIO1 TI81XX */

static struct omap_hwmod_irq_info ti81xx_gpio1_irqs[] = {
	{ .irq = 96 },
	{ .irq = 97 },
	{ .irq = -1 }
};

/* gpio1 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio1_slaves[] = {
	&ti81xx_l4_slow__gpio1,
};

static struct omap_hwmod_opt_clk gpio1_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio1_dbck" },
};

static struct omap_hwmod ti81xx_gpio1_hwmod = {
	.name		= "gpio1",
	.class		= &ti81xx_gpio_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_gpio1_irqs,
	.main_clk	= "gpio1_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= gpio1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio1_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio1_slaves),
};

/* GPIO2 TI81XX*/

static struct omap_hwmod_irq_info ti81xx_gpio2_irqs[] = {
	{ .irq = 98 },
	{ .irq = 99 },
	{ .irq = -1 }
};

/* gpio2 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio2_slaves[] = {
	&ti81xx_l4_slow__gpio2,
};

static struct omap_hwmod_opt_clk gpio2_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio2_dbck" },
};

static struct omap_hwmod ti81xx_gpio2_hwmod = {
	.name		= "gpio2",
	.class		= &ti81xx_gpio_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_gpio2_irqs,
	.main_clk	= "gpio2_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= gpio2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio2_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio2_slaves),
};

/* GPIO3 TI814X */

static struct omap_hwmod_irq_info ti814x_gpio3_irqs[] = {
	{ .irq = 32 },
	{ .irq = 33 },
	{ .irq = -1 }
};

/* gpio3 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio3_slaves[] = {
	&ti814x_l4_slow__gpio3,
};

static struct omap_hwmod_opt_clk gpio3_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio3_dbck" },
};

static struct omap_hwmod ti814x_gpio3_hwmod = {
	.name		= "gpio3",
	.class		= &ti81xx_gpio_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_gpio3_irqs,
	.main_clk	= "gpio3_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= gpio3_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio3_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio3_slaves),
};

/* GPIO4 TI814X*/

static struct omap_hwmod_irq_info ti814x_gpio4_irqs[] = {
	{ .irq = 62 },
	{ .irq = 63 },
	{ .irq = -1 }
};

/* gpio4 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio4_slaves[] = {
	&ti814x_l4_slow__gpio4,
};

static struct omap_hwmod_opt_clk gpio4_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio4_dbck" },
};

static struct omap_hwmod ti814x_gpio4_hwmod = {
	.name		= "gpio4",
	.class		= &ti81xx_gpio_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_gpio4_irqs,
	.main_clk	= "gpio4_ick",
	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET | HWMOD_INIT_NO_RESET,
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= gpio4_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio4_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio4_slaves),
};

/* usbss */
static struct omap_hwmod_addr_space ti81xx_usbss_addr_space[] = {
	{
		.name		= "usbss",
		.pa_start	= 0x47400000,
		.pa_end		= 0x47400000 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb0",
		.pa_start	= 0x47401000,
		.pa_end		= 0x47401000 + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb1",
		.pa_start	= 0x47401800,
		.pa_end		= 0x47401800 + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_class_sysconfig ti81xx_usbhsotg_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			MSTANDBY_FORCE | MSTANDBY_NO | MSTANDBY_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type2,
};

static struct omap_hwmod_class ti81xx_usbotg_class = {
	.name 		= "usbotg",
	.sysc 		= &ti81xx_usbhsotg_sysc, 
};

static struct omap_hwmod_irq_info ti81xx_usbss_mpu_irqs[] = {
	{ .name = "usbss-irq", .irq = 17, },
	{ .name = "musb0-irq", .irq = 18, },
	{ .name = "musb1-irq", .irq = 19, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if ti81xx_l3_slow__usbss = {
	.master		= &ti81xx_alwon_l3s_hwmod,
	.slave		= &ti81xx_usbss_hwmod,
	.clk		= "usbotg_ick",
	.addr		= ti81xx_usbss_addr_space,
	.user		= OCP_USER_MPU,
	.flags		= OCPIF_SWSUP_IDLE,
};

static struct omap_hwmod_ocp_if *ti81xx_usbss_slaves[] = {
	&ti81xx_l3_slow__usbss,
};

static struct omap_hwmod ti81xx_usbss_hwmod = {
	.name		= "usb_otg_hs",
	.class		= &ti81xx_usbotg_class,
	.clkdm_name = "default_usb_clkdm",
	.mpu_irqs	= ti81xx_usbss_mpu_irqs,
	.main_clk	= "usbotg_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_DEFAULT_USB_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_usbss_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_usbss_slaves),
//	.flags		= HWMOD_CONTROL_OPT_CLKS_IN_RESET,
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};

/*
 * 'mailbox' class
 * mailbox module allowing communication between the on-chip processors
 * using a queued mailbox-interrupt mechanism.
 */

static struct omap_hwmod_class_sysconfig ti81xx_mailbox_sysc = {
	.rev_offs	= 0x000,
	.sysc_offs	= 0x010,
	.syss_offs	= 0x014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type2,
};

static struct omap_hwmod_class ti81xx_mailbox_hwmod_class = {
	.name = "mailbox",
	.sysc = &ti81xx_mailbox_sysc,
};

static struct omap_hwmod_irq_info ti81xx_mailbox_irqs[] = {
	{ .irq = 77 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mailbox_addrs[] = {
	{
		.pa_start	= 0x480C8000,
		.pa_end		= 0x480C8000 + (SZ_4K - 1),
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

/* l4_slow -> mailbox */
static struct omap_hwmod_ocp_if ti81xx_l4_slow__mailbox = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mailbox_hwmod,
	.addr		= ti81xx_mailbox_addrs,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti81xx_mailbox_slaves[] = {
	&ti81xx_l4_slow__mailbox,
};

static struct omap_hwmod ti81xx_mailbox_hwmod = {
	.name		= "mailbox",
	.class		= &ti81xx_mailbox_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mailbox_irqs,
	.main_clk	= "mailbox_ick",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs	= TI81XX_CM_ALWON_MAILBOX_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_mailbox_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mailbox_slaves),
};

/* 'mcasp' class */
static struct omap_hwmod_class_sysconfig ti81xx_mcasp_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x4,
	.sysc_flags	= SYSC_HAS_SIDLEMODE,
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type3,
};

static struct omap_hwmod_class ti81xx_mcasp_hwmod_class = {
	.name	= "mcasp",
	.sysc	= &ti81xx_mcasp_sysc,
};

/* mcasp0 */
static struct omap_hwmod_irq_info ti81xx_mcasp0_irqs[] = {
	{ .name = "ax", .irq = 80, },
	{ .name = "ar", .irq = 81, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_mcasp0_edma_reqs[] = {
	{ .name = "tx", .dma_req = 8, },
	{ .name = "rx", .dma_req = 9, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcasp0_addr_space[] = {
	{
		.pa_start	= 0x48038000,
		.pa_end		= 0x48038000 + (SZ_1K * 12) - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcasp0 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcasp0_hwmod,
	.addr		= ti81xx_mcasp0_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcasp0_slaves[] = {
	&ti81xx_l4_slow__mcasp0,
};

static struct omap_hwmod ti81xx_mcasp0_hwmod = {
	.name		= "mcasp0",
	.class		= &ti81xx_mcasp_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mcasp0_irqs,
	.sdma_reqs	= ti81xx_mcasp0_edma_reqs,
	.main_clk	= "mcasp0_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_MCASP0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_mcasp0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcasp0_slaves),
};

/* mcasp1 */
static struct omap_hwmod_irq_info ti81xx_mcasp1_irqs[] = {
	{ .name = "ax", .irq = 82, },
	{ .name = "ar", .irq = 83, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_mcasp1_edma_reqs[] = {
	{ .name = "tx", .dma_req = 10, },
	{ .name = "rx", .dma_req = 11, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcasp1_addr_space[] = {
	{
		.pa_start	= 0x4803C000,
		.pa_end		= 0x4803C000 + (SZ_1K * 12) - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcasp1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcasp1_hwmod,
	.addr		= ti81xx_mcasp1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcasp1_slaves[] = {
	&ti81xx_l4_slow__mcasp1,
};

static struct omap_hwmod ti81xx_mcasp1_hwmod = {
	.name		= "mcasp1",
	.class		= &ti81xx_mcasp_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mcasp1_irqs,
	.sdma_reqs	= ti81xx_mcasp1_edma_reqs,
	.main_clk	= "mcasp1_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_MCASP1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_mcasp1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcasp1_slaves),
};

/* mcasp2 */
static struct omap_hwmod_irq_info ti81xx_mcasp2_irqs[] = {
	{ .name = "ax", .irq = 84, },
	{ .name = "ar", .irq = 85, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_mcasp2_edma_reqs[] = {
	{ .name = "tx", .dma_req = 12, },
	{ .name = "rx", .dma_req = 13, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcasp2_addr_space[] = {
	{
		.pa_start	= 0x48050000,
		.pa_end		= 0x48050000 + (SZ_1K * 12) - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcasp2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcasp2_hwmod,
	.addr		= ti81xx_mcasp2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcasp2_slaves[] = {
	&ti81xx_l4_slow__mcasp2,
};

static struct omap_hwmod ti81xx_mcasp2_hwmod = {
	.name		= "mcasp2",
	.class		= &ti81xx_mcasp_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mcasp2_irqs,
	.sdma_reqs	= ti81xx_mcasp2_edma_reqs,
	.main_clk	= "mcasp2_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_MCASP2_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_mcasp2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcasp2_slaves),
};

/*
 * 'mcspi' class
 * multichannel serial port interface (mcspi) / master/slave synchronous serial
 * bus
 */
static struct omap_hwmod_class_sysconfig ti81xx_mcspi_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0110,
	.syss_offs	= 0x0114,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
			SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE |
			SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_spi_hwmod_class = {
	.name		= "mcspi",
	.sysc		= &ti81xx_mcspi_sysc,
	.rev		= OMAP4_MCSPI_REV,
};

/*
 * HACK: We actually have a variable number of chipselects
 * per MCSPI peripheral, but we don't have enough DMA channels
 * to go around.
 */
static struct omap2_mcspi_dev_attr mcspi_attrib = {
	.num_chipselect = 2,
};

/* mcspi1 */
static struct omap_hwmod_irq_info ti81xx_spi1_irqs[] = {
	{ .irq = 65 },
	{ .irq = -1 }
};

struct omap_hwmod_dma_info ti81xx_spi1_edma_reqs[] = {
	{ .name = "rx0", .dma_req = 17 },
	{ .name = "tx0", .dma_req = 16 },
	{ .name = "rx1", .dma_req = 19 },
	{ .name = "tx1", .dma_req = 18 },
#if 0 /* Leaving these DMA channels alone for the EDMA crossbar. */
	{ .name = "rx2", .dma_req = 21 },
	{ .name = "tx2", .dma_req = 20 },
	{ .name = "rx3", .dma_req = 23 },
	{ .name = "tx3", .dma_req = 22 },
#endif
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi1_addr_space[] = {
	{
		.pa_start	= 0x48030000,
		.pa_end		= 0x480301ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcspi1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi1_hwmod,
	.clk		= "mcspi_ick",
	.addr		= ti81xx_mcspi1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi1_slaves[] = {
	&ti81xx_l4_slow__mcspi1,
};


static struct omap_hwmod ti81xx_mcspi1_hwmod = {
	.name		= "mcspi1",
	.class		= &ti81xx_spi_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_spi1_irqs,
	.main_clk	= "mcspi_fck",
	.sdma_reqs	= ti81xx_spi1_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs = TI81XX_CM_ALWON_SPI_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi_attrib,
	.slaves		= ti81xx_mcspi1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi1_slaves),
};

/* mcspi2 */
static struct omap_hwmod_irq_info ti81xx_spi2_irqs[] = {
	{ .irq = 125 },
	{ .irq = -1 }
};

struct omap_hwmod_dma_info ti81xx_spi2_edma_reqs[] = {
	{ .name = "rx0", .dma_req = 43 },
	{ .name = "tx0", .dma_req = 42 },
	{ .name = "rx1", .dma_req = 45 },
	{ .name = "tx1", .dma_req = 44 },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi2_addr_space[] = {
	{
		.pa_start	= 0x481A0000,
		.pa_end		= 0x481A01ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcspi2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi2_hwmod,
	.clk		= "mcspi_ick",
	.addr		= ti81xx_mcspi2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi2_slaves[] = {
	&ti81xx_l4_slow__mcspi2,
};

static struct omap_hwmod ti81xx_mcspi2_hwmod = {
	.name		= "mcspi2",
	.class		= &ti81xx_spi_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_spi2_irqs,
	.main_clk	= "mcspi_fck",
	.sdma_reqs	= ti81xx_spi2_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs = TI81XX_CM_ALWON_SPI_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi_attrib,
	.slaves		= ti81xx_mcspi2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi2_slaves),
};

/* mcspi3 */
static struct omap_hwmod_irq_info ti81xx_spi3_irqs[] = {
	{ .irq = 126 },
	{ .irq = -1 }
};

/* EDMA channels borrowed from spi1 via the EDMA crossbar. */
struct omap_hwmod_dma_info ti81xx_spi3_edma_reqs[] = {
	{ .name = "tx0", .dma_req = 79 },
	{ .name = "rx0", .dma_req = 80 },
	{ .name = "tx1", .dma_req = 81 },
	{ .name = "rx1", .dma_req = 82 },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi3_addr_space[] = {
	{
		.pa_start	= 0x481A2000,
		.pa_end		= 0x481A21ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcspi3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi3_hwmod,
	.clk		= "mcspi_ick",
	.addr		= ti81xx_mcspi3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi3_slaves[] = {
	&ti81xx_l4_slow__mcspi3,
};

static struct omap_hwmod ti81xx_mcspi3_hwmod = {
	.name		= "mcspi3",
	.class		= &ti81xx_spi_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_spi3_irqs,
	.main_clk	= "mcspi_fck",
	.sdma_reqs	= ti81xx_spi3_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs = TI81XX_CM_ALWON_SPI_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi_attrib,
	.slaves		= ti81xx_mcspi3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi3_slaves),
};

/* mcspi4 */
static struct omap_hwmod_irq_info ti81xx_spi4_irqs[] = {
	{ .irq = 127 },
	{ .irq = -1 }
};

/* TODO: No EDMA channel available for spi4. */

static struct omap_hwmod_addr_space ti81xx_mcspi4_addr_space[] = {
	{
		.pa_start	= 0x481A4000,
		.pa_end		= 0x481A41ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mcspi4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi4_hwmod,
	.clk		= "mcspi_ick",
	.addr		= ti81xx_mcspi4_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi4_slaves[] = {
	&ti81xx_l4_slow__mcspi4,
};

static struct omap2_mcspi_dev_attr mcspi4_attrib = {
	.num_chipselect = 1,
};

static struct omap_hwmod ti81xx_mcspi4_hwmod = {
	.name		= "mcspi4",
	.class		= &ti81xx_spi_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_spi4_irqs,
	.main_clk	= "mcspi_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs = TI81XX_CM_ALWON_SPI_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi4_attrib,
	.slaves		= ti81xx_mcspi4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi4_slaves),
};

/* 'mmc' class */
static struct omap_hwmod_class_sysconfig ti81xx_mmc_sysc = {
	.rev_offs	= 0x1fc,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x14,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
			SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_mmc_hwmod_class = {
	.name		= "mmc",
	.sysc		= &ti81xx_mmc_sysc,
};

/* mmc0 */
static struct omap_hwmod_irq_info ti81xx_mmc0_irqs[] = {
	{ .irq = 64 },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_mmc0_edma_reqs[] = {
	{ .name = "tx", .dma_req = 24, },
	{ .name = "rx", .dma_req = 25, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_opt_clk ti81xx_mmc0_clks[] = {
	{ .role = "dbclk", .clk = "mmc0_dbck" },
};

static struct omap_hwmod_addr_space ti81xx_mmc0_addr_space[] = {
	{
		.pa_start	= 0x48060100,
		.pa_end		= 0x48060100 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mmc0 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mmc0_hwmod,
	.clk		= "mmc0_ick",
	.addr		= ti81xx_mmc0_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc0_slaves[] = {
	&ti81xx_l4_slow__mmc0,
};

static struct omap_mmc_dev_attr ti81xx_mmc0_dev_attr = {
	.flags	= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti81xx_mmc0_hwmod = {
	.name		= "mmc1",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mmc0_irqs,
	.main_clk	= "mmc0_fck",
	.sdma_reqs	= ti81xx_mmc0_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= ti81xx_mmc0_clks,
	.opt_clks_cnt	= ARRAY_SIZE(ti81xx_mmc0_clks),
	.dev_attr	= &ti81xx_mmc0_dev_attr,
	.slaves		= ti81xx_mmc0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mmc0_slaves),
};

/* mmc1 */
static struct omap_hwmod_irq_info ti81xx_mmc1_irqs[] = {
	{ .irq = 28 },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti81xx_mmc1_edma_reqs[] = {
	{ .name = "tx", .dma_req = 2, },
	{ .name = "rx", .dma_req = 3, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_opt_clk ti81xx_mmc1_clks[] = {
	{ .role = "dbclk", .clk = "mmc1_dbck" },
};

static struct omap_hwmod_addr_space ti81xx_mmc1_addr_space[] = {
	{
		.pa_start	= 0x481D8100,
		.pa_end		= 0x481D8100 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mmc1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mmc1_hwmod,
	.clk		= "mmc1_ick",
	.addr		= ti81xx_mmc1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc1_slaves[] = {
	&ti81xx_l4_slow__mmc1,
};

static struct omap_mmc_dev_attr ti81xx_mmc1_dev_attr = {
	.flags		= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti81xx_mmc1_hwmod = {
	.name		= "mmc2",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mmc1_irqs,
	.main_clk	= "mmc1_fck",
	.sdma_reqs	= ti81xx_mmc1_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= ti81xx_mmc1_clks,
	.opt_clks_cnt	= ARRAY_SIZE(ti81xx_mmc1_clks),
	.dev_attr	= &ti81xx_mmc1_dev_attr,
	.slaves		= ti81xx_mmc1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mmc1_slaves),
};

/* mmc2 */
static struct omap_hwmod_irq_info ti81xx_mmc2_irqs[] = {
	{ .irq = 29 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mmc2_addr_space[] = {
	{
		.pa_start	= 0x47810000,
		.pa_end		= 0x47810000 + SZ_64K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_opt_clk ti81xx_mmc2_clks[] = {
	{ .role = "dbclk", .clk = "mmc2_dbck" },
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__mmc2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_mmc2_hwmod,
	.clk		= "mmc2_ick",
	.addr		= ti81xx_mmc2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc2_slaves[] = {
	&ti81xx_l4_slow__mmc2,
};

static struct omap_mmc_dev_attr ti81xx_mmc2_dev_attr = {
	.flags		= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti81xx_mmc2_hwmod = {
	.name		= "mmc3",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_mmc2_irqs,
	.main_clk	= "mmc2_fck",
//	.sdma_reqs	= ti81xx_mmc2_edma_reqs, /* TODO: Need to figure out EDMA crossbar. */
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_2_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.opt_clks	= ti81xx_mmc2_clks,
	.opt_clks_cnt	= ARRAY_SIZE(ti81xx_mmc2_clks),
	.dev_attr	= &ti81xx_mmc2_dev_attr,
	.slaves		= ti81xx_mmc2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mmc2_slaves),
};

/* 'timer' class */
static struct omap_hwmod_class_sysconfig ti81xx_timer_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			SIDLE_SMART_WKUP),
	.sysc_fields	= &omap_hwmod_sysc_type2,
};

static struct omap_hwmod_class ti81xx_timer_hwmod_class = {
	.name		= "timer",
	.sysc		= &ti81xx_timer_sysc,
};

/* timer1 */
static struct omap_hwmod_irq_info ti81xx_timer1_irqs[] = {
	{ .irq = 67 },
	{ .irq = -1 }
};
static struct omap_hwmod_addr_space ti81xx_timer1_addr_space[] = {
	{
		.pa_start	= 0x4802e000,
		.pa_end		= 0x4802e000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer1_hwmod,
	.clk		= "timer1_ick",
	.addr		= ti81xx_timer1_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer1_slaves[] = {
	&ti814x_l4_slow__timer1,
};
static struct omap_hwmod ti814x_timer1_hwmod = {
	.name		= "timer1",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer1_irqs,
	.main_clk	= "timer1_fck",
	.slaves		= ti814x_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer1_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer1_hwmod,
	.clk		= "timer1_ick",
	.addr		= ti81xx_timer1_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer1_slaves[] = {
	&ti816x_l4_slow__timer1,
};
static struct omap_hwmod ti816x_timer1_hwmod = {
	.name		= "timer1",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer1_irqs,
	.main_clk	= "timer1_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer1_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer2 */
static struct omap_hwmod_irq_info ti81xx_timer2_irqs[] = {
	{ .irq = 68 },
	{ .irq = -1 }
};
static struct omap_hwmod_addr_space ti81xx_timer2_addr_space[] = {
	{
		.pa_start	= 0x48040000,
		.pa_end		= 0x48040000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer2_hwmod,
	.clk		= "timer2_ick",
	.addr		= ti81xx_timer2_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer2_slaves[] = {
	&ti814x_l4_slow__timer2,
};
static struct omap_hwmod ti814x_timer2_hwmod = {
	.name		= "timer2",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer2_irqs,
	.main_clk	= "timer2_fck",
	.slaves		= ti814x_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer2_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer2_hwmod,
	.clk		= "timer2_ick",
	.addr		= ti81xx_timer2_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer2_slaves[] = {
	&ti816x_l4_slow__timer2,
};
static struct omap_hwmod ti816x_timer2_hwmod = {
	.name		= "timer2",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer2_irqs,
	.main_clk	= "timer2_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer2_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer3 */
static struct omap_hwmod_irq_info ti81xx_timer3_irqs[] = {
	{ .irq = 69 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_timer3_addr_space[] = {
	{
		.pa_start	= 0x48042000,
		.pa_end		= 0x48042000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer3_hwmod,
	.clk		= "timer3_ick",
	.addr		= ti81xx_timer3_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer3_slaves[] = {
	&ti814x_l4_slow__timer3,
};
static struct omap_hwmod ti814x_timer3_hwmod = {
	.name		= "timer3",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer3_irqs,
	.main_clk	= "timer3_fck",
	.slaves		= ti814x_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer3_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer3_hwmod,
	.clk		= "timer3_ick",
	.addr		= ti81xx_timer3_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer3_slaves[] = {
	&ti816x_l4_slow__timer3,
};
static struct omap_hwmod ti816x_timer3_hwmod = {
	.name		= "timer3",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer3_irqs,
	.main_clk	= "timer3_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_2_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer3_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};


/* timer4 */
static struct omap_hwmod_irq_info ti81xx_timer4_irqs[] = {
	{ .irq = 92 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_timer4_addr_space[] = {
	{
		.pa_start	= 0x48044000,
		.pa_end		= 0x48044000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer4_hwmod,
	.clk		= "timer4_ick",
	.addr		= ti81xx_timer4_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer4_slaves[] = {
	&ti814x_l4_slow__timer4,
};
static struct omap_hwmod ti814x_timer4_hwmod = {
	.name		= "timer4",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer4_irqs,
	.main_clk	= "timer4_fck",
	.slaves		= ti814x_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer4_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer4_hwmod,
	.clk		= "timer4_ick",
	.addr		= ti81xx_timer4_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer4_slaves[] = {
	&ti816x_l4_slow__timer4,
};
static struct omap_hwmod ti816x_timer4_hwmod = {
	.name		= "timer4",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer4_irqs,
	.main_clk	= "timer4_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_3_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer4_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer5 */
static struct omap_hwmod_irq_info ti81xx_timer5_irqs[] = {
	{ .irq = 93 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_timer5_addr_space[] = {
	{
		.pa_start	= 0x48046000,
		.pa_end		= 0x48046000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer5 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer5_hwmod,
	.clk		= "timer5_ick",
	.addr		= ti81xx_timer5_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer5_slaves[] = {
	&ti814x_l4_slow__timer5,
};
static struct omap_hwmod ti814x_timer5_hwmod = {
	.name		= "timer5",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer5_irqs,
	.main_clk	= "timer5_fck",
	.slaves		= ti814x_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer5_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer5 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer5_hwmod,
	.clk		= "timer5_ick",
	.addr		= ti81xx_timer5_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer5_slaves[] = {
	&ti816x_l4_slow__timer5,
};
static struct omap_hwmod ti816x_timer5_hwmod = {
	.name		= "timer5",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer5_irqs,
	.main_clk	= "timer5_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_4_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer5_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer6 */
static struct omap_hwmod_irq_info ti81xx_timer6_irqs[] = {
	{ .irq = 94 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_timer6_addr_space[] = {
	{
		.pa_start	= 0x48048000,
		.pa_end		= 0x48048000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer6 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer6_hwmod,
	.clk		= "timer6_ick",
	.addr		= ti81xx_timer6_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer6_slaves[] = {
	&ti814x_l4_slow__timer6,
};
static struct omap_hwmod ti814x_timer6_hwmod = {
	.name		= "timer6",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer6_irqs,
	.main_clk	= "timer6_fck",
	.slaves		= ti814x_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer6_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer6 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer6_hwmod,
	.clk		= "timer6_ick",
	.addr		= ti81xx_timer6_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer6_slaves[] = {
	&ti816x_l4_slow__timer6,
};
static struct omap_hwmod ti816x_timer6_hwmod = {
	.name		= "timer6",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer6_irqs,
	.main_clk	= "timer6_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_5_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer6_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer7 */
static struct omap_hwmod_irq_info ti81xx_timer7_irqs[] = {
	{ .irq = 95 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_timer7_addr_space[] = {
	{
		.pa_start	= 0x4804a000,
		.pa_end		= 0x4804a000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer7 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer7_hwmod,
	.clk		= "timer7_ick",
	.addr		= ti81xx_timer7_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer7_slaves[] = {
	&ti814x_l4_slow__timer7,
};
static struct omap_hwmod ti814x_timer7_hwmod = {
	.name		= "timer7",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer7_irqs,
	.main_clk	= "timer7_fck",
	.slaves		= ti814x_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer7_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__timer7 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_timer7_hwmod,
	.clk		= "timer7_ick",
	.addr		= ti81xx_timer7_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti816x_timer7_slaves[] = {
	&ti816x_l4_slow__timer7,
};
static struct omap_hwmod ti816x_timer7_hwmod = {
	.name		= "timer7",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti81xx_timer7_irqs,
	.main_clk	= "timer7_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI816X_CM_ALWON_TIMER_6_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti816x_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer7_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET),
};

/* timer8 */
static struct omap_hwmod_irq_info ti814x_timer8_irqs[] = {
	{ .irq = 11 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti814x_timer8_addr_space[] = {
	{
		.pa_start	= 0x481c1000,
		.pa_end		= 0x481c1000 + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__timer8 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_timer8_hwmod,
	.clk		= "timer8_ick",
	.addr		= ti814x_timer8_addr_space,
	.user		= OCP_USER_MPU,
};
static struct omap_hwmod_ocp_if *ti814x_timer8_slaves[] = {
	&ti814x_l4_slow__timer8,
};
static struct omap_hwmod ti814x_timer8_hwmod = {
	.name		= "timer8",
	.class		= &ti81xx_timer_hwmod_class,
	.clkdm_name	= "alwon_l3s_clkdm",
	.mpu_irqs	= ti814x_timer8_irqs,
	.main_clk	= "timer8_fck",
	.slaves		= ti814x_timer8_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_timer8_slaves),
	.flags		= (HWMOD_INIT_NO_IDLE | HWMOD_INIT_NO_RESET | HWMOD_NO_IDLEST),
};

/* tpcc */
#define TI81XX_TPCC_BASE		0x49000000
#define TI81XX_TPTC0_BASE		0x49800000
#define TI81XX_TPTC1_BASE		0x49900000
#define TI81XX_TPTC2_BASE		0x49a00000
#define TI81XX_TPTC3_BASE		0x49b00000

/* 'tpcc' class */
static struct omap_hwmod_class ti81xx_tpcc_hwmod_class = {
	.name		= "tpcc",
};

static struct omap_hwmod_irq_info ti81xx_tpcc_irqs[] = {
	{ .name	= "edma0", .irq = 12 },
	{ .name = "edma0_mperr", .irq = 13, },
	{ .name	= "edma0_err", .irq = 14 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_tpcc_addr_space[] = {
	{
		.name		= "edma_cc0",
		.pa_start	= TI81XX_TPCC_BASE,
		.pa_end		= TI81XX_TPCC_BASE + SZ_32K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l3_main__tpcc = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti81xx_tpcc_hwmod,
	.addr		= ti81xx_tpcc_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_tpcc_slaves[] = {
	&ti81xx_l3_main__tpcc,
};

static struct omap_hwmod ti81xx_tpcc_hwmod = {
	.name		= "tpcc",
	.class		= &ti81xx_tpcc_hwmod_class,
	.clkdm_name	= "alwon_l3f_clkdm",
	.mpu_irqs	= ti81xx_tpcc_irqs,
	.main_clk	= "tpcc_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPCC_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tpcc_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tpcc_slaves),
};

static struct omap_hwmod_class_sysconfig ti81xx_tptc_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET |
			SYSC_HAS_MIDLEMODE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_SMART | MSTANDBY_FORCE),
	.sysc_fields	= &omap_hwmod_sysc_type2,
};

/* 'tptc' class */
static struct omap_hwmod_class ti81xx_tptc_hwmod_class = {
	.name		= "tptc",
	.sysc		= &ti81xx_tptc_sysc,
};

/* tptc0 */
static struct omap_hwmod_irq_info ti81xx_tptc0_irqs[] = {
	{ .irq = 112 },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_tptc0_addr_space[] = {
	{
		.name		= "edma_tc0",
		.pa_start	= TI81XX_TPTC0_BASE,
		.pa_end		= TI81XX_TPTC0_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__tptc0 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti81xx_tptc0_hwmod,
	.addr		= ti81xx_tptc0_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_tptc0_slaves[] = {
	&ti81xx_l3_main__tptc0,
};

static struct omap_hwmod ti81xx_tptc0_hwmod = {
	.name		= "tptc0",
	.class		= &ti81xx_tptc_hwmod_class,
	.clkdm_name	= "alwon_l3f_clkdm",
	.mpu_irqs	= ti81xx_tptc0_irqs,
	.main_clk	= "tptc0_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC0_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tptc0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tptc0_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};


/* tptc1 */
static struct omap_hwmod_irq_info ti81xx_tptc1_irqs[] = {
	{ .irq = 113 },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_tptc1_addr_space[] = {
	{
		.name		= "edma_tc1",
		.pa_start	= TI81XX_TPTC1_BASE,
		.pa_end		= TI81XX_TPTC1_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__tptc1 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti81xx_tptc1_hwmod,
	.addr		= ti81xx_tptc1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_tptc1_slaves[] = {
	&ti81xx_l3_main__tptc1,
};

static struct omap_hwmod ti81xx_tptc1_hwmod = {
	.name		= "tptc1",
	.class		= &ti81xx_tptc_hwmod_class,
	.clkdm_name	= "alwon_l3f_clkdm",
	.mpu_irqs	= ti81xx_tptc1_irqs,
	.main_clk	= "tptc1_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC1_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tptc1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tptc1_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};

/* tptc2 */
static struct omap_hwmod_irq_info ti81xx_tptc2_irqs[] = {
	{ .irq = 114 },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_tptc2_addr_space[] = {
	{
		.name		= "edma_tc2",
		.pa_start	= TI81XX_TPTC2_BASE,
		.pa_end		= TI81XX_TPTC2_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__tptc2 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti81xx_tptc2_hwmod,
	.addr		= ti81xx_tptc2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_tptc2_slaves[] = {
	&ti81xx_l3_main__tptc2,
};

static struct omap_hwmod ti81xx_tptc2_hwmod = {
	.name		= "tptc2",
	.class		= &ti81xx_tptc_hwmod_class,
	.clkdm_name	= "alwon_l3f_clkdm",
	.mpu_irqs	= ti81xx_tptc2_irqs,
	.main_clk	= "tptc2_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC2_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tptc2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tptc2_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};

/* tptc3 */
static struct omap_hwmod_irq_info ti81xx_tptc3_irqs[] = {
	{ .irq = 115 },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_tptc3_addr_space[] = {
	{
		.name		= "edma_tc3",
		.pa_start	= TI81XX_TPTC3_BASE,
		.pa_end		= TI81XX_TPTC3_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__tptc3 = {
	.master		= &ti81xx_alwon_l3f_hwmod,
	.slave		= &ti81xx_tptc3_hwmod,
	.addr		= ti81xx_tptc3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_tptc3_slaves[] = {
	&ti81xx_l3_main__tptc3,
};

static struct omap_hwmod ti81xx_tptc3_hwmod = {
	.name		= "tptc3",
	.class		= &ti81xx_tptc_hwmod_class,
	.clkdm_name	= "alwon_l3f_clkdm",
	.mpu_irqs	= ti81xx_tptc3_irqs,
	.main_clk	= "tptc3_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC3_CLKCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tptc3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tptc3_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};

static __initdata struct omap_hwmod *ti81xx_hwmods[] = {
	/* bus interconnect class */
	&ti81xx_l3_main_hwmod,
	&ti81xx_default_l3s_hwmod,
	&ti81xx_alwon_l3s_hwmod,
	&ti81xx_alwon_l3m_hwmod,
	&ti81xx_alwon_l3f_hwmod,
	&ti81xx_l4_slow_hwmod,
	&ti81xx_l4_fast_hwmod,
	&ti81xx_mpu_hwmod,
	&ti81xx_uart1_hwmod,
	&ti81xx_uart2_hwmod,
	&ti81xx_uart3_hwmod,
	&ti81xx_i2c1_hwmod,
	&ti81xx_i2c2_hwmod,
	&ti81xx_i2c3_hwmod,
	&ti81xx_i2c4_hwmod,
	&ti81xx_gpmc_hwmod,
	&ti81xx_gpio1_hwmod,
	&ti81xx_gpio2_hwmod,
	&ti814x_gpio3_hwmod,
	&ti814x_gpio4_hwmod,
	&ti81xx_usbss_hwmod,
	&ti81xx_elm_hwmod,
	/* mailbox class */
	&ti81xx_mailbox_hwmod,
	/* mcasp class */
	&ti81xx_mcasp0_hwmod,
	&ti81xx_mcasp1_hwmod,
	&ti81xx_mcasp2_hwmod,
	/* mcspi class */
	&ti81xx_mcspi1_hwmod,
	&ti81xx_mcspi2_hwmod,
	&ti81xx_mcspi3_hwmod,
	&ti81xx_mcspi4_hwmod,
	/* mmc class */
	&ti81xx_mmc0_hwmod,
	&ti81xx_mmc1_hwmod,
	&ti81xx_mmc2_hwmod,
	/* tptc class */
	&ti81xx_tptc0_hwmod,
	&ti81xx_tptc1_hwmod,
	&ti81xx_tptc2_hwmod,
	&ti81xx_tptc3_hwmod,
	/* tpcc class */
	&ti81xx_tpcc_hwmod,
	NULL,
};

static __initdata struct omap_hwmod *ti814x_hwmods[] = {
	/* uart class */
	&ti814x_uart4_hwmod,
	&ti814x_uart5_hwmod,
	&ti814x_uart6_hwmod,
	/* dcan class */
//	&ti814x_dcan0_hwmod,
//	&ti814x_dcan1_hwmod,
	/* cpgmac0 class */
	&ti814x_cpgmac0_hwmod,
	/* mdio class */
	&ti814x_mdio_hwmod,
	/* timer class */
	&ti814x_timer1_hwmod,
	&ti814x_timer2_hwmod,
	&ti814x_timer3_hwmod,
	&ti814x_timer4_hwmod,
	&ti814x_timer5_hwmod,
	&ti814x_timer6_hwmod,
	&ti814x_timer7_hwmod,
	&ti814x_timer8_hwmod,
	/* watchdog timer class */
	&ti814x_wd_timer1_hwmod,
	NULL,
};

static __initdata struct omap_hwmod *ti816x_hwmods[] = {
	/* timer class */
	&ti816x_timer1_hwmod,
	&ti816x_timer2_hwmod,
	&ti816x_timer3_hwmod,
	&ti816x_timer4_hwmod,
	&ti816x_timer5_hwmod,
	&ti816x_timer6_hwmod,
	&ti816x_timer7_hwmod,
	/* watchdog timer class */
	&ti816x_wd_timer2_hwmod,
	NULL,
};

int __init ti81xx_hwmod_init(void)
{
	int r;
	struct omap_hwmod **h = NULL;

	/* Register hwmods common to all TI81XX */
	r = omap_hwmod_register(ti81xx_hwmods);
	if (r < 0)
		return r;

	/* Register CPU-specific hwmods */
	if (cpu_is_ti814x())
		h = ti814x_hwmods;
	else if (cpu_is_ti816x())
		h = ti816x_hwmods;
	
	if (h)
		r = omap_hwmod_register(h);
	
	return r;
}
