/*
 * Code for Chronos 1.4 High Speed Camera.
 *
 * Copyright (C) 2017 Kron Technologies Inc. - http://www.krontech.ca/
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
#include <linux/i2c/pcf857x.h>
#include <linux/i2c/atmel_mxt_ts.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/mfd/tps65910.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/hardware/asp.h>

#include <plat/mcspi.h>
#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>
#include <plat/usb.h>
#include <plat/mmc.h>
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/hdmi_lib.h>

#include <plat/ti81xx-vpss.h>
#include <linux/vps_capture.h>
#include <linux/ti81xxfb.h>
#include <linux/ti81xx.h>

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "common.h"
#include "control.h"
#include "cm81xx.h"
#include "devices.h"

#define GPIO_TSC               (0*32+12)		//GPIO 0.12

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

static struct regulator_consumer_supply vmmc0_supply[] = {
	REGULATOR_SUPPLY("vmmc", "omap_hsmmc.0"),
};
static struct regulator_consumer_supply vmmc1_supply[] = {
	REGULATOR_SUPPLY("vmmc", "omap_hsmmc.1"),
};

static struct regulator_init_data vmmc0_data = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies = ARRAY_SIZE(vmmc0_supply),
	.consumer_supplies = vmmc0_supply,
};
static struct regulator_init_data vmmc1_data = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies = ARRAY_SIZE(vmmc1_supply),
	.consumer_supplies = vmmc1_supply,
};

static struct fixed_voltage_config vmmc0_config = {
	.supply_name = "vmmc",
	.microvolts = 3300000, /* 3.3V */
	.gpio = 32,
	.startup_delay = 70000, /* 70msec */
	.enable_high = 1,
	.enabled_at_boot = 0,
	.init_data = &vmmc0_data,
};
static struct fixed_voltage_config vmmc1_config = {
	.supply_name = "vmmc",
	.microvolts = 3300000, /* 3.3V */
	.gpio = -1,
	.startup_delay = 70000, /* 70msec */
	.enable_high = 1,
	.enabled_at_boot = 0,
	.init_data = &vmmc1_data,
};

enum fixed_regulator_id {
	FIXED_REG_ID_VMMC0 = 0,
	FIXED_REG_ID_VMMC1,
};

static struct platform_device vmmc0_device = {
	.name		= "reg-fixed-voltage",
	.id		= FIXED_REG_ID_VMMC0,
	.dev = {
		.platform_data = &vmmc0_config,
	},
};
static struct platform_device vmmc1_device = {
	.name		= "reg-fixed-voltage",
	.id		= FIXED_REG_ID_VMMC1,
	.dev = {
		.platform_data = &vmmc1_config,
	},
};

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 2,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= -EINVAL, /* Dedicated pins for CD and WP */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= 14,
		.gpio_wp	= 13,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

/* Video Input Support */
/*
 * FIXME: This doesn't actually work due to broken dependencies on
 * the tvp7002, which doesn't actually exist on the Chronos board.
 * We don't care too much, since we're really just using OMX to do
 * the video capture.
 */
int vps_ti814x_select_video_decoder(int vid_decoder_id) {return 0;}
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard) {return 0;}
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
static struct mxt_platform_data ts_platform_data = {
	.x_line		= 18,
	.y_line		= 12,
	.x_size		= 800,
	.y_size		= 480,
	.blen		= 0x01,
	.threshold	= 30,
	.voltage	= 2800000,
	.orient		= MXT_HORIZONTAL_FLIP,
};

static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};

static struct i2c_board_info __initdata i2c_boardinfo_bus1[] = {
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
		I2C_BOARD_INFO("qt602240_ts", 0x4a),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("s35390a", 0x30),
	},
};

static struct i2c_board_info __initdata i2c_boardinfo_bus3[] = {
	{
		I2C_BOARD_INFO("ft5x06-ts", 0x38),
	},
};

static void __init chronos14_tsc_init(void)
{
	int error;
    
#define MLBP_SIG_IO_CTRL_FOR_GPb7b8                0x48140E18
#define MLBP_SIG_IO_CTRL_FOR_GPb9b10               0x48140E1c

#define MLBP_SIG_IO_CTRL_VAL                       0x3F    //set 3LSB to enable: 2 lvcmos buffer, padn receiver, padp receiver
//enable GPIO on MLBP pins
omap_writel(0x34, 0x48140E18);
omap_writel(0x34, 0x48140E1c);

	/* Already muxed in u-boot */
	/* omap_mux_init_signal("mlb_clk.gpio0_31", TI814X_PULL_DIS | (1 << 18)); */

	error = gpio_request(GPIO_TSC, "ts_irq");
	if (error < 0) {
		printk(KERN_ERR "%s: failed to request GPIO for TSC IRQ"
			": %d\n", __func__, error);
		return;
	}

	gpio_direction_input(GPIO_TSC);
	i2c_boardinfo_bus3[0].irq = gpio_to_irq(GPIO_TSC);

	gpio_export(GPIO_TSC, true);
}

