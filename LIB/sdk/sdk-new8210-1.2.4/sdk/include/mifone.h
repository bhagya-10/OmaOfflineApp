/*
 * mifone.h
 *
 *  Created on: Oct 14, 2010
 *      Author: neal
 */

#ifndef MIFONE_H_
#define MIFONE_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

// Function:
//		poll the mifare classic card in RF field,and return the card information
// Input:
//		fd      ----  device id, return from mif_open(...)
//		mode  ----  poll mode, value can be
//				   0  --- only poll the mifare classic cards which in IDLE state now
//				   1  --- poll the mifare classic cards which in IDLE state and HALT state now
// Output:
//		uid	  ---- the uid information of mifare classic, 5bytes long, 4bytes UID+BCC
//		cardtype ---- the type of mifare classic card, refer to contactless card type definition in contactless.h
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
int mifare_one_poll(int fd, uint8_t mode, void *uid , uint8_t *cardtype);

// Function:
//		reset the mifare classic card, and put it into HALT state
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code	
//				EIO		---- hardware error
int mifare_one_halt(int fd);

// Function:
//		authenticate one block of mifare classic card
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno	---- the block no be authenticated, 0~63 for S50, and 0~255 for S70
//		keytype	---- key type for authenticated
//					'A'  --- authenticate key A
//					'B'  --- authenticate key B
//		key		---- key information, 6bytes long
//		uid		---- the uid information of mifare classic card, 4bytes which return from mifare_one_poll(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout	
int mifare_one_authenticate(int fd, uint8_t blockno, uint8_t keytype,const void *key, const void *uid);

// Function:
//		read one block from mifare classic card
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the block no be read out, 0~63 for S50, and 0~255 for S70
// Output:
//		blockdata	  ---- the block data read from mifare classic, 16bytes long
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_one_read_block(int fd, uint8_t blockno, void *blockdata);

// Function:
//		write data into one block of mifare classic card
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the block no be read out, 0~63 for S50, and 0~255 for S70
//		blockdata	  ---- the data wrote into mifare classic, 16bytes long
// Return Value:
//		0    ----- success.
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_one_write_block(int fd, uint8_t blockno, const void *blockdata);

// Function:
//		initialize a data block as value block, and set the initial value
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the block no be set, 0~63 for S50, and 0~255 for S70
//		value  ---- initial value
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
int mifare_one_set_value(int fd, uint8_t blockno, uint32_t value);

// Function:
//		increment value
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the value block no be incremented, 0~63 for S50, and 0~255 for S70
//		value  ----  value
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
//				ENOSYS  ---- the block is not a value block
int mifare_one_increment(int fd, uint8_t blockno, uint32_t value);

// Function:
//		decrement value
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the value block no be decremented, 0~63 for S50, and 0~255 for S70
//		value  ---- value
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
//				ENOSYS  ---- the block is not a value block
int mifare_one_decrement(int fd, uint8_t blockno, uint32_t value);

// Function:
//		value restore
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the value block no be restore, 0~63 for S50, and 0~255 for S70
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
//				ENOSYS  ---- the block is not a value block
int mifare_one_restore(int fd, uint8_t blockno);

// Function:
//		transfer value
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the value block no be transfered, 0~63 for S50, and 0~255 for S70
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
//				ENOSYS  ---- the block is not a value block
int mifare_one_transfer(int fd, uint8_t blockno);

// Function:
//		retrive value of value block
// Input:
//		fd      ----  device id, return from mif_open(...)
//		blockno --- the value block no be read, 0~63 for S50, and 0~255 for S70
// Output:
//		value   ---- the value of value block
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare classic
//				EACCESS	---- the card in field is not a mifare classic card
//				EPERM	 ---  authentication error
//				ETIME	---- timeout
//				EBADE	---- mifare classic card return invalid information
//				ENOSYS  ---- the block is not a value block
int mifare_one_get_value(int fd, uint8_t blockno, uint32_t *value);
#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* MIFONE_H_ */
