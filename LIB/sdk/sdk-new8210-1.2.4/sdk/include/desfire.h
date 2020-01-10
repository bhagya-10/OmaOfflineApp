/*
 * desfire.h
 *
 *  Created on: Oct 14, 2010
 *      Author: neal
 */

#ifndef DESFIRE_H_
#define DESFIRE_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

// Special error code used for mifare DESFire operation
//	1036   ---- No changes done to backup files,CommitTransaction/AbortTransaction not necessary
//	1038   ---- Insufficient NV-Memory to complete command
//	1052   ---- Command code not supported
//	1054   ---- CRC or MAC does not match data ,Padding bytes not valid
//	1088   ---- Invalid key number specified
//	1150   ---- Length of command string invalid
//	1181   ---- Current configuration / status does not allow the requested command
//	1182   ---- Value of the parameter(s) invalid
//	1184   ---- Requested AID not present on PICC
//	1185   ---- Unrecoverable error within application, application will be disabled
//	1198   ---- Current authentication status does not allow the requested command
//	1199   ---- Additional data frame is expected to be sent
//   1214   ---- Attempt to read/write data from/to beyond the file's/record's limits.
//			    or Atempt to exceed the limits of a value file
//	1217   ---- Unrecoverable error within PICC,PICC will be disabled
//	1226   ---- Previous Command was not fully completed. Not all Frames were requested or provided by the PICC
//	1229   ---- PICC was disabled by an unrecoverable error
//	1230   ---- Number of Applications limited to 28,no additional CreateApplication possible
//	1246   ---- Creation of file/application failed because file/application with same number already exists
//	1262   ---- Could not complete NV-write operation due to loss of power,interal backup/rollback mechanism activated
//	1264   ---- Specified file number does not exist
//	1265   ---- Unrecoverable error within file, file will be disabled.


// Function:
//		poll the mifare DESFire card in RF field,and return the card information
// Input:
//		fd      ----  device id, return from mif_open(...)
//		mode  ----  poll mode, value can be
//				   0  --- only poll the mifare classic cards which in IDLE state now
//				   1  --- poll the mifare classic cards which in IDLE state and HALT state now
// Output:
//		uid	  ---- the uid information of mifare classic, 7bytes long
//		atslen	---- the length of ATS data
//		ats		---- ATS data
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with ISO14443
//				EBADE	---- card return error response information
//				ENODATA ---  no mifare DESFire card found in RF field
//				ETIME	---- timeout during poll card
//				EMULTIHOP	---- more than one card in RF field
int desfire_poll(int fd,uint8_t mode,void *uid, uint8_t *atslen, void *ats);

// Function:
//		reset the mifare DESFire card, and put it into IDLE state
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code	
int desfire_deactive(int fd);

// Function:
//		confirms that both entities (PICC and PCD) can trust each other, DES/3DES algorithm
// Input:
//		fd      ----  device id, return from mif_open(...)
//		keyno ----  the key no used to authentication process
//		key    ----  key used for authentication, 16bytes need.
//					if the actual key is only 8bytes long, then should extended to 16bytes: key[0...7] || key[0...7].
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the configuration of the application (represented by its AID), an authentication has to be done
// 			to perform specific operations:
//			1.1 Gather information about the application
// 			1.2 Change the keys of the application
//			1.3 Create and delete files within the application
//			1.4 Change access rights
//			1.5 Access data files in the authenticated application
//		2. Depending on the security configuration of the PICC, the following commands may require an authentication
//			with the PICC master keys:
//			2.1 Gather information about the applications on the PICC
//			2.2 Change the PICC master key itself
//			2.3 Change the PICC key settings
//			2.4 Create a new application
//			2.5 Delete an existing application
//		3. The authentication state is invalidated by
// 			3.1 Selecting an application
// 			3.2 Changing the key which was used for reaching the currently valid authentication status
// 			3.3 A failed authentication
int desfire_authenticate(int fd,uint8_t keyno, const void *key);

// Function:
//		confirms that both entities (PICC and PCD) can trust each other, DES/3DES /3KDES algorithm
// Input:
//		fd      ----  device id, return from mif_open(...)
//		keyno ----  the key no used to authentication process
//		key    ----  key used for authentication, 24bytes need.
//					if the actual key is only 8bytes long, then should extended to 24bytes: key[0...7] || key[0...7] || key[0...7]
//					if the actual key is only 16bytes long, then should extended to 24bytes: key[0...7] || key[8...15] || key[0...7]
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the configuration of the application (represented by its AID), an authentication has to be done
// 			to perform specific operations:
//			1.1 Gather information about the application
// 			1.2 Change the keys of the application
//			1.3 Create and delete files within the application
//			1.4 Change access rights
//			1.5 Access data files in the authenticated application
//		2. Depending on the security configuration of the PICC, the following commands may require an authentication
//			with the PICC master keys:
//			2.1 Gather information about the applications on the PICC
//			2.2 Change the PICC master key itself
//			2.3 Change the PICC key settings
//			2.4 Create a new application
//			2.5 Delete an existing application
//		3. The authentication state is invalidated by
// 			3.1 Selecting an application
// 			3.2 Changing the key which was used for reaching the currently valid authentication status
// 			3.3 A failed authentication
int desfire_authenticate_iso(int fd,uint8_t keyno, const void *key);

