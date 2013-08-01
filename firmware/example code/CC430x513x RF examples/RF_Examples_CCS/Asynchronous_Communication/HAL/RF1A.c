#include "RF1A.h"
#include "cc430x513x.h"

// *****************************************************************************
// @fn          Strobe
// @brief       Send a command strobe to the radio. Includes workaround for RF1A7
// @param       unsigned char strobe        The strobe command to be sent
// @return      unsigned char statusByte    The status byte that follows the strobe
// *****************************************************************************
unsigned char Strobe(unsigned char strobe)
{
  unsigned char statusByte = 0;
  unsigned int  gdo_state;
  
  // Check for valid strobe command 
  if((strobe == 0xBD) || ((strobe >= RF_SRES) && (strobe <= RF_SNOP)))
  {
    // Clear the Status read flag 
    RF1AIFCTL1 &= ~(RFSTATIFG);    
    
    // Wait for radio to be ready for next instruction
    while( !(RF1AIFCTL1 & RFINSTRIFG));
    
    // Write the strobe instruction
    if ((strobe > RF_SRES) && (strobe < RF_SNOP))
    {
      gdo_state = ReadSingleReg(IOCFG2);    // buffer IOCFG2 state
      WriteSingleReg(IOCFG2, 0x29);         // chip-ready to GDO2
      
      RF1AINSTRB = strobe; 
      if ( (RF1AIN&0x04)== 0x04 )           // chip at sleep mode
      {
        if ( (strobe == RF_SXOFF) || (strobe == RF_SPWD) || (strobe == RF_SWOR) ) { }
        else  	
        {
          while ((RF1AIN&0x04)== 0x04);     // chip-ready ?
          // Delay for ~810usec at 1.05MHz CPU clock, see erratum RF1A7
          __delay_cycles(850);	            
        }
      }
      WriteSingleReg(IOCFG2, gdo_state);    // restore IOCFG2 setting
    
      while( !(RF1AIFCTL1 & RFSTATIFG) );
    }
    else		                    // chip active mode (SRES)
    {	
      RF1AINSTRB = strobe; 	   
    }
    statusByte = RF1ASTATB;
  }
  return statusByte;
}

// *****************************************************************************
// @fn          ReadSingleReg
// @brief       Read a single byte from the radio register
// @param       unsigned char addr      Target radio register address
// @return      unsigned char data_out  Value of byte that was read
// *****************************************************************************
unsigned char ReadSingleReg(unsigned char addr)
{
  unsigned char data_out;
  
  // Check for valid configuration register address, 0x3E refers to PATABLE 
  if ((addr <= 0x2E) || (addr == 0x3E))
    // Send address + Instruction + 1 dummy byte (auto-read)
    RF1AINSTR1B = (addr | RF_SNGLREGRD);    
  else
    // Send address + Instruction + 1 dummy byte (auto-read)
    RF1AINSTR1B = (addr | RF_STATREGRD);    
  
  while (!(RF1AIFCTL1 & RFDOUTIFG) );
  data_out = RF1ADOUTB;                    // Read data and clears the RFDOUTIFG

  return data_out;
}

// *****************************************************************************
// @fn          WriteSingleReg
// @brief       Write a single byte to a radio register
// @param       unsigned char addr      Target radio register address
// @param       unsigned char value     Value to be written
// @return      none
// *****************************************************************************
void WriteSingleReg(unsigned char addr, unsigned char value)
{   
  while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for the Radio to be ready for next instruction
  RF1AINSTRB = (addr | RF_SNGLREGWR);	    // Send address + Instruction

  RF1ADINB = value; 			    // Write data in 

  __no_operation(); 
}
        
// *****************************************************************************
// @fn          ReadBurstReg
// @brief       Read multiple bytes to the radio registers
// @param       unsigned char addr      Beginning address of burst read
// @param       unsigned char *buffer   Pointer to data table
// @param       unsigned char count     Number of bytes to be read
// @return      none
// *****************************************************************************
void ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count)
{
  unsigned int i;
  if(count > 0)
  {
    while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for INSTRIFG
    RF1AINSTR1B = (addr | RF_REGRD);          // Send addr of first conf. reg. to be read 
                                              // ... and the burst-register read instruction
    for (i = 0; i < (count-1); i++)
    {
      while (!(RFDOUTIFG&RF1AIFCTL1));        // Wait for the Radio Core to update the RF1ADOUTB reg
      buffer[i] = RF1ADOUT1B;                 // Read DOUT from Radio Core + clears RFDOUTIFG
                                              // Also initiates auo-read for next DOUT byte
    }
    buffer[count-1] = RF1ADOUT0B;             // Store the last DOUT from Radio Core  
  }
}  

// *****************************************************************************
// @fn          WriteBurstReg
// @brief       Write multiple bytes to the radio registers
// @param       unsigned char addr      Beginning address of burst write
// @param       unsigned char *buffer   Pointer to data table
// @param       unsigned char count     Number of bytes to be written
// @return      none
// *****************************************************************************
void WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count)
{  
  unsigned char i;

  if(count > 0)
  {
    while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for the Radio to be ready for next instruction
    RF1AINSTRW = ((addr | RF_REGWR)<<8 ) + buffer[0]; // Send address + Instruction
  
    for (i = 1; i < count; i++)
    {
      RF1ADINB = buffer[i];                   // Send data
      while (!(RFDINIFG & RF1AIFCTL1));       // Wait for TX to finish
    } 
    i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte  
  }
}

