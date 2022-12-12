#include "stm32g0xx.h"
#include <stdint.h>
#include "app_bsp.h"

void HAL_MspInit( void )
{
   
}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan){

    GPIO_InitTypeDef GPIO_Struct;

    if (hfdcan->Instance == FDCAN1)
    {
        __HAL_RCC_FDCAN_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_Struct.Mode        = GPIO_MODE_AF_PP;
        GPIO_Struct.Alternate   = GPIO_AF3_FDCAN1;
        GPIO_Struct.Pin         = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_Struct.Pull        = GPIO_NOPULL;
        GPIO_Struct.Speed       = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_Struct);
        
    }
    
}