// Function:
//		confirms that both entities (PICC and PCD) can trust each other, AES algorithm
// Input:
//		fd      ----  device id, return from mif_open(...)
//		keyno ----  the key no used to authentication process
//		key    ----  key used for authentication, 16bytes need.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the configuration of the application (represented by its AID), an authentication has to be done
// 			to perform specific operations:
//			1.1 Gather information about the application
// 			1.2 Change the keys of the application
//			1.3 Create and delete files within the application
//			1.4 Change access rights
//			1.5 Access data files in the authenticated application
//		2. Depending on the security configuration of the PICC, the following commands may require an authentication
//			with the PICC master keys:
//			2.1 Gather information about the applications on the PICC
//			2.2 Change the PICC master key itself
//			2.3 Change the PICC key settings
//			2.4 Create a new application
//			2.5 Delete an existing application
//		3. The authentication state is invalidated by
// 			3.1 Selecting an application
// 			3.2 Changing the key which was used for reaching the currently valid authentication status
// 			3.3 A failed authentication
int desfire_authenticate_aes(int fd,uint8_t keyno, const void *key);

// Function:
//		Changes the master key configuration settings depending on the currently selected AID.
//		if the current aid is "00 00 00", the change applies to the PICC key settings , otherwise it applies
//		to the application key settings of the currently selected application.
// Input:
//		fd      ----  device id, return from mif_open(...)
//		key_setting		------	new master key settings.
//						------  for PICC master key:
//								bit7~bit4	----- 0000 RFU
//								bit3   	----- whether a change of the PICC master key settings is allowed
//								bit2   	----- whether PICC master key authentication is needed before Create- / DeleteApplication
//								bit1   	----- whether PICC master key authentication is needed for application directory access
//								bit0	  	----- whether the PICC master key is changeable
//						------  for Application master key:
//								bit7~bit4	----- hold the Access Rights for changing application keys (ChangeKey command)
//										0x0   	---- Application master key authentication is necessary to change any key (default)
//										0x1~0x0D	---- Authentication with the specified key is necessary to change any key.
//										0x0E		---- Authentication with the key to be changed (same KeyNo) is necessary to change a key.
//										0x0F		---- All Keys (except application master key, see Bit0) within this application are frozen.
//								bit3		----- whether a change of the application master key settings is allowed
//								bit2		----- whether application master key authentication is needed before CreateFile / DeleteFile
//								bit1		----- whether application master key authentication is needed for file directory access
//								bit0		----- whether the application master key is changeable
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. a successful preceding authentication with the master key is required
//		2. This command only succeeds if the "configuration changeable" bit of the current key settings was not cleared before.
int desfire_change_key_settings(int fd,uint8_t key_setting);

// Function:
//		get configuration information on the PICC and application master key configuration settings,
//		and get maximum number of keys which can be stored within the selected application.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		key_setting		----- current master key setting, value as following:
//						------  for PICC master key:
//								bit7~bit4	----- 0000 RFU
//								bit3   	----- whether a change of the PICC master key settings is allowed
//								bit2   	----- whether PICC master key authentication is needed before Create- / DeleteApplication
//								bit1   	----- whether PICC master key authentication is needed for application directory access
//								bit0	  	----- whether the PICC master key is changeable
//						------  for Application master key:
//								bit7~bit4	----- hold the Access Rights for changing application keys (ChangeKey command)
//										0x0   	---- Application master key authentication is necessary to change any key (default)
//										0x1~0x0D	---- Authentication with the specified key is necessary to change any key.
//										0x0E		---- Authentication with the key to be changed (same KeyNo) is necessary to change a key.
//										0x0F		---- All Keys (except application master key, see Bit0) within this application are frozen.
//								bit3		----- whether a change of the application master key settings is allowed
//								bit2		----- whether application master key authentication is needed before CreateFile / DeleteFile
//								bit1		----- whether application master key authentication is needed for file directory access
//								bit0		----- whether the application master key is changeable
//		max_key_num			-----  maximum number of keys which can be stored within the selected application
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		Depending on the master key settings, a preceding authentication with the master key is required.
int desfire_get_key_settings(int fd,uint8_t *key_setting, uint8_t *max_key_num);

// Function:
//		change PICC master key 
// Input:
//		fd      ----  device id, return from mif_open(...)
//		type	  ---- PICC master key type,
//				0x00  --- 16bytes DES/3DES key
//				0x01  --- 24bytes 3KDES key
//				0x02  --- 16bytes AES key
//		key    ---- key information (16/24 bytes)
//		version ---- key version, only valid when type = 0x02.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//          1. according to PICC master key setting, a authentication with PICC master key is necessary
//		2. After a successful change of the key used to reach the current authentication status, this
//		  authentication is invalidated i.e. an authentication with the new key is necessary for subsequent operations. 
int desfire_change_picc_masterkey(int fd,uint8_t type, const void *key,uint8_t version);

// Function:
//		change application master key 
// Input:
//		fd      ----  device id, return from mif_open(...)
//		keysetting  ---- application master key setting
//		type	  ---- PICC master key type,
//				0x00  --- 16bytes DES/3DES key
//				0x01  --- 24bytes 3KDES key
//				0x02  --- 16bytes AES key
//		keyno  ---- the key to change
//		oldkey   ---- old key value
//		newkey ---- new key value
//		version ---- key version, only valid when type = 0x02.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//        	1. according to application master key setting, a authentication with specified key is necessary
//		2. After a successful change of the key used to reach the current authentication status, this
//		  authentication is invalidated i.e. an authentication with the new key is necessary for subsequent operations. 
int desfire_change_app_key(int fd,
								 uint8_t keysetting,
								 uint8_t type, 
								 uint8_t keyno, 
								 const void *oldkey,
								 const void *newkey, 
								 uint8_t version);
