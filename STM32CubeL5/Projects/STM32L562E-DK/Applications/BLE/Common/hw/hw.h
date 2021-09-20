/**
 ******************************************************************************
 * @file    hw.h
 * @author  MCD Application Team
 * @brief   Hardware
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license SLA0044,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        http://www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_H
#define __HW_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "stm32l5xx.h"
#include "stm32l5xx_ll_pwr.h"
#include "stm32l5xx_ll_cortex.h"
#include "stm32l5xx_ll_exti.h"
#include "stm32l5xx_ll_rcc.h"
#include "stm32l5xx_ll_rtc.h"
#include "stm32l5xx_ll_spi.h"
#include "stm32l5xx_ll_dma.h"



  /******************************************************************************
   * HW Low Power Management (lpm)
   ******************************************************************************/
  void HW_SleepMode(void);
  void HW_StopMode(uint32_t configuration);
  void HW_OffMode(uint32_t configuration);


  /******************************************************************************
   * HW TimerServer 
   ******************************************************************************/
  /* Exported types ------------------------------------------------------------*/
  /**
   * This setting is used when standby mode is supported.
   * hw_ts_InitMode_Limited should be used when the device restarts from Standby Mode. In that case, the Timer Server does
   * not re-initialized its context. Only the Hardware register which content has been lost is reconfigured
   * Otherwise, hw_ts_InitMode_Full should be requested (Start from Power ON) and everything is re-initialized.
   */
  typedef enum
  {
    hw_ts_InitMode_Full,
    hw_ts_InitMode_Limited,
  } HW_TS_InitMode_t;

  /**
   * When a Timer is created as a SingleShot timer, it is not automatically restarted when the timeout occurs. However,
   * the timer is kept reserved in the list and could be restarted at anytime with HW_TS_Start()
   *
   * When a Timer is created as a Repeated timer, it is automatically restarted when the timeout occurs.
   */
  typedef enum
  {
    hw_ts_SingleShot,
    hw_ts_Repeated
  } HW_TS_Mode_t;

  /**
   * hw_ts_Successful is returned when a Timer has been successfully created with HW_TS_Create(). Otherwise, hw_ts_Failed
   * is returned. When hw_ts_Failed is returned, that means there are not enough free slots in the list to create a
   * Timer. In that case, CFG_HW_TS_MAX_NBR_CONCURRENT_TIMER should be increased
   */
  typedef enum
  {
    hw_ts_Successful,
    hw_ts_Failed,
  }HW_TS_ReturnStatus_t;

  typedef void (*HW_TS_pTimerCb_t)(void);

  /**
   * @brief  Initialize the timer server
   *         This API shall be called by the application before any timer is requested to the timer server. It
   *         configures the RTC module to be connected to the LSI input clock.
   *
   * @param  TimerInitMode: When the device restarts from Standby, it should request hw_ts_InitMode_Limited so that the
   *         Timer context is not re-initialized. Otherwise, hw_ts_InitMode_Full should be requested
   * @param  hrtc: RTC Handle
   * @retval None
   */
  void HW_TS_Init(HW_TS_InitMode_t TimerInitMode, RTC_HandleTypeDef *hrtc);

  /**
   * @brief  Interface to create a virtual timer
   *         The user shall call this API to create a timer. Once created, the timer is reserved to the module until it
   *         has been deleted. When creating a timer, the user shall specify the mode (single shot or repeated), the
   *         callback to be notified when the timer expires and a module ID to identify in the timer interrupt handler
   *         which module is concerned. In return, the user gets a timer ID to handle it.
   *
   * @param  TimerProcessID:  This is an identifier provided by the user and returned in the callback to allow
   *                          identification of the requester
   * @param  pTimerId: Timer Id returned to the user to request operation (start, stop, delete)
   * @param  TimerMode: Mode of the virtual timer (Single shot or repeated)
   * @param  pTimerCallBack: Callback when the virtual timer expires
   * @retval HW_TS_ReturnStatus_t: Return whether the creation is successful or not
   */
  HW_TS_ReturnStatus_t HW_TS_Create(uint32_t TimerProcessID, uint8_t *pTimerId, HW_TS_Mode_t TimerMode, HW_TS_pTimerCb_t pTimerCallBack);

  /**
   * @brief  Stop a virtual timer
   *         This API may be used to stop a running timer. A timer which is stopped is move to the pending state.
   *         A pending timer may be restarted at any time with a different timeout value but the mode cannot be changed.
   *         Nothing is done when it is called to stop a timer which has been already stopped
   *
   * @param  TimerID:  Id of the timer to stop
   * @retval None
   */
  void HW_TS_Stop(uint8_t TimerID);

  /**
   * @brief  Start a virtual timer
   *         This API shall be used to start a timer. The timeout value is specified and may be different each time.
   *         When the timer is in the single shot mode, it will move to the pending state when it expires. The user may
   *         restart it at any time with a different timeout value. When the timer is in the repeated mode, it always
   *         stay in the running state. When the timer expires, it will be restarted with the same timeout value.
   *         This API shall not be called on a running timer.
   *
   * @param  TimerID:  The ID Id of the timer to start
   * @param  timeout_ticks: Number of ticks of the virtual timer (Maximum value is (0xFFFFFFFF-0xFFFF = 0xFFFF0000)
   * @retval None
   */
  void HW_TS_Start(uint8_t TimerID, uint32_t timeout_ticks);

  /**
   * @brief  Delete a virtual timer from the list
   *         This API should be used when a timer is not needed anymore by the user. A deleted timer is removed from
   *         the timer list managed by the timer server. It cannot be restarted again. The user has to go with the
   *         creation of a new timer if required and may get a different timer id
   *
   * @param  TimerID:  The ID of the timer to remove from the list
   * @retval None
   */
  void HW_TS_Delete(uint8_t TimerID);

  /**
   * @brief  Schedule the timer list on the timer interrupt handler
   *         This interrupt handler shall be called by the application in the RTC interrupt handler. This handler takes
   *         care of clearing all status flag required in the RTC and EXTI peripherals
   *
   * @param  None
   * @retval None
   */
  void HW_TS_RTC_Wakeup_Handler(void);

  /**
   * @brief  Return the number of ticks to count before the interrupt
   *         This API returns the number of ticks left to be counted before an interrupt is generated by the
   *         Timer Server. This API may be used by the application for power management optimization. When the system
   *         enters low power mode, the mode selection is a tradeoff between the wakeup time where the CPU is running
   *         and the time while the CPU will be kept in low power mode before next wakeup. The deeper is the
   *         low power mode used, the longer is the wakeup time. The low power mode management considering wakeup time
   *         versus time in low power mode is implementation specific
   *         When the timer is disabled (No timer in the list), it returns 0xFFFF
   *
   * @param  None
   * @retval The number of ticks left to count
   */
  uint16_t HW_TS_RTC_ReadLeftTicksToCount(void);

  /**
   * @brief  Notify the application that a registered timer has expired
   *         This API shall be implemented by the user application.
   *         This API notifies the application that a timer expires. This API is running in the RTC Wakeup interrupt
   *         context. The application may implement an Operating System to change the context priority where the timer
   *         callback may be handled. This API provides the module ID to identify which module is concerned and to allow
   *         sending the information to the correct task
   *
   * @param  TimerProcessID: The TimerProcessId associated with the timer when it has been created
   * @param  TimerID: The TimerID of the expired timer
   * @param  pTimerCallBack: The Callback associated with the timer when it has been created
   * @retval None
   */
  void HW_TS_RTC_Int_AppNot(uint32_t TimerProcessID, uint8_t TimerID, HW_TS_pTimerCb_t pTimerCallBack);

  /**
   * @brief  Notify the application that the wakeupcounter has been updated
   *         This API should be implemented by the user application
   *         This API notifies the application that the counter has been updated. This is expected to be used along
   *         with the HW_TS_RTC_ReadLeftTicksToCount () API. It could be that the counter has been updated since the
   *         last call of HW_TS_RTC_ReadLeftTicksToCount () and before entering low power mode. This notification
   *         provides a way to the application to solve that race condition to reevaluate the counter value before
   *         entering low power mode
   *
   * @param  None
   * @retval None
   */
  void HW_TS_RTC_CountUpdated_AppNot(void);



  /******************************************************************************
   * HW BlueNRG SPI 
   ******************************************************************************/

  /* Exported Defines  */

