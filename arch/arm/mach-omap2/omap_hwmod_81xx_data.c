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
#include <plat/serial.h>
#include <plat/l4_3xxx.h>
#include <plat/ti81xx.h>

#include "omap_hwmod_common_data.h"

#include "control.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"

static struct omap_hwmod ti81xx_mpu_hwmod;
static struct omap_hwmod ti81xx_l3_slow_hwmod;
static struct omap_hwmod ti81xx_l4_slow_hwmod;

/* L3 SLOW -> L4_SLOW Peripheral interface */
static struct omap_hwmod_ocp_if ti81xx_l3_slow__l4_slow = {
	.master	= &ti81xx_l3_slow_hwmod,
	.slave	= &ti81xx_l4_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU -> L3 SLOW interface */
static struct omap_hwmod_ocp_if ti81xx_mpu__l3_slow = {
	.master = &ti81xx_mpu_hwmod,
	.slave	= &ti81xx_l3_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* Slave interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l3_slow_slaves[] = {
	&ti81xx_mpu__l3_slow,
};

/* Master interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l3_slow_masters[] = {
	&ti81xx_l3_slow__l4_slow,
};

/* L3 SLOW */
static struct omap_hwmod ti816x_l3_slow_hwmod = {
	.name		= "l3_slow",
	.class		= &l3_hwmod_class,
	.masters	= ti816x_l3_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_l3_slow_masters),
	.slaves		= ti816x_l3_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
	.flags		= HWMOD_NO_IDLEST,
};

static struct omap_hwmod ti816x_uart1_hwmod;
static struct omap_hwmod ti816x_uart2_hwmod;
static struct omap_hwmod ti816x_uart3_hwmod;
static struct omap_hwmod ti814x_uart4_hwmod;
static struct omap_hwmod ti814x_uart5_hwmod;
static struct omap_hwmod ti814x_uart6_hwmod;
static struct omap_hwmod ti816x_wd_timer2_hwmod;
static struct omap_hwmod ti814x_wd_timer1_hwmod;
static struct omap_hwmod ti81xx_i2c1_hwmod;
static struct omap_hwmod ti816x_i2c2_hwmod;
static struct omap_hwmod ti814x_i2c3_hwmod;
static struct omap_hwmod ti814x_i2c4_hwmod;
static struct omap_hwmod ti81xx_gpio1_hwmod;
static struct omap_hwmod ti81xx_gpio2_hwmod;
static struct omap_hwmod ti814x_gpio3_hwmod;
static struct omap_hwmod ti814x_gpio4_hwmod;
static struct omap_hwmod ti81xx_usbss_hwmod;
static struct omap_hwmod ti81xx_elm_hwmod;
struct omap_hwmod_ocp_if ti81xx_l4_slow__elm;

/* L4 SLOW -> UART1 interface */
static struct omap_hwmod_addr_space ti816x_uart1_addr_space[] = {
	{
		.pa_start	= TI81XX_UART1_BASE,
		.pa_end		= TI81XX_UART1_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_uart1_hwmod,
	.clk		= "uart1_ick",
	.addr		= ti816x_uart1_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart1_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART2 interface */
static struct omap_hwmod_addr_space ti816x_uart2_addr_space[] = {
	{
		.pa_start	= TI81XX_UART2_BASE,
		.pa_end		= TI81XX_UART2_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_uart2_hwmod,
	.clk		= "uart2_ick",
	.addr		= ti816x_uart2_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart2_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART3 interface */
static struct omap_hwmod_addr_space ti816x_uart3_addr_space[] = {
	{
		.pa_start	= TI81XX_UART3_BASE,
		.pa_end		= TI81XX_UART3_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_uart3_hwmod,
	.clk		= "uart3_ick",
	.addr		= ti816x_uart3_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart3_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART4 interface */
static struct omap_hwmod_addr_space ti814x_uart4_addr_space[] = {
	{
		.pa_start	= TI814X_UART4_BASE,
		.pa_end		= TI814X_UART4_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart4_hwmod,
	.clk		= "uart4_ick",
	.addr		= ti814x_uart4_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart4_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART5 interface */
static struct omap_hwmod_addr_space ti814x_uart5_addr_space[] = {
	{
		.pa_start	= TI814X_UART5_BASE,
		.pa_end		= TI814X_UART5_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart5 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart5_hwmod,
	.clk		= "uart5_ick",
	.addr		= ti814x_uart5_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart5_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART6 interface */
static struct omap_hwmod_addr_space ti814x_uart6_addr_space[] = {
	{
		.pa_start	= TI814X_UART6_BASE,
		.pa_end		= TI814X_UART6_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart6 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_uart6_hwmod,
	.clk		= "uart6_ick",
	.addr		= ti814x_uart6_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart6_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> Watchdog */
static struct omap_hwmod_addr_space ti816x_wd_timer2_addrs[] = {
	{
		.pa_start	= 0x480C2000,
		.pa_end		= 0x480C2FFF,
		.flags		= ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_addr_space ti814x_wd_timer1_addrs[] = {
	{
		.pa_start	= 0x481C7000,
		.pa_end		= 0x481C7FFF,
		.flags		= ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__wd_timer2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_wd_timer2_hwmod,
	.clk		= "wdt2_ick",
	.addr		= ti816x_wd_timer2_addrs,
	.addr_cnt	= ARRAY_SIZE(ti816x_wd_timer2_addrs),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__wd_timer1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_wd_timer1_hwmod,
	.clk		= "wdt1_ick",
	.addr		= ti814x_wd_timer1_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_wd_timer1_addrs),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C1 */
static struct omap_hwmod_addr_space ti816x_i2c1_addr_space[] = {
	{
		.pa_start	= 0x48028000,
		.pa_end		= 0x48028000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_i2c1_hwmod,
	.clk		= "i2c1_ick",
	.addr		= ti816x_i2c1_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_i2c1_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C2 */
static struct omap_hwmod_addr_space ti816x_i2c2_addr_space[] = {
	{
		.pa_start	= 0x4802A000,
		.pa_end		= 0x4802A000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti816x_i2c2_hwmod,
	.clk		= "i2c2_ick",
	.addr		= ti816x_i2c2_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_i2c2_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti814x_i2c3_addr_space[] = {
	{
		.pa_start	= 0x4819C000,
		.pa_end		= 0x4819C000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__i2c3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_i2c3_hwmod,
	.clk		= "i2c3_ick",
	.addr		= ti814x_i2c3_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_i2c3_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti814x_i2c4_addr_space[] = {
	{
		.pa_start       = 0x4819E000,
		.pa_end         = 0x4819E000 + SZ_4K - 1,
		.flags          = ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__i2c4 = {
	.master         = &ti81xx_l4_slow_hwmod,
	.slave          = &ti814x_i2c4_hwmod,
	.clk            = "i2c4_ick",
	.addr           = ti814x_i2c4_addr_space,
	.addr_cnt       = ARRAY_SIZE(ti814x_i2c4_addr_space),
	.user           = OCP_USER_MPU,
};



/* L4 SLOW -> GPIO1 */
static struct omap_hwmod_addr_space ti81xx_gpio1_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO0_BASE,
		.pa_end		= TI81XX_GPIO0_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio1 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_gpio1_hwmod,
	.addr		= ti81xx_gpio1_addrs,
	.addr_cnt	= ARRAY_SIZE(ti81xx_gpio1_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO2 */
static struct omap_hwmod_addr_space ti81xx_gpio2_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO1_BASE,
		.pa_end		= TI81XX_GPIO1_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio2 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti81xx_gpio2_hwmod,
	.addr		= ti81xx_gpio2_addrs,
	.addr_cnt	= ARRAY_SIZE(ti81xx_gpio2_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO3 */
static struct omap_hwmod_addr_space ti814x_gpio3_addrs[] = {
	{
		.pa_start	= TI814X_GPIO2_BASE,
		.pa_end		= TI814X_GPIO2_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio3 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_gpio3_hwmod,
	.addr		= ti814x_gpio3_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio3_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO4 */
static struct omap_hwmod_addr_space ti814x_gpio4_addrs[] = {
	{
		.pa_start	= TI814X_GPIO3_BASE,
		.pa_end		= TI814X_GPIO3_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio4 = {
	.master		= &ti81xx_l4_slow_hwmod,
	.slave		= &ti814x_gpio4_hwmod,
	.addr		= ti814x_gpio4_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio4_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* Slave interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l4_slow_slaves[] = {
	&ti81xx_l3_slow__l4_slow,
};

/* Master interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti81xx_l4_slow_masters[] = {
	&ti816x_l4_slow__uart1,
	&ti816x_l4_slow__uart2,
	&ti816x_l4_slow__uart3,
	&ti814x_l4_slow__uart4,
	&ti814x_l4_slow__uart5,
	&ti814x_l4_slow__uart6,
	&ti816x_l4_slow__wd_timer2,
	&ti816x_l4_slow__i2c1,
	&ti816x_l4_slow__i2c2,
	&ti814x_l4_slow__i2c3,
	&ti814x_l4_slow__i2c4,
	&ti81xx_l4_slow__gpio1,
	&ti81xx_l4_slow__gpio2,
	&ti814x_l4_slow__gpio3,
	&ti814x_l4_slow__gpio4,
	&ti81xx_l4_slow__elm,
};

/* L4 SLOW */
static struct omap_hwmod ti81xx_l4_slow_hwmod = {
	.name		= "l4_slow",
	.class		= &l4_hwmod_class,
	.masters	= ti81xx_l4_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_l4_slow_masters),
	.slaves		= ti81xx_l4_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_l4_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
	.flags		= HWMOD_NO_IDLEST,
};

/* Master interfaces on the MPU device */
static struct omap_hwmod_ocp_if *ti81xx_mpu_masters[] = {
	&ti81xx_mpu__l3_slow,
};

/* MPU */
static struct omap_hwmod ti81xx_mpu_hwmod = {
	.name		= "mpu",
	.class		= &mpu_hwmod_class,
	.main_clk	= "mpu_ck",
	.vdd_name	= "mpu",
	.masters	= ti81xx_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(ti81xx_mpu_masters),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* cpgmac0 */
static struct omap_hwmod_class_sysconfig ti81xx_cpgmac_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x8,
	.syss_offs	= 0x4,
	.rst_offs	= 0x8,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_MIDLEMODE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | MSTANDBY_FORCE |
			MSTANDBY_NO),
	.sysc_fields	= &omap_hwmod_sysc_type3,
};

static struct omap_hwmod_class ti81xx_cpgmac0_hwmod_class = {
	.name		= "cpsw",
	.sysc		= &ti81xx_cpgmac_sysc,
	.reset		= ti81xx_cpgmac_reset,
};

/* Used by driver */
struct omap_hwmod_addr_space ti81xx_cpgmac0_addr_space[] = {
	/* cpsw ss */
	{
		.pa_start	= 0x4A100000,
		.pa_end		= 0x4A100000 + SZ_2K - 1,
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

struct omap_hwmod_ocp_if ti81xx_l3_main__cpgmac0 = {
	.master		= &ti81xx_l3_main_hwmod,
	.slave		= &ti81xx_cpgmac0_hwmod,
	.addr		= ti81xx_cpgmac0_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti81xx_cpsw_sl1_addr_space[] = {
	/* cpsw sl1 */
	{
		.pa_start	= 0x4A100700,
		.pa_end		= 0x4A100700 + SZ_32 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__cpsw_sl1 = {
	.master		= &am33xx_l3_main_hwmod,
	.slave		= &ti81xx_cpgmac0_hwmod,
	.addr		= ti81xx_cpsw_sl1_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti81xx_cpsw_sl2_addr_space[] = {
	/* cpsw sl2 */
	{
		.pa_start	= 0x4A100740,
		.pa_end		= 0x4A100740 + SZ_32 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__cpsw_sl2 = {
	.master		= &ti81xx_l3_main_hwmod,
	.slave		= &ti81xx_cpgmac0_hwmod,
	.addr		= ti81xx_cpsw_sl2_addr_space,
	.user		= OCP_USER_MPU,
};

struct omap_hwmod_addr_space ti81xx_cpsw_cpdma_addr_space[] = {
	/* cpsw cpdma */
	{
		.pa_start	= 0x4A100100,
		.pa_end		= 0x4A100100 + SZ_32 - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__cpsw_cpdma = {
	.master		= &ti81xx_l3_main_hwmod,
	.slave		= &ti81xx_cpgmac0_hwmod,
	.addr		= ti81xx_cpsw_cpdma_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_cpgmac0_slaves[] = {
	&ti81xx_l3_main__cpgmac0,
	&ti81xx_l3_main__cpsw_sl1,
	&ti81xx_l3_main__cpsw_sl2,
	&ti81xx_l3_main__cpsw_cpdma,
};

static struct omap_hwmod_irq_info ti81xx_cpgmac0_irqs[] = {
	{ .name = "c0_rx_thresh_pend", .irq = 40 },
	{ .name = "c0_rx_pend", .irq = 41 },
	{ .name = "c0_tx_pend", .irq = 42 },
	{ .name = "c0_misc_pend", .irq = 43 },
	{ .irq = -1 }
};

static struct omap_hwmod ti81xx_cpgmac0_hwmod = {
	.name		= "cpgmac0",
	.class		= &ti81xx_cpgmac0_hwmod_class,
	.clkdm_name	= "cpsw_125mhz_clkdm",
	.mpu_irqs	= ti81xx_cpgmac0_irqs,
	.main_clk	= "cpgmac0_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ETHERNET_CLKSTCTRL_OFFSET,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_cpgmac0_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_cpgmac0_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY |
				HWMOD_SWSUP_RESET_BEFORE_IDLE),
};

/* mdio class */
static struct omap_hwmod_class ti81xx_mdio_hwmod_class = {
	.name		= "davinci_mdio",
};

struct omap_hwmod_addr_space ti81xx_mdio_addr_space[] = {
	{
		.pa_start	= 0x4A100800,
		.pa_end		= 0x4A100800 + SZ_256 - 1,
		.flags		= ADDR_MAP_ON_INIT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_cpgmac0__mdio = {
	.master		= &ti81xx_cpgmac0_hwmod,
	.slave		= &ti81xx_mdio_hwmod,
	.addr		= ti81xx_mdio_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mdio_slaves[] = {
	&ti81xx_cpgmac0__mdio,
};

static struct omap_hwmod ti81xx_mdio_hwmod = {
	.name		= "mdio",
	.class		= &ti81xx_mdio_hwmod_class,
	.clkdm_name	= "cpsw_125mhz_clkdm",
	.main_clk	= "cpgmac0_ick",
	.slaves		= ti81xx_mdio_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mdio_slaves),
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

static struct omap_hwmod_ocp_if ti814x_l4_per__dcan0 = {
	.master		= &am33xx_l4per_hwmod,
	.slave		= &ti814x_dcan0_hwmod,
	.clk		= "dcan0_ick",
	.addr		= ti81xx_dcan0_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *ti814x_dcan0_slaves[] = {
	&ti814x_l4_per__dcan0,
};

static struct omap_hwmod ti814x_dcan0_hwmod = {
	.name		= "d_can0",
	.class		= &ti81xx_dcan_hwmod_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_dcan0_irqs,
	.main_clk	= "dcan0_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_DCAN_0_1_CLKCTRL,
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

static struct omap_hwmod_ocp_if ti814x_l4_per__dcan0 = {
	.master		= &am33xx_l4per_hwmod,
	.slave		= &ti814x_dcan1_hwmod,
	.clk		= "dcan1_ick",
	.addr		= ti814x_dcan1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *ti814x_dcan1_slaves[] = {
	&ti814x_l4_per__dcan1,
};

static struct omap_hwmod ti814x_dcan1_hwmod = {
	.name		= "d_can1",
	.class		= &ti81xx_dcan_hwmod_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_dcan1_irqs,
	.main_clk	= "dcan1_fck",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_DCAN_0_1_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti814x_dcan1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_dcan1_slaves),
};

/* UART common */

static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
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
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x90,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class i2c_class = {
	.name = "i2c",
	.sysc = &i2c_sysc,
};

/*
 * 'gpio' class
 * general purpose io module
 */
static struct omap_hwmod_class_sysconfig ti81xx_gpio_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0114,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_ENAWAKEUP |
			   SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET |
			   SYSS_HAS_RESET_STATUS),
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

static struct omap_hwmod_irq_info uart1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART0, },
};

/*
 * There is no SDMA on TI81XX, instead we have EDMA. Presently using dummy
 * channel numbers as the omap UART driver (drivers/serial/omap-serial.c)
 * requires these values to be filled in even if we don't have DMA enabled. Same
 * applies for other UARTs below.
 */
static struct omap_hwmod_dma_info uart1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart1_slaves[] = {
	&ti816x_l4_slow__uart1,
};

static struct omap_hwmod ti816x_uart1_hwmod = {
	.name		= "uart1",
	.mpu_irqs	= uart1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart1_mpu_irqs),
	.sdma_reqs	= uart1_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart1_edma_reqs),
	.main_clk	= "uart1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_0_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart1_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART2 */

static struct omap_hwmod_irq_info uart2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART1, },
};

static struct omap_hwmod_dma_info uart2_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart2_slaves[] = {
	&ti816x_l4_slow__uart2,
};

static struct omap_hwmod ti816x_uart2_hwmod = {
	.name		= "uart2",
	.mpu_irqs	= uart2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart2_mpu_irqs),
	.sdma_reqs	= uart2_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart2_edma_reqs),
	.main_clk	= "uart2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_1_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart2_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART3 */

static struct omap_hwmod_irq_info uart3_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART2, },
};

static struct omap_hwmod_dma_info uart3_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart3_slaves[] = {
	&ti816x_l4_slow__uart3,
};

static struct omap_hwmod ti816x_uart3_hwmod = {
	.name		= "uart3",
	.mpu_irqs	= uart3_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart3_mpu_irqs),
	.sdma_reqs	= uart3_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart3_edma_reqs),
	.main_clk	= "uart3_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_2_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart3_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART4 */

static struct omap_hwmod_irq_info uart4_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART3, },
};

static struct omap_hwmod_dma_info uart4_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart4_slaves[] = {
	&ti814x_l4_slow__uart4,
};

static struct omap_hwmod ti814x_uart4_hwmod = {
	.name		= "uart4",
	.mpu_irqs	= uart4_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart4_mpu_irqs),
	.sdma_reqs	= uart4_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart4_edma_reqs),
	.main_clk	= "uart4_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_3_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart4_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X),
};

/* UART5 */

static struct omap_hwmod_irq_info uart5_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART4, },
};

static struct omap_hwmod_dma_info uart5_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart5_slaves[] = {
	&ti814x_l4_slow__uart5,
};

static struct omap_hwmod ti814x_uart5_hwmod = {
	.name		= "uart5",
	.mpu_irqs	= uart5_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart5_mpu_irqs),
	.sdma_reqs	= uart5_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart5_edma_reqs),
	.main_clk	= "uart5_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_4_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart5_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X),
};

/* UART6 */

static struct omap_hwmod_irq_info uart6_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART5, },
};

static struct omap_hwmod_dma_info uart6_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart6_slaves[] = {
	&ti814x_l4_slow__uart6,
};

static struct omap_hwmod ti814x_uart6_hwmod = {
	.name		= "uart6",
	.mpu_irqs	= uart6_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart6_mpu_irqs),
	.sdma_reqs	= uart6_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart6_edma_reqs),
	.main_clk	= "uart6_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_5_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart6_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X),
};

/* Watchdog */

static struct omap_hwmod_ocp_if *ti816x_wd_timer2_slaves[] = {
	&ti816x_l4_slow__wd_timer2,
};

static struct omap_hwmod_ocp_if *ti814x_wd_timer1_slaves[] = {
	&ti814x_l4_slow__wd_timer1,
};

static struct omap_hwmod ti816x_wd_timer2_hwmod = {
	.name		= "wd_timer2",
	.main_clk	= "wdt2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
		},
	},
	.slaves		= ti816x_wd_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_wd_timer2_slaves),
	.class		= &wd_timer_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.flags	  = HWMOD_INIT_NO_RESET,
};


static struct omap_hwmod ti814x_wd_timer1_hwmod = {
	.name		= "wd_timer1",
	.main_clk	= "wdt1_fck",
	.prcm		= {
		.omap4		= {
			.clkctrl_reg = TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
		},
	},
	.slaves		= ti814x_wd_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_wd_timer1_slaves),
	.class		= &wd_timer_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385),
	.flags	  = HWMOD_INIT_NO_RESET,
};

/* I2C1 */

static struct omap_hwmod_irq_info i2c1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C0, },
};

static struct omap_hwmod_dma_info i2c1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_i2c1_slaves[] = {
	&ti816x_l4_slow__i2c1,
};

static struct omap_hwmod ti81xx_i2c1_hwmod = {
	.name		= "i2c1",
	.mpu_irqs	= i2c1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c1_mpu_irqs),
	.sdma_reqs	= i2c1_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c1_edma_reqs),
	.main_clk	= "i2c1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_0_CLKCTRL,
		},
	},
	.slaves		= ti816x_i2c1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_i2c1_slaves),
	.class		= &i2c_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* I2C2 */

static struct omap_hwmod_irq_info i2c2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C1, },
};

static struct omap_hwmod_dma_info i2c2_edma_reqs[] = {
	{ .name = "tx", .dma_req = 0, },
	{ .name = "rx", .dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_i2c2_slaves[] = {
	&ti816x_l4_slow__i2c2,
};

static struct omap_hwmod ti816x_i2c2_hwmod = {
	.name           = "i2c2",
	.mpu_irqs       = i2c2_mpu_irqs,
	.mpu_irqs_cnt   = ARRAY_SIZE(i2c2_mpu_irqs),
	.sdma_reqs      = i2c2_edma_reqs,
	.sdma_reqs_cnt  = ARRAY_SIZE(i2c2_edma_reqs),
	.main_clk       = "i2c2_fck",
	.prcm           = {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_1_CLKCTRL,
		},
	},
	.slaves         = ti816x_i2c2_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti816x_i2c2_slaves),
	.class          = &i2c_class,
	.omap_chip      = OMAP_CHIP_INIT(CHIP_IS_TI816X),
};
/* I2C3 */
static struct omap_hwmod_irq_info i2c3_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_I2C2, },
};

static struct omap_hwmod_dma_info i2c3_edma_reqs[] = {
	{ .name = "tx", .dma_req = 0, },
	{ .name = "rx", .dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_i2c3_slaves[] = {
	&ti814x_l4_slow__i2c3,
};

static struct omap_hwmod ti814x_i2c3_hwmod = {
	.name           = "i2c3",
	.mpu_irqs       = i2c3_mpu_irqs,
	.mpu_irqs_cnt   = ARRAY_SIZE(i2c3_mpu_irqs),
	.sdma_reqs      = i2c3_edma_reqs,
	.sdma_reqs_cnt  = ARRAY_SIZE(i2c3_edma_reqs),
	.main_clk       = "i2c3_fck",
	.prcm           = {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_0_CLKCTRL,
		},
	},
	.slaves         = ti814x_i2c3_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti814x_i2c3_slaves),
	.class          = &i2c_class,
	.omap_chip      = OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385),
};

/* I2C4 */

static struct omap_hwmod_irq_info i2c4_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_I2C3, },
};

static struct omap_hwmod_dma_info i2c4_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_i2c4_slaves[] = {
	&ti814x_l4_slow__i2c4,
};

static struct omap_hwmod ti814x_i2c4_hwmod = {
	.name		= "i2c4",
	.mpu_irqs	= i2c4_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c4_mpu_irqs),
	.sdma_reqs	= i2c4_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c4_edma_reqs),
	.main_clk	= "i2c4_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_1_CLKCTRL,
		},
	},
	.slaves		= ti814x_i2c4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_i2c4_slaves),
	.class		= &i2c_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385),
};

