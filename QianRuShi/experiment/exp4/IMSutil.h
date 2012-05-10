#ifndef IMS_UTIL_H
#define IMS_UTIL_H

//some Define function
#define char2BCD( x ) ( ((x)/10) *16 + ((x)%10) )

//A delay function used to work at 11.0952MHz
void delay_ms( unsigned int ucMs );







#endif
