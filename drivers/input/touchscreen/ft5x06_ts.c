/* 
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x0x TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * VERSION      	DATE			AUTHOR
 *    1.0		  2010-01-05			WenFS
 *
 * note: only support mulititouch	Wenfs 2010-10-01
 */

//#define HAS_EARLYSUSPEND
//#define HAS_LDO

#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/gpio.h>
#ifdef HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#ifdef HAS_LDO
#include <mach/ldo.h>
#endif
#include <mach/irqs.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <mach/gpio.h>
#include "ft5x06_ts.h"

//#define TOUCH_VIRTUAL_KEYS
#define CFG_SUPPORT_TOUCH_KEY 0
//#define FT_UPGRADE_WITH_I
#define TS_DEBUG_MSG 			1
#define I2C_BOARD_INFO_METHOD   1
#define TS_DATA_THRESHOLD_CHECK 0
#define TS_WIDTH_MAX			800
//#ifdef  TOUCH_VIRTUAL_KEYS
#ifdef CFG_SUPPORT_TOUCH_KEY
#define TOUCH_HEIGHT_MIN 850
#define TOUCH_HEIGHT_MAX 950
#define	TS_HEIGHT_MAX			920//540
#else
#define	TS_HEIGHT_MAX			480//480
#endif
#define FT_UPGRADE_DELAY  100
static int debug_level=0;

#if TS_DEBUG_MSG
#define TS_DBG(format, ...)	\
	if(debug_level == 1)	\
		printk(KERN_INFO "FT5X0X " format "\n", ## __VA_ARGS__)
#else
#define TS_DBG(format, ...)
#endif


struct sprd_i2c_setup_data {
	unsigned i2c_bus;  //the same number as i2c->adap.nr in adapter probe function
	unsigned short i2c_address;
	int irq;
	char type[I2C_NAME_SIZE];
};


extern int sprd_3rdparty_gpio_tp_pwr;
extern int sprd_3rdparty_gpio_tp_rst;
extern int sprd_3rdparty_gpio_tp_irq;


static struct i2c_client *this_client;
#if !I2C_BOARD_INFO_METHOD
static struct sprd_i2c_setup_data ft5x0x_ts_setup={0, FT5206_TS_ADDR, 0, FT5206_TS_NAME};
static void ft5x0x_ts_reset(void);
#endif
/* Attribute */
static ssize_t ft5x0x_show_version(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_update(struct device* cd, struct device_attribute *attr, const char* buf, size_t len);
static ssize_t ft5x0x_show_debug(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t ft5x0x_store_debug(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);
static ssize_t ft5x0x_show_chipinfo(struct device* cd,struct device_attribute *attr, char* buf);
static unsigned char ft5x0x_read_fw_ver(void);
static int ft5x0x_read_reg(u8 addr, u8 *pdata);
#ifdef HAS_EARLYSUSPEND
static unsigned char suspend_flag=0; //0: sleep out; 1: sleep in
static void ft5x0x_ts_suspend(struct early_suspend *handler);
static void ft5x0x_ts_resume(struct early_suspend *handler);
#endif
static int fts_ctpm_fw_update(void);
//static int fts_ctpm_fw_upgrade_with_i_file(void);
//#define CONFIG_FT5X0X_MULTITOUCH 1
#ifdef HAS_LDO
static void ft5x0x_chip_vdd_input(bool turn_on);
#endif

struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u16	x3;
	u16	y3;
	u16	x4;
	u16	y4;
	u16	x5;
	u16	y5;
	u16	pressure;
    u8  touch_point;
};

struct ft5x0x_ts_data {
	struct input_dev	*input_dev;
	struct ts_event		event;
	struct work_struct 	pen_event_work;
	struct workqueue_struct *ts_workqueue;
#ifdef FT_UPGRADE_WITH_I
	struct delayed_work 	pen_upgrade_work;
	struct workqueue_struct *ts_upgrade_workqueue;
#endif
#ifdef HAS_EARLYSUSPEND
	struct early_suspend	early_suspend;
#endif
};


static DEVICE_ATTR(update, S_IRUGO | S_IWUSR, ft5x0x_show_version, ft5x0x_update);
static DEVICE_ATTR(debug, S_IRUGO | S_IWUSR, ft5x0x_show_debug, ft5x0x_store_debug);
static DEVICE_ATTR(chipid, S_IRUGO, ft5x0x_show_chipinfo, NULL);

static ssize_t ft5x0x_show_debug(struct device* cd,struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;
	
	sprintf(buf, "FT5206 Debug %d\n",debug_level);
	
	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_store_debug(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	debug_level = on_off;

	printk("%s: debug_level=%d\n",__func__, debug_level);
	
	return len;
}

static ssize_t ft5x0x_print_chipreg(char *buf, unsigned char addr, const char *name)
{
	unsigned char val;
	ft5x0x_read_reg(addr, &val);
	return sprintf(buf, "%s: 0x%02x\n", name, val);
}

static ssize_t ft5x0x_show_chipinfo(struct device* cd, struct device_attribute *attr, char* buf)
{
	ssize_t len = 0;
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_AUTO_CLB_MODE, "AUTO_CLB_MODE");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_LIB_VERSION_H, "LIB_VERSION_H");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_LIB_VERSION_L, "LIB_VERSION_L");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_CIPHER, 		 "CIPHER");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_MODE,          "MODE");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_PMODE,         "PMODE");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_FIRMID,        "FIRMID");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_STATE,         "STATE");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_FT5201ID,      "FT5201ID");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_ERR,           "ERR");
	len += ft5x0x_print_chipreg(buf + len, FT5X0X_REG_CLB,           "CLB");

	return len;
}