// Function:
//		read out the current key version of any key stored on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		keyno ----  key no
// Output:
//		version	---- key version of this key
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. This command can be issued without valid authentication.
int desfire_get_key_version(int fd,uint8_t keyno,uint8_t *version);

typedef struct
{
	uint8_t AID[3];		// Application Identifies	
	uint8_t MasterKeySetting; // Application master key setting
	uint8_t NumberOfKey; // Number of keys that can be stored within the application	
	uint8_t CryptoMode;	//  the key type of application, 0 - DES/3DES key, 1 - 3KDES key, 2 - AES key	
	uint8_t IsSupFID; // whether use File Identifies
	uint8_t IsSupISOFID; // whether use of 2 byte ISO/IEC 7816-4 File Identifies for files within the Application				
	uint8_t ISOFID[2];	// 2 byte ISO/IEC 7816-4 File Identifies for files	
	uint8_t LenOfDFName; // the length of "DF-name" used in 7816-4 mode, 0 - does not carries "DF-name"
	uint8_t DFName[16]; // "DF-name" used in 7816-4 mode in combination with the ISO SELECT command
	uint8_t RFU;
}desfire_appinfo;

// Function:
//		create new applications on the PICC.
// Input:
//		fd      ----  device id, return from mif_open(...)
//		info    ----  application information
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Application Identifier 0x00 00 00 is reserved as a reference to the PICC itself.
//		2. After application be created, All keys are initialised with a string consisting of 0x00 bytes 
//		3. It is strongly recommended to personalise the keys latest at card issuing using the command ChangeKey
int desfire_create_application(int fd,const desfire_appinfo *info);

// Function:
//		allows to permanently deactivate applications on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		aid    ----  application Identifies (3bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the PICC master key settings,an PICC master key authentication is required.
//		2. The AID allocation is removed, therefore it is possible to create a new application with the deleted
//		   application's AID. However, the deleted memory blocks can only be recovered by using the FormatPICC
//		   command which erases the full user memory of the PICC.
int desfire_delete_application(int fd,const void *aid);

// Function:
//		returns the Application IDentifiers of all active applications on a PICC.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		num	  ---- the number of all active applications on a PICC
//		aid	  ---- Application IDentifiers (3bytes each application)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the PICC master key settings a successful authentication with the PICC
//		   master key might be required to execute this command.
//		2. the buffer size of aid should be allocated at least 28*3bytes
int desfire_get_application_ids(int fd,uint8_t *num, void *aid);

typedef struct  
{
	uint8_t AID[3];	// application identifier
	uint8_t FID[2];	// 2 byte ISO/IEC 7816-4 File Identifies for files	
	uint8_t LenOfDFName;// the length of "DF-name" used in 7816-4 mode, 0 - does not carries "DF-name"
	uint8_t DFName[16];// "DF-name" used in 7816-4 mode in combination with the ISO SELECT command
	uint8_t RFU[2];	
}dfname_info;

// Function:
//		Returns the ISO/IEC 7816-4 DF-Names of all active applications on a PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		appnum  ---- the number of active applications
//		info		---- the DF-Name information of each active application
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. Depending on the PICC master key settings a successful authentication with the PICC
//		   master key might be required to execute this command.
//		2. the buffer size of aid should be allocated at least 28*sizeof(dfname_info) bytes
int desfire_get_dfnames(int fd,uint8_t *appnum,dfname_info *info);

// Function:
//		select one specific application for further access.
// Input:
//		fd      ----  device id, return from mif_open(...)
//		aid	  ---- Application Identifier (3bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. each SelectApplication command invalidates the current authentication status.
//		2. If this pAID is 0x00 00 00, the PICC level is selected and any further operations (typically commands
//		   like CreateApplication, DeleteApplication＃) are related to this level.
//		3. If an application with the specified AID is found in the application directory of the PICC, the subsequent
//		   commands interact with this application.
int desfire_select_application(int fd,const void *aid);

// Function:
//		This command releases the PICC user memory.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. The FormatPICC Command releases all allocated user memory on the PICC.
//		2. All applications are deleted and all files within those applications are deleted.
//		3. This command always requires a preceding authentication with the PICC master key.
//		4. The PICC master key and the PICC master key settings keep their currently set values, they are not
//		   influenced by this command.
int desfire_format_picc(int fd);

typedef struct  
{
	uint8_t HW_VendorID; 	// codes the vendor ID ( 0x04 for PHILIPS )
	uint8_t HW_Type;		// codes the type (here 0x01 )
	uint8_t HW_SubType;		// codes the subtype (here 0x01 )
	uint8_t HW_MajorVer;	// codes the major version number
	uint8_t HW_MinorVer;	// codes the minor version number
	uint8_t HW_Size;		// codes the storage size (here 0x1A = 8192 bytes )
	uint8_t HW_Protocol;	// codes the communication protocol type (here 0x05 meaning ISO 14443-2 and -3 )
	
	uint8_t SW_VendorID;	// codes the vendor ID ( here 0x04 for PHILIPS )
	uint8_t SW_Type;		// codes the type ( here 0x01 )	
	uint8_t SW_SubType;		// codes the subtype ( here 0x01 )	
	uint8_t SW_MajorVer;	// codes the major version		
	uint8_t SW_MinorVer;	// codes the minor version	
	uint8_t SW_Size;		// codes the storage size (here 0x1A = 8192 bytes )		
	uint8_t SW_Protocol;	// codes the communication protocol type (here 0x05 meaning ISO 14443-3 and -4 )	
	
	uint8_t UID[7];			// code the unique serial number
	uint8_t BatchNo[5];		// code the production batch number
	uint8_t CW_Prod;		// codes the calendar week of production
	uint8_t Year_Prod;		// codes the year of production
}desfire_version;