#define BNRG_SPI_IRQ_PIN                GPIO_PIN_6
#define BNRG_SPI_IRQ_PORT               GPIOG
#define BNRG_SPI_IRQ_WAKEUP_PIN         LL_PWR_WAKEUP_PIN1
#define BNRG_SPI_READ_WAKEUP_FLAG       LL_PWR_IsActiveFlag_WU1
#define BNRG_SPI_CLEAR_WAKEUP_FLAG      LL_PWR_ClearFlag_WU1

  // EXTI External Interrupt for SPI
  // NOTE: if you change the IRQ pin remember to implement a corresponding handler
  // function like EXTI0_1_IRQHandler() in the user project
#define BNRG_SPI_EXTI_PIN               BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT              BNRG_SPI_IRQ_PORT
#define BNRG_SPI_EXTI_IRQn              EXTI6_IRQn
#define BNRG_SPI_EXTI_IRQHandler        EXTI6_IRQHandler
  
  /* Exported Type  */
  
   typedef enum
  {
    HW_BNRG_Limited,
    HW_BNRG_Full,
  } HW_BNRG_InitMode_t;

  /* Exported function prototypes */
  
  /**
   * @brief  Closes the SPI when BLE is disabled by the application
   *         Releases allocated resources
   * @param  None
   * @retval None
   */
  void HW_BNRG_Close(void);

  /**
   * @brief  Initializes the SPI communication with the BlueNRG Shield.
   * @param  InitMode: Either Full Init or Limited Init depending on which low power mode was entered
   * @retval None
   */
  void HW_BNRG_Init(HW_BNRG_InitMode_t InitMode);

  /**
   * @brief  Writes data from local buffer to SPI.
   * @param  payload_data: Data buffer to be written
   * @param  payload_size: Size of data buffer to be written
   * @retval None
   */
  void HW_BNRG_Send(uint8_t* payload_data, uint16_t payload_size);

  /**
   * @brief  BlueNRG SPI request event
   * @param  buffer: the event
   * @param  buff_size: the event size
   * @retval None
   */
  void HW_BNRG_EvtReq(uint8_t *buffer, uint16_t buff_size);

  /**
   * @brief BlueNRG SPI IRQ Callback
   *        This interrupt handler shall be called by the application in the EXTI interrupt handler related to the
   *        IO pin where SPI IRQ has been defined in the file stm32l4xx_bluenrg_ms_msp.h.
   * @param  None
   * @retval None
   */
  void HW_BNRG_SpiIrqCb(void);

  /**
   * @brief Tx Transfer complete callback
   *        This interrupt handler shall be called by the application in the DMA interrupt handler related to the
   *        channel used for the SPI Tx as defined in the file stm32l4xx_bluenrg_ms_msp.h. This handler checks whether
   *        its own channel is pending and takes care of clearing all status flag required in the DMA peripheral.
   * @param  None
   * @retval None
   */
  void HW_BNRG_DMATxCb(void);

  /**
   * @brief Rx Transfer complete callback
   *        This interrupt handler shall be called by the application in the DMA interrupt handler related to the
   *        channel used for the SPI Rx as defined in the file stm32l4xx_bluenrg_ms_msp.h. This handler checks whether
   *        its own channel is pending and takes care of clearing all status flag required in the DMA peripheral.
   * @param  None
   * @retval None
   */
  void HW_BNRG_DMARxCb(void);

  /**
   * @brief  It sets the IO configuration to be applied when in standby mode
   *         This API shall be called before entering Standby Mode. This can be done at any time after the initialization
   *         and the time when the device enters StandBy Mode.
   *         It should be called after the APC bit has been cleared in the PWR (This means the PU/PD configuration from
   *         the PWR Peripheral does not apply)
   *         It is recommended to call this API after the initialization rather than in LPM_EnterOffMode() as this one
   *         is called in critical section
   *
   * @param  None
   * @retval None
   */
  void HW_BNRG_SetupStdbyMode(void);

  /**
   * The SPI driver implements some timeout. These timeout may be requested in an interrupt context. As the Timer Server
   * does not support nested request and the SPI driver does not know if there is a timer request pending from the
   * application, it notifies the application with HW_BNRG_TimStartReq() that it needs to start a timer.
   * The application shall return with HW_BNRG_TimStartAllowed() if no request are pending to the Timer Server.
   * In the current implementation, HW_BNRG_TimStartAllowed() is called within HW_BNRG_TimStartReq()
   * because CFG_HW_TS_USE_PRIMASK_AS_CRITICAL_SECTION is set to 1 which means that all Timer Server API may be called freely
   * as they cannot be nested.
   */
  /**
   * @brief  The timer may be start
   * @param  None
   * @retval None
   */
  void HW_BNRG_TimStartAllowed(void);

  /* __weak function prototypes */
  
/**
   * @brief  Request a timer to be started
   * @param  None
   * @retval None
   * @Note   If the specific application need to wait before starting the timer 
   *         this function can be overloaded in the specific application itself 
   */
  void HW_BNRG_TimStartReq(void);



  /**
   * @brief  BlueNRG SPI event received
   *         This API is called in interrupt context. It notifies that an event has been received in the allocated buffer
   * @param  event_payload_len: the size of the event received
   * @retval None
   */
  void HW_BNRG_Isr(uint16_t event_payload_len);


#ifdef __cplusplus
}
#endif

#endif /*__HW_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