/* ELM */
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
/* 'elm' class */
static struct omap_hwmod_class ti81xx_elm_hwmod_class = {
	.name = "elm",
	.sysc = &ti81xx_elm_sysc,
};

static struct omap_hwmod_irq_info ti81xx_elm_irqs[] = {
	{ .irq = TI81XX_IRQ_ELM },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_elm_addr_space[] = {
	{
		.pa_start	= TI81XX_ELM_BASE,
		.pa_end		= TI81XX_ELM_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	}
};

struct omap_hwmod_ocp_if ti81xx_l4_slow__elm = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_elm_hwmod,
	.addr		= ti81xx_elm_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti81xx_elm_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_elm_slaves[] = {
	&ti81xx_l4_slow__elm,
};

/* elm */
static struct omap_hwmod ti81xx_elm_hwmod = {
	.name           = "elm",
	.class          = &ti81xx_elm_hwmod_class,
	.main_clk       = "elm_fck",
	.mpu_irqs		= ti81xx_elm_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_elm_irqs),
	.slaves			= ti81xx_elm_slaves,
	.slaves_cnt		= ARRAY_SIZE(ti81xx_elm_slaves),
	.omap_chip		= OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X
						| CHIP_IS_DM385),
};

/* GPIO1 TI81XX */

static struct omap_hwmod_irq_info ti81xx_gpio1_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_0A },
	{ .irq = TI81XX_IRQ_GPIO_0B },
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
	.mpu_irqs	= ti81xx_gpio1_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_gpio1_irqs),
	.main_clk	= "gpio1_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_0_CLKCTRL,
		},
	},
	.opt_clks	= gpio1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio1_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio1_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* GPIO2 TI81XX*/