#ifdef HAS_EARLYSUSPEND
static ssize_t ft5x0x_show_suspend(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;

	if(suspend_flag==1)
		sprintf(buf, "FT5206 Suspend\n");
	else
		sprintf(buf, "FT5206 Resume\n");
	
	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_store_suspend(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	suspend_flag = on_off;
	
	if(on_off==1)
	{
		printk("FT5206 Entry Suspend\n");
		ft5x0x_ts_suspend(NULL);
	}
	else
	{
		printk("FT5206 Entry Resume\n");
		ft5x0x_ts_resume(NULL);
	}
	
	return len;
}

static DEVICE_ATTR(suspend, S_IRUGO | S_IWUSR, ft5x0x_show_suspend, ft5x0x_store_suspend);
#endif

static ssize_t ft5x0x_show_version(struct device* cd,
				     struct device_attribute *attr, char* buf)
{
	ssize_t ret = 0;
	unsigned char uc_reg_value; 
	
    //get some register information
    uc_reg_value = ft5x0x_read_fw_ver();

	sprintf(buf, "ft5x0x firmware id is V%x\n", uc_reg_value);

	
	ret = strlen(buf) + 1;

	return ret;
}

static ssize_t ft5x0x_update(struct device* cd, struct device_attribute *attr,
		       const char* buf, size_t len)
{
	unsigned long on_off = simple_strtoul(buf, NULL, 10);
	unsigned char uc_reg_value; 

    //get some register information
    uc_reg_value = ft5x0x_read_fw_ver();

	if(on_off==1)
	{
		printk("ft5x0x update, current firmware id is V%x\n", uc_reg_value);
		fts_ctpm_fw_update();	
		//fts_ctpm_fw_upgrade_with_i_file();
	}

	return len;
}

static int ft5x0x_create_sysfs(struct i2c_client *client)
{
	int err;
	struct device *dev = &(client->dev);

	TS_DBG("%s", __func__);

#ifdef HAS_EARLYSUSPEND
	err = device_create_file(dev, &dev_attr_suspend);
#endif
	err = device_create_file(dev, &dev_attr_update);
	err = device_create_file(dev, &dev_attr_debug);
	err = device_create_file(dev, &dev_attr_chipid);
	
	return err;
}
#if CFG_SUPPORT_TOUCH_KEY
struct tp_sprd_virtual_key
{
	int x_min;
	int x_max;
	int y_min;
	int y_max;
	int key_code;
	char * key_name;
};

struct tp_sprd_virtual_key tsp_virtual_key[] = 
{
    {
        .x_min=20,
        .x_max=148,
        .y_min=880,
        .y_max=920,
        .key_code=KEY_MENU,
        .key_name= "Menu",
    },
    {
        .x_min=200,
        .x_max=300,
        .y_min=880,
        .y_max=920,
        .key_code=KEY_HOME,
        .key_name= "Home",
    },
    {
        .x_min=300,
        .x_max=420,
        .y_min=880,
        .y_max=920,
        .key_code=KEY_BACK,
        .key_name= "back",
    },
};
#define CFG_NUMOFKEYS (sizeof(tsp_virtual_key)/sizeof(tsp_virtual_key[0]))
static bool tsp_keystatus[CFG_NUMOFKEYS];
#endif
#ifdef TOUCH_VIRTUAL_KEYS

#define FT5X0X_KEY_MENU	139
#define FT5X0X_KEY_BACK	158
#define FT5X0X_KEY_HOME	102

static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *
attr, char *buf)
{
	return sprintf(buf,
	 __stringify(EV_KEY) ":" __stringify(FT5X0X_KEY_MENU) ":100:1050:80:80"
	 ":" __stringify(EV_KEY) ":" __stringify(FT5X0X_KEY_HOME) ":200:1050:80:80"
	 ":" __stringify(EV_KEY) ":" __stringify(FT5X0X_KEY_BACK) ":300:1050:80:80"
	 "\n");
}

static struct kobj_attribute virtual_keys_attr = {
    .attr = {
        .name = "virtualkeys.pixcir_ts",//FT5X0X_NAME
        .mode = S_IRUGO,
    },
    .show = &virtual_keys_show,
};

static struct attribute *properties_attrs[] = {
    &virtual_keys_attr.attr,
    NULL
};

static struct attribute_group properties_attr_group = {
    .attrs = properties_attrs,
};