// Function:
//		returns manufacturing related data of the PICC.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		version		------ manufacturing related data (28bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
int desfire_get_version(int fd, desfire_version *version);

// Function:
//		set PICC configuration
// Input:
//		fd      ----  device id, return from mif_open(...)
//		option  ---- configuration type, value as following
//				0x00  --- *info data is the configuration byte
//				0x01  --- *info data is the default key version and default key
//						all applications will be personalized during creation with this default key and version instead of 0x00
//				0x02  --- *info data is the user defined ATS
//				0xxx  --- RFU
//		len   ---- the length of *info
//		info  ---- configuration information, according to option
//				--- if option = 0x00, the configuration byte showed as following:
//								bit0 = 0   Format card enabled
//								bit0 = 1  Format card disabled;can not be reset
//								bit1 = 0   Random ID disabled
//								bit1 = 1   Random ID enabled; can not be reset
//				--- if option = 0x01, the *info should be 24bytes key and 1byte default version
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remark:
//		PICC master key authentication on card level needs to be performed prior to this command.
int desfire_set_configuration(int fd,uint8_t option, uint8_t len,const void *info);

// Function:
//		Returns the available bytes on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		freesize   --- the available bytes on the PICC
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information		
int desfire_free_memory(int fd,uint32_t *freesize);

// Function:
//		return the uid of PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		uid	  ---- uid information of PICC (7bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remark:
//		1. An authentication with any key needs to be performed prior to this command
//		2. This command is only available when desfire_authenticate_iso(...) or desfire_authenticate_aes(...) called
int desfire_get_card_uid(int fd,void *uid);

// Function:
//		returns the File IDentifiers of all active files within the currently selected application.
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		num	  ----- return how many files within the currently selected application
//		fid	  ----- file IDs (up to 32bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. Depending on the application master key settings, a preceding authentication with the
//		   application master key might be required.
//		2. Each File ID is coded in one byte and is in the range from 0x00 to 0x1F.
int desfire_get_fids(int fd,uint8_t *num, void *fid);

// Function:
//		Returns the 2 byte ISO/IEC 7816-4 File IDentifiers of all active files within the currently selected application
// Input:
//		fd      ----  device id, return from mif_open(...)
// Output:
//		num	  ---- eturn how many files within the currently selected application
// 		fid	  ---- SO/IEC 7816-4 File IDentifiers (up to 64bytes)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. Depending on the application master key settings, a preceding authentication with the
//		   application master key might be required.
//		2. Each  ISO File ID is coded in two byte .
int desfire_get_iso_fids(int fd,uint8_t *num, void *fid);

typedef struct
{
	uint8_t file_type;  // DESfire file type:
						//  0x00  --- Standard Data Files
						//  0x01  --- Backup Data Files
						//  0x02  --- Value Files wih Backup
						//  0x03  --- Linear record Files with Backup
						//  0x04  --- Cyclic Record Files with Backup
	uint8_t com_sett;	// Communication settings:
						//  0x00 or 0x02  --- Plain communication
						//  0x01 		     --- Plain communication secured by MACing
						//  0x03             --- Fully enciphered communication

	// Access right capability, 0x0E means free access, and 0x0F means deny access.
	uint8_t read_access_right_keyno; // the reference number of the key which needs to be authenticated prior to Read Access and Read&Write Access
	uint8_t write_access_right_keyno;// the reference number of the key which needs to be authentication prior to Write Access and Read&Write Access
	uint8_t read_and_write_access_right_keyno;// the reference number of the key which needs to be authentication prior to  Read&Write Access
	uint8_t change_access_right_keyno;// the reference number of the key,which is necessary to be authenticated with in order to change the access rights for the file
									  // and to link each access right to key numbers
	
	uint32_t file_size;	//  the user file size in bytes, only available when file_type = 0x00 or file_type = 0x01

	// the following four parameters only available when file_type = 0x02
	uint32_t lower_limit;	// lower limit of the value file	
	uint32_t upper_limit;	// upper limit of the value file
	uint32_t limited_credit_value; // the current maximum" limited credit" value
	uint8_t  limited_credit_enabled; // if the LimitedCredit command is allowed for this file (0x00 for disabled, 0x01 for enabled)

	// the following three parameters  only available when file_type = 0x03 or file_type = 0x04
	uint32_t record_size;  // the size of one single record (as deefined at file creation)
	uint32_t max_number_of_records;  // the maximum number of records within the record file (as defined at file creation)
	uint32_t current_number_of_records; // the current number of records within the record file
}desfire_file;