static struct omap_hwmod_irq_info ti81xx_gpio2_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_1A },
	{ .irq = TI81XX_IRQ_GPIO_1B },
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
	.mpu_irqs	= ti81xx_gpio2_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_gpio2_irqs),
	.main_clk	= "gpio2_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio2_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio2_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* GPIO3 TI814X */

static struct omap_hwmod_irq_info ti814x_gpio3_irqs[] = {
	{ .irq = TI814X_IRQ_GPIO_2A },
	{ .irq = TI814X_IRQ_GPIO_2B },
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
	.mpu_irqs	= ti814x_gpio3_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio3_irqs),
	.main_clk	= "gpio3_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio3_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio3_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio3_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385),
};

/* GPIO4 TI814X*/

static struct omap_hwmod_irq_info ti814x_gpio4_irqs[] = {
	{ .irq = TI814X_IRQ_GPIO_3A },
	{ .irq = TI814X_IRQ_GPIO_3B },
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
	.mpu_irqs	= ti814x_gpio4_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio4_irqs),
	.main_clk	= "gpio4_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio4_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio4_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio4_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385),
};

/* L3 SLOW -> USBSS interface */
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
};

static struct omap_hwmod_class_sysconfig ti81xx_usbhsotg_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
};

static struct omap_hwmod_class ti81xx_usbotg_class = {
	.name = "usbotg",
	.sysc = &ti81xx_usbhsotg_sysc,
};

