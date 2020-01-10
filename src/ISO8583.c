
#include "global.h"


/********************** Internal macros declaration ************************/
/********************* Internal functions declaration **********************/
static int PackElement(FIELD_ATTR *pAttr, uint8_t *pusIn, uint8_t *pusOut,
                        uint *puiOutLen);
static int UnPackElement(FIELD_ATTR *pAttr, uint8_t *pusIn, uint8_t *pusOut,
                        uint *puiInLen);

/********************** Internal variables declaration *********************/
// 说明：FIELD_ATTR结构定义请参见ISO8583处理模块定义
FIELD_ATTR glEdcDataDef[] =
{
	{Attr_n, Attr_fix,  LEN_MSG_CODE},		// message code(MTI)
	{Attr_b, Attr_fix,  LEN_BITMAP},		// 1 - bitmap
	{Attr_n, Attr_var1, LEN_PAN},			// 2 - PAN
	{Attr_n, Attr_fix,  LEN_PROC_CODE},		// 3 - process code
	{Attr_n, Attr_fix,  LEN_TRAN_AMT},		// 4 - txn amount
	{Attr_n, Attr_fix,  LEN_FRN_AMT},		// 5 - Foreign Amt(DCC)
	{Attr_UnUsed, Attr_fix, 0},				// 6 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 7 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 8 - not used
	{Attr_a, Attr_fix, LEN_DCC_RATE},		// 9 - DCC rate
	{Attr_UnUsed, Attr_fix, 0},				// 10 - not used
	{Attr_n, Attr_fix, LEN_STAN},			// 11 - STAN
	{Attr_n, Attr_fix, LEN_LOCAL_TIME},		// 12 - local int
	{Attr_n, Attr_fix, LEN_LOCAL_DATE},		// 13 - local date
	{Attr_n, Attr_fix, LEN_EXP_DATE},		// 14 - expire
	{Attr_UnUsed, Attr_fix, 0},				// 15 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 16 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 17 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 18 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 19 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 20 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 21 - not used
	{Attr_n, Attr_fix, LEN_ENTRY_MODE},		// 22 - entry modes
	{Attr_n, Attr_fix, LEN_PAN_SEQ_NO},		// 23 - PAN Seq #
	{Attr_n, Attr_fix, LEN_NII},			// 24 - NII
	{Attr_n, Attr_fix, LEN_COND_CODE},		// 25 - condition code
	{Attr_UnUsed, Attr_fix, 0},				// 26 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 27 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 28 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 29 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 30 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 31 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 32 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 33 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 34 - not used
	{Attr_z, Attr_var1, LEN_TRACK2},		// 35 - track 2
	{Attr_z, Attr_var2, LEN_TRACK3},		// 36 - track 3
	{Attr_a, Attr_fix,  LEN_RRN},			// 37 - RRN
	{Attr_a, Attr_fix,  LEN_AUTH_CODE},		// 38 - auth. code
	{Attr_a, Attr_fix,  LEN_RSP_CODE},		// 39 - response code
	{Attr_UnUsed, Attr_fix, 0},				// 40 - not uesed
	{Attr_a, Attr_fix, LEN_TERM_ID},		// 41 - TID
	{Attr_a, Attr_fix, LEN_MERCHANT_ID},	// 42 - MID
	{Attr_UnUsed, Attr_fix, 0},				// 43 - not used
	{Attr_a, Attr_var1, LEN_ADDL_RSP},		// 44 - Add'l rsp data
	{Attr_a, Attr_var1, LEN_TRACK1},		// 45 - track 1
	{Attr_UnUsed, Attr_fix, 0},				// 46 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 47 - not used
	{Attr_n, Attr_fix,  LEN_FIELD48},		// 48 - add'l req data
	{Attr_a, Attr_fix,  LEN_FRN_CURCY_CODE},// 49 - FRN Curcy code
	{Attr_a, Attr_fix,  LEN_CURCY_CODE},	// 50 - Currency code
	{Attr_UnUsed, Attr_fix, 0},				// 51 - not used
	{Attr_b, Attr_fix, LEN_PIN_DATA},		// 52 - PIN data
	{Attr_UnUsed, Attr_fix, 0},				// 53 - not used
	{Attr_a, Attr_var2, LEN_EXT_AMOUNT},	// 54 - Extra Amount
	{Attr_b, Attr_var2, LEN_ICC_DATA},		// 55 - ICC data
	{Attr_b, Attr_var2, LEN_ICC_DATA2},		// 56 - ICC data 2(HK)
	{Attr_b, Attr_var2, LEN_FIELD57},		// 57 - encrypted data
	{Attr_UnUsed, Attr_fix, 0},				// 58 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 59 - not used
	{Attr_b, Attr_var2, LEN_FIELD60},		// 60 - Private used
	{Attr_a, Attr_var2, LEN_FIELD61},		// 61 - POS SN/desc code
	{Attr_b, Attr_var2, LEN_FIELD62},		// 62 - ROC/SOC
	{Attr_b, Attr_var2, LEN_FIELD63},		// 63 - Private used
	{Attr_b, Attr_fix,  LEN_MAC},			// 64 - MAC(not used)
	{Attr_Over,   Attr_fix, 0}
};

