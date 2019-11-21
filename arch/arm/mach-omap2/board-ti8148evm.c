/*
 * Code for TI8148 EVM.
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
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/phy.h>
#include <linux/i2c/at24.h>
#include <linux/i2c/qt602240_ts.h>
#include <linux/i2c/pcf857x.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65910.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/mcspi.h>
#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>
#include <plat/asp.h>
#include <plat/usb.h>
#include <plat/mmc.h>
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/hdmi_lib.h>

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"
#include "cm81xx.h"

#define GPIO_TSC               31

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	/* VOUT0 DVO2 configuration */
	TI814X_MUX(VOUT0_FID,     OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_CLK,     OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_HSYNC,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_VSYNC,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_AVID,    OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C2, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C3, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C4, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C5, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C6, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C7, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C8, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_B_CB_C9, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC2, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC3, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC4, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC5, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC6, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC7, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC8, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_G_Y_YC9, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR2,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR3,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR4,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR5,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR6,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR7,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR8,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	TI814X_MUX(VOUT0_R_CR9,   OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT),
	/* HDMI low-speed sinals */
	TI814X_MUX(I2C1_SCL,      OMAP_MUX_MODE1 | TI814X_PIN_INPUT_PULLUP),
	TI814X_MUX(I2C1_SDA,      OMAP_MUX_MODE1 | TI814X_PIN_INPUT_PULLUP),
	TI814X_MUX(GPMC_A22,      OMAP_MUX_MODE4 | TI814X_PIN_INPUT_PULLUP),
	TI814X_MUX(GPMC_A23,      OMAP_MUX_MODE4 | TI814X_PIN_INPUT_PULLDOWN),
	/* I2C2 configuration functon 6 */
	TI814X_MUX(UART0_DCDN,    OMAP_MUX_MODE5 | TI814X_PIN_INPUT_PULLUP), /* i2c2_scl_mux0 */
	TI814X_MUX(UART0_DSRN,    OMAP_MUX_MODE5 | TI814X_PIN_INPUT_PULLUP), /* i2c2_sda_mux0 */
	/* VIN0 configuration */
	TI814X_MUX(VIN0_CLK1,     OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_DE0_MUX0, OMAP_MUX_MODE0 | TI814X_PIN_OUTPUT), /* force pin pad of  vin0_de0_mux0 to 0 */
	TI814X_MUX(VIN0_FLD0_MUX0, OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_CLK0,     OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_HSYNC0,   OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_VSYNC0,   OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D0,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D1,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D2,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D3,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D4,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D5,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D6,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D7,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D8,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D9,       OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D10,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D11,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D12,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D13,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D14,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D15,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D16,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D17,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D18,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D19,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D20,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D21,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D22,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_D23,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_DE0_MUX1, OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_DE1,      OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_FLD0_MUX1, OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	TI814X_MUX(VIN0_FLD1,     OMAP_MUX_MODE0 | TI814X_PIN_INPUT),
	/* VIN1 configuration */
	TI814X_MUX(GPMC_CS3,      OMAP_MUX_MODE1 | TI814X_PIN_INPUT); /* vin1_clk1 */
	TI814X_MUX(VOUT1_CLK,     OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1_hsync0 */
	TI814X_MUX(VOUT1_HSYNC,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1_vsync0 */
	TI814X_MUX(VOUT1_VSYNC,   OMAP_MUX_MODE3 | TI814X_PIN_INPUT), /* vin1_de0 */
	TI814X_MUX(VOUT1_AVID,    OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1_clk0 */
	TI814X_MUX(VOUT1_B_CB_C3, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d0 */
	TI814X_MUX(VOUT1_B_CB_C4, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d1 */
	TI814X_MUX(VOUT1_B_CB_C5, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d2 */
	TI814X_MUX(VOUT1_B_CB_C6, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d3 */
	TI814X_MUX(VOUT1_B_CB_C7, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d4 */
	TI814X_MUX(VOUT1_B_CB_C8, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d5 */
	TI814X_MUX(VOUT1_B_CB_C9, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d6 */
	TI814X_MUX(VOUT1_B_CB_C2, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d7 */
	TI814X_MUX(VOUT1_G_Y_YC3, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d8 */
	TI814X_MUX(VOUT1_G_Y_YC4, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d9 */
	TI814X_MUX(VOUT1_G_Y_YC5, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d10 */
	TI814X_MUX(VOUT1_G_Y_YC6, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d11 */
	TI814X_MUX(VOUT1_G_Y_YC7, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d12 */
	TI814X_MUX(VOUT1_G_Y_YC8, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d13 */
	TI814X_MUX(VOUT1_G_Y_YC9, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d14 */
	TI814X_MUX(VOUT1_R_CR4,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d15 */
	TI814X_MUX(VOUT1_R_CR5,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d16 */
	TI814X_MUX(VOUT1_R_CR6,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d17 */
	TI814X_MUX(VOUT1_R_CR7,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d18 */
	TI814X_MUX(VOUT1_R_CR8,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d19 */
	TI814X_MUX(VOUT1_R_CR9,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d20 */
	TI814X_MUX(VOUT1_G_Y_YC2, OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d21 */
	TI814X_MUX(VOUT1_R_CR3,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d22 */
	TI814X_MUX(VOUT1_R_CR2,   OMAP_MUX_MODE2 | TI814X_PIN_INPUT), /* vin1a_d23 */
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 2,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= -EINVAL, /* Dedicated pins for CD and WP */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

#define GPIO_LCD_PWR_DOWN	0

static int setup_gpio_ioexp(struct i2c_client *client, int gpio_base,
	 unsigned ngpio, void *context) {
	int ret = 0;
	int gpio = gpio_base + GPIO_LCD_PWR_DOWN;

	ret = gpio_request(gpio, "lcd_power");
	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Power"
			": %d\n", __func__, ret);
		return ret;
	}

	gpio_export(gpio, true);
	gpio_direction_output(gpio, 0);

	return 0;
}

/* IO expander data */
static struct pcf857x_platform_data io_expander_data = {
	.gpio_base	= 4 * 32,
	.setup		= setup_gpio_ioexp,
};
static struct i2c_board_info __initdata ti814x_i2c_boardinfo1[] = {
	{
		I2C_BOARD_INFO("pcf8575_1", 0x21),
	},

};

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)

#define VPS_SEL_TVP7002_DECODER	0
#define VPS_SEL_SIL9135_DECODER	1

static const struct i2c_device_id pcf8575_video_id[] = {
	{ "pcf8575_1", 0 },
	{ }
};
static struct i2c_client *pcf8575_client;
static unsigned char pcf8575_port[2] = {0x4F, 0x7F};
int vps_ti814x_select_video_decoder(int vid_decoder_id);

static int pcf8575_video_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	pcf8575_client = client;
	vps_ti814x_select_video_decoder(0);
	return 0;
}

static int __devexit pcf8575_video_remove(struct i2c_client *client)
{
	pcf8575_client = NULL;
	return 0;
}

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.name   = "pcf8575_1",
	},
	.probe          = pcf8575_video_probe,
	.remove         = pcf8575_video_remove,
	.id_table       = pcf8575_video_id,
};

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)

static int vps_ti814x_select_video_decoder(int vid_decoder_id)
{
	int ret = 0;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};
	msg.buf = pcf8575_port;
	if (VPS_SEL_TVP7002_DECODER == vid_decoder_id)
		pcf8575_port[1] &= ~VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	else
		pcf8575_port[1] |= VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	ret = (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;
}

#define I2C_RETRY_COUNT 10u

static int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard)
{
	int filter_sel;
	int ret;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};

	pcf8575_port[0] &= ~(VPS_VC_IO_EXP_THS7368_DISABLE_MASK
		| VPS_VC_IO_EXP_THS7368_BYPASS_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER1_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER2_MASK);
	switch (standard) {
	case FVID2_STD_1080P_60:
	case FVID2_STD_1080P_50:
	case FVID2_STD_SXGA_60:
	case FVID2_STD_SXGA_75:
	case FVID2_STD_SXGAP_60:
	case FVID2_STD_SXGAP_75:
	case FVID2_STD_UXGA_60:
		filter_sel = 0x03u;  /* Filter2: 1, Filter1: 1 */
		break;
	case FVID2_STD_1080I_60:
	case FVID2_STD_1080I_50:
	case FVID2_STD_1080P_24:
	case FVID2_STD_1080P_30:
	case FVID2_STD_720P_60:
	case FVID2_STD_720P_50:
	case FVID2_STD_SVGA_60:
	case FVID2_STD_SVGA_72:
	case FVID2_STD_SVGA_75:
	case FVID2_STD_SVGA_85:
	case FVID2_STD_XGA_60:
	case FVID2_STD_XGA_70:
	case FVID2_STD_XGA_75:
	case FVID2_STD_XGA_85:
	case FVID2_STD_WXGA_60:
	case FVID2_STD_WXGA_75:
	case FVID2_STD_WXGA_85:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	case FVID2_STD_480P:
	case FVID2_STD_576P:
	case FVID2_STD_VGA_60:
	case FVID2_STD_VGA_72:
	case FVID2_STD_VGA_75:
	case FVID2_STD_VGA_85:
		filter_sel = 0x02u;  /* Filter2: 1, Filter1: 0 */
		break;
	case FVID2_STD_NTSC:
	case FVID2_STD_PAL:
	case FVID2_STD_480I:
	case FVID2_STD_576I:
	case FVID2_STD_D1:
		filter_sel = 0x00u;  /* Filter2: 0, Filter1: 0 */
		break;

	default:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	}
	pcf8575_port[0] |=
		(filter_sel << VPS_VC_IO_EXP_THS7368_FILTER_SHIFT);
	msg.buf = pcf8575_port;
	ret =  (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0) {
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	return ret;
}

/* Video Input Support */
struct ti81xxvin_interface tvp7002_pdata = {
	.clk_polarity = 0,
	.hs_polarity = 0,
	.vs_polarity = 1,
	.fid_polarity = 0,
	.sog_polarity = 0,
};
static struct ti81xxvin_subdev_info hdvpss_capture_subdevs[] = {
	{
		.name	= TVP7002_INST0,
		.board_info = {
			/* TODO Find the correct address
				of the TVP7002 connected */
			I2C_BOARD_INFO("tvp7002", 0x5d),
			.platform_data = &tvp7002_pdata,
		},
		.vip_port_cfg = {
			.ctrlChanSel = VPS_VIP_CTRL_CHAN_SEL_15_8,
			.ancChSel8b = VPS_VIP_ANC_CH_SEL_DONT_CARE,
			.pixClkEdgePol = VPS_VIP_PIX_CLK_EDGE_POL_RISING,
			.invertFidPol = 0,
			.embConfig = {
				.errCorrEnable = 1,
				.srcNumPos = VPS_VIP_SRC_NUM_POS_DONT_CARE,
				.isMaxChan3Bits = 0,
			},
			.disConfig = {
				.fidSkewPostCnt = 0,
				.fidSkewPreCnt = 0,
				.lineCaptureStyle =
					VPS_VIP_LINE_CAPTURE_STYLE_DONT_CARE,
				.fidDetectMode =
					VPS_VIP_FID_DETECT_MODE_DONT_CARE,
				.actvidPol = VPS_VIP_POLARITY_DONT_CARE,
				.vsyncPol =  VPS_VIP_POLARITY_DONT_CARE,
				.hsyncPol = VPS_VIP_POLARITY_DONT_CARE,
			}
		},
		.video_capture_mode =
		   VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC,
		.video_if_mode = VPS_CAPT_VIDEO_IF_MODE_16BIT,
		.input_data_format = FVID2_DF_YUV422P,
		.ti81xxvin_select_decoder = vps_ti814x_select_video_decoder,
		.ti81xxvin_set_mode = vps_ti814x_set_tvp7002_filter,
	},
	{
		.name	= TVP7002_INST1,
		.board_info = {
			I2C_BOARD_INFO("tvp7002", 0x5c),
			.platform_data = &tvp7002_pdata,
		},
		.vip_port_cfg = {
			.ctrlChanSel = VPS_VIP_CTRL_CHAN_SEL_15_8,
			.ancChSel8b = VPS_VIP_ANC_CH_SEL_DONT_CARE,
			.pixClkEdgePol = VPS_VIP_PIX_CLK_EDGE_POL_RISING,
			.invertFidPol = 0,
			.embConfig = {
				.errCorrEnable = 1,
				.srcNumPos = VPS_VIP_SRC_NUM_POS_DONT_CARE,
				.isMaxChan3Bits = 0,
			},
			.disConfig = {
				.fidSkewPostCnt = 0,
				.fidSkewPreCnt = 0,
				.lineCaptureStyle =
					VPS_VIP_LINE_CAPTURE_STYLE_DONT_CARE,
				.fidDetectMode =
					VPS_VIP_FID_DETECT_MODE_DONT_CARE,
				.actvidPol = VPS_VIP_POLARITY_DONT_CARE,
				.vsyncPol =  VPS_VIP_POLARITY_DONT_CARE,
				.hsyncPol = VPS_VIP_POLARITY_DONT_CARE,
			}
		},
		.video_capture_mode =
		   VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC,
		.video_if_mode = VPS_CAPT_VIDEO_IF_MODE_16BIT,
		.input_data_format = FVID2_DF_YUV422P,
	},
};

/* Touchscreen platform data */
static struct qt602240_platform_data ts_platform_data = {
	.x_line		= 18,
	.y_line		= 12,
	.x_size		= 800,
	.y_size		= 480,
	.blen		= 0x01,
	.threshold	= 30,
	.voltage	= 2800000,
	.orient		= QT602240_HORIZONTAL_FLIP,
};

static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};

static struct regulator_consumer_supply ti8148evm_mpu_supply =
	REGULATOR_SUPPLY("mpu", NULL);

/*
 * DM814x/AM387x (TI814x) devices have restriction that none of the supply to
 * the device should be turned of.
 *
 * NOTE: To prevent turning off regulators not explicitly consumed by drivers
 * depending on it, ensure following:
 *	1) Set always_on = 1 for them OR
 *	2) Avoid calling regulator_has_full_constraints()
 *
 * With just (2), there will be a warning about incomplete constraints.
 * E.g., "regulator_init_complete: incomplete constraints, leaving LDO8 on"
 *
 * In either cases, the supply won't be disabled.
 *
 * We are taking approach (1).
 */
static struct regulator_init_data tps65911_reg_data[] = {
	/* VRTC */
	{
		.constraints = {
			.min_uV = 1800000,
			.max_uV = 1800000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VIO -VDDA 1.8V */
	{
		.constraints = {
			.min_uV = 1500000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDD1 - MPU */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
		.num_consumer_supplies	= 1,
		.consumer_supplies	= &ti8148evm_mpu_supply,
	},

	/* VDD2 - DSP */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDDCtrl - CORE */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1400000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO1 - VDAC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO2 - HDMI */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO3 - GPIO 3.3V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO4 - PLL 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO5 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO6 - CDC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO7 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO8 - USB 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},
};

static struct tps65910_board __refdata tps65911_pdata = {
	.irq				= 0,	/* No support currently */
	.gpio_base			= 0,	/* No support currently */
	.tps65910_pmic_init_data	= tps65911_reg_data,
};

static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("eeprom", 0x50),
		.platform_data	= &eeprom_info,
	},
	{
		I2C_BOARD_INFO("cpld", 0x23),
	},
	{
		I2C_BOARD_INFO("tlv320aic3x", 0x18),
	},
	{
		I2C_BOARD_INFO("IO Expander", 0x20),
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},
	{
		I2C_BOARD_INFO("pcf8575", 0x21),
		.platform_data = &io_expander_data,
	},
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4A),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
};

static void __init ti814x_tsc_init(void)
{
	int error;

	omap_mux_init_signal("mlb_clk.gpio0_31", TI814X_PULL_DIS | (1 << 18));

	error = gpio_request(GPIO_TSC, "ts_irq");
	if (error < 0) {
		printk(KERN_ERR "%s: failed to request GPIO for TSC IRQ"
			": %d\n", __func__, error);
		return;
	}

	gpio_direction_input(GPIO_TSC);
	ti814x_i2c_boardinfo[6].irq = gpio_to_irq(GPIO_TSC);

	gpio_export(31, true);
}

static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
	omap_register_i2c_bus(3, 100, ti814x_i2c_boardinfo1,
				ARRAY_SIZE(ti814x_i2c_boardinfo1));
}

static u8 ti8148_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data ti8148_evm_snd_data = {
	.tx_dma_offset	= 0x46800000,
	.rx_dma_offset	= 0x46800000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(ti8148_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= ti8148_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 1,
	.rxnumevt	= 1,
};

/* NOR Flash partitions */
static struct mtd_partition ti814x_evm_norflash_partitions[] = {
	/* bootloader (U-Boot, etc) in first 5 sectors */
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 2 * SZ_128K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	/* bootloader params in the next 1 sectors */
	{
		.name		= "env",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_128K,
		.mask_flags	= 0,
	},
	/* kernel */
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 2 * SZ_2M,
		.mask_flags	= 0
	},
	/* file system */
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 25 * SZ_2M,
		.mask_flags	= 0
	},
	/* reserved */
	{
		.name		= "reserved",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
		.mask_flags	= 0
	}
};

/* NAND flash information */
static struct mtd_partition ti814x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,    /* Offset = 0x0 */
		.size           = SZ_128K,
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,/* Offset = 0x0 + 128K */
		.size           = 18 * SZ_128K,
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x260000 */
		.size           = 1 * SZ_128K,
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x280000 */
		.size           = 34 * SZ_128K,
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x6C0000 */
		.size           = 1601 * SZ_128K,
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xCEE0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

/* SPI fLash information */
struct mtd_partition ti8148_spi_partitions[] = {
	/* All the partition sizes are listed in terms of erase size */
	{
		.name		= "U-Boot-min",
		.offset		= 0,	/* Offset = 0x0 */
		.size		= 32 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND, /* 0x0 + (32*4K) */
		.size		= 64 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND, /* 0x40000 + (32*4K) */
		.size		= 2 * SZ_4K,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND, /* 0x42000 + (32*4K) */
		.size		= 640 * SZ_4K,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND, /* 0x2C2000 + (32*4K) */
		.size		= MTDPART_SIZ_FULL, /* size ~= 1.1 MiB */
	}
};

const struct flash_platform_data ti8148_spi_flash = {
	.type		= "w25x32",
	.name		= "spi_flash",
	.parts		= ti8148_spi_partitions,
	.nr_parts	= ARRAY_SIZE(ti8148_spi_partitions),
};

struct spi_board_info __initdata ti8148_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &ti8148_spi_flash,
		.irq		= -1,
		.max_speed_hz	= 75000000,
		.bus_num	= 1,
		.chip_select	= 0,
	},
};

void __init ti8148_spi_init(void)
{
	spi_register_board_info(ti8148_spi_slave_info,
				ARRAY_SIZE(ti8148_spi_slave_info));
}

static struct omap_musb_board_data musb_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
#ifdef CONFIG_USB_MUSB_OTG
	.mode           = MUSB_OTG,
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
	.mode           = MUSB_PERIPHERAL,
#endif
	.power		= 500,
	.instances	= 1,
};

static struct vps_platform_data hdvpss_pdata = {
	.cpu = CPU_DM814X,
	.numvencs = 3,
	.vencmask = (1 << VPS_DC_MAX_VENC) - 1 - VPS_DC_VENC_HDCOMP
};

static struct platform_device hdvpss_device = {
	.name = "vpss",
	.id = -1,
	.dev = {
		.platform_data = &hdvpss_pdata,
	},
};

static struct platform_device ti81xx_hdmi_device = {
	.name	= "TI81XX_HDMI",
	.id		= -1,
};

static u64 ti81xxfb_dma_mask = ~(u32)0;
static struct ti81xxfb_platform_data ti81xxfb_config;

static struct platform_device ti81xx_fb_device = {
	.name		= "ti81xxfb",
	.id		= -1,
	.dev = {
		.dma_mask		= &ti81xxfb_dma_mask,
		.coherent_dma_mask	= ~(u32)0,
		.platform_data		= &ti81xxfb_config,
	},
	.num_resources = 0,
};

static struct resource ti81xx_vidout_resource[VPS_DISPLAY_INST_MAX] = {};
static struct platform_device ti81xx_vidout_device = {
	.name		= "t81xx_vidout",
	.resource       = &ti81xx_vidout_resource[0],
	.num_resources  = ARRAY_SIZE(ti81xx_vidout_resource),
	.id             = -1,
};

static struct snd_hdmi_platform_data ti8148_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device ti8148_hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &ti8148_snd_hdmi_pdata,
	}
};

static struct platform_device ti8148_hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

static struct platform_device *ti8148_devices[] __initdata = {
	&hdvpss_device,
	&ti81xx_fb_device,
	&ti81xx_vidout_device,
	&ti81xx_hdmi_video_device,
	&ti8148_hdmi_audio_device,
	&ti8148_hdmi_codec_device,
};

/*
 * HDMI Audio Auto CTS MCLK configuration.
 * sysclk20, sysclk21, sysclk21 and CLKS(external)
 * setting sysclk20 as the parent of hdmi_i2s_ck
 * ToDo:
 */
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
void __init ti8148_hdmi_clk_init(void)
{
	int ret = 0;
	struct clk *parent, *child;

	/* modify the clk name to choose diff clk*/
	parent = clk_get(NULL, "sysclk20_ck");
	if (IS_ERR(parent))
		pr_err("Unable to get [sysclk20_ck] clk\n");

	child = clk_get(NULL, "hdmi_i2s_ck");
	if (IS_ERR(child))
		pr_err("Unable to get [hdmi_i2s_ck] clk\n");

	ret = clk_set_parent(child, parent);
	if (ret < 0)
		pr_err("Unable to set parent clk [hdmi_i2s_ck]\n");

	clk_put(child);
	clk_put(parent);
	pr_debug("{{HDMI Audio MCLK setup completed}}\n");
}
#endif

#define LSI_PHY_ID		0x0282F014
#define LSI_PHY_MASK		0xffffffff
#define PHY_CONFIG_REG		22

static int ti8148_evm_lsi_phy_fixup(struct phy_device *phydev)
{
	unsigned int val;

	/* This enables TX_CLK-ing in case of 10/100MBps operation */
	val = phy_read(phydev, PHY_CONFIG_REG);
	phy_write(phydev, PHY_CONFIG_REG, (val | BIT(5)));

	return 0;
}

static void __init ti8148_evm_init(void)
{
	int bw; /* bus-width */

	ti814x_mux_init(board_mux);
	omap_serial_init();
	ti814x_tsc_init();
	ti814x_evm_i2c_init();
	omap3_register_mcasp(&ti8148_evm_snd_data, 2);
	i2c_add_driver(&pcf8575_driver);
	ti81xx_register_hdvpss(hdvpss_capture_subdevs, ARRAY_SIZE(hdvpss_capture_subdevs));

	omap2_hsmmc_init(mmc);

	/* nand initialisation */
	if (cpu_is_ti814x()) {
		u32 *control_status = TI81XX_CTRL_REGADDR(0x40);
		if (*control_status & (1<<16))
			bw = 2; /*16-bit nand if BTMODE BW pin on board is ON*/
		else
			bw = 0; /*8-bit nand if BTMODE BW pin on board is OFF*/

		board_nand_init(ti814x_nand_partitions,
			ARRAY_SIZE(ti814x_nand_partitions), 0, bw);
	} else
		board_nand_init(ti814x_nand_partitions,
		ARRAY_SIZE(ti814x_nand_partitions), 0, NAND_BUSWIDTH_16);

	/* initialize usb */
	usb_musb_init(&musb_board_data);

	ti8148_spi_init();
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	/*setup the clokc for HDMI MCLK*/
	ti8148_hdmi_clk_init();
	__raw_writel(0x0, DSS_HDMI_RESET);
	platform_add_devices(ti8148_devices, ARRAY_SIZE(ti8148_devices));
#endif
	regulator_use_dummy_regulator();
	board_nor_init(ti814x_evm_norflash_partitions,
		ARRAY_SIZE(ti814x_evm_norflash_partitions), 0);

	/* LSI Gigabit Phy fixup */
	phy_register_fixup_for_uid(LSI_PHY_ID, LSI_PHY_MASK,
				   ti8148_evm_lsi_phy_fixup);
}

MACHINE_START(TI8148EVM, "ti8148evm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= omap3_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= omap3_init_irq,
	.init_machine	= ti8148_evm_init,
	.timer		= &omap_timer,
MACHINE_END
