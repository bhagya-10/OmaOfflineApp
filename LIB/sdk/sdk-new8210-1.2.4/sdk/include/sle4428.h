/*
 * sle4428.h
 *
 *  Created on: Oct 15, 2010
 *      Author: neal
 */

#ifndef SLE4428_H_
#define SLE4428_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

#define SLE4428_MAX_ADDR		0x03FF /* SLE4428 MAX ADDR, can ready data */
#define SLE4428_MAX_DATA_ADDR	0x03FC /* SLE4428 MAX DATA ADDR, can write data, write protect */

// Function:
//		power up and reset the sle4428 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			ENXIO	----- no card in slot
//			EPROTO	----- not sle4428 card
int sle4428_open(int fd);

// Function:
//		power down sle4428 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
int sle4428_close(int fd);

// Function:
//		read data from sle4428 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		start_addr    ---- start address
//		len		     ---- length want to be read out
// Output:
//		data		     ---- data from sle4428 card
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			EINVAL	---- invalid argument
//			ENXIO	---- no card in slot
int sle4428_read(int fd,int start_addr,int len,void *data);

// Function:
//		get the error count of user password authentication
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Output:
//		count   ----- error count
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			EINVAL	---- invalid argument
//			ENXIO	---- no card in slot
int sle4428_read_error_count (int fd,uint8_t *count);

// Function:
//		user password authentication
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		pwd  ----- user password, 2bytes
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			 EINVAL	---- invalid argument
//			 ENXIO   ---- no card in slot
//			 EPROTO  ---- autherticate error
//			 ETIME	 ---- time out
int sle4428_verify_pwd(int fd,const void *pwd);

// Function:
//		change user password
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		new_pwd  ----- new user password, 2bytes
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			 EINVAL	---- invalid argument
//			 ENXIO   ---- no card in slot
//			 EPROTO  ---- autherticate error
//			 ETIME	 ---- time out
int sle4428_change_key(int fd,const void *new_pwd);

// Function:
//		lock sle4428 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			 ENXIO   ---- no card in slot
//			 EPROTO  ---- lock card error
//			 ETIME	 ---- time out
int sle4428_lock(int fd);

// Function:
//		check the protect bit of given address
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		addr  ----- the given address
// Output:
//		status   ---- the state of protect bit, 0 --- with protect, 1 --- without protect
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL   ---- invalid argument
//				ENXIO	---- no card in slot
int sle4428_read_status (int fd,int addr,uint8_t *status);

// Function:
//		write data into sle4428 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		start_addr   ---- start address
//		len	 ---- the length want to written into sle4428 card
//		data  ---- data to be written
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL	----- invalid argument
//				ENXIO	----- no card int slot
//				ETIME	----- time out
//				EACCES	----- the given address with protect, can can not be updated
int sle4428_write(int fd,int start_addr,int len,const void *data);

// Function:
//		write data into sle4428 card with protect
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		start_addr   ---- start address
//		len	 ---- the length want to written into sle4428 card
//		data  ---- data to be written
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL	----- invalid argument
//				ENXIO	----- no card int slot
//				ETIME	----- time out
//				EACCES	----- the given address with protect, can can not be updated
int sle4428_write_protect(int fd,int start_addr,int len,const void *data);

// Function:
//		write protect bit with data comparison (verification)
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		addr  ----- given address
//		data  ----- data for comparison
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			ENXIO	---- no card in slot
//			EPROTO	---- write protect bit error
//			ETIME	---- time out
//			EACCES	---- the given address with protect, can can not be updated
int sle4428_check_data (int fd,int addr,uint8_t data);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* SLE4428_H_ */