// TMS消息定义
FIELD_ATTR glTMSDataDef[] =
{
	{Attr_n, Attr_fix,  LEN_MSG_CODE},		// message code(MTI)
	{Attr_b, Attr_fix,  LEN_BITMAP},		// 1 - bitmap
	{Attr_UnUsed, Attr_fix, 0},				// 2 - PAN
	{Attr_n, Attr_fix,  LEN_PROC_CODE},		// 3 - process code
	{Attr_UnUsed, Attr_fix, 0},				// 4 - txn amount
	{Attr_UnUsed, Attr_fix, 0},				// 5 - Foreign Amt(DCC)
	{Attr_UnUsed, Attr_fix, 0},				// 6 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 7 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 8 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 9 - DCC rate
	{Attr_UnUsed, Attr_fix, 0},				// 10 - not used
	{Attr_n, Attr_fix, LEN_STAN},			// 11 - STAN
	{Attr_n, Attr_fix, LEN_LOCAL_TIME},		// 12 - local int
	{Attr_n, Attr_fix, LEN_LOCAL_DATE},		// 13 - local date
	{Attr_UnUsed, Attr_fix, 0},				// 14 - expire
	{Attr_UnUsed, Attr_fix, 0},				// 15 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 16 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 17 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 18 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 19 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 20 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 21 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 22 - entry modes
	{Attr_UnUsed, Attr_fix, 0},				// 23 - PAN Seq #
	{Attr_n, Attr_fix, LEN_NII},			// 24 - NII
	{Attr_UnUsed, Attr_fix, 0},				// 25 - condition code
	{Attr_UnUsed, Attr_fix, 0},				// 26 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 27 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 28 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 29 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 30 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 31 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 32 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 33 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 34 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 35 - track 2
	{Attr_UnUsed, Attr_fix, 0},				// 36 - track 3
	{Attr_UnUsed, Attr_fix, 0},				// 37 - RRN
	{Attr_UnUsed, Attr_fix, 0},				// 38 - auth. code
	{Attr_a, Attr_fix,  LEN_RSP_CODE},		// 39 - response code
	{Attr_UnUsed, Attr_fix, 0},				// 40 - not uesed
	{Attr_a, Attr_fix, LEN_TERM_ID},		// 41 - TID
	{Attr_a, Attr_fix, LEN_MERCHANT_ID},	// 42 - MID
	{Attr_UnUsed, Attr_fix, 0},				// 43 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 44 - Add'l rsp data
	{Attr_UnUsed, Attr_fix, 0},				// 45 - track 1
	{Attr_UnUsed, Attr_fix, 0},				// 46 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 47 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 48 - add'l req data
	{Attr_UnUsed, Attr_fix, 0},				// 49 - FRN Curcy code
	{Attr_UnUsed, Attr_fix, 0},				// 50 - Currency code
	{Attr_UnUsed, Attr_fix, 0},				// 51 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 52 - PIN data
	{Attr_UnUsed, Attr_fix, 0},				// 53 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 54 - Extra Amount
	{Attr_UnUsed, Attr_fix, 0},				// 55 - ICC data
	{Attr_UnUsed, Attr_fix, 0},				// 56 - ICC data 2(HK)
	{Attr_UnUsed, Attr_fix, 0},				// 57 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 58 - not used
	{Attr_UnUsed, Attr_fix, 0},				// 59 - not used
	{Attr_b, Attr_var2, LEN_TMSFIELD60},	// 60 - Private used
	{Attr_a, Attr_var2, LEN_FIELD61},		// 61 - POS SN/desc code
	{Attr_UnUsed, Attr_fix, 0},				// 62 - ROC/SOC
	{Attr_UnUsed, Attr_fix, 0},				// 63 - Private used
	{Attr_UnUsed, Attr_fix, 0},				// 64 - MAC(not used)
	{Attr_Over,   Attr_fix, 0}
};

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/


