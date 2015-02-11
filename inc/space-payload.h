/*
 * =====================================================================================
 *
 *       Filename:  space-payload.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:04:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

/*
#ifndef _
#define SPACE_PAYLOAD_H

// TODO prototype all functions here


static struct spi_board_info stapm9g20_spi_devices[] = {
{
  .modalias = "spidev",//activation of spi-device driver .modalias = "lis3lv02d_spi"
  .chip_select = 0,//chip selection
  .max_speed_hz = 1*1000*1000,//SPI speed between MCK and MCK/255
  .bus_num = 1,//if SPI0 is used
  .mode = SPI_MODE_0,//default mode
}
}
atQ6_add_device_spi (stamp9g20_spi_devices, ARRAY_SIZE(stamp9g20_spi_devices));
//


#endif /* !SPACE_PAYLOAD_H */
*/
/** \file
* \brief Code to read from the Q6 ADC
*/
/* $Id: cmx_load.c 65 2010-09-03 20:06:15Z jpl $
*
*
*
* Copyright (c) 2011, Xiphos Technologies Inc.
* All rights reserved.
*
* $Author: jpl $
* $Revision: 65 $
* $Date: 2010-09-03 16:06:15 -0400 (Fri, 03 Sep 2010) $
*
*/

#ifndef __XSC_Q6_ADC_H_
#define __XSC_Q6_ADC_H_

/**
* \addtogroup Q6ADC
* @{
*/


/**
* The ADC has nominally 8 input channels, plus a 'loopback' temperature
* reading.
*/
#define AD7689_ADC_CHANNEL_COUNT 8

/**
* \internal
* Internal data structure to control access to the device.
*
*/
typedef struct {
  int iADCFd;
  unsigned short pusInputChannelConf[AD7689_ADC_CHANNEL_COUNT];
  unsigned short usReferenceSelection;

} XSC_Q6ADC_Conf_t;


/**
* [12:10] INCC : Input channel configuration. Selection of pseudo bipolar,
* pseudo differential pairs, single-ended, or temperature sensor.
*/
#define AD7689_MASK_INCC              0x1c00
#define AD7689_INCC_UNI_REF_GND       0x1c00
#define AD7689_INCC_UNI_REF_COM       0x1800
#define AD7689_INCC_UNI_DIFF_REF_GND  0x1000
#define AD7689_INCC_TEMP              0x0c00
#define AD7689_INCC_BIP_REF_COM       0x0800
#define AD7689_INCC_BIP_DIFF_REF_VRF  0x0000

/**
*  [5:3] REF : Reference/buffer selection. Selection of internal, external,
* external buffered, and enabling of the on-chip temperature sensor.
*/
#define AD7689_MASK_REF               0x0038
#define AD7689_REF_EXT_IBUF_TEMP_DIS  0x0038
#define AD7689_REF_EXT_TEMP_DIS       0x0030
#define AD7689_REF_EXT_IBUF_TEMP_EN   0x0018
#define AD7689_REF_EXT_TEMP_EN        0x0010
#define AD7689_REF_INT_4V096          0x0008
#define AD7689_REF_INT_2V5            0x0000




/**
* Configure the channel to be unipolar - referenced to ground
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetUnipolarGND(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_UNI_REF_GND;
  return(0);
}

/**
* Configure the channel to be unipolar - referenced to COM
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetUnipolarCOM(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_UNI_REF_COM;
  return(0);
}

/**
* Configure the channel to be Unipolar differential pairs; INx- referenced to
* GND +/- 0.1 V.
*
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetUnipolarDiff(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_UNI_DIFF_REF_GND;
  return(0);
}

/**
* Configure the channel to be temperature sensor
*
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetEnableTemp(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_TEMP;
  return(0);
}

/**
* Configure the channel to be Bipolar; INx referenced to COM = VREF/2 +/- 0.1 V.
*
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetBipolarCOM(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_BIP_REF_COM;
  return(0);
}

/**
* Configure the channel to be Bipolar differential pairs; INx- referenced to
* VREF/2 +/- 0.1 V.
*
*
* \retval 0 Success
* \retval -EINVAL Channel out of range or config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetBipolarDiff(int iChannel, ///< Input: The
///< channel that is to be configured
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (iChannel < 0 || iChannel >= AD7689_ADC_CHANNEL_COUNT || pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->pusInputChannelConf[iChannel] = AD7689_INCC_BIP_DIFF_REF_VRF;
  return(0);
}
/**
* Configure the reference to be Internal reference, REF = 2.5V output
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefInt2V5(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_INT_2V5;
  return(0);
}

/**
* Configure the reference to be Internal reference, REF = 4.906V output
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefInt4V096(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_INT_4V096;
  return(0);
}
/**
* Configure the reference to be External reference, temperature enabled.
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefExtTemp(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_EXT_TEMP_EN;
  return(0);
}
/**
* Configure the reference to be External reference, internal buffer,
* temperature enabled.
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefExtIbufTemp(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_EXT_IBUF_TEMP_EN;
  return(0);
}
/**
* Configure the reference to be External reference, temperature disabled.
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefExt(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_EXT_TEMP_DIS;
  return(0);
}
/**
* Configure the reference to be External reference, internal buffer,
* temperature disabled.
*
* \retval 0 Success
* \retval -EINVAL Config pointer is NULL.
*/
static inline int XSC_Q6ADC_SetRefExtIbuf(
XSC_Q6ADC_Conf_t * pxConfig ///< Input: Pointer to configuration data
){
  if (pxConfig == NULL) {
    return(-EINVAL);
  }
  pxConfig->usReferenceSelection = AD7689_REF_EXT_IBUF_TEMP_DIS;
  return(0);
}

/** @} */ // Ends addtogroup Q6ADC




int XSC_Q6ADC_Open(const char * pcSpiDevName, XSC_Q6ADC_Conf_t * pxConfig);
int XSC_Q6ADC_Close(XSC_Q6ADC_Conf_t * pxConfig);
int XSC_Q6ADC_Read(unsigned short * pusADCValues, XSC_Q6ADC_Conf_t * pxConfig);

#endif /* __XSC_Q6_ADC_H_ */
