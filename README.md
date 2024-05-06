STM32 UART receive (RX) consisting of a struct of 10 sensor values with a 6 key value for each transmitted packet using UART DMA and Interrupt, there is also an incorporated timeout function if no message is received after 10 seconds. UART3 is the RX Channel and UART2 is the information to PC channel.

I have tested this also using 2x HC05 Bluetooth tranceivers that have been paired just connect UART3[RX] -> HC05[TX].


Updated [06.05.24] - Now includes UART Circular DMA, a 0.5 Second timout routine, and RX packet failure routine to reset RX line in case of overflow (Tested and working).