// change 8583 struct to 8583 packet
int PubPack8583(FIELD_ATTR *pDataAttr, void *pSt8583, char *pusOut, uint *puiOutLen)
{
	int     iRet;
	uint    i, iMsgLen, iDataLen, iFieldLen, uiBitmap;
	uint8_t   *pusI, *pusO, *pusB;
	
	pusI        = (uint8_t *)pSt8583;
	pusO        = (uint8_t *)pusOut;
	iMsgLen     = 0;
	iDataLen    = 0;
	*puiOutLen  = 0;
	
	iRet = PackElement(pDataAttr, pusI, pusO, (uint *)&iFieldLen);
	if( iRet<=0 ){
		return -1;
	}
	pusI    += (pDataAttr->uiLength + 2);
	pusO    += iFieldLen;
	iMsgLen += iFieldLen;
	
	/*** generate data and bitmap of 8583 packet ***/
	uiBitmap =  (pDataAttr+1)->uiLength;
	pusI     += (uiBitmap*2);
	pusB     =  pusO;
	
	pusO     += (uiBitmap*2);
	iDataLen =  uiBitmap*2;
	memset(pusB, 0, uiBitmap*2);
	
	for(i=1; i<uiBitmap*2*8; i++){
		if( (pDataAttr+i+1)->eElementAttr==Attr_Over ){
			break;
		}
		if( (pDataAttr+i+1)->eElementAttr==Attr_UnUsed ){
			continue;
		}
		iRet = PackElement(pDataAttr+i+1, pusI, pusO, (uint *)&iFieldLen);
		if( iRet<0 ){
			return ((-1)*(1000+(i+1)));
		}
		if( iRet>0 ){
			pusO     += iFieldLen;
			iDataLen += iFieldLen;
			*(pusB+(i/8)) |= (0x80>>(i%8));
		}
		pusI+= ((pDataAttr+i+1)->uiLength + 2);
	}
	if( i%((pDataAttr+1)->uiLength*8) ){
		return ((-1)*(2000+(i+1)));
	}
	
	/* process bitmap */
	for(i=uiBitmap*2-1; i>=0; i--){
		if(*(pusB+i))   break;
	}
	if( i>=8 ){
		*pusB |= 0x80;
	}else{
		memmove(pusB+uiBitmap, pusB+uiBitmap*2, iDataLen-uiBitmap*2);
		iDataLen -= uiBitmap;
	}
	
	*puiOutLen = iMsgLen+iDataLen;
	
	return 0;
}


// change 8583 packet to 8583 struct
int PubUnPack8583(FIELD_ATTR *pDataAttr, char *pusIn, uint uiInLen, void *pSt8583)
{
	int     iRet;
	uint    i, iLen, iFieldLen, uiBitmap;
	uint8_t   *pusI, *pusO, *pusB;
	
	pusI = (uint8_t *)pusIn;
	pusO = (uint8_t *)pSt8583;
	iLen = 0;
	
	/*** generate message of 8583 struct ***/
	iRet = UnPackElement(pDataAttr, pusI, pusO, (uint *)&iFieldLen);
	if( iRet<0 ){
		return -1;
	}
	pusI += iFieldLen;
	pusO += (pDataAttr->uiLength + 2);
	iLen += iFieldLen;
	
	/*** generate data and bitmap of 8583 struct ***/
	pusB = pusI;
	if( pusB[0] & 0x80 ){
		uiBitmap = (pDataAttr+1)->uiLength*2;
	}else{
		uiBitmap= (pDataAttr+1)->uiLength;
	}
	memcpy(pusO, pusB, uiBitmap);
	pusI += uiBitmap;
	pusO += (pDataAttr+1)->uiLength*2;
	iLen += uiBitmap;
	
	for(i=1; i<(uiBitmap*8); i++){
		if( (pDataAttr+i+1)->eElementAttr==Attr_Over ){
			return ((-1)*(2000+(i+1)));
		}
		if( *(pusB+i/8) & (0x80>>i%8) ){
			if( (pDataAttr+i+1)->eElementAttr==Attr_UnUsed ){
				return ((-1)*(2000+(i+1)));
			}
			iRet = UnPackElement(pDataAttr+i+1, pusI, pusO, (uint *)&iFieldLen);
			if( iRet<0 ){
				return ((-1)*(1000+(i+1)));
			}
			pusI += iFieldLen;
			iLen += iFieldLen;
		}
		
		if( (pDataAttr+i+1)->eElementAttr==Attr_UnUsed ){
			continue;
		}
		pusO += ((pDataAttr+i+1)->uiLength + 2);
	}   /*** for ***/
	
	if( uiInLen!=iLen ){
		return ERR_UNPACK_LEN;
	}
	
	return 0;
}


