/*
 * contactless.h
 *
 *  Created on: Oct 14, 2010
 *      Author: neal
 */

#ifndef ISO14443_H_
#define ISO14443_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

#if defined(__KERNEL__)
enum {
	MIF_TYPE_A		= 0x00,	// TypeA
	MIF_TYPE_B,        		// TypeB
	MIF_TYPE_C1,        	// FeliCa, 212kbits/s
	MIF_TYPE_C2,			// FeliCa, 424kbits/s
	
	MIF_TYPE_UNKNOWN,
};
#else
	#define MIF_TYPE_A				0x00        // TypeA
	#define MIF_TYPE_B				0x01        // TypeB
	#define MIF_TYPE_C1				0x02        // FeliCa, 212kbits/s
	#define MIF_TYPE_C2				0x03		// FeliCa, 424kbits/s
	#define MIF_TYPE_UNKNOWN		0x04
#endif

// Function:
//		Open the contactless card reader driver file, and return the device id
// Input:
//		filename  ---- driver file name, now is "/dev/nfc"
// Return Value:
//		>=0   ---- return the contactless card reader device id
//		-1      ---- fail to open the device, "errno" coded the actual error code
int mif_open(const char *filename);

// Function:
//		Close the contactless card reader device
// Input:
//		fd   ---- device id, return from mif_open(...)
// Return Value:
//		0    ---- success to close the device
//		-1  ---- fail to close the device,"errno" coded the actual error code
int mif_close(int fd);

struct mif_timeout{
	uint32_t WaitTime_PollTypeA; // the Frame Delay Time PCD to PICC during TypeA poll procedure (Unit:us, default: 100us)		
	uint32_t WaitTime_PollTypeB; // the Frame Delay Time PCD to PICC during TypeB poll procedure (Unit:etu, default: 500etu)		
	uint32_t WaitTime_Mifare;	 // the Frame Wait Time during mifare classic and ultralight card opertaion (Unit:etu, default: 10240etu)
	uint32_t WaitTime_RATS;		 // the Frame Wait Time during T=CL card activate (Unit:etu, default: 500etu)		
	uint32_t WaitTime_CardResponse;	// the maxium Frame Wait Time during waiting response from PICC (Unit:etu, default: 573440etu)
	uint32_t WaitTime_CompleteCmd;	// the maxium Wait Time for completing one data exchange between PCD and PICC (Unit:ms, default: 6000ms)
	uint32_t WaitTime_DelayBefore;	// the Frame Delay Time PICC to PCD (Unit:us, default: 500us)
	uint32_t WaitTime_IdleRF;		// the delay time while carrier off (Unit:ms, default: 6ms)			
};
int mif_set_timeout(const struct mif_timeout *ptime);
int mif_get_timeout(struct mif_timeout *ptime);

int iso14443_delay_etu(int fd,uint32_t numofetu);
int iso14443_reset_picc(int fd);
// Function:
//		Configure the contactless interface type
// Input:
//		fd     ----  device id, return from mif_open(...)
//		type  ---- contactless interface type, value can be 
//				  0x00  ----- ISO14443 TypeA
//				  0x01  ----- ISO14443 TypeB
//				  0x02  ----- Sony FeliCa, 212kbits/s
//				  0x03  ----- Sony FeliCa, 424kbits/s
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
int mif_select_carrier_type(int fd, int32_t type);

// Function:
//		Request command, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
// Output:
//		atqa  ---- ATQA info return from PICC, 2bytes
// Return Value:
//		0   ----- success
//		-1 ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout
//				EMULTIHOP	---- more than one card in RF field		
int iso14443_ReqA(int fd, void *atqa);

// Function:
//		Wake-Up command, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
// Output:
//		atqa  ---- ATQA info return from PICC, 2bytes
// Return Value:
//		0   ----- success
//		-1 ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout 
//				EMULTIHOP	---- more than one card in RF field	
int iso14443_WupA(int fd, void *atqa);

