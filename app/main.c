#include "stm32g0xx.h"
#include "app_bsp.h"
#include "CUBA.h" //<-- Include CUBA library

/* Function prototypes */
void SystemClock_Config(void);

/* Global variables */
CUBA_HandleTypeDef      CUBA_Handle;    //Structure type variable that contains the configuration information for the CUBA library

FDCAN_HandleTypeDef     USER_CAN_H;
FDCAN_TxHeaderTypeDef   USER_CAN_TxHeader; 
FDCAN_RxHeaderTypeDef   USER_CAN_RxHeader;

uint32_t tick;

int main( void )
{
    /* Initialization function calls */
    HAL_Init();
    SystemClock_Config();
    MOD_CUBA_Init(&CUBA_Handle);

    tick = HAL_GetTick();

    USER_CAN_H.Instance                     = FDCAN1;
    USER_CAN_H.Init.Mode                    = FDCAN_MODE_NORMAL;
    USER_CAN_H.Init.AutoRetransmission      = ENABLE;   
    USER_CAN_H.Init.ClockDivider            = FDCAN_CLOCK_DIV1;
    USER_CAN_H.Init.TxFifoQueueMode         = FDCAN_TX_FIFO_OPERATION;
    USER_CAN_H.Init.TransmitPause           = DISABLE;
    USER_CAN_H.Init.ExtFiltersNbr           = 0;
    USER_CAN_H.Init.ProtocolException       = DISABLE;
    USER_CAN_H.Init.StdFiltersNbr           = 0;
    USER_CAN_H.Init.FrameFormat             = FDCAN_FRAME_CLASSIC;
    USER_CAN_H.Init.NominalPrescaler        = 30;
    USER_CAN_H.Init.NominalSyncJumpWidth    = 1;
    USER_CAN_H.Init.NominalTimeSeg1         = 13;
    USER_CAN_H.Init.NominalTimeSeg2         = 2;
    (void)HAL_FDCAN_Init(&USER_CAN_H);

    (void)HAL_FDCAN_Start(&USER_CAN_H);

    USER_CAN_TxHeader.DataLength            = FDCAN_DLC_BYTES_8;
    USER_CAN_TxHeader.Identifier            = 0x1EF;
    USER_CAN_TxHeader.IdType                = FDCAN_STANDARD_ID;
    USER_CAN_TxHeader.FDFormat              = FDCAN_CLASSIC_CAN;
    USER_CAN_TxHeader.TxFrameType           = FDCAN_DATA_FRAME;
    USER_CAN_TxHeader.BitRateSwitch         = FDCAN_BRS_OFF;
    USER_CAN_TxHeader.TxEventFifoControl    = FDCAN_NO_TX_EVENTS;

    USER_CAN_RxHeader.BitRateSwitch         = FDCAN_BRS_OFF;
    USER_CAN_RxHeader.DataLength            = FDCAN_DLC_BYTES_8;
    USER_CAN_RxHeader.ErrorStateIndicator   = FDCAN_ESI_PASSIVE;
    USER_CAN_RxHeader.FDFormat              = FDCAN_CLASSIC_CAN;

    uint32_t TxIndex;

    uint8_t message[8] = {0x48, 0x49, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44};

    HAL_FDCAN_AddMessageToTxFifoQ(&USER_CAN_H, &USER_CAN_TxHeader, message);

    TxIndex = HAL_FDCAN_GetLatestTxFifoQRequestBuffer( &USER_CAN_H);

    while (HAL_FDCAN_IsTxBufferMessagePending(&USER_CAN_H, TxIndex));
    

    for( ; ; )
    {
        if ((HAL_GetTick() - tick) >= 10u)
        {
            tick = HAL_GetTick();
            (void)MOD_CUBA_PeriodicTask(&CUBA_Handle);
        }
        
    }

    return 0u;
}

/**
  * @brief Function that configures system clock. (48Mhz)
  * @param void
  * @retval none
  **/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 6;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}