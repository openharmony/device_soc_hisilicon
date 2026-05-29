1. Before using the gyro sensor, Ensure the following configuration: smp/a55_linux/vendor/motionsensor/motionsensor_spi/motionsensor_spi.c
static td_void spi_pin_muxing(td_void)
{
    ot_reg_write(io1_addr(0x164), 0x1241); /* SPI1_SCLK */
    ot_reg_write(io1_addr(0x158), 0x1241); /* SPI1_CSN0 */
    ot_reg_write(io1_addr(0x15c), 0x1251); /* SPI1_SDI */
    ot_reg_write(io1_addr(0x160), 0x1051); /* SPI1_SDO */
}

2. insert gyro ko after sys_config.ko and update the file: smp/a55_linux/mpp/out/ko/load_ss928v100
2.1  insert_gyro
2.2  rmmod_gyro

3. modify the makefile parameter: smp/a55_linux/mpp/sample/Makefile.param.
    ################ open GYRO_DIS sample ########################
	GYRO_DIS ?= y

Rebuild the sample and get the sample_dis.
