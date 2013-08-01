
void MCU_IO_set()
{
    // P2DIR |= BIT0 + BIT4;		//P2.0 and P2.4 are outputs
    // P1DIR |= BIT0;		//P1.0 is output
     P2DIR = 0xFF; // Set ALL the pins to output!!
     P1DIR = 0xFF; //
}
