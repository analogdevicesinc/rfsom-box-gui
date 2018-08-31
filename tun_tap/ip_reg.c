
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
extern char *optarg;
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

/***************************************************************************//**
* @brief main
*******************************************************************************/
int main(int argc, char *argv[])
{
	uint32_t data;
	uint32_t reg;
	int opt;
	uint32_t i;
	uint8_t quiet;
//	printf("Configuring modem IP\n");
	quiet=0;	
	while ((opt = getopt(argc, argv, "qf:e:s:d:t:xr:")) != -1) {
        switch (opt) {
		case 'q':
			quiet = 1;
		break;
		case 'f': //FRLoopBw
			if(!quiet) 
				printf("Setting the FRLoopBw\n");
			data = atoi(optarg);
			modem_write(0x100, data); 
		break;
		case 'e': //EQmu
			if(!quiet) 
				printf("Setting the EQmu\n");
			data = atoi(optarg);
			modem_write(0x104, data); 
		break;
		case 's': //Scope select
			if(!quiet) 
				printf("Setting the Scope select\n");
			data = atoi(optarg);
			modem_write(0x108, data);  
		break;
		case 'd': //Debug selector
			if(!quiet) 
				printf("Setting the Debug selector\n");
			data = atoi(optarg);
			modem_write(0x10C, data);  
		break;			
		case 't': //Tx DMA select
			if(!quiet) 
				printf("Setting the Tx DMA select\n");
			data = atoi(optarg);
			modem_write(0x110, data);  
		break;
		case 'r': // read
			reg = strtol(optarg,NULL,16);
//			if(reg % 4 ==0 && reg>=0x00 && reg<=0x134) {
				modem_write(0x10c,reg);
				modem_read(0x140, &data);
				printf("%d\n",data);
//			} else { 
//				if(!quiet) 
//					printf("No register @ address 0x%x", reg); 
//			}
		break;

		case 'x':
			if(!quiet) 
				printf("Setting the defaults\n");
			modem_write(0x118, (uint32_t)0);  	//Rx Enable
			modem_write(0x100, (uint32_t)40); 	//FRLoopBw
			modem_write(0x104, (uint32_t)200); 	//EQmu
			modem_write(0x108, (uint32_t)2);  	//Scope select
			modem_write(0x110, (uint32_t)0);  	//Tx DMA select
			modem_write(0x114, (uint32_t)0);  	//EQ Bypass
			modem_write(0x11C, (uint32_t)10);  	//PD Threshold
			modem_write(0x120, (uint32_t)0);  	//Packet Toggle Transmit
			modem_write(0x124, (uint32_t)0);  	//Packet Transmit Always
			modem_write(0x128, (uint32_t)0);  	//Packet Source Select [0==DMA, 1==Internal Packet Generator]
			modem_write(0x12C, (uint32_t)1);  	//Loopback control [0 Loopback, 1 RF]
			modem_write(0x130, (uint32_t)0);  	//DMA To DMA Direct Control  [0==Direct,1==Through Full Modem]
			sleep(1);
			modem_write(0x118, (uint32_t)1);  	//Rx Enable
			break;
		default: /* '?' */
            fprintf(stderr, "Usage: %s [-f FRLoopBw] [-e EQmu] [-s Scope select] [-d Debug Selector] [-t Tx DMA Select] [-x Default values] [-r]\n", argv[0]);
		}
    }
	
	modem_read(0x134, &data); //payloadLen
	if(!quiet) { 
		printf("payloadLen: %d\n", data);
		printf("All Done\n");
	}
	
	return 0;
}