// Function:
//		Anticollision command, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
//		sel    ---- cascade level
//				 0x93    ---- first cascade level
//				 0x95    ---- second cascade level
//				 0x97    ---- third cascade level 
// Output:
//		uid  ---- uid information of current cascade level, 5bytes
// Return Value:
//		0   ----- success
//		-1 ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout
//				EMULTIHOP	---- more than one card in RF field	
int iso14443_Anticollision(int fd,uint8_t sel,void *uid);

// Function:
//		Select command, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
//		sel    ---- SELect code, Type A
//				 0x93    ---- first cascade level
//				 0x95    ---- second cascade level
//				 0x97    ---- third cascade level
//		uid    ---- Unique Identifier, Type A
// Output:
//		sak   ---- Select AcKnowledge, Type A
// Return Value:
//		0   ----- success
//		-1 ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout
int iso14443_Select(int fd,uint8_t sel, const void *uid, uint8_t *sak);

int iso14443_get_typeA_uid(int fd, uint8_t *uidlen, void *uid);
uint8_t iso14443_get_cardtype(void);

// Function:
//		Halt command, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
// Return Value:
//		0   ----- success
//		-1 ----- fail, please check "errno" for actual error code
//				EIO		---- hardware error
int iso14443_HaltA(int fd);

// Function:
//		Request for Answer To Select, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
//		cid    ----  Card Identifier, default is 0
//		fsdi   ----  Frame Size for PCD integer
//				0  ---- 16 bytes
//				1  ---- 24 bytes
//				2  ---- 32 bytes
//				3  ---- 40 bytes
//				4  ---- 48 bytes
//				5  ---- 64 bytes
//				6  ---- 96 bytes
//				7  ---- 128 bytes
//				>=8 ---- 256 bytes
// Output:
//		atslen    ---- the length of Answer To Select response
//		ats        ---- Answer To Select
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout		
int iso14443_RATS(int fd,uint8_t cid,uint8_t fsdi,uint8_t *atslen,void *ats);

// Function:
//		Protocol and Parameter Select, Type A
// Input:
//		fd     ----  device id, return from mif_open(...)
//		cid    ----  Card Identifier, the same as used in iso14443_rats(...)
//		br	  ----  the bit rate capability, bit8~bit5 code bit rate from PCD to PICC, and bit4~bit code bit rate from PICC to PCD 
//				   0  --- 106kbits/s
//				   1  --- 212kbits/s
//				   2  --- 424kbits/s
//				   3  --- 848kbits/s
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout
int iso14443_PPS(int fd,uint8_t cid,uint8_t br);

// Function:
//		Request command, Type B
// Input:
//		fd     ----  device id, return from mif_open(...)
//		afi    ----  Application Family Identifier, Type B. 0 for all applications
//		slotnum  ---- Number of slots,
//					0    ---- only one slot
//					1    ---- 2 slots
//					2    ---- 4 slots
//					3    ---- 8 slots
//					4    ---- 16 slots
// Output:
//		lenofatqb		---- the length of Answer To Request
//		atqb			---- Answer to Request,Type B
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout	
int iso14443_ReqB(int fd,uint8_t afi,uint8_t slotnum,uint8_t *lenofatqb,void *atqb);

// Function:
//		Wake-Up command, Type B
// Input:
//		fd     ----  device id, return from mif_open(...)
//		afi    ----  Application Family Identifier, Type B. 0 for all applications
//		slotnum  ---- Number of slots,
//					0    ---- only one slot
//					1    ---- 2 slots
//					2    ---- 4 slots
//					3    ---- 8 slots
//					4    ---- 16 slots
// Output:
//		lenofatqb		---- the length of Answer To Request
//		atqb			---- Answer to Request,Type B
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout	
int iso14443_WupB(int fd,uint8_t afi,uint8_t slotnum,uint8_t *lenofatqb,void *atqb);

// Function:
//		Slot Marker command, Type B
// Input:
//		fd     ----  device id, return from mif_open(...)
//		slot   ----  time slot, the value should be less than 16 according to slotnum parameter of ReqB or WupB
// Output:
//		lenofatqb		---- the length of Answer To Request
//		atqb			---- Answer to Request,Type B
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout
int iso14443_SlotMarker(int fd,uint8_t slot,uint8_t *lenofatqb,void *atqb);