// Function:
//		get information on the properties of a specific file.
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ----  the specific file no, value 0~0x1F allowed
// Output:
//		filesetting	----- file setting information,The information depends on the type of the file which is queried
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. This file number must be in the range between 0x00 and 0x1F.
//		2. Depending on the application master key settings, a preceding authentication with the application master key might be required.
//		3. After updating a value file's value but before issuing the CommitTransaction command, the GetFileSettings
//		   command will always retrieve the old, unchanged limit for the limited credit value.
int desfire_get_filesetting(int fd,uint8_t fileno, desfire_file *filesetting);

// Function:
//		changes the access parameters of an existing file
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ----  the specific file no, value 0~0x1F allowed
//          comm_set					----  communication setting
//  											0x00 or 0x02  --- Plain communication
// 	 										0x01 		--- Plain communication secured by MACing
//  											0x03              --- Fully enciphered communication
//		old_change_access_keyno		----  old Change Access Right Key No
//		new_read_access_keyno		----  new Read Access Right Key No
//		new_write_access_keyno		----  new Write Access Right Key No
//		new_read_write_access_keyno	----  new Read and Write Access Right Key No
//		new_change_access_keyno		----  new Change Access Right Key No
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. This change only succeeds if the current "Change Access Right" is different from "never", that is old_change_access_keyno != 0x0E
//		2. To guarantee that the ChangeFileSettings command is coming from the same party which did the preceding
//		   authentication, it is necessary to apply basically the same security mechanism as used with the ChangeKey command
int desfire_change_filesettings(int fd,
								uint8_t fileno, 
								uint8_t comm_set,
								uint8_t old_change_access_keyno,
								uint8_t new_read_access_keyno,
								uint8_t new_write_access_keyno,
								uint8_t new_read_write_access_keyno,
								uint8_t new_change_access_keyno);

typedef struct  
{	
	uint8_t FlgISOFileID; // whether ISO/IEC 7816-4 File IDentifiers enabled (0x00 - disabled, 0x01-enabled)
	uint8_t ISOFileID[2]; // 2bytes ISO/IEC 7816-4 File IDentifiers 
	uint8_t com_sett; // Communication settings:
					  //  0x00 or 0x02  --- Plain communication
					  //  0x01 		     --- Plain communication secured by MACing
					  //  0x03             --- Fully enciphered communication
	uint8_t read_access_right_keyno; // the reference number of the key which needs to be authenticated prior to Read Access and Read&Write Access
	uint8_t write_access_right_keyno;// the reference number of the key which needs to be authentication prior to Write Access and Read&Write Access
	uint8_t read_and_write_access_right_keyno;// the reference number of the key which needs to be authentication prior to  Read&Write Access
	uint8_t change_access_right_keyno;// the reference number of the key,which is necessary to be authenticated with in order to change the access rights for the file
									  // and to link each access right to key numbers
	uint32_t file_size; // the user file size in bytes
}desfire_std_backup_file_info;

// Function:
//		create files for the storage of plain unformatted user data within an existing application on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ---- the file number of the new file within the range 0x00 to 0x1F
//		fileinfo ---- file settings
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
int desfire_create_std_datafile(int fd,
									uint8_t fileno, 
									const desfire_std_backup_file_info *fileinfo);
// Function:
//		create files for the storage of plain unformatted user data within an existing application on the PICC, 
//		additionally supporting the feature of an integrated backup mechanism
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ---- the file number of the new file within the range 0x00 to 0x1F
//		fileinfo ---- file settings
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. Due to the mirror image a BackupDataFile always consumes DOUBLE the NV-memory on the PICC
//		   compared to a StdDataFile with the same specified FileSize.
//		2. Every Write command is done in a independent mirror image of this file. To validate a write access to this file
//		   type, it is necessary to confirm it with a CommitTransaction command. If no
//		   CommitTransaction command is send by the PCD, only the mirror image is changed, the original data
//		   remains unchanged and valid
int desfire_create_backup_datafile(int fd,
										uint8_t fileno, 
										const desfire_std_backup_file_info *fileinfo);

typedef struct  
{
	uint8_t com_sett; // Communication settings:
					  //  0x00 or 0x02  --- Plain communication
					  //  0x01 		     --- Plain communication secured by MACing
					  //  0x03             --- Fully enciphered communication
	uint8_t read_access_right_keyno; // the reference number of the key which needs to be authenticated prior to Read Access and Read&Write Access
	uint8_t write_access_right_keyno;// the reference number of the key which needs to be authentication prior to Write Access and Read&Write Access
	uint8_t read_and_write_access_right_keyno;// the reference number of the key which needs to be authentication prior to  Read&Write Access
	uint8_t change_access_right_keyno;// the reference number of the key,which is necessary to be authenticated with in order to change the access rights for the file
									  // and to link each access right to key numbers
	
	uint32_t lower_limit; // the lower limit value of this value file
	uint32_t upper_limit; // the upper limit value of this value file
	uint32_t init_value;  // the initial value of this value file
	uint8_t  limited_credit_enabled; // if the LimitedCredit command is allowed for this file (0x00 for disabled, 0x01 for enabled)
}desfire_value_file_info;

// Function:
//		create files for the storage and manipulation of 32bit signed
//		integer values within an existing application on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ---- the file number of the new file within the range 0x00 to 0x1F
//		fileinfo ---- file settings
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		ValueFiles feature always the integrated backup mechanism. Therefore every access changing the value
//		needs to be validated using the CommitTransaction command
int desfire_create_valuefile(int fd, 
								uint8_t fileno, 
								const desfire_value_file_info *fileinfo);

