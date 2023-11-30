/**
  ******************************************************************************
  * @file    DAC
  * @author  Muaaz Al-Jbawi
  * @brief   This example shows the use of DAC peripheral to generate
several signals using DMA controller.
  ******************************************************************************
  **/

/**
 * @file main.cpp
 * @brief This file contains the main application code for DAC waveform generation.
 */

#include "main.h"

/**
 * @class DACController
 * @brief This class handles the configuration and control of the DAC (Digital to Analog Converter).
 */
class DACController {
private:
    DAC_HandleTypeDef DacHandle;
    DAC_ChannelConfTypeDef sConfig;
    const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
    __IO uint8_t ubSelectedWavesForm = 1;
    __IO uint8_t ubKeyPressed = SET;

    /**
     * @brief Configure DAC channel 1 for Triangle waveform.
     */
    void DAC_Ch1_TriangleConfig();

    /**
     * @brief Configure DAC channel 1 for Escalator waveform.
     */
    void DAC_Ch1_EscalatorConfig();

    /**
     * @brief Configure TIM6 for timing and trigger generation.
     */
    void TIM6_Config();

    /**
     * @brief Configure the system clock.
     */
    void SystemClock_Config();

    /**
     * @brief Handle errors.
     */
    void Error_Handler();

public:
    /**
     * @brief Constructor for DACController class.
     */
    DACController();

    /**
     * @brief Run the DACController main loop.
     */
    void Run();
};

/**
 * @brief Constructor for DACController class.
 */
DACController::DACController() {
    HAL_Init();
    SystemClock_Config();
    BSP_LED_Init(LED3);
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
    DacHandle.Instance = DACx;
    TIM6_Config();
}

/**
 * @brief Main loop of the DACController class.
 */
void DACController::Run() {
    while (1) {
        if (ubKeyPressed != RESET) {
            HAL_DAC_DeInit(&DacHandle);

            if (ubSelectedWavesForm == 1) {
                DAC_Ch1_TriangleConfig();
            } else {
                DAC_Ch1_EscalatorConfig();
            }

            ubKeyPressed = RESET;
        }
    }
}

/**
 * @brief Configure the system clock.
 */
void DACController::SystemClock_Config() {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    RCC_OscInitStruct.PLL.PLLR = 6;

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK) {
        Error_Handler();
    }

    ret = HAL_PWREx_EnableOverDrive();
    if(ret != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    if(ret != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Handle errors.
 */
void DACController::Error_Handler() {
    BSP_LED_On(LED3);
    while (1) {
    }
}

/**
 * @brief Configure DAC channel 1 for Escalator waveform.
 */
void DACController::DAC_Ch1_EscalatorConfig() {
    if (HAL_DAC_Init(&DacHandle) != HAL_OK) {
        Error_Handler();
    }

    sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 6, DAC_ALIGN_8B_R) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Configure DAC channel 1 for Triangle waveform.
 */
void DACController::DAC_Ch1_TriangleConfig() {
    if (HAL_DAC_Init(&DacHandle) != HAL_OK) {
        Error_Handler();
    }

    sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_DACEx_TriangleWaveGenerate(&DacHandle, DACx_CHANNEL, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL, DAC_ALIGN_12B_R, 0x100) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Configure TIM6 for timing and trigger generation.
 */
void DACController::TIM6_Config() {
    TIM_HandleTypeDef htim;
    TIM_MasterConfigTypeDef sMasterConfig;

    htim.Instance = TIM6;
    htim.Init.Period = 0x7FF;
    htim.Init.Prescaler = 0;
    htim.Init.ClockDivision = 0;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&htim);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
    HAL_TIM_Base_Start(&htim);
}


/**
 * @brief Main function.
 */

extern "C" int main(void) {
    DACController dacController;
    dacController.Run();
    return 0;
}