static void ft5x0x_ts_virtual_keys_init(void)
{
    int ret;
    struct kobject *properties_kobj;
	
    TS_DBG("%s\n",__func__);
	
    properties_kobj = kobject_create_and_add("board_properties", NULL);
    if (properties_kobj)
        ret = sysfs_create_group(properties_kobj,
                     &properties_attr_group);
    if (!properties_kobj || ret)
        pr_err("failed to create board_properties\n");    
}

#endif

/***********************************************************************************************
Name	:	ft5x0x_i2c_rxdata 

Input	:	*rxdata
                     *length

Output	:	ret

function	:	

***********************************************************************************************/
static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
		},
	};

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
	
	return ret;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int ft5x0x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};

   	//msleep(1);
	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}
/***********************************************************************************************
Name	:	 ft5x0x_write_reg

Input	:	addr -- address
                     para -- parameter

Output	:	

function	:	write register of ft5x0x

***********************************************************************************************/
static int ft5x0x_write_reg(u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;

    buf[0] = addr;
    buf[1] = para;
    ret = ft5x0x_i2c_txdata(buf, 2);
    if (ret < 0) {
        pr_err("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }
    
    return 0;
}

/***********************************************************************************************
Name	:	ft5x0x_read_reg 

Input	:	addr
                     pdata

Output	:	

function	:	read register of ft5x0x

***********************************************************************************************/
static int ft5x0x_read_reg(u8 addr, u8 *pdata)
{
	int ret;
	u8 buf[2] = {addr, 0};
	struct i2c_msg msgs[2] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= buf,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf,
		},
	};

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

	*pdata = buf[0];
	return ret;
  
}


/***********************************************************************************************
Name	:	 ft5x0x_read_fw_ver

Input	:	 void
                     

Output	:	 firmware version 	

function	:	 read TP firmware version

***********************************************************************************************/
static unsigned char ft5x0x_read_fw_ver(void)
{
	unsigned char ver;
	ft5x0x_read_reg(FT5X0X_REG_FIRMID, &ver);
	return(ver);
}


#define CONFIG_SUPPORT_FTS_CTP_UPG


#ifdef CONFIG_SUPPORT_FTS_CTP_UPG

typedef enum
{
    ERR_OK,
    ERR_MODE,
    ERR_READID,
    ERR_ERASE,
    ERR_STATUS,
    ERR_ECC,
    ERR_DL_ERASE_FAIL,
    ERR_DL_PROGRAM_FAIL,
    ERR_DL_VERIFY_FAIL
}E_UPGRADE_ERR_TYPE;

typedef unsigned char         FTS_BYTE;     //8 bit
typedef unsigned short        FTS_WORD;    //16 bit
typedef unsigned int          FTS_DWRD;    //16 bit
typedef unsigned char         FTS_BOOL;    //8 bit

#define FTS_NULL                0x0
#define FTS_TRUE                0x01
#define FTS_FALSE              0x0

#define I2C_CTPM_ADDRESS       0x70

/*******************************************************************************
* Function    :  ft5x0x_ts_config_pins
* Description :  config gpio pins and set relative TS pins
* Parameters  :  void
* Return      :    int irq_num;
*******************************************************************************/
#if !I2C_BOARD_INFO_METHOD
static int ft5x0x_ts_config_pins(void)
{
	int ft5x0x_irq;

	gpio_direction_output(sprd_3rdparty_gpio_tp_rst, 1);
	gpio_direction_input(sprd_3rdparty_gpio_tp_irq);	
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	ft5x0x_irq=sprd_alloc_gpio_irq(sprd_3rdparty_gpio_tp_irq);
    ft5x0x_ts_reset();

	msleep(10); //wait for stable
	
	return ft5x0x_irq;
}
#endif
/*
[function]: 
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
FTS_BOOL i2c_read_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;
    
    ret=i2c_master_recv(this_client, pbt_buf, dw_lenth);

    if(ret<=0)
    {
        printk("[TSP]i2c_read_interface error\n");
        return FTS_FALSE;
    }
  
    return FTS_TRUE;
}

/*
[function]: 
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
FTS_BOOL i2c_write_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    int ret;
    ret=i2c_master_send(this_client, pbt_buf, dw_lenth);
    if(ret<=0)
    {
        printk("[TSP]i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
        return FTS_FALSE;
    }

    return FTS_TRUE;
}

/*
[function]: 
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;    
    btPara2[in]    :parameter 2;    
    btPara3[in]    :parameter 3;    
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL cmd_write(FTS_BYTE btcmd,FTS_BYTE btPara1,FTS_BYTE btPara2,FTS_BYTE btPara3,FTS_BYTE num)
{
    FTS_BYTE write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);
}

/*
[function]: 
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_write(FTS_BYTE* pbt_buf, FTS_DWRD dw_len)
{
    
    return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}

/*
[function]: 
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_read(FTS_BYTE* pbt_buf, FTS_BYTE bt_len)
{
    return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
}


/*
[function]: 
    burn the FW to ctpm.
[parameters]:(ref. SPEC)
    pbt_buf[in]    :point to Head+FW ;
    dw_lenth[in]:the length of the FW + 6(the Head length);    
    bt_ecc[in]    :the ECC of the FW
[return]:
    ERR_OK        :no error;
    ERR_MODE    :fail to switch to UPDATE mode;
    ERR_READID    :read id fail;
    ERR_ERASE    :erase chip fail;
    ERR_STATUS    :status error;
    ERR_ECC        :ecc error.
*/