static void __init chronos14_i2c_init(void)
{
	omap_register_i2c_bus(1, 100, i2c_boardinfo_bus1, ARRAY_SIZE(i2c_boardinfo_bus1));
	omap_register_i2c_bus(3, 400, i2c_boardinfo_bus3, ARRAY_SIZE(i2c_boardinfo_bus3));
}

static u8 ti8148_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data snd_data = {
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

static struct spi_board_info __initdata spi_slave_info[] = {
	{
		.modalias = "spidev",
		.mode   = SPI_MODE_0,
		.max_speed_hz = 48000000,
		.bus_num = 3,
		.chip_select = 0,
	},
	{
		.modalias = "spidev",
		.mode   = SPI_MODE_0,
		.max_speed_hz = 48000000,
		.bus_num = 3,
		.chip_select = 1,
		.controller_data = (void *) 33, /* GP1[0] -> 1 x 16 + 0 = 16*/
	},
};

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

static struct snd_hdmi_platform_data snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &snd_hdmi_pdata,
	}
};

static struct platform_device hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

/*
 * HDMI Audio Auto CTS MCLK configuration.
 * sysclk20, sysclk21, sysclk21 and CLKS(external)
 * setting sysclk20 as the parent of hdmi_i2s_ck
 * ToDo:
 */
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static void __init chronos14_hdmi_clk_init(void)
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
#else
static inline void chronos14_hdmi_clk_init(void) {}
#endif

static void __init chronos14_eth_clk_init(void)
{
	struct clk *rgmii_clk;
	struct clk *parent, *child;
	int ret = 0;

	/* Configure the CPTS reference clock mux */
	parent = clk_get(NULL, "audio_dpll_ck");
	if (IS_ERR(parent))
		pr_err("Unable to get [audio_dpll_ck] clk\n");

	child = clk_get(NULL, "cpts_rft_clk_ck");
	if (IS_ERR(child))
		pr_err("Unable to get [cpts_rft_clk_ck] clk\n");
	
	/* Configure for 250MHz clock */
	ret = clk_set_rate(parent, 250000000);
	if (ret < 0)
		pr_err("Unable to set configure PLL [audio_dpll_ck]\n");

	ret = clk_set_parent(child, parent);
	if (ret < 0)
		pr_err("Unable to select parent clk [cpts_rft_clk_ck]\n");

	rgmii_clk = clk_get(NULL, "emac_rgmii_fck");
	if (IS_ERR(rgmii_clk)) {
		pr_err("Cannot clk_get rgmii_clk\n");
	}
	else {
		clk_enable(rgmii_clk);
	}
}

static struct platform_device *chronos14_devices[] __initdata = {
	&vmmc0_device,
	&vmmc1_device,
	&hdvpss_device,
	&ti81xx_fb_device,
	&ti81xx_vidout_device,
	&ti81xx_hdmi_device,
	&hdmi_audio_device,
	&hdmi_codec_device,
};

#define LSI_PHY_ID		0x0282F014
#define LSI_PHY_MASK		0xffffffff
#define PHY_CONFIG_REG		22

static int chronos14_lsi_phy_fixup(struct phy_device *phydev)
{
	unsigned int val;

	printk("%s: applying LSI PHY fixup\n", __func__);

	/* This enables TX_CLK-ing in case of 10/100MBps operation */
	val = phy_read(phydev, PHY_CONFIG_REG);
	phy_write(phydev, PHY_CONFIG_REG, (val | BIT(5)));

	return 0;
}

#define MICREL_PHY_ID	0x00221611
#define MICREL_PHY_MASK	0xffffffff

static int chronos14_micrel_phy_fixup(struct phy_device *phydev)
{
	printk("%s: applying Micrel PHY fixup\n", __func__);

	phy_write(phydev, 0x104, 0xf0f0);	/* RGMII Clock and Control Pad Pkew. */
	phy_write(phydev, 0x105, 0);		/* RGMII RX Data Pad Skew */
	phy_write(phydev, 0x106, 0);		/* RGMII TX Data Pad Skew */
	
	return 0;
}