typedef struct  
{	
	uint8_t FlgISOFileID; // whether ISO/IEC 7816-4 File IDentifiers enabled (0x00 - disabled, 0x01-enabled)
	uint8_t ISOFileID[2]; // 2bytes ISO/IEC 7816-4 File IDentifiers 
	uint8_t com_sett; // Communication settings:
					  //  0x00 or 0x02  --- Plain communication
					  //  0x01 		     --- Plain communication secured by MACing
					  //  0x03             --- Fully enciphered communication
	uint8_t read_access_right_keyno; // the reference number of the key which needs to be authenticated prior to Read Access and Read&Write Access
	uint8_t write_access_right_keyno;// the reference number of the key which needs to be authentication prior to Write Access and Read&Write Access
	uint8_t read_and_write_access_right_keyno;// the reference number of the key which needs to be authentication prior to  Read&Write Access
	uint8_t change_access_right_keyno;// the reference number of the key,which is necessary to be authenticated with in order to change the access rights for the file
									  // and to link each access right to key numbers

	uint32_t record_size;	// the size of one single record 
	uint32_t max_number_of_records;	// the maximum number of records within the record file 
	uint32_t FlgRndAcc; // whether specifies Random write access option, (0x00 - not, 0x01 - yes)
	uint32_t rnd_acc; // whether allowed Random write access, (0x00 - disabled, 0x01 - enabled)					
}desfire_record_file_info;

// Function:
//		create files for multiple storage of structural data, for
//		example for loyalty programs, within an existing application on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ---- the file number of the new file within the range 0x00 to 0x1F
//		fileinfo ---- file settings
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. Once the file is filled completely with data records, further writing to the file is not possible unless it is cleared, see command
//		   ClearRecordFile.
//		2. Linear Record Files feature always the integrated backup mechanism. Therefore every access appending a
//		   record needs to be validated using the CommitTransaction command
int desfire_create_linear_recordfile(int fd,
										uint8_t fileno,
										const desfire_record_file_info *fileinfo);
// Function:
//		create files for multiple storage of structural data, for
//		example for loyalty programs, within an existing application on the PICC
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ----  the file number of the new file within the range 0x00 to 0x1F
//		fileinfo ---- file settings
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information	
// Remarks:
//		1. Once the file is filled completely with data records, further writing to the file is not possible unless it is cleared, see command
//		   ClearRecordFile.
//		2. Linear Record Files feature always the integrated backup mechanism. Therefore every access appending a
//		   record needs to be validated using the CommitTransaction command
int desfire_create_cyclic_recordfile(int fd,
										uint8_t fileno,
										const desfire_record_file_info *fileinfo);

// Function:
//		 permanently deactivates a file within the file directory of the currently selected application.
// Input:
//		fd      ----  device id, return from mif_open(...)
//		fileno ---- the file number within the file directory of the currently selected application.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. The operation of this command invalidates the file directory entry of the specified file which means that the
//		   file can't be accessed anymore.
//		2. Depending on the application master key settings, a preceding authentication with the application master key is required.
//		3. Allocated memory blocks associated with the deleted file are not set free. The FileNo of the deleted file can
//		   be re-used to create a new file within that application.
//		4. To release memory blocks for re-use, the whole PICC user NV-memory needs to be erased using the FormatPICC command.
int desfire_delete_file(int fd,uint8_t fileno);

// Function:
//		Read data from Standard Data Files or Backup Data Files
// Input:
//		fd      			----  device id, return from mif_open(...)
//		fileno			----  the file number
//		comm_set		----  Communication settings:
//  							0x00 or 0x02  --- Plain communication
//  							0x01 		--- Plain communication secured by MACing
//  							0x03             --- Fully enciphered communication
//		offset			----  the starting position for the read operation within the file
//		len				----  the number of data bytes want to be read
// Output:
//		outlen			--------  the actual length of data return from PICC
//		dataout			--------  the out data
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. This offset has to be in the range from 0 to file size -1.
//		2. If the len is coded as 0, the entire data file, starting from the position specified in the offset value, is read.
//		3. If Backup Data Files are read after writing to them, but before issuing the CommitTransaction command, the ReadData command 
//		   will always retrieve the old, unchanged data stored in the PICC. All
//		   data written to a Backup Data File is validated and externally "visible" for a ReadData command only after a
//		   CommitTransaction command.
//		4. The Read command requires a preceding authentication either with the key specified for "Read" or "Read&Write" access
int desfire_read_data(int fd,
						uint8_t fileno, 
						uint8_t comm_set, 	
						uint32_t offset, 
						uint32_t len, 
						uint32_t *outlen, 
						void *dataout);


// Function:
//		Read data from Standard Data Files or Backup Data Files
// Input:
//		fd      			----  device id, return from mif_open(...)
//		fileno			----  the file number
//		comm_set		----  Communication settings:
//  							0x00 or 0x02  --- Plain communication
//  							0x01 		--- Plain communication secured by MACing
//  							0x03             --- Fully enciphered communication
//		offset			----  the starting position for the read operation within the file
//		len				----  the number of data bytes want to be read
//		dataout_size	----  the buffer spcae of dataout, when call this API, should set 
//							  outlen = space of dataout.
// Output:
//		outlen			--------  the actual length of data return from PICC
//		dataout			--------  the out data
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. This offset has to be in the range from 0 to file size -1.
//		2. If the len is coded as 0, the entire data file, starting from the position specified in the offset value, is read.
//		3. If Backup Data Files are read after writing to them, but before issuing the CommitTransaction command, the ReadData command 
//		   will always retrieve the old, unchanged data stored in the PICC. All
//		   data written to a Backup Data File is validated and externally "visible" for a ReadData command only after a
//		   CommitTransaction command.
//		4. The Read command requires a preceding authentication either with the key specified for "Read" or "Read&Write" access