// Function:
//		PICC selection command, Type B
// Input:
//		fd     ----  device id, return from mif_open(...)
//		cid     ----- card identifier
//		fsdi   ----  Frame Size for PCD integer
//				0  ---- 16 bytes
//				1  ---- 24 bytes
//				2  ---- 32 bytes
//				3  ---- 40 bytes
//				4  ---- 48 bytes
//				5  ---- 64 bytes
//				6  ---- 96 bytes
//				7  ---- 128 bytes
//				>=8 ---- 256 bytes
//		uid	  ---- Unique Identifier, Type B
//		br	  ----  the bit rate capability, bit8~bit5 code bit rate from PCD to PICC, and bit4~bit code bit rate from PICC to PCD 
//				   0  --- 106kbits/s
//				   1  --- 212kbits/s
//				   2  --- 424kbits/s
//				   3  --- 848kbits/s
// Input:
//		len	 ----- the length of Higher layer-INF of AttriB command,0 for none
//		info   ----- Higher layer-INF 
// Output:
//		len    ----- the length of Answer to an Attrib command
//		info   ----- Answer of to an ATTRIB Command
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout		
int iso14443_AttriB(int fd,uint8_t cid,uint8_t fsdi,const void *uid,uint8_t br,uint8_t *len,void *info);

// Function:
//		Halt Command, Type B
// Input:
//		fd     ----  device id, return from mif_open(...)
//		uid    ----  Unique Identifier, Type B
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ETIME	---- timeout		
int iso14443_HaltB(int fd,const void *uid);

// contactless card type definition
#define MIF_M1_S50          0x00    // mifare classic S50
#define MIF_M1_S70          0x01 	// mifare classic S70   
#define MIF_ULTRALIGHT      0x02    // mifare Ultralight
#define MIF_DESFIRE         0x03    // mifare DESFire D40 or EV1  
#define MIF_PRO             0x04  	// mifare ProX  
#define MIF_PRO_S50         0x05	// mifare ProX and embed mifare classic S50,compatible with T=CL protocol
#define MIF_PRO_S70         0x06	// mifare ProX and embed mifare classic S70,compatible with T=CL protocol
#define MIF_TYPEB           0x07	// ISO14443 TypeB, and not compatible with T=CL protocol
#define MIF_TYPEB_TCL       0x08    // ISO14443 TypeB, and compatible with T=CL protocol
#define MIF_FELICA          0x09    // Sony FeliCa
#define MIF_UNKNOWEN_TYPEA  0x0A    // ISO14443 TypeA, and not compatible with T=CL protocol
// Function:
//		poll the RF field for T=CL card, and return the card information
// Input:
//		fd      ----  device id, return from mif_open(...)
//		mode  ----  poll mode, value can be
//				   0  --- only poll the T=CL cards which in IDLE state now
//				   1  --- poll the T=CL cards which in IDLE state and HALT state now
// Output:
//		cardinfo_len		---- the length of T=CL card information
//		cardinfo			---- the T=CL card information. the cardinfo organized as following:
//							cardinfo[0] --- return the T=CL card type, refer to contactless card type definition
//							for TypeA, the following information is ATQA[2bytes] + UidLen(1byte) + Uid(UidLen bytes) + SAK(1byte)
//							for TypeB, the follwoing information is ATQB[12bytes]
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ENODATA ---  no T=CL card found in RF field
//				ETIME	---- timeout during poll card
//				EMULTIHOP	---- more than one card in RF field
int iso14443_tcl_poll(int fd,int mode,uint8_t *cardinfo_len,void *cardinfo);

