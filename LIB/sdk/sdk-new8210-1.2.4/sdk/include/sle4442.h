/*
 * sle4442.h
 *
 *  Created on: Oct 15, 2010
 *      Author: neal
 */

#ifndef SLE4442_H_
#define SLE4442_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

#define SLE4442_MAX_ADDR	0xFF /* SLE4442 MAX ADDR */
#define	SLE4442_PROTMAXADDR	0x1F /* Protected MAX ADDR */

// Function:
//		power up and reset the sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			ENXIO	----- no card in slot
//			EPROTO	----- not sle4442 card
int sle4442_open(int fd);

// Function:
//		power down sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
int sle4442_close(int fd);

// Function:
//		verify the user password of sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		pwd  ----- password, 3bytes
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			EINVAL	---- invalid argument
//			EPROTO	---- authentication fail
//			ETIME	---- time out
int sle4442_verify(int fd,const void *pwd);

// Function:
//		read error count of password verification
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Output:
//		count ----- error count
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			EINVAL	---- invalid argument			
int sle4442_read_error_count(int fd,uint8_t *count);

// Function:
//		change the user password of sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		new_pwd  ----- new password, 3bytes
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EPROTO	---- fail to change user password
//				ETIME 	---- time out
int sle4442_change_key(int fd,const void *new_pwd);

// Function:
//		lock the sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			ENXIO	---- no card in slot
//			ETIME	---- time out
//			EPROTO	---- fail to lock sle4442 card
int sle4442_lock(int fd);

// Function:
//		read data from sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		start_addr  ---- start address
//		len	 ----- the length want to be read out
// Output:
//		data    ----- data fom sle4442 card
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				ENXIO	---- no card in slot
int sle4442_read(int fd,int start_addr,int len,void *data);

// Function:
//		write data into sle4442 card
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		start_addr   ---- start address
//		len	 ----- the length want to be written into
// 		data  ----- data to be written into
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//				EINVAL	----- invalid argument
//				ENXIO	----- no card in slot
//				ETIME	----- time out
int sle4442_write(int fd,int start_addr,int len,const void *data);

// Function:
//		read protection memory status
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		addr	 ----- the address of protect memory
// Output:
//		status   ---- the status of the given address, 1 --- with protect, 0 ---- without protect
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			EINVAL	---- invalid argument
//			ENXIO	---- no card in slot
int sle4442_read_status(int fd,int addr,uint8_t *status);

// Function:
//		compare verification data
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		addr  ----- address for comparison
//		data  ----- data for comparison
// Return Value:
//		0      ----- success
//		-1    ----- please check "errno" for actual error code
//			ENXIO	---- no card in slot
//			ETIME	---- time out
//			EACCES	---- the address with protect
//			EPROTO  ---- fail to write protect bit
int sle4442_check_data (int fd,int addr,uint8_t data);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* SLE4442_H_ */