static int init_lcd_backlight(void)
{
	int ret;

	ret = gpio_request(11, "lcd_sc");
	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Display On"
			": %d\n", __func__, ret);
		return ret;
	}

	ret = gpio_request(10, "lcd_dispon");
	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Display On"
			": %d\n", __func__, ret);
		return ret;
	}

	ret = gpio_request(18, "lcd_blctrl");
	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Backlight Control"
			": %d\n", __func__, ret);
		return ret;
	}

	gpio_export(11, true);
	gpio_direction_output(11, 1);

	gpio_export(10, true);
	gpio_direction_output(10, 1);

	gpio_export(18, true);
	gpio_direction_output(18, 1);

	return 0;
}

#include <asm/termios.h>
#include <linux/syscalls.h>

//Write a command to ttyO0 to turn off system power
static void camera_power_off(void)
{
	int fd, ret;
	int i;
	const unsigned char shutdownCode[10] = {0x5a, 0x00, 0x05, 0x01, 0xfe, 0xe1, 0xde, 0xad, 0x56, 0xdf};
	mm_segment_t oldfs;
	struct termios tty;

	oldfs = get_fs();
	set_fs (KERNEL_DS);

	if((fd = sys_open("/dev/ttyO0",  O_RDWR | O_NOCTTY | O_NONBLOCK, 0)) < 0) {
		printk(KERN_ALERT "Recieved error %u when attempting to open /dev/ttyO0\n", fd);
		set_fs(oldfs);
		return;
		}

	//Zero structure
	for(i = 0; i < sizeof(tty); i++)
		*((unsigned char *)(((unsigned int)(&tty))+i)) = 0;

        if ((ret = sys_ioctl(fd, TCGETS, (unsigned int)&tty)) != 0)
        {
                printk(KERN_ALERT "Recieved error %u when attempting to get TTY attributes!\n", ret);
		set_fs(oldfs);
		return;
        }

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= 0;		//Parity 0
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag &= ~CBAUD;
        tty.c_cflag |= B57600;

        tty.c_cc[VMIN]  = 1;		// Blocking enabled;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if ((ret = sys_ioctl(fd, TCSETS, (unsigned int)&tty)) != 0)
        {
                printk(KERN_ALERT "Recieved error %u when attempting to set TTY attributes!\n", ret);
		set_fs(oldfs);
		return;
        }

        if ((ret = sys_write (fd, shutdownCode, 10)) < 0)
        {
                printk(KERN_ALERT "Recieved error %u when attempting to write to TTY!\n", ret);
		set_fs(oldfs);
		return;
        }

	set_fs(oldfs);
}

static void __init chronos14_init(void)
{
	ti814x_mux_init(board_mux);
	omap_serial_init();
	chronos14_tsc_init();
	chronos14_i2c_init();
	omap3_register_mcasp(&snd_data, 2);
	omap2_hsmmc_init(mmc);
	am33xx_cpsw_init(AM33XX_CPSW_MODE_RGMII, NULL, NULL);
	phy_register_fixup_for_uid(MICREL_PHY_ID, MICREL_PHY_MASK, chronos14_micrel_phy_fixup);
	phy_register_fixup_for_uid(LSI_PHY_ID, LSI_PHY_MASK, chronos14_lsi_phy_fixup);

	pm_power_off = camera_power_off;

	/* Setup clocks trees. */
	chronos14_eth_clk_init();
	chronos14_hdmi_clk_init();
	__raw_writel(0x0, DSS_HDMI_RESET);

	usb_musb_init(&musb_board_data);
	spi_register_board_info(spi_slave_info, ARRAY_SIZE(spi_slave_info));
	ti81xx_register_hdvpss(hdvpss_capture_subdevs, ARRAY_SIZE(hdvpss_capture_subdevs));
	platform_add_devices(chronos14_devices, ARRAY_SIZE(chronos14_devices));
	regulator_use_dummy_regulator();


	/* Turn on LCD backlight */
	init_lcd_backlight();
}

MACHINE_START(CHRONOS14, "chronos1.4")
	/* Maintainer: Kron Technologies */
	.atag_offset	= 0x100,
	.reserve	= ti81xx_reserve,
	.map_io		= ti81xx_map_io,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= chronos14_init,
	.timer		= &omap3_timer,
MACHINE_END

/*
 * There are many cameras out in the wild that shipped with old
 * bootloaders that specify the MACH_TYPE as the TI8148-EVM.
 */
#ifndef CONFIG_MACH_TI8148EVM
MACHINE_START(TI8148EVM, "chronos1.4-evm")
	/* Maintainer: Kron Technologies */
	.atag_offset	= 0x100,
	.reserve	= ti81xx_reserve,
	.map_io		= ti81xx_map_io,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= chronos14_init,
	.timer		= &omap3_timer,
MACHINE_END
#endif