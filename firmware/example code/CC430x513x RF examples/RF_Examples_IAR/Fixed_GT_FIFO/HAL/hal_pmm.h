//****************************************************************************//
// Function Library for setting the PMM
//    File: hal_pmm.h
//
//    Texas Instruments
//
//    Version 1.2
//    10/17/09
//
//    V1.0  Initial Version
//    V1.1  Adjustment to UG
//    V1.2  Added return values
//****************************************************************************////====================================================================


#ifndef __PMM
#define __PMM

#define PMM_STATUS_OK     0
#define PMM_STATUS_ERROR  1

//====================================================================
/**
  * Set the VCore to a new level if it is possible and return a
  * error - value.
  *
  * \param      level       PMM level ID
  * \return	int	    1: error / 0: done
  */
unsigned int SetVCore (unsigned char level);

//====================================================================
/**
  * Set the VCore to a higher level, if it is possible.
  * Return a 1 if voltage at highside (Vcc) is to low
  * for the selected Level (level).
  *
  * \param      level       PMM level ID
  * \return	int	    1: error / 0: done
  */
unsigned int SetVCoreUp (unsigned char level);

//====================================================================
/**
  * Set the VCore to a lower level.
  * Return a 1 if voltage at highside (Vcc) is still to low
  * for the selected Level (level).
  *
  * \param      level       PMM level ID
  * \return	int	    1: done with error / 0: done without error
  */
unsigned int SetVCoreDown (unsigned char level);

#endif /* __PMM */