int desfire_read_data_safe(int fd,
						uint8_t fileno, 
						uint8_t comm_set, 							
						uint32_t offset, 
						uint32_t len, 
						uint32_t *outlen, 
						void *dataout, 
						uint32_t dataout_size);
// Function:
//		Write data to Standard Data Files and Backup Data Files.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		offset				----  the starting position for the write operation within the
//									file (= offset value). This parameter has to be in the range from 0 to file size -1
//		len					----  specifies the number of data bytes to be written. This parameter can be 
//									in the range from 0x01 to 0xFFFFFF
//		datain				----  Data to send
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. The Write command requires a preceding authentication either with the key specified for  "Write" or "Read&Write" access.
//		2. If the WriteData operation is performed on a Backup Data File, it is necessary to validate the written data
//		   with a CommitTransaction command. An AbortTransaction command will invalidate all changes.
int desfire_write_data(int fd, 
						uint8_t fileno, 
						uint8_t comm_set, 
						uint32_t offset, 
						uint32_t len, 
						const void *datain);

// Function:
//		Read the currently stored value from Value Files.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
// Output:
//		value			------- the retrive value
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. The GetValue command requires a preceding authentication with the key specified for Read, Write or Read&Write access
//		2. After updating a value file's value but before issuing the CommitTransaction command, the GetValue
//		   command will always retrieve the old, unchanged value which is still the valid one.
int desfire_get_value(int fd,uint8_t fileno,uint8_t comm_set,uint32_t *value);

// Function:
//		Increase a value stored in a Value File.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		value			------  the value which will be increased on the current value
//							   stored in the file. Only positive values are allowed for the Credit command.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. It is necessary to validate the updated value with a CommitTransaction command. 
//		   An AbortTransaction command will invalidate all changes
//		2. The value modifications of Credit, Debit and LimitedCredit commands are cumulated until a
// 		   CommitTransaction command is issued.
//		3. Credit commands do NEVER modify the Limited Credit Value of a Value file. However, if the Limited Credit
//		   Value needs to be set to 0, a LimitedCredit with value 0 can be used.
//		4. The Credit command requires a preceding authentication with the key specified for "Read&Write" access.
int desfire_credit(int fd,uint8_t fileno,uint8_t comm_set,uint32_t value);

// Function:
//		Decrease a value stored in a Value File.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		value			------  the value which will be subtracted from the current value
//							   stored in the file. Only positive values are allowed for the Debit command.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. It is necessary to validate the updated value with a CommitTransaction command. An
//		   AbortTransaction command will invalidate all changes
//		2. The value modifications of Credit, Debit and LimitedCredit commands are cumulated until a
//		   CommitTransaction command is issued.
//		3. The Debit command requires a preceding authentication with one of the keys specified for Read, Write or
//		   Read&Write access.
//		4. If the usage of the LimitedCredit feature is enabled, the new limit for a subsequent LimitedCredit command is
//		   set to the sum of Debit commands within one transaction before issuing a CommitTransaction command.
//           This assures that a LimitedCredit command can not re-book more values than a debiting transaction
//		   deducted before.
int desfire_debit(int fd,uint8_t fileno,uint8_t comm_set,uint32_t value);

// Function:
//		Allows a limited increase of a value stored in a Value File without having full
//		Read&Write permissions to the file. This feature can be enabled or disabled during value file creation.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		value				-------	the value which will be increased on the current value
//							   		stored in the file. Only positive values are allowed for the Credit command.
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. It is necessary to validate the updated value with a CommitTransaction command. An
//		   AbortTransaction command will invalidate all changes.
//		2. The value modifications of Credit, Debit and LimitedCredit commands are cumulated until a
//		   CommitTransaction command is issued.
//		3. The LimitedCredit command requires a preceding authentication with the key specified for "Write" or "Read
//		   &Write" access.
//		4. The value for LimitedCredit is limited to the sum of the Debit commands on this value file within the most
//		   recent transaction containing at least one Debit. After executing the LimitedCredit command the new limit is
//      	   set to 0 regardless of the amount which has been re-booked. Therefore the LimitedCredit command can only
//		   be used once after a Debit transaction.
int desfire_limited_credit(int fd,uint8_t fileno,uint8_t comm_set,uint32_t value);

// Function:
//		The WriteRecord command allows to write data to a record in a Cyclic or Linear Record File.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		offset	 			---- the offset within one single record, the value has to be in therange from 0 to record size - 1.
//		len	 				---- the length of data which is to be written to the record file, the value has to be in the range from 1 to record size - offset.
//		info					---- Record Information
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. The WriteRecord command appends one record at the end of the linear record file, it erases and overwrites
//		   the oldest record in case of a cyclic record file if it is already full. The entire new record is cleared before data
//		   is written to it.
//		2. If no CommitTransaction command  is sent after a WriteRecord command, the next
//		   WriteRecord command to the same file writes to the already created record. After sending a
//		   CommitTransaction command, a new WriteRecord command will create a new record in the record file. An
//		   AbortTransaction command will invalidate all changes
//		3. After issuing a ClearRecordFile command, but before a CommitTransaction / AbortTransaction command, a
//		   WriteRecord command to the same record file will fail.
//		4. The WriteRecord command requires a preceding authentication either with the key specified for "Write" or
//		  "Read&Write" access.
int desfire_write_record(int fd,
							uint8_t fileno, 
							uint8_t comm_set, 
							int offset, 
							int len, 
							const void *info);

