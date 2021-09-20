#ifndef APPLICATION_H_
#define APPLICATION_H_

// First call in main before initializing any peripherals
int application_on_reset(void);

// Last call before starting FreeRTOS, after all peripherals set up
// @note Do not use kernel delay functions since it isn't running
int application_init(void);

// Called in an infinite loop only if FreeRTOS exits
int application_end_of_main(void);

void application_start_task_idle(void *p_handle);

void application_start_task_serial(void *p_handle,
                                   void *p_request_queue_handle);

void application_start_task_i2c_bus(void *p_handle,
                                    void *p_request_queue_handle);

void application_start_task_button(void *p_handle);

void application_start_task_display(void *p_handle);

void application_start_task_motion(void *p_handle);

void application_start_task_adc(void *p_handle);

void application_start_task_sample_logging(void *p_handle,
                                           void *p_request_queue_handle);

#endif // APPLICATION_H_
