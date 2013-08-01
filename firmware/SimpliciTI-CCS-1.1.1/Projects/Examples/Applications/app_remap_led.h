#ifdef BSP_BOARD_EXP461x

  #undef BSP_TURN_ON_LED2
  #undef BSP_TURN_OFF_LED2
  #undef BSP_TOGGLE_LED2
  #undef BSP_LED2_IS_ON

  #define BSP_TURN_ON_LED2()      BSP_TURN_ON_LED3()
  #define BSP_TURN_OFF_LED2()     BSP_TURN_OFF_LED3()
  #define BSP_TOGGLE_LED2()       BSP_TOGGLE_LED3()
  #define BSP_LED2_IS_ON()        BSP_LED3_IS_ON()

#endif