// Function:
//		The ReadRecords command allows to read out a set of complete records from a Cyclic or Linear Record File.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		recordsize			---- the size of single record
//		first					---- the first record which is read out. In case	of 0x00 the latest record is read out. The value must be in the range from 0x00 
//								to number of existing records 每 1.
//		num					---- the number of records to be read from the PICC.
//								Records are always transmitted by the PICC in chronological order (= starting with the oldest,
//								which is number of records 每 1 before the one addressed by the given offset). If this parameter 
//								is set to 0x00 then all records, from the oldest record up to and including the newest record 
//								(given by the offset parameter) are read.
// Output:
//		outlen		----- the length of all record information
//		info			----- Record Information, shall be allocated at least (len * recordsize) bytes
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. In cyclic record files the maximum number of stored valid records is one less than the number of
//		   records specified in the CreateCyclicRecordFile command.
//		2. A ReadRecords command on an empty record file (directly after creation or after a committed clearance will result in an error.
//		3. The ReadRecords command requires a preceding authentication either with the key specified for "Read" or  "Read&Write" access.
int desfire_read_records(int fd,
							uint8_t fileno, 
							uint8_t comm_set,
							uint32_t recordsize,
							int first, 
							int num, 
							uint32_t *outlen, 
							void *info);

// Function:
//		The ReadRecords command allows to read out a set of complete records from a Cyclic or Linear Record File.
// Input:
//		fd      			----  device id, return from mif_open(...)
//		fileno				----  the file number
//		comm_set			----  Communication settings:
//  								0x00 or 0x02  --- Plain communication
//  								0x01 		--- Plain communication secured by MACing
//  								0x03             --- Fully enciphered communication
//		recordsize			---- the size of single record
//		first				---- the first record which is read out. In case	of 0x00 the latest record is read out. The value must be in the range from 0x00 
//								to number of existing records 每 1.
//		num					---- the number of records to be read from the PICC.
//								Records are always transmitted by the PICC in chronological order (= starting with the oldest,
//								which is number of records 每 1 before the one addressed by the given offset). If this parameter 
//								is set to 0x00 then all records, from the oldest record up to and including the newest record 
//								(given by the offset parameter) are read.
//		info_size			---- the buffer spcae of info, when call this API, should set 
//							  info_size = space of info.
// Output:
//		outlen			----- the length of all record information
//		info			----- Record Information, shall be allocated at least (len * recordsize) bytes
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. In cyclic record files the maximum number of stored valid records is one less than the number of
//		   records specified in the CreateCyclicRecordFile command.
//		2. A ReadRecords command on an empty record file (directly after creation or after a committed clearance will result in an error.
//		3. The ReadRecords command requires a preceding authentication either with the key specified for "Read" or  "Read&Write" access.

int desfire_read_records_safe(int fd,
							uint8_t fileno, 
							uint8_t comm_set,
							uint32_t recordsize,
							int first, 
							int num, 
							uint32_t *outlen, 
							void *info, 
							uint32_t info_size);

// Function:
//		The ClearRecordFile command allows to reset a Cyclic or Linear Record File to the empty state.
// Input:
//		fd      				----  device id, return from mif_open(...)
//		fileno				----  the file number
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		1. After executing the ClearRecordFile command but before CommitTransaction, all subsequent WriteRecord commands 
//		   will fail.
//		2. The ReadRecords command will return the old still valid records.
//		3. After the CommitTransaction command is issued, a ReadRecords command will fail, WriteRecord
//		   commands will be successful.
//		4. An AbortTransaction command (instead of CommitTransaction) will invalidate the clearance
int desfire_clear_recordfile(int fd,uint8_t fileno);

// Function:
//		The CommitTransaction command allows to validate all previous write access on Backup Data Files, Value
//		Files and Record Files within one application.
// Input:
//		fd   ----  device id, return from mif_open(...)
// Return Value:
//		0    ----- success
//		-1  ----- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
// Remarks:
//		The CommitTransaction is typically the last command of a transaction before the ISO 14443-4 Deselect
//		command or before proceeding with another application (SelectApplication command).
int desfire_commit_transaction(int fd);

// Function:
//		The AbortTransaction command allows to invalidate all previous write access on Backup Data Files, Value
//		Files and Record Files within one application.
//		This is useful to cancel a transaction without the need for re-authentication to the PICC, which would lead to
//		the same functionality.	
// Input:
//		fd   ----  device id, return from mif_open(...)
// Return Value:
//		0    ---- success
//		-1  ---- fail, please check "errno" for actual error code
//				EINVAL	---- invalid argument
//				EIO		---- hardware error
//				ECOMM	---- Parity Error or CRC error
//				EPROTO	---- the response from card not compatile with mifare DESFire
//				EACCESS	---- the card in field is not a mifare DESFire card
//				ETIME	---- timeout
//				EBADE	---- mifare DESFire card return invalid information
int desfire_abort_transaction(int fd);
#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* DESFIRE_H_ */