#define    FTS_PACKET_LENGTH        128

#if 0
static unsigned char CTPM_FW[]=
{
#include "Z8805_ft5206_WKD_Va5_2011_05_30.i"
};
#endif

#ifdef FT_UPGRADE_WITH_I
static unsigned char CTPM_FW[]=
{
	#include "ft_app.i"
};
E_UPGRADE_ERR_TYPE  fts_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
    FTS_BYTE reg_val[2] = {0};
    FTS_DWRD i = 0;

    FTS_DWRD  packet_number;
    FTS_DWRD  j;
    FTS_DWRD  temp;
    FTS_DWRD  lenght;
    FTS_BYTE  packet_buf[FTS_PACKET_LENGTH + 6];
    FTS_BYTE  auc_i2c_write_buf[10];
    FTS_BYTE bt_ecc;
    int      i_ret;

    /*********Step 1:Reset  CTPM *****/
    /*write 0xaa to register 0xfc*/
    ft5x0x_write_reg(0xfc,0xaa);
    msleep(50);
     /*write 0x55 to register 0xfc*/
    ft5x0x_write_reg(0xfc,0x55);
    printk("[TSP] Step 1: Reset CTPM test, bin-length=%d\n",dw_lenth);
   
    msleep(30);  


    /*********Step 2:Enter upgrade mode *****/
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
        i ++;
        i_ret = ft5x0x_i2c_txdata(auc_i2c_write_buf, 2);
        msleep(5);
    }while(i_ret <= 0 && i < 5 );

    /*********Step 3:check READ-ID***********************/        
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
    {
        printk("[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
    	printk("%s: ERR_READID, ID1 = 0x%x,ID2 = 0x%x\n", __func__,reg_val[0],reg_val[1]);
        return ERR_READID;
        //i_is_new_protocol = 1;
    }

     /*********Step 4:erase app*******************************/
    cmd_write(0x61,0x00,0x00,0x00,1);
   
    msleep(1500);
    printk("[TSP] Step 4: erase.\n");

    /*********Step 5:write firmware(FW) to ctpm flash*********/
    bt_ecc = 0;
    printk("[TSP] Step 5: start upgrade.\n");
    dw_lenth = dw_lenth - 8;
    packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
    packet_buf[0] = 0xbf;
    packet_buf[1] = 0x00;
    for (j=0;j<packet_number;j++)
    {
        temp = j * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        lenght = FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(lenght>>8);
        packet_buf[5] = (FTS_BYTE)lenght;

        for (i=0;i<FTS_PACKET_LENGTH;i++)
        {
            packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }
        
        byte_write(&packet_buf[0],FTS_PACKET_LENGTH + 6);
        msleep(FTS_PACKET_LENGTH/6 + 1);
        if ((j * FTS_PACKET_LENGTH % 1024) == 0)
        {
              printk("[TSP] upgrade the 0x%x th byte.\n", ((unsigned int)j) * FTS_PACKET_LENGTH);
        }
    }

    if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
    {
        temp = packet_number * FTS_PACKET_LENGTH;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;

        temp = (dw_lenth) % FTS_PACKET_LENGTH;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;

        for (i=0;i<temp;i++)
        {
            packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }

        byte_write(&packet_buf[0],temp+6);    
        msleep(20);
    }

    //send the last six byte
    for (i = 0; i<6; i++)
    {
        temp = 0x6ffa + i;
        packet_buf[2] = (FTS_BYTE)(temp>>8);
        packet_buf[3] = (FTS_BYTE)temp;
        temp =1;
        packet_buf[4] = (FTS_BYTE)(temp>>8);
        packet_buf[5] = (FTS_BYTE)temp;
        packet_buf[6] = pbt_buf[ dw_lenth + i]; 
        bt_ecc ^= packet_buf[6];

        byte_write(&packet_buf[0],7);  
        msleep(20);
    }

    /*********Step 6: read out checksum***********************/
    /*send the opration head*/
    
    cmd_write(0xcc,0x00,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], bt_ecc);
    if(reg_val[0] != bt_ecc)
    {
    	printk("%s: ERR_ECC\n", __func__);
        return ERR_ECC;
    }
    
    /*********Step 7: reset the new FW***********************/
    cmd_write(0x07,0x00,0x00,0x00,1);

    return ERR_OK;
}
unsigned char fts_ctpm_get_upg_ver(void)
{
	unsigned int ui_sz;
    ui_sz = sizeof(CTPM_FW);
    if (ui_sz > 2)
    {
        return CTPM_FW[ui_sz - 2];
    }
    else
    {
        //TBD, error handling?
        return 0xff; //default value
    }
}
int fts_ctpm_fw_upgrade_with_i_file(void)
{
   FTS_BYTE*     pbt_buf = FTS_NULL;
   int i_ret;
    
    //=========FW upgrade========================*/
   pbt_buf = CTPM_FW;
   /*call the upgrade function*/
   i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW));
   if (i_ret != 0)
   {
       //error handling ...
       //TBD
   }
}
static void ft5x0x_ts_upgrade_work(struct work_struct *work)
{
	unsigned char uc_reg_value; 
	unsigned char local_FW_value; 
	uc_reg_value = fts_ctpm_get_upg_ver();
	printk("[FST] Firmware version = 0x%x\n", uc_reg_value);
	local_FW_value=ft5x0x_read_fw_ver();
	printk("[FST] local  version = 0x%x\n", local_FW_value);
	if(1){
		fts_ctpm_fw_upgrade_with_i_file();
	}
}
#endif