// Function:
//		activate the T=CL card, the put them into active state
// Input:
//		fd      ----  device id, return from mif_open(...)
//		cid	  ----  the logical card no, usually is set as 0
//		br	  ----  the bit rate capability, bit8~bit5 code bit rate from PCD to PICC, and bit4~bit code bit rate from PICC to PCD 
//				   0  --- 106kbits/s
//				   1  --- 212kbits/s
//				   2  --- 424kbits/s
//				   3  --- 848kbits/s
//		fsdi	 ---- code the maxium frame length from PICC to PCD
//				0 ---- 16bytes
// 				1 ---- 24bytes
// 				2 ---- 32bytes
// 				3 ---- 40bytes
// 				4 ---- 48bytes
// 				5 ---- 64bytes
// 				6 ---- 96bytes
// 				7 ---- 128bytes
// 				8 ---- 256bytes
// 				other ---- 256bytes
// Output:
//		info_len	  --- the length of card information 
//		info		  --- card information.
//					For Type A, the info field is organized  as following:
//						Card Type(1byte) || ATQA(2bytes) || uid len(1byte) || uid (uidlen bytes)
//					For TypeB, the info field is organized as following:
//						Card Type(1byte) || ATQB(12/13bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout
int iso14443_tcl_active(int fd, uint8_t cid, uint8_t br,uint8_t fsdi,uint8_t *info_len, void *info);

// Function:
//		exchange T=CL data packet between PCD and PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		sendlen ---- the data packet length send to PICC
//		senddata	--- the data send to PICC
// Output:
//		recvlen	---- the data packet length return from PICC
//		recvdata	---- the data return from PICC
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int iso14443_tcl_exchange(int fd,uint32_t sendlen, const void* senddata,uint32_t *recvlen,void *recvdata);

// Function:
//		reset all card in RF field, and put them into IDLE state
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, plase check "errno" for actual error code		
int iso14443_tcl_deactive(int fd);

// Function:
//		exchange data between PCD and PICC, not organized in ISO14443 T=CL data packet
// Input:
//		fd      ----  device id, return from mif_open(...)
//		sendlen ---- the data packet length send to PICC
//		senddata	--- the data send to PICC
//		timeout	--- wait time (unit:etu)
// Output:
//		recvlen	---- the data packet length return from PICC
//		recvdata	---- the data return from PICC
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int iso14443_no_tcl_exchange(int fd,uint32_t sendlen, const void* senddata, uint32_t *recvlen,void *recvdata,uint32_t timeout);

// Function:
//		Check if more than one card in RF field
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ENODATA ---  no T=CL card found in RF field
//				EMULTIHOP	---- more than one card in RF field		
int iso14443_check_multi_card(int fd);

// Function:
//		exchange data between PCD and Sony FeliCa
// Input:
//		fd      ----  device id, return from mif_open(...)
//		sendlen ---- the data packet length send to PICC
//		senddata	--- the data send to PICC
//		timeout	--- wait time (unit:etu)
// Output:
//		recvlen	---- the data packet length return from PICC
//		recvdata	---- the data return from PICC
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int felica_exchange(int fd,uint32_t sendlen,const void *senddata,uint32_t *recvlen,void *recvdata,uint32_t timeout);


enum {
	NFC_OP_NULL			= 0x00,
		
	NFC_OP_POLL,			/* 轮询卡片 */
	NFC_OP_ANTICOLL, 		/* 防冲突处理 */
	NFC_OP_REMOVE,			/* 移除卡片, 应用程序获取不到此状态, emv_contactless_deactive_picc返回即表示移卡完成 */
	NFC_OP_EXCHANGE_APDU,	/* 数据交换 */
};


// Function:
//		check a card, and active it
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
// Notice:
//		If more than 2 cards in RF field, we can't active neither of it
//		this function is not a BLOCK function, it will quit immediately.
int emv_contactless_active_picc(int fd);

// Function:
//		deactive a card that actived before
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_deactive_picc(int fd);

// Function:
//		get current operation status.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		status	----  current operation status, et. NFC_OP_POLL NFC_OP_ANTICOLL ...
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_obtain_status(int fd, int *status);

// Function:
//		get last error code during the poll, anti-collision, active, exchange, deactive.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		error	----  last error code 
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_get_lasterror(int fd, int *error);

