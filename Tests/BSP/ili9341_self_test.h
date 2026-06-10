/**
  ******************************************************************************
  * @file    ili9341_self_test.h
  * @brief   ILI9341 self-test header
  *
  * Compile-time gated BSP test for ili9341.c. Define ILI9341_SELF_TEST to
  * enable; otherwise the test is compiled out with zero impact on the normal
  * production path.
  ******************************************************************************
  */

#ifndef __ILI9341_SELF_TEST_H
#define __ILI9341_SELF_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void ili9341_self_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __ILI9341_SELF_TEST_H */