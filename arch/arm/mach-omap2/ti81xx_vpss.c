/*
 *
 * Framebuffer device registration for TI TI816x platforms
 *
 * Copyright (C) 2009 Texas Instruments Inc.
 * Author: Yihe Hu <yihehu@ti.com>
 *
 * Some code and ideas taken from TI OMAP2 Platforms
 * by Tomi Valkeinen.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/io.h>
#include <linux/vps_capture.h>
#include <linux/ti81xxfb.h>
#include <linux/ti81xx.h>
#include <mach/hardware.h>
#include <asm/mach/map.h>

static u64 ti81xx_dma_mask = ~(u32)0;

#define HDVPSS_CAPTURE_INST0_BASE	0x48105500
#define HDVPSS_CAPTURE_INST0_SIZE	1024u

#define HDVPSS_CAPTURE_INST2_BASE	0x48105A00
#define HDVPSS_CAPTURE_INST2_SIZE	1024u

u8 ti81xx_card_name[] = "TI81xx_catalogue";

static const struct v4l2_dv_preset hdvpss_inst0_inp0_presets[] = {
	{
		.preset = V4L2_DV_720P60,
	},
	{
		.preset = V4L2_DV_1080I60,
	},
	{
		.preset = V4L2_DV_1080P60,
	},
	{
		.preset = V4L2_DV_1080P30,
	},
};

static const struct v4l2_dv_preset hdvpss_inst2_inp0_presets[] = {
	{
		.preset = V4L2_DV_720P60,
	},
	{
		.preset = V4L2_DV_1080I60,
	},
	{
		.preset = V4L2_DV_1080P60,
	},
	{
		.preset = V4L2_DV_1080P30,
	},
};

static const struct ti81xxvin_input hdvpss_inst0_inputs[] = {
	{
		.input = {
			.index		= 0,
			.name		= "Component",
			.type		= V4L2_INPUT_TYPE_CAMERA,
			.std		= V4L2_STD_UNKNOWN,
			.capabilities	= V4L2_OUT_CAP_PRESETS,
		},
		.subdev_name	= TVP7002_INST0,
		.dv_presets	= hdvpss_inst0_inp0_presets,
		.num_dv_presets	= ARRAY_SIZE(hdvpss_inst0_inp0_presets),
	},
};

static const struct ti81xxvin_input hdvpss_inst1_inputs[] = {
	{
		.input = {
			.index		= 0,
			.name		= "Component",
			.type		= V4L2_INPUT_TYPE_CAMERA,
			.std		= V4L2_STD_UNKNOWN,
			.capabilities	= V4L2_OUT_CAP_PRESETS,
		},
		.subdev_name	= TVP7002_INST1,
		.dv_presets	= hdvpss_inst2_inp0_presets,
		.num_dv_presets	= ARRAY_SIZE(hdvpss_inst2_inp0_presets),
	},
};

/* 16 bit decoders are present on the Port A of VIP0 and VIP1 instances. Which
represents the VIP0 and VIP2 instances in software. While Port B of VIP0 and
VIP1 are represented by VIP1 and VIP3 instances. On these two instances no
decoders are present.
*/
static struct ti81xxvin_config ti81xx_hsvpss_capture_cfg = {
	.card_name = ti81xx_card_name,
	.inst_config[0] = {
		.inputs = hdvpss_inst0_inputs,
		.input_count = ARRAY_SIZE(hdvpss_inst0_inputs),
	},
	.inst_config[1] = {
		.inputs = hdvpss_inst0_inputs,
		.input_count = 0,
	},
	.inst_config[2] = {
		.inputs = hdvpss_inst1_inputs,
		.input_count = ARRAY_SIZE(hdvpss_inst1_inputs),
	},
	.inst_config[3] = {
		.inputs = hdvpss_inst1_inputs,
		.input_count = 0,
	},

};

static struct resource ti81xx_hdvpss_capture_resource[] = {
	[0] = {
		.start = HDVPSS_CAPTURE_INST0_BASE,
		.end   = (HDVPSS_CAPTURE_INST0_BASE +
				HDVPSS_CAPTURE_INST0_SIZE - 1),
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = HDVPSS_CAPTURE_INST2_BASE,
		.end   = (HDVPSS_CAPTURE_INST2_BASE +
				HDVPSS_CAPTURE_INST2_SIZE - 1),
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device hdvpss_capture_dev = {
	.name		= "ti81xxvin",
	.id		= -1,
	.dev		= {
			.dma_mask		= &ti81xx_dma_mask,
			.coherent_dma_mask	= ~(u32)0,
			.platform_data	= &ti81xx_hsvpss_capture_cfg,
	},
	.num_resources = 2,
	.resource = ti81xx_hdvpss_capture_resource,
};

int __init ti81xx_register_hdvpss(struct ti81xxvin_subdev_info *subdevs, int subdev_count)
{
	int r;

	ti81xx_hsvpss_capture_cfg.subdev_info = subdevs;
	ti81xx_hsvpss_capture_cfg.subdev_count = subdev_count;
	r = platform_device_register(&hdvpss_capture_dev);
	if (r)
		printk(KERN_ERR "unable to register ti81xx_vin device\n");
	else
		printk(KERN_INFO "registered ti81xx_vin device\n");
	return r;
}