/****************************************************************************
 Function:          generate data element of 8583 packet
 Param in:
    pAttr           struct pointer to define attribute of data element
    pusIn           source data
 Param out:
    pusOut          object data
    puiOutLen       bytes of object data
 Return Code:
    = 0             absent
    = 1             present
    < 0             length overflow
****************************************************************************/
int PackElement(FIELD_ATTR *pAttr, uint8_t *pusIn, uint8_t *pusOut,
                 uint *puiOutLen)
{
   uint    i, j, iInLen, iOutLen;

   iOutLen =  0;
    *puiOutLen = 0;

    if( pAttr->eElementAttr!=Attr_b ){
        iInLen = strlen((char *)pusIn);
    }else{
        iInLen = ( (((uint)pusIn[0]<<8) & 0xFFFF) | ((uint)pusIn[1] & 0xFF) );
        pusIn += 2;
    }

    if( iInLen<=0 ){
        return 0;
    }
    if( iInLen>pAttr->uiLength ){
        return -1;
    }

    switch( pAttr->eLengthAttr ){
    case Attr_fix:
         iOutLen = pAttr->uiLength;
         break;
    case Attr_var1:
         pusOut[0] = ((iInLen/10)<<4) | (iInLen%10);
         pusOut++;
         iOutLen = 1+iInLen;
         break;
    case Attr_var2:
         pusOut[0] = iInLen/100;
         pusOut[1] = (((iInLen%100)/10)<<4) | ((iInLen%100)%10);
         pusOut += 2;
         iOutLen = 2+iInLen;
         break;
    }   /*** switch(pAttr->eLengthAttr ***/

    switch( pAttr->eElementAttr ){
    case Attr_n:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            iOutLen= (pAttr->uiLength+1)/2;

            memset(pusOut, 0, iOutLen);
            for(i=0,j=0; i<iInLen; i+=2,j++){
                if( i==iInLen-1 ){
                    if( toupper(pusIn[iInLen-i-1])<'A' ){
                        pusOut[iOutLen-j-1] = (pusIn[iInLen-i-1]&0x0F);
                    }else{
                        pusOut[iOutLen-j-1] =
                           toupper(pusIn[iInLen-i-1])-'A'+0x0A;
                    }
                 }else{
                    if( toupper(pusIn[iInLen-i-2])<'A' ){
                        pusOut[iOutLen-j-1] = (pusIn[iInLen-i-2]&0x0F)<<4;
                    }else{
                        pusOut[iOutLen-j-1] =
                           (toupper(pusIn[iInLen-i-2])-'A'+0x0A)<<4;
                    }

                    if( toupper(pusIn[iInLen-i-1])<'A' ){
                        pusOut[iOutLen-j-1] |= (pusIn[iInLen-i-1]&0x0F);
                    }else{
                        pusOut[iOutLen-j-1] |=
                           (toupper(pusIn[iInLen-i-1])-'A'+0x0A);
                    }
                 }
            }   /*** for(i=0; ***/
            break;

        case Attr_var1:
        case Attr_var2:
            iOutLen = iOutLen - iInLen + (iInLen+1)/2;

#ifndef VarAttrN_RightJustify
            for(i=0; i<(iInLen+1)/2; i++){
                if( toupper(pusIn[2*i])<'A' ){
                    pusOut[i] = (pusIn[2*i]&0x0F)<<4;
                }else{
                     pusOut[i] =
                        (toupper(pusIn[2*i])-'A'+0x0A)<<4;
                }

                if( toupper(pusIn[2*i+1])<'A'){
                     pusOut[i] |= (pusIn[2*i+1]&0x0F);
                }else{
                     pusOut[i] |=
                        (toupper(pusIn[2*i+1])-'A'+0x0A);
                }
            }

#else
            memset(pusOut, 0, (iInLen+1)/2);
            for(i=0,j=0; i<iInLen; i+=2,j++){
                if( i==iInLen-1 ){
                    if( toupper(pusIn[iInLen-i-1])<'A' ){
                        pusOut[(iInLen+1)/2-j-1] = (pusIn[iInLen-i-1]&0x0F);
                    }else{
                        pusOut[(iInLen+1)/2-j-1] =
                           toupper(pusIn[iInLen-i-1])-'A'+0x0A;
                    }
                }else{
                    if( toupper(pusIn[iInLen-i-2])<'A' ){
                        pusOut[(iInLen+1)/2-j-1] =
                            (pusIn[iInLen-i-2]&0x0F)<<4;
                    }else{
                        pusOut[(iInLen+1)/2-j-1] =
                           (toupper(pusIn[iInLen-i-2])-'A'+0x0A)<<4;
                    }

                    if( toupper(pusIn[iInLen-i-1])<'A' ){
                        pusOut[(iInLen+1)/2-j-1] |= (pusIn[iInLen-i-1]&0x0F);
                    }else{
                        pusOut[(iInLen+1)/2-j-1] |=
                           (toupper(pusIn[iInLen-i-1])-'A'+0x0A);
                    }
                }
            }   /*** for ***/
#endif
            break;
        }   /*** switch(pAttr->eLengthAttr) ***/
        break;

    case Attr_z:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            iOutLen = (pAttr->uiLength+1)/2;

            memset( pusOut, 0, iOutLen );
            for(i=0,j=0; i<iInLen; i+=2,j++){
                if( i==iInLen-1 ){
                    pusOut[iOutLen-j-1] = (pusIn[iInLen-i-1]&0x0F);
                }else{
                    pusOut[iOutLen-j-1] = ((pusIn[iInLen-i-2]&0x0F)<<4) |
                                    (pusIn[iInLen-i-1]&0x0F);
                }
            }   /*** for ***/
            break;

        case Attr_var1:
        case Attr_var2:
            iOutLen = iOutLen - iInLen + (iInLen+1)/2;
            for(i=0; i<(iInLen+1)/2; i++){
                pusOut[i] = ((pusIn[2*i]&0x0F)<<4) | (pusIn[2*i+1]&0x0F);
            }
            break;
        }   /*** switch(pAttr->eLengthAttr) ***/
        break;

    case Attr_b:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            memset(pusOut, 0, iOutLen);
            memcpy(pusOut, pusIn, iInLen);
            break;

        case Attr_var1:
        case Attr_var2:
            memcpy(pusOut, pusIn, iInLen);
            break;
        }
        break;

    case Attr_a:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            memset(pusOut, ' ', iOutLen);
            memcpy(pusOut, pusIn, iInLen);
            break;

        case Attr_var1:
        case Attr_var2:
            memcpy(pusOut, pusIn, iInLen);
            break;
        }
        break;
   }    /*** switch(pAttr->eElementAttr) ***/

   *puiOutLen = iOutLen;

   return 1;
}