static int fts_ctpm_fw_update(void)
{
    int ret = 0;
    const struct firmware *fw;
    unsigned char *fw_buf;
	struct platform_device *pdev;
	
	pdev = platform_device_register_simple("ft5206_ts", 0, NULL, 0);
	if (IS_ERR(pdev)) {
		printk("%s: Failed to register firmware\n", __func__);
		return -1;
	}

    ret = request_firmware(&fw, "ft5206_fw.bin", &pdev->dev);
    if (ret) {
		printk("%s: request_firmware error\n",__func__);
		platform_device_unregister(pdev);
        return -1;
    }

	platform_device_unregister(pdev);
    printk("%s:fw size=%d\n", __func__,fw->size);
	
    fw_buf = kzalloc(fw->size, GFP_KERNEL | GFP_DMA);
    memcpy(fw_buf, fw->data, fw->size);
	
   	//fts_ctpm_fw_upgrade(fw_buf, fw->size);

	printk("%s: update finish\n", __func__);
    release_firmware(fw);
    kfree(fw_buf);

	return 0;    
}

#endif


/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_ts_release(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
#if CFG_SUPPORT_TOUCH_KEY
    int i;
    for(i = 0; i <CFG_NUMOFKEYS; i++ )
    {
        if(tsp_keystatus[i])
        {
            input_report_key(data->input_dev, tsp_virtual_key[i].key_code, 0);
            tsp_keystatus[i] = KEY_RELEASE;
        }
    }
#endif 
#ifdef CONFIG_FT5X0X_MULTITOUCH	
	input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
#else
	input_report_abs(data->input_dev, ABS_PRESSURE, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
#endif
	input_sync(data->input_dev);
	TS_DBG("%s",__func__);
}

static int ft5x0x_read_data(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
//	u8 buf[14] = {0};
	u8 buf[32] = {0};
	int ret = -1;

#ifdef CONFIG_FT5X0X_MULTITOUCH
//	ret = ft5x0x_i2c_rxdata(buf, 13);
	ret = ft5x0x_i2c_rxdata(buf, 31);
#else
    ret = ft5x0x_i2c_rxdata(buf, 7);
#endif
    if (ret < 0) {
		printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;
	}

	memset(event, 0, sizeof(struct ts_event));
//	event->touch_point = buf[2] & 0x03;// 0000 0011
	event->touch_point = buf[2] & 0x07;// 000 0111

    if (event->touch_point == 0) {
        ft5x0x_ts_release();
        return 1; 
    }

#ifdef CONFIG_FT5X0X_MULTITOUCH
    switch (event->touch_point) {
		case 5:
			event->x5 = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
			event->y5 = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
			TS_DBG("===x5 = %d,y5 = %d ====",event->x5,event->y5);
		case 4:
			event->x4 = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
			event->y4 = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
			TS_DBG("===x4 = %d,y4 = %d ====",event->x4,event->y4);
		case 3:
			event->x3 = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
			event->y3 = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
			TS_DBG("===x3 = %d,y3 = %d ====",event->x3,event->y3);
		case 2:
			event->x2 = (s16)(buf[9] & 0x0F)<<8 | (s16)buf[10];
			event->y2 = (s16)(buf[11] & 0x0F)<<8 | (s16)buf[12];
			TS_DBG("===x2 = %d,y2 = %d ====",event->x2,event->y2);
		case 1:
			event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
			event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
			TS_DBG("===x1 = %d,y1 = %d ====",event->x1,event->y1);
            printk("ft5x0x_read_data x1=%d, y1=%d" ,event->x1,event->y1);
            break;
		default:
		    return -1;
	}
#else
    if (event->touch_point == 1) {
    	event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
		event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
    }
#endif
    event->pressure = 200;

	//printk("%d (%d, %d), (%d, %d)\n", event->touch_point, event->x1, event->y1, event->x2, event->y2);
#if TS_DATA_THRESHOLD_CHECK
	#ifdef CONFIG_FT5X0X_MULTITOUCH
		if((event->x1>TS_WIDTH_MAX || event->y1>TS_HEIGHT_MAX)||
		   (event->x2>TS_WIDTH_MAX || event->y2>TS_HEIGHT_MAX)||
		   (event->x3>TS_WIDTH_MAX || event->y3>TS_HEIGHT_MAX)||
		   (event->x4>TS_WIDTH_MAX || event->y4>TS_HEIGHT_MAX)||
		   (event->x5>TS_WIDTH_MAX || event->y5>TS_HEIGHT_MAX)) {
				printk("%s: get dirty data x1=%d,y1=%d\n",__func__, event->x1, event->y1);
				return -1;
		}
	#else
		if(event->x1>TS_WIDTH_MAX || event->y1>TS_HEIGHT_MAX){
				printk("%s: get dirty data x1=%d,y1=%d\n",__func__, event->x1, event->y1);
				return -1;
		}
	#endif
#endif

	return 0;
}
#if CFG_SUPPORT_TOUCH_KEY
int ft5x0x_touch_key_process(struct input_dev *dev, int x, int y)
{
    int i;
    int key_id = 0x0f;
    for(i = 0; i <CFG_NUMOFKEYS; i++ )
    {
    	if ((tsp_virtual_key[i].x_min <x)
        	&&(tsp_virtual_key[i].x_max >x)
        	&&(tsp_virtual_key[i].y_min <y)
        	&&(tsp_virtual_key[i].y_max >y))
    	{
    		key_id = i;
    		break;
    	}
    }
    if ((key_id != 0x0f) && (tsp_keystatus[key_id] == KEY_RELEASE))
    {
        input_report_key(dev, tsp_virtual_key[key_id].key_code, 1);
        tsp_keystatus[key_id] = KEY_PRESS;
    }
    return 0;
}    
#endif
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_report_value(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;

//		printk("==ft5x0x_report_value =\n");
#ifdef CONFIG_FT5X0X_MULTITOUCH
	switch(event->touch_point) {
		case 5:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x5);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y5);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
			
		case 4:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x4);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y4);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
			
		case 3:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x3);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y3);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
			
		case 2:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_mt_sync(data->input_dev);
			
		case 1:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