// *****************************************************************************
// @fn          ResetRadioCore
// @brief       Reset the radio core using RF_SRES command
// @param       none
// @return      none
// *****************************************************************************
void ResetRadioCore (void)
{
  Strobe(RF_SRES);                          // Reset the Radio Core
  Strobe(RF_SNOP);                          // Reset Radio Pointer
}

// *****************************************************************************
// @fn          WriteRfSettings
// @brief       Write the minimum set of RF configuration register settings
// @param       RF_SETTINGS *pRfSettings  Pointer to the structure that holds the rf settings
// @return      none
// *****************************************************************************
void WriteRfSettings(RF_SETTINGS *pRfSettings) {
    WriteSingleReg(FSCTRL1,  pRfSettings->fsctrl1);
    WriteSingleReg(FSCTRL0,  pRfSettings->fsctrl0);
    WriteSingleReg(FREQ2,    pRfSettings->freq2);
    WriteSingleReg(FREQ1,    pRfSettings->freq1);
    WriteSingleReg(FREQ0,    pRfSettings->freq0);
    WriteSingleReg(MDMCFG4,  pRfSettings->mdmcfg4);
    WriteSingleReg(MDMCFG3,  pRfSettings->mdmcfg3);
    WriteSingleReg(MDMCFG2,  pRfSettings->mdmcfg2);
    WriteSingleReg(MDMCFG1,  pRfSettings->mdmcfg1);
    WriteSingleReg(MDMCFG0,  pRfSettings->mdmcfg0);
    WriteSingleReg(CHANNR,   pRfSettings->channr);
    WriteSingleReg(DEVIATN,  pRfSettings->deviatn);
    WriteSingleReg(FREND1,   pRfSettings->frend1);
    WriteSingleReg(FREND0,   pRfSettings->frend0);
    WriteSingleReg(MCSM0 ,   pRfSettings->mcsm0);
    WriteSingleReg(FOCCFG,   pRfSettings->foccfg);
    WriteSingleReg(BSCFG,    pRfSettings->bscfg);
    WriteSingleReg(AGCCTRL2, pRfSettings->agcctrl2);
    WriteSingleReg(AGCCTRL1, pRfSettings->agcctrl1);
    WriteSingleReg(AGCCTRL0, pRfSettings->agcctrl0);
    WriteSingleReg(FSCAL3,   pRfSettings->fscal3);
    WriteSingleReg(FSCAL2,   pRfSettings->fscal2);
    WriteSingleReg(FSCAL1,   pRfSettings->fscal1);
    WriteSingleReg(FSCAL0,   pRfSettings->fscal0);
    WriteSingleReg(FSTEST,   pRfSettings->fstest);
    WriteSingleReg(TEST2,    pRfSettings->test2);
    WriteSingleReg(TEST1,    pRfSettings->test1);
    WriteSingleReg(TEST0,    pRfSettings->test0);
    WriteSingleReg(FIFOTHR,  pRfSettings->fifothr);
    WriteSingleReg(IOCFG2,   pRfSettings->iocfg2);
    WriteSingleReg(IOCFG0,   pRfSettings->iocfg0);
    WriteSingleReg(PKTCTRL1, pRfSettings->pktctrl1);
    WriteSingleReg(PKTCTRL0, pRfSettings->pktctrl0);
    WriteSingleReg(ADDR,     pRfSettings->addr);
    WriteSingleReg(PKTLEN,   pRfSettings->pktlen);
}

// *****************************************************************************
// @fn          WritePATable
// @brief       Write data to power table
// @param       unsigned char value		Value to write
// @return      none
// *****************************************************************************
void WriteSinglePATable(unsigned char value)
{
  while( !(RF1AIFCTL1 & RFINSTRIFG));
  RF1AINSTRW = 0x3E00 + value;              // PA Table single write
  
  while( !(RF1AIFCTL1 & RFINSTRIFG));
  RF1AINSTRB = RF_SNOP;                     // reset PA_Table pointer
}

// *****************************************************************************
// @fn          WritePATable
// @brief       Write to multiple locations in power table 
// @param       unsigned char *buffer	Pointer to the table of values to be written 
// @param       unsigned char count	Number of values to be written
// @return      none
// *****************************************************************************
void WriteBurstPATable(unsigned char *buffer, unsigned char count)
{
  volatile char i = 0; 
  
  while( !(RF1AIFCTL1 & RFINSTRIFG));
  RF1AINSTRW = 0x7E00 + buffer[i];          // PA Table burst write   

  for (i = 1; i < count; i++)
  {
    RF1ADINB = buffer[i];                   // Send data
    while (!(RFDINIFG & RF1AIFCTL1));       // Wait for TX to finish
  } 
  i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte

  while( !(RF1AIFCTL1 & RFINSTRIFG));
  RF1AINSTRB = RF_SNOP;                     // reset PA Table pointer
}