/****************************************************************************
 Function           generate data element of 8583 struct
 Param in:
    pAttr           struct pointer to define attribute of data element
    pusIn           source data
 Param out:
    pusOut          object data
    puiInLen        bytes of source data to be used
 Return Code:
    = 0             success
    < 0             length overflow
****************************************************************************/
int UnPackElement(FIELD_ATTR *pAttr, uint8_t *pusIn, uint8_t *pusOut,
                   uint *puiInLen)
{
    uint    i, j, iInLen, iTmpLen;

    memset(pusOut, 0, pAttr->uiLength);
    *puiInLen = 0;
	i = j = iInLen = iTmpLen = 0;

    switch( pAttr->eLengthAttr ){
    case Attr_fix:
        iInLen  = pAttr->uiLength;
        iTmpLen = iInLen;
        break;

    case Attr_var1:
        iTmpLen= ((pusIn[0]>>4) &0x0F)*10 + (pusIn[0]&0x0F);
        pusIn++;
        iInLen= 1+iTmpLen;
        break;

    case Attr_var2:
        iTmpLen = (pusIn[0]&0x0F)*100 + ((pusIn[1]>>4) & 0x0F)*10 + (pusIn[1]&0x0F);
        pusIn += 2;
        iInLen = 2+iTmpLen;
        break;
    }   /*** switch(pAttr->eLengthAttr ***/

    if( iTmpLen>pAttr->uiLength ){
        return -1;
    }
    if( pAttr->eElementAttr==Attr_b ){
        pusOut[0] = (uint8_t)(iTmpLen>>8);
        pusOut[1] = (uint8_t)iTmpLen;
        pusOut   += 2;
    }

    switch( pAttr->eElementAttr ){
    case Attr_n:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            iInLen= (pAttr->uiLength+1)/2;

            for(i=0,j=0; i<pAttr->uiLength; i+=2,j++){
                if( (pusIn[iInLen-j-1]&0x0F)<0x0A ){
                    pusOut[pAttr->uiLength-i-1] = (pusIn[iInLen-j-1]&0x0F)|0x30;
                }else{
                    pusOut[pAttr->uiLength-i-1] =
                        (pusIn[iInLen-j-1]&0x0F)-0x0A+'A';
                }

                if( i!=pAttr->uiLength-1 ){
                    if( (pusIn[iInLen-j-1]>>4)<0x0A ){
                        pusOut[pAttr->uiLength-i-2] =
                           (pusIn[iInLen-j-1]>>4)|0x30;
                    }else{
                        pusOut[pAttr->uiLength-i-2]=
                           (pusIn[iInLen-j-1]>>4)-0x0A+'A';
                    }
                }
            }
            break;

        case Attr_var1:
        case Attr_var2:
            iInLen = iInLen - iTmpLen + (iTmpLen+1)/2;

#ifndef VarAttrN_RightJustify
            for(i=0; i<(iTmpLen/2); i++){
                if( (pusIn[i]>>4)<0x0A ){
                    pusOut[2*i] = (pusIn[i]>>4) | 0x30;
                }else{
                     pusOut[2*i] = (pusIn[i]>>4) -0x0A + 'A';
                }

                if( (pusIn[i]&0x0F)<0x0A ){
                     pusOut[2*i+1] = (pusIn[i]&0x0F) | 0x30;
                }else{
                     pusOut[2*i+1] = (pusIn[i]&0x0F) - 0x0A + 'A';
                }
            }

            if( iTmpLen%2 ){
                if( (pusIn[i]>>4)<0x0A ){
                     pusOut[2*i] = (pusIn[i]>>4) | 0x30;
                }else{
                     pusOut[2*i] = (pusIn[i]>>4) - 0x0A + 'A';
                }
            }
#else
            for(i=0,j=0; i<iTmpLen; i+=2,j++){
                if( (pusIn[(iTmpLen+1)/2-j-1]&0x0F)<0x0A ){
                    pusOut[iTmpLen-i-1] = (pusIn[(iTmpLen+1)/2-j-1]&0x0F)|0x30;
                }else{
                     pusOut[iTmpLen-i-1] =
                        (pusIn[(iTmpLen+1)/2-j-1]&0x0F)-0x0A+'A';
                }

                if( i!=pAttr->uiLength-1 ){
                    if( (pusIn[(iTmpLen+1)/2-j-1]>>4)<0x0A ){
                        pusOut[iTmpLen-i-2] =
                           (pusIn[(iTmpLen+1)/2-j-1]>>4)|0x30;
                    }else{
                        pusOut[iTmpLen-i-2] =
                           (pusIn[(iTmpLen+1)/2-j-1]>>4)-0x0A+'A';
                    }
                }
            }
#endif
            break;
        }   /*** switch(pAttr->eLengthAttr) ***/
        break;

    case Attr_z:
        switch( pAttr->eLengthAttr ){
        case Attr_fix:
            iInLen = (pAttr->uiLength+1)/2;

            for(i=0,j=0; i<pAttr->uiLength; i+=2,j++){
                pusOut[pAttr->uiLength-i-1]= (pusIn[iInLen-j-1]&0x0F)|0x30;
                if( i!=pAttr->uiLength-1 ){
                     pusOut[pAttr->uiLength-i-2]= (pusIn[iInLen-j-1]>>4)|0x30;
                }
            }
            break;

        case Attr_var1:
        case Attr_var2:
            iInLen = iInLen - iTmpLen + (iTmpLen+1)/2;

            for(i=0; i<(iTmpLen/2); i++){
                pusOut[2*i]   = (pusIn[i]>>4) | 0x30;
                pusOut[2*i+1] = (pusIn[i]&0x0F) | 0x30;
            }
            if( iTmpLen%2 ){
                pusOut[2*i]= (pusIn[i]>>4) | 0x30;
            }
            break;
        }   /*** switch(pAttr->eLengthAttr) ***/
        break;

    case Attr_b:
        memcpy(pusOut, pusIn, iTmpLen);
        break;

    case Attr_a:
        memcpy(pusOut, pusIn, iTmpLen);
        break;
    }    /*** switch(pAttr->eElementAttr) ***/

    *puiInLen = iInLen;

    return 0;
}

// end of file
