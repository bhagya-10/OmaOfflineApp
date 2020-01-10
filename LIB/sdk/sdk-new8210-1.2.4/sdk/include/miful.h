/*
 * miful.h
 *
 *  Created on: Oct 14, 2010
 *      Author: neal
 */

#ifndef MIFUL_H_
#define MIFUL_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

// Function:
//		poll the mifare ultralight card in RF field,and return the card information
// Input:
//		fd      ----  device id, return from mif_open(...)
//		mode  ----  poll mode, value can be
//				   0  --- only poll the mifare ultralight cards which in IDLE state now
//				   1  --- poll the mifare ultralight cards which in IDLE state and HALT state now
// Output:
//		uid	  ---- the uid information of mifare ultralight, 7bytes long
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ENODATA ---  no mifare classic card found in RF field
//				ETIME	---- timeout during poll card
//				EMULTIHOP	---- more than one card in RF field
int mifare_ultralight_poll(int fd, uint8_t mode, void *uid);

// Function:
//		reset the mifare ultralight card, and put it into HALT state
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code	
//				EIO		---- hardware error
int mifare_ultralight_halt(int fd);

// Function:
//		read four page data from mifare ultralight
// Input:
// 		fd      ----  device id, return from mif_open(...)
//		page  ---- the page no to be read out, 0~15.
// Output:
//		data  ---- the four page data from mifare ultralight card, 16bytes total
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare ultralight
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_ultralight_read_page(int fd,uint8_t page,void *data);

// Function:
//		write one page data into mifare ultralight card
// Input:
// 		fd      ----  device id, return from mif_open(...)
//		page  ---- the page no to be wrote, 0~15.
//		data  ---- the page data to be wrote, 4bytes
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare ultralight
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_ultralight_write_page(int fd,uint8_t page,const void *data);

// Function:
//		write one page data into mifare ultralight card
// Input:
// 		fd      ----  device id, return from mif_open(...)
//		page  ---- the page no to be wrote, 0~15.
//		data  ---- the page data to be wrote, 16 bytes
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare ultralight
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_ultralight_write_page16(int fd,uint8_t page,const void *data);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* MIFUL_H_ */
