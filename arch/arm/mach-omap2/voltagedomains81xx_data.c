/*
 * TI81XX voltage domain data
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
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

#include "voltage.h"

static struct voltagedomain ti81xx_voltdm_core = {
	.name		= "core",
};

static struct voltagedomain ti81xx_voltdm_active = {
	.name		= "active",
};

static struct voltagedomain ti81xx_voltdm_hdvicp = {
	.name		= "hdvicp",
};

static struct voltagedomain *voltagedomains_ti81xx[] __initdata = {
	&ti81xx_voltdm_core,
	&ti81xx_voltdm_active,
	&ti81xx_voltdm_hdvicp,
	NULL,
};

void __init ti81xx_voltagedomains_init(void)
{
	voltdm_init(voltagedomains_ti81xx);
}