#if CFG_SUPPORT_TOUCH_KEY
//printk("ft lvjie event->y1 =\n",event->y1);
            if ((event->y1 >= TOUCH_HEIGHT_MIN) && (event->y1 < TOUCH_HEIGHT_MAX))
            {
                ft5x0x_touch_key_process(data->input_dev, event->x1, event->y1);
            }
#endif
			input_mt_sync(data->input_dev);
			

		default:
//			printk("==touch_point default =\n");
			break;
	}
#else	/* CONFIG_FT5X0X_MULTITOUCH*/
	if (event->touch_point == 1) {
		input_report_abs(data->input_dev, ABS_X, event->x1);
		input_report_abs(data->input_dev, ABS_Y, event->y1);
		input_report_abs(data->input_dev, ABS_PRESSURE, event->pressure);
	}
	input_report_key(data->input_dev, BTN_TOUCH, 1);
#endif	/* CONFIG_FT5X0X_MULTITOUCH*/
	input_sync(data->input_dev);
}	/*end ft5x0x_report_value*/
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
{
#if 1
	int ret = -1;
//	printk("==work 1=\n");
	ret = ft5x0x_read_data();	
	if (ret == 0) {	
		ft5x0x_report_value();
	}
//	else printk("data package read error\n");
//	printk("==work 2=\n");
//    	msleep(1);
#endif
        
        enable_irq(this_client->irq);

}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static irqreturn_t ft5x0x_ts_interrupt(int irq, void *dev_id)
{

	struct ft5x0x_ts_data *ft5x0x_ts = (struct ft5x0x_ts_data *)dev_id;

    disable_irq_nosync(this_client->irq);
	if (!work_pending(&ft5x0x_ts->pen_event_work)) {
		queue_work(ft5x0x_ts->ts_workqueue, &ft5x0x_ts->pen_event_work);
	}
	//printk("==int=, 11irq=%d\n", this_client->irq);
	return IRQ_HANDLED;
}

#if !I2C_BOARD_INFO_METHOD
static void ft5x0x_ts_reset(void)
{
	TS_DBG("%s",__func__);
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	msleep(1);
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 0);
	msleep(10);
	gpio_set_value(sprd_3rdparty_gpio_tp_rst, 1);
	msleep(1);
}
#endif

/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
#ifdef HAS_EARLYSUSPEND
static void ft5x0x_ts_suspend(struct early_suspend *handler)
{
//	struct ft5x0x_ts_data *ts;
//	ts =  container_of(handler, struct ft5x0x_ts_data, early_suspend);

	printk("==ft5x0x_ts_suspend=\n");
//	disable_irq(this_client->irq);
//	disable_irq(IRQ_EINT(6));
//	cancel_work_sync(&ts->pen_event_work);
//	flush_workqueue(ts->ts_workqueue);
	// ==set mode ==, 
    disable_irq_nosync(this_client->irq);
   	ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
#ifdef HAS_LDO
    ft5x0x_chip_vdd_input(false);
#endif
}
#endif
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
#ifdef HAS_EARLYSUSPEND
static void ft5x0x_ts_resume(struct early_suspend *handler)
{	
	printk("==%s==\n", __FUNCTION__);
#ifdef HAS_LDO
    ft5x0x_chip_vdd_input(true);
#endif
	mdelay(20);
	ft5x0x_ts_reset();
	mdelay(50);    
    enable_irq(this_client->irq);
}
#endif