// Function:
//		exchange data between PCD and PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		sendlen ----  the length of data (in bytes) to send
//		senddata----  the data to be send to PICC
// Output:
//		recvlen ----  the length of data (in bytes) that received
//		recvdata----  the data that received from PICC
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_exchange_apdu(int fd, uint32_t sendlen, const void *senddata,
									uint32_t *recvlen, void *recvdata);

// Function:
//		send data from PCD to PICC nonblock, after sending return immediately 
// Input:
//		fd      ----  device id, return from mif_open(...)
//		sendlen ----  the length of data (in bytes) to send
//		senddata----  the data to be send to PICC
// 
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_exchange_apdu_nonblock_send(int fd, uint32_t sendlen, const void *senddata);

// Function:
//		get response data from PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		error	----  return the error during exchange, such as ETIME, EPROTO etc.
//					  if error not equal 0, that means error occur, and exchange stopped, try send APDU again or deactive.
//		status  ----  show current operation state:NFC_OP_POLL, NFC_OP_ANTICOLL, NFC_OP_ANTICOLL, NFC_OP_REMOVE.
//		finish  ----  1:exchange finish, then get response data from parameter recvlen and recvdata.
//					  0:not finish, wait until finish. If error occur(the first parameter error != 0), no need to wait further.
//		recvlen ----  When finish == 1, the length of data (in bytes) that received, 
//		recvdata----  When finish == 1, the data that received from PICC
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_exchange_apdu_nonblock_response(int fd, int *error, int *status, int *finish, uint32_t *recvlen, void *recvdata);

// Function:
//		abort current exchange, only if using API "emv_contactless_exchange_apdu_nonblock_send" to send data
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				EACCES   ---  The card in RF field is not a T=CL card
//				ETIME	---- timeout during poll card
int emv_contactless_exchange_apdu_nonblock_abort(int fd);

/**
 * ext_contactless_online_check() - check external contactless is online or not
 *
 * filename: USB - /dev/extnfc, UART - /dev/ttyS3
 *
 * times: ms
 *
 */
int ext_contactless_online_check(const char *filename, int times);


int compat_InATR(int fd, uint32_t *RespLen, void *ATRInfo);
int compat_InPSL(int fd, uint8_t BRit, uint8_t BRti);
int compat_InListPassiveTarget(int fd, uint8_t Mode, int InitLen, const void *InitInfo, int *CardInfoLen, void *pCardInfo);
int compat_InDataExchange(int fd, int SendLen, const void *SendData, uint32_t *RecvLen, void *RecvData);
void compat_InRelease(int fd);
int compat_mifCheckOnlyOneCard(int fd, uint8_t Mode);

#define MIF_LED_1				0x01   
#define MIF_LED_2          		0x02    
#define MIF_LED_3            	0x04    
#define MIF_LED_4           	0x08   

#define MIF_LED_OFF          	 0x00    // 关闭
#define MIF_LED_ON           	 0x01    // 开启
// Function:
//		control nfc leds
// Input:
//		fd      ----  device id, return from mif_open(...)
//		LEDNo   ----  the led index ,eg : MIF_LED_1 | MIF_LED_2 | MIF_LED_3
//		Mode    ----  control leds status:0 - OFF ,1 - ON
// Output:
//		NULL
// Return Value:
//		0   ----- success
//		-1  ----- fail, please check "errno" for actual error code
//eg: nfc_led_control(fd,MIF_LED_1 | MIF_LED_2 | MIF_LED_3, MIF_LED_ON);// turn on MIF_LED_1 , MIF_LED_2 and MIF_LED_3
//	  nfc_led_control(fd,MIF_LED_2 | MIF_LED_4, MIF_LED_OFF);// turn off MIF_LED_2 and MIF_LED_4
int nfc_led_control(int fd,uint8_t LEDNo, uint8_t Mode);
void ext_contactless_beep(int fd, unsigned int size);



#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* CONTACTLESS_H_ */
