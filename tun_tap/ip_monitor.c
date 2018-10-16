
/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
/* Userspace Paths */
#define MODEM_UIO_DEV			"/dev/uio0"

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/
int			rx_dma_uio_fd;
void		*rx_dma_uio_addr;
uint32_t	rx_buff_mem_size;
uint32_t	rx_buff_mem_addr;

/***************************************************************************//**
 * @brief get_file_info
*******************************************************************************/
int32_t get_file_info(const char *filename, uint32_t *info)
{
	int32_t ret;
	FILE* fp;

	fp = fopen(filename,"r");
	if (!fp) {
		printf("%s: File %s cannot be opened.", __func__, filename);
		return -1;
	}
	ret = fscanf(fp,"0x%x", info);
	if (ret < 0) {
		printf("%s: Cannot read info from file %s.", __func__, filename);
		return -1;
	}
	fclose(fp);

	return 0;
}

/***************************************************************************//**
 * @brief uio_read
*******************************************************************************/
void uio_read(void *uio_addr, uint32_t reg_addr, uint32_t *data)
{
	*data = (*((unsigned *) (uio_addr + reg_addr)));
}

/***************************************************************************//**
 * @brief uio_write
*******************************************************************************/
void uio_write(void *uio_addr, uint32_t reg_addr, uint32_t data)
{
	*((unsigned *) (uio_addr + reg_addr)) = data;
}

/***************************************************************************//**
 * @brief uio_write
*******************************************************************************/
int32_t modem_write(uint32_t reg_addr, uint32_t data)
{
	int			modem_uio_fd_rx;
	void		*modem_uio_addr_rx;

	modem_uio_fd_rx = open(MODEM_UIO_DEV, O_RDWR);
	if(modem_uio_fd_rx < 1) {
		printf("%s: Can't open Rx modem_uio device\n\r", __func__);

		return modem_uio_fd_rx;
	}

	modem_uio_addr_rx = mmap(NULL,
			      4096,
			      PROT_READ|PROT_WRITE,
			      MAP_SHARED,
			      modem_uio_fd_rx,
			      0);

	uio_write(modem_uio_addr_rx, reg_addr, data);

	munmap(modem_uio_addr_rx, 4096);

	close(modem_uio_fd_rx);

	return 0;
}

/***************************************************************************//**
 * @brief uio_write
*******************************************************************************/
int32_t modem_read(uint32_t reg_addr, uint32_t *data)
{
	int			modem_uio_fd_rx;
	void		*modem_uio_addr_rx;

	modem_uio_fd_rx = open(MODEM_UIO_DEV, O_RDWR);
	if(modem_uio_fd_rx < 1) {
		printf("%s: Can't open Rx modem_uio device\n\r", __func__);

		return modem_uio_fd_rx;
	}

	modem_uio_addr_rx = mmap(NULL,
			      4096,
			      PROT_READ|PROT_WRITE,
			      MAP_SHARED,
			      modem_uio_fd_rx,
			      0);

	uio_read(modem_uio_addr_rx, reg_addr, data);

	munmap(modem_uio_addr_rx, 4096);

	close(modem_uio_fd_rx);

	return 0;
}

void reset_modem()
{

	printf("Resesting modem\n");
	modem_write(0x0, 0x1); //reset
	modem_write(0x118, (uint32_t)0);  //Rx Enable [0=disable]
	modem_write(0x100, (uint32_t)10); //FRLoopBw
	modem_write(0x104, (uint32_t)200); //EQmu
	modem_write(0x108, (uint32_t)3);  //Scope select
	modem_write(0x110, (uint32_t)0);  //Tx DMA select [0=Modem,1=DMA]
	modem_write(0x114, (uint32_t)0);  //EQ Bypass
	modem_write(0x11C, (uint32_t)10);  //PD Threshold
	modem_write(0x120, (uint32_t)0);  //Packet Toggle Transmit
	modem_write(0x124, (uint32_t)0);  //Packet Transmit Always
	modem_write(0x128, (uint32_t)0);  //Packet Source Select
	modem_write(0x12C, (uint32_t)1);  //Loopback control [1=RF]
	modem_write(0x130, (uint32_t)0);  //Coding Bypass [1=Bypass]
	//modem_write(0x134, (uint32_t)1);  //Viterbi Index
	//sleep(1);
	modem_write(0x118, (uint32_t)1);  //Rx Enable

}


/***************************************************************************//**
* @brief main
*******************************************************************************/
int main(int argc, char *argv[])
{
	uint32_t data;
	unsigned int dropped = 0;
	unsigned int hdropped = 0;
	unsigned long history[10];
	unsigned long hhistory[10];
	unsigned int histIndx = 0;
	unsigned int limit = 20;
	unsigned int k, j;
	unsigned long long clock = 0;
				
	reset_modem();
	for (j=0; j<10; j++)
	{
		history[j] = 0;
		hhistory[j] = 0;
	}

	while (1)
	{
		// Check 
		modem_write(0x10C, 3);
		modem_read(0x140, &data);
		hdropped = data;
		modem_write(0x10C, 4);
		modem_read(0x140, &data);
		dropped = data;
		printf("CRC Errors %lu | Header Errors: %lu | %llu\n", dropped, hdropped, clock);
		clock++;

		// Check history
		for (k=0; k<10; k++)
		{
			if ( ( abs(dropped - history[k]) > limit ) || ( abs(hdropped - hhistory[k]) > limit ) )
			{
				reset_modem();
				for (j=0; j<10; j++)
				{
					hhistory[j] = hdropped;
					history[j] = dropped;
				}
				break;
			}
		}

		// Add to history
		history[histIndx] = dropped;
		hhistory[histIndx] = hdropped;
		histIndx++;
		histIndx = histIndx % 10;
		sleep(1);
		//usleep(1000000);
	}	

	return 0;
}