#ifdef HAS_LDO
static void ft5x0x_chip_vdd_input(bool turn_on)
{
    if (turn_on) {
        printk("### %s turn on LDO_SIM2\n", __func__);
    	LDO_SetVoltLevel(LDO_LDO_SIM2, LDO_VOLT_LEVEL0);
    	LDO_TurnOnLDO(LDO_LDO_SIM2);
        msleep(1);
    }
    else {
        printk("### %s turn off LDO_SIM2\n", __func__);
	    LDO_TurnOffLDO(LDO_LDO_SIM2);
    }
}
#endif
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int 
ft5x0x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ft5x0x_ts_data *ft5x0x_ts;
	struct input_dev *input_dev;
	int err = 0;
	//unsigned char uc_reg_value; 
#if CFG_SUPPORT_TOUCH_KEY
    int i;
#endif
	printk("==ft5x0x_ts_probe=\n");
	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	TS_DBG("==kzalloc=");
	ft5x0x_ts = kzalloc(sizeof(*ft5x0x_ts), GFP_KERNEL);
	if (!ft5x0x_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}


	this_client = client;
	i2c_set_clientdata(client, ft5x0x_ts);

	TS_DBG("I2C addr=%x", client->addr);

	INIT_WORK(&ft5x0x_ts->pen_event_work, ft5x0x_ts_pen_irq_work);

	ft5x0x_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!ft5x0x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}
#ifdef FT_UPGRADE_WITH_I
	INIT_DELAYED_WORK(&ft5x0x_ts->pen_upgrade_work,ft5x0x_ts_upgrade_work);
	ft5x0x_ts->ts_upgrade_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	queue_delayed_work(ft5x0x_ts->ts_upgrade_workqueue, &ft5x0x_ts->pen_upgrade_work,FT_UPGRADE_DELAY);
#endif
//	printk("==request_irq=\n");
	TS_DBG("%s IRQ number is %d", client->name, client->irq);
	err = request_irq(client->irq, ft5x0x_ts_interrupt, IRQF_TRIGGER_FALLING, client->name, ft5x0x_ts);
	if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

//	__gpio_as_irq_fall_edge(pdata->intr);		//
//	disable_irq(this_client->irq);
	disable_irq(client->irq);
#ifdef TOUCH_VIRTUAL_KEYS
	ft5x0x_ts_virtual_keys_init();
#endif

//	printk("==input_allocate_device=\n");
	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	ft5x0x_ts->input_dev = input_dev;

#ifdef CONFIG_FT5X0X_MULTITOUCH
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);

	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
#else
	set_bit(ABS_X, input_dev->absbit);
	set_bit(ABS_Y, input_dev->absbit);
	set_bit(ABS_PRESSURE, input_dev->absbit);
	set_bit(BTN_TOUCH, input_dev->keybit);

	input_set_abs_params(input_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_PRESSURE, 0, PRESS_MAX, 0 , 0);
#endif

	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);

#ifdef  TOUCH_VIRTUAL_KEYS
	set_bit(KEY_MENU,  input_dev->keybit);
	set_bit(KEY_BACK,  input_dev->keybit);
#endif
#if CFG_SUPPORT_TOUCH_KEY
    set_bit(EV_SYN, input_dev->evbit);
    set_bit(BTN_TOUCH, input_dev->keybit);
    input_dev->keycode = tsp_virtual_key;
    for(i = 0; i < CFG_NUMOFKEYS; i++)
    {
        input_set_capability(input_dev, EV_KEY, (tsp_virtual_key[i].key_code));
        tsp_keystatus[i] = KEY_RELEASE;
    }
#endif
	input_dev->name		= FT5X0X_NAME;		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x0x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}
#ifdef HAS_EARLYSUSPEND
	TS_DBG("==register_early_suspend =");
	ft5x0x_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ft5x0x_ts->early_suspend.suspend = ft5x0x_ts_suspend;
	ft5x0x_ts->early_suspend.resume	= ft5x0x_ts_resume;
	register_early_suspend(&ft5x0x_ts->early_suspend);
#endif
#if 0
    msleep(50);
    //get some register information
    uc_reg_value = ft5x0x_read_fw_ver();
    TS_DBG("[FST] Firmware version = 0x%x", uc_reg_value);
#endif


//    fts_ctpm_fw_upgrade_with_i_file();


    
//wake the CTPM
//	__gpio_as_output(GPIO_FT5X0X_WAKE);		
//	__gpio_clear_pin(GPIO_FT5X0X_WAKE);		//set wake = 0,base on system
//	 msleep(100);
//	__gpio_set_pin(GPIO_FT5X0X_WAKE);			//set wake = 1,base on system
//	msleep(100);
//	ft5x0x_set_reg(0x88, 0x05); //5, 6,7,8
//	ft5x0x_set_reg(0x80, 30);
//	msleep(50);
//    	enable_irq(this_client->irq);

	ft5x0x_create_sysfs(client);

	enable_irq(client->irq);

	printk("==probe over =\n");
    return 0;

exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(client->irq, ft5x0x_ts);
exit_irq_request_failed:
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
exit_create_singlethread:
	printk("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(ft5x0x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
#if !I2C_BOARD_INFO_METHOD
	sprd_free_gpio_irq(ft5x0x_ts_setup.irq);
#endif
	return err;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int __devexit ft5x0x_ts_remove(struct i2c_client *client)
{

	struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);

	printk("==ft5x0x_ts_remove=\n");
#ifdef HAS_EARLYSUSPEND
	unregister_early_suspend(&ft5x0x_ts->early_suspend);
#endif
	free_irq(client->irq, ft5x0x_ts);
#if !I2C_BOARD_INFO_METHOD
	sprd_free_gpio_irq(ft5x0x_ts_setup.irq);
#endif
	input_unregister_device(ft5x0x_ts->input_dev);
	kfree(ft5x0x_ts);
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
	i2c_set_clientdata(client, NULL);
	return 0;
}

static const struct i2c_device_id ft5x0x_ts_id[] = {
	{ FT5X0X_NAME, 0 },{ }
};


MODULE_DEVICE_TABLE(i2c, ft5x0x_ts_id);

static struct i2c_driver ft5x0x_ts_driver = {
	.probe		= ft5x0x_ts_probe,
	.remove		= __devexit_p(ft5x0x_ts_remove),
	.id_table	= ft5x0x_ts_id,
	.driver	= {
		.name	= FT5X0X_NAME,
		.owner	= THIS_MODULE,
	},
};

#if I2C_BOARD_INFO_METHOD
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int __init ft5x0x_ts_init(void)
{
	int ret;
	printk("==ft5x0x_ts_init==\n");
	ret = i2c_add_driver(&ft5x0x_ts_driver);
	return ret;
//	return i2c_add_driver(&ft5x0x_ts_driver);
}

/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void __exit ft5x0x_ts_exit(void)
{
	printk("==ft5x0x_ts_exit==\n");
	i2c_del_driver(&ft5x0x_ts_driver);
}
#else //register i2c device&driver dynamicly

int sprd_add_i2c_device(struct sprd_i2c_setup_data *i2c_set_data, struct i2c_driver *driver)
{
	struct i2c_board_info info;
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	int ret,err;


	TS_DBG("%s : i2c_bus=%d; slave_address=0x%x; i2c_name=%s",__func__,i2c_set_data->i2c_bus, \
		    i2c_set_data->i2c_address, i2c_set_data->type);

	memset(&info, 0, sizeof(struct i2c_board_info));
	info.addr = i2c_set_data->i2c_address;
	strlcpy(info.type, i2c_set_data->type, I2C_NAME_SIZE);
	if(i2c_set_data->irq > 0)
		info.irq = i2c_set_data->irq;

	adapter = i2c_get_adapter( i2c_set_data->i2c_bus);
	if (!adapter) {
		printk("%s: can't get i2c adapter %d\n",
			__func__,  i2c_set_data->i2c_bus);
		err = -ENODEV;
		goto err_driver;
	}

	client = i2c_new_device(adapter, &info);
	if (!client) {
		printk("%s:  can't add i2c device at 0x%x\n",
			__func__, (unsigned int)info.addr);
		err = -ENODEV;
		goto err_driver;
	}

	i2c_put_adapter(adapter);

	ret = i2c_add_driver(driver);
	if (ret != 0) {
		printk("%s: can't add i2c driver\n", __func__);
		err = -ENODEV;
		goto err_driver;
	}	

	return 0;

err_driver:
	return err;
}

void sprd_del_i2c_device(struct i2c_client *client, struct i2c_driver *driver)
{
	TS_DBG("%s : slave_address=0x%x; i2c_name=%s",__func__, client->addr, client->name);
	i2c_unregister_device(client);
	i2c_del_driver(driver);
}

static int __init ft5x0x_ts_init(void)
{
	int ft5x0x_irq;
	
	printk("%s\n", __func__);
#ifdef HAS_LDO
	ft5x0x_chip_vdd_input(true);
#endif
	ft5x0x_irq=ft5x0x_ts_config_pins();
	ft5x0x_ts_setup.i2c_bus = 2;
	ft5x0x_ts_setup.i2c_address = FT5206_TS_ADDR;
	strcpy (ft5x0x_ts_setup.type,FT5206_TS_NAME);
	ft5x0x_ts_setup.irq = ft5x0x_irq;
	return sprd_add_i2c_device(&ft5x0x_ts_setup, &ft5x0x_ts_driver);
}

static void __exit ft5x0x_ts_exit(void)
{
	printk("%s\n", __func__);
	sprd_del_i2c_device(this_client, &ft5x0x_ts_driver);
	
}
#endif

module_init(ft5x0x_ts_init);
module_exit(ft5x0x_ts_exit);

MODULE_AUTHOR("<wenfs@Focaltech-systems.com>");
MODULE_DESCRIPTION("FocalTech ft5x0x TouchScreen driver");
MODULE_LICENSE("GPL");
