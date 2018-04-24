#ifndef __MAC_H__
#define __MAC_H__

int32_t modem_reset(void);
int32_t modem_setup(uint32_t rx_size);
int32_t modem_start(void);
int32_t modem_stop(void);
int32_t modem_close(void);
int32_t modem_running(void);
int32_t modem_write(uint64_t* buf, uint32_t size, uint32_t cyclic);
void *modem_read(void);

#endif /*__MAC_H__ */

