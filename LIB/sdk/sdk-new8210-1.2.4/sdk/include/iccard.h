/*
 * iccard.h
 *
 *  Created on: Oct 15, 2010
 *      Author: neal
 */

#ifndef ICCARD_H_
#define ICCARD_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

// VCC mode definition
#define CVCC_1_8_VOLT   	1
#define CVCC_3_VOLT      	2
#define CVCC_5_VOLT      	3

// operation mode definition
#define EMV_MODE	0
#define ISO_MODE   	1
#define ISO_19200   2
#define ISO_38400   3

// Function:
//		open ic card reader device
// Input:
//	    	filename     ------ card slot name
// Return Value:
//		>= 0    device id, used by following reader api function
//		-1        failed, please check "errno" for actual error code
int iccard_open_cardslot(const char *filename);

// Function:
//		close ic card reader device
// Input:
//		fd      ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0    ---- success
//		-1  ---- failed, please check "errno" for actual error code
int iccard_close_cardslot(int fd);

// Function:
//		check if card existed in slot
// Input:
//		fd      ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0       ----- card in slot
//          -1     ----- failed, please check "errno" for actual error code
//			ENXIO	----- card not in slot
int iccard_check_card_in_slot(int fd);

// Function:
//		reset cpucard and get the atr information
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		vcc    ----- card operation vcc value:
//				CVCC_1_8_VOLT     ----- 1.8v voltage
//				CVCC_3_VOLT		 ----- 3v voltage
//				CVCC_5_VOLT		 ----- 5v voltage
//		mode  ----- card operation mode:
//				ISO_MODE		----- iso7816 mode
//				EMV_MODE		----- EMV specification mode
//				ISO_19200		----- higher rate iso7816 mode, 19200bps
//				ISO_38400		----- higher rate iso7816 mode, 38400bps
// Output:
//		len		----- length of atr information
//		atr		----- atr information
// Return Value:
//		0	----- success
//		-1    ----- failed, please check "errno" for actual error code
//			ENXIO	----- card  not in slot
//			EINVAL	----- invalid argument
//			ETIME	----- timeout
//			ECOMM	----- parity error or edc error
//			EPROTO	----- invalid information from iccard
int iccard_power_up(int fd, int vcc, int mode, int *len, void *atr);

// Function:
//		exchange apdu packet between reader and iccard
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
//		sendlen ---- length of c-apdu information
//		senddata ---- c-apdu information
// Output:
//		recvlen   ----- length of r-apdu information from iccard
//		recvdata ----- r-apdu information from iccard
// Return Value:
//		0	----- success
//		-1    ----- failed, please check "errno" for actual error code
//			ENXIO	----- card  not in slot
//			EINVAL	----- invalid argument
//			ETIME	----- timeout
//			ECOMM	----- parity error or edc error
//			EPROTO	----- invalid information from iccard
//			EACCES	----- card need be reset first
int iccard_exchange_apdu(int fd, uint32_t sendlen, const void *senddata, uint32_t *recvlen, void *recvdata);

// Function:
//		power down iccard
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		0     ----- success
//		-1   ----- failed, please check "errno" for actual error code
int iccard_power_down(int fd);

// Function:
//		obtain the number of slot, this function used by ped
// Input:
//		fd     ----- device id, return from iccard_open_cardslot(...)
// Return Value:
//		>=0  ----- slot number
//		-1   ----- failed, please check "errno" for actual error code
int iccard_get_slotnum(int fd);


#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* ICCARD_H_ */