static struct omap_hwmod_irq_info ti81xx_usbss_mpu_irqs[] = {
	{ .name = "usbss-irq", .irq = 17, },
	{ .name = "musb0-irq", .irq = 18, },
	{ .name = "musb1-irq", .irq = 19, },
};

static struct omap_hwmod_ocp_if ti81xx_l3_slow__usbss = {
	.master		= &ti816x_l3_slow_hwmod,
	.slave		= &ti81xx_usbss_hwmod,
	.clk		= "usb_ick",
	.addr		= ti81xx_usbss_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti81xx_usbss_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_usbss_slaves[] = {
	&ti81xx_l3_slow__usbss,
};

static struct omap_hwmod ti81xx_usbss_hwmod = {
	.name		= "usb_otg_hs",
	.mpu_irqs	= ti81xx_usbss_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_usbss_mpu_irqs),
	.main_clk	= "usb_ick",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_DEFAULT_USB_CLKCTRL,
		},
	},
	.slaves		= ti81xx_usbss_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_usbss_slaves),
	.class		= &ti81xx_usbotg_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX)
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
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_mailbox_hwmod_class = {
	.name = "mailbox",
	.sysc = &ti81xx_mailbox_sysc,
};

static struct omap_hwmod ti81xx_mailbox_hwmod;
static struct omap_hwmod_irq_info ti81xx_mailbox_irqs[] = {
	{ .irq = 77 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mailbox_addrs[] = {
	{
		.pa_start	= 0x480C8000,
		.pa_end		= 0x480C81ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

/* l4_slow -> mailbox */
static struct omap_hwmod_ocp_if ti81xx_l4_core__mailbox = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_mailbox_hwmod,
	.addr		= ti81xx_mailbox_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* mailbox slave ports */
static struct omap_hwmod_ocp_if *ti81xx_mailbox_slaves[] = {
	&ti81xx_l4_core__mailbox,
};

static struct omap_hwmod ti81xx_mailbox_hwmod = {
	.name		= "mailbox",
	.class		= &ti81xx_mailbox_hwmod_class,
	.mpu_irqs	= ti81xx_mailbox_irqs,
	.main_clk	= "mailboxes_ick",
	.prcm		= {
		.omap2 = {
			.prcm_reg_id = 1,
			.module_bit = OMAP3430_EN_MAILBOXES_SHIFT,
			.module_offs = CORE_MOD,
			.idlest_reg_id = 1,
			.idlest_idle_bit = OMAP3430_ST_MAILBOXES_SHIFT,
		},
	},
	.slaves		= ti81xx_mailbox_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mailbox_slaves),
};

/* l4 core -> mcspi4 interface */
static struct omap_hwmod_addr_space omap34xx_mcspi4_addr_space[] = {
	{
		.pa_start	= 0x480ba000,
		.pa_end		= 0x480ba0ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if omap34xx_l4_core__mcspi4 = {
	.master		= &omap3xxx_l4_core_hwmod,
	.slave		= &omap34xx_mcspi4,
	.clk		= "mcspi4_ick",
	.addr		= omap34xx_mcspi4_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/*
 * 'mcspi' class
 * multichannel serial port interface (mcspi) / master/slave synchronous serial
 * bus
 */

static struct omap_hwmod_class_sysconfig ti81xx_mcspi_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
			SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE |
			SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_mcspi_class = {
	.name = "mcspi",
	.sysc = &omap34xx_mcspi_sysc,
	.rev = OMAP4_MCSPI_REV,
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
	{ .name = "rx2", .dma_req = 21 },
	{ .name = "tx2", .dma_req = 20 },
	{ .name = "rx3", .dma_req = 23 },
	{ .name = "tx3", .dma_req = 22 },
	{ .dma_req = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi1_addr_space[] = {
	{
		.pa_start	= 0x48030100,
		.pa_end		= 0x480301ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_core__mcspi1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi1,
	.clk		= "mcspi1_ick",
	.addr		= ti81xx_mcspi1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi1_slaves[] = {
	&ti81xx_l4_core__mcspi1,
};

static struct omap2_mcspi_dev_attr mcspi1_attrib = {
	.num_chipselect = 4,
};

static struct omap_hwmod ti81xx_mcspi1_hwmod = {
	.name		= "mcspi1",
	.class		= &ti81xx_mcspi_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_spi1_irqs,
	.main_clk	= "mcspi1_fck",
	.sdma_reqs	= ti81xx_spi1_edma_reqs,
	.prcm		= {
		.omap4	= {
			/* HACK: It seems like this clkctrl_reg is shared by all the devices. */
			.clkctrl_reg = TI81XX_CM_ALWON_SPI_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi1_attrib,
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
		.pa_start	= 0x481A0100,
		.pa_end		= 0x481A01ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_core__mcspi2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_mcspi2,
	.clk		= "mcspi2_ick",
	.addr		= ti81xx_mcspi2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi2_slaves[] = {
	&ti81xx_l4_core__mcspi2,
};

static struct omap2_mcspi_dev_attr mcspi2_attrib = {
	.num_chipselect = 2,
};

static struct omap_hwmod ti81xx_mcspi2_hwmod = {
	.name		= "mcspi2",
	.class		= &ti81xx_mcspi_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_spi2_irqs,
	.main_clk	= "mcspi2_fck",
	.sdma_reqs	= ti81xx_spi2_edma_reqs,
	.prcm		= {
		.omap4	= {
			/* HACK: It seems like this clkctrl_reg is shared by all the devices. */
			.clkctrl_reg = TI81XX_CM_ALWON_SPI_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi2_attrib,
	.slaves		= ti81xx_mcspi2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi2_slaves),
};

/* mcspi3 */
static struct omap_hwmod_irq_info ti81xx_spi3_irqs[] = {
	{ .irq = 126 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi3_addr_space[] = {
	{
		.pa_start	= 0x481A2100,
		.pa_end		= 0x481A21ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_core__mcspi3 = {
	.master		= &ti816x_l4_core_hwmod,
	.slave		= &ti81xx_mcspi3,
	.clk		= "mcspi3_ick",
	.addr		= ti81xx_mcspi3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi3_slaves[] = {
	&ti81xx_l4_core__mcspi3,
};

static struct omap2_mcspi_dev_attr mcspi3_attrib = {
	.num_chipselect = 2,
};

static struct omap_hwmod ti81xx_mcspi3_hwmod = {
	.name		= "mcspi3",
	.class		= &ti81xx_mcspi_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_spi3_irqs,
	.main_clk	= "mcspi3_fck",
	.prcm		= {
		.omap4	= {
			/* HACK: It seems like this clkctrl_reg is shared by all the devices. */
			.clkctrl_reg = TI81XX_CM_ALWON_SPI_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &mcspi3_attrib,
	.slaves		= ti81xx_mcspi3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mcspi3_slaves),
};

/* SPI4 */
static struct omap_hwmod_irq_info ti81xx_spi4_irqs[] = {
	{ .irq = 127 },
	{ .irq = -1 }
};

static struct omap_hwmod_addr_space ti81xx_mcspi4_addr_space[] = {
	{
		.pa_start	= 0x481A4100,
		.pa_end		= 0x481A41ff,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if *ti81xx_mcspi4_slaves[] = {
	&ti81xx_l4_core__mcspi4,
};

static struct omap2_mcspi_dev_attr mcspi4_attrib = {
	.num_chipselect = 1,
};

static struct omap_hwmod ti81xx_mcspi4_hwmod = {
	.name		= "mcspi4",
	.class		= &ti81xx_mcspi_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_spi4_irqs,
	.main_clk	= "mcspi4_fck",
	.prcm		= {
		.omap4	= {
			/* HACK: It seems like this clkctrl_reg is shared by all the devices. */
			.clkctrl_reg = TI81XX_CM_ALWON_SPI_CLKCTRL,
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

static struct omap_hwmod_addr_space ti81xx_mmc0_addr_space[] = {
	{
		.pa_start	= 0x48060100,
		.pa_end		= 0x48060100 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4ls__mmc0 = {
	.master		= &ti816x_l4_core_hwmod,
	.slave		= &ti81xx_mmc0_hwmod,
	.clk		= "mmc0_ick",
	.addr		= ti81xx_mmc0_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc0_slaves[] = {
	&ti81xx_l4ls__mmc0,
};

static struct omap_mmc_dev_attr ti81xx_mmc0_dev_attr = {
	.flags		= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti81xx_mmc0_hwmod = {
	.name		= "mmc1",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_mmc0_irqs,
	.main_clk	= "mmc0_fck",
	.sdma_reqs	= ti81xx_mmc0_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_0_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
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

static struct omap_hwmod_addr_space ti81xx_mmc1_addr_space[] = {
	{
		.pa_start	= 0x481D8100,
		.pa_end		= 0x481D8100 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4ls__mmc1 = {
	.master		= &ti816x_l4_core_hwmod,
	.slave		= &ti81xx_mmc1_hwmod,
	.clk		= "mmc1_ick",
	.addr		= ti81xx_mmc1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc1_slaves[] = {
	&ti81xx_l4ls__mmc1,
};

static struct omap_mmc_dev_attr ti81xx_mmc1_dev_attr = {
	.flags		= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti81xx_mmc1_hwmod = {
	.name		= "mmc2",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "l4ls_clkdm",
	.mpu_irqs	= ti81xx_mmc1_irqs,
	.main_clk	= "mmc1_fck",
	.sdma_reqs	= ti81xx_mmc1_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_1_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
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

static struct omap_hwmod_ocp_if ti81xx_l3_main__mmc2 = {
	.master		= &ti816x_l3_slow_hwmod,
	.slave		= &ti81xx_mmc2_hwmod,
	.clk		= "mmc2_ick",
	.addr		= ti81xx_mmc2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_mmc2_slaves[] = {
	&ti81xx_l3_main__mmc2,
};

static struct omap_mmc_dev_attr ti81xx_mmc2_dev_attr = {
	.flags		= OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};
static struct omap_hwmod ti81xx_mmc2_hwmod = {
	.name		= "mmc3",
	.class		= &ti81xx_mmc_hwmod_class,
	.clkdm_name	= "l3s_clkdm",
	.mpu_irqs	= ti81xx_mmc2_irqs,
	.main_clk	= "mmc2_fck",
	.sdma_reqs	= ti81xx_mmc2_edma_reqs,
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI814X_CM_ALWON_MMCHS_2_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.dev_attr	= &ti81xx_mmc2_dev_attr,
	.slaves		= ti81xx_mmc2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_mmc2_slaves),
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

static struct omap_hwmod_addr_space am33xx_tpcc_addr_space[] = {
	{
		.name		= "edma_cc0",
		.pa_start	= TI81XX_TPCC_BASE,
		.pa_end		= TI81XX_TPCC_BASE + SZ_32K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l3_main__tpcc = {
	.master		= &ti816x_l3_slow_hwmod,
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
	.clkdm_name	= "l3_clkdm",
	.mpu_irqs	= ti81xx_tpcc_irqs,
	.main_clk	= "tpcc_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPCC_CLKCTRL,
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
	.master		= &ti816x_l3_slow_hwmod,
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
	.clkdm_name	= "l3_clkdm",
	.mpu_irqs	= ti81xx_tptc0_irqs,
	.main_clk	= "tptc0_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC0_CLKCTRL,
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
		.pa_start	= TI81xx_TPTC1_BASE,
		.pa_end		= TI81XX_TPTC1_BASE + SZ_1K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

struct omap_hwmod_ocp_if ti81xx_l3_main__tptc1 = {
	.master		= &ti816x_l3_slow_hwmod,
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
	.clkdm_name	= "l3_clkdm",
	.mpu_irqs	= ti81xx_tptc1_irqs,
	.main_clk	= "tptc1_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC1_CLKCTRL,
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
	.master		= &ti816x_l3_slow_hwmod,
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
	.clkdm_name	= "l3_clkdm",
	.mpu_irqs	= ti81xx_tptc2_irqs,
	.main_clk	= "tptc2_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC2_CLKCTRL,
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
	.master		= &ti816x_l3_slow_hwmod,
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
	.clkdm_name	= "l3_clkdm",
	.mpu_irqs	= ti81xx_tptc3_irqs,
	.main_clk	= "tptc3_ick",
	.prcm		= {
		.omap4	= {
			.clkctrl_offs	= TI81XX_CM_ALWON_TPTC3_CLKCTRL,
			.modulemode	= MODULEMODE_SWCTRL,
		},
	},
	.slaves		= ti81xx_tptc3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_tptc3_slaves),
	.flags		= (HWMOD_SWSUP_SIDLE | HWMOD_SWSUP_MSTANDBY),
};

static __initdata struct omap_hwmod *ti81xx_hwmods[] = {
	&ti816x_l3_slow_hwmod,
	&ti816x_l4_slow_hwmod,
	&ti816x_mpu_hwmod,
	&ti816x_uart1_hwmod,
	&ti816x_uart2_hwmod,
	&ti816x_uart3_hwmod,
	&ti814x_uart4_hwmod,
	&ti814x_uart5_hwmod,
	&ti814x_uart6_hwmod,
	&ti816x_wd_timer2_hwmod,
	&ti814x_wd_timer1_hwmod,
	&ti81xx_i2c1_hwmod,	/* Note: In TI814X this enables I2C0/2 */
	&ti816x_i2c2_hwmod,
	&ti814x_i2c3_hwmod,	/* Note: In TI814X this enables I2C1/3 */
	&ti814x_i2c4_hwmod,	/* Note: In TI814X this enables I2C1/3 */
	&ti81xx_gpio1_hwmod,
	&ti81xx_gpio2_hwmod,
	&ti814x_gpio3_hwmod,
	&ti814x_gpio4_hwmod,
	&ti81xx_usbss_hwmod,
	&ti81xx_elm_hwmod,
	&ti81xx_mailbox_hwmod,
	&ti81xx_mcspi1_hwmod,
	&ti81xx_mcspi2_hwmod,
	&ti81xx_mcspi3_hwmod,
	&ti81xx_mcspi4_hwmod,
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
	/* dcan class */
	&ti814x_dcan0_hwmod,
	&ti814x_dcan1_hwmod,
	/* cpgmac0 class */
	&ti814x_cpgmac0_hwmod,
	/* mdio class */
	&ti814x_mdio_hwmod,
	NULL,
};

int __init ti81xx_hwmod_init(void)
{
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
