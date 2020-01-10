/*
 * at24xx.h
 *
 *  Created on: Oct 15, 2010
 *      Author: neal
 */

#ifndef AT24XX_H_
#define AT24XX_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

// at24 card definition
#define AT24C01			1			
#define AT24C02			2			
#define AT24C04			4			
#define AT24C08			8			
#define AT24C16			16			
#define AT24C32			32			
#define AT24C64			64			
#define AT24C128		128			
#define AT24C256		256			
#define AT24C512		512			
#define AT24C1024		1024

// Function:
//		power up  and reset the at24 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0	---- success
//		-1   ---- failed, please check "errno" for actual error code
//			ENXIO	---- no card in slot
int at24_open(int fd);

// Function:
//	 	read data from at24 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		card_type	----- at24 card type
//		start_addr	----- start address
//		len			----- the length want to be read out
// Output:
//		data			---- data read from at24 card
// Return Value:
//		0		---- success
//		-1		---- failed, please check "errno" for actual error code
//			EINVAL	---- invalid parameter
//			ENXIO	---- no card in slot
//			ETIME	---- time out
int at24_read(int fd,int card_type,int start_addr,int len,void *data);

// Function:
//		write data into at24 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		card_type	----- at24 card type
//		start_addr	----- start address
//		len			----- the length want to be written
//		data			----- data
//		0		---- success
//		-1		---- failed, please check "errno" for actual error code
//			EINVAL	---- invalid parameter
//			ENXIO	---- no card in slot
//			ETIME	---- time out
int at24_write(int fd,int card_type,int start_addr,int len,const void *data);

// Function:
//		power down at24 card
// Input:
// 		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0	---- success
//		-1   ---- failed, please check "errno" for actual error code
int at24_close(int fd);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* AT24XX_H_ */
