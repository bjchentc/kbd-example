/*
 * Copyright (c) 2025. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MAIN_H
#define MAIN_H

#define MAKE_EVENT              0
#define BREAK_EVENT             1
#define REPEAT_EVENT            2

/**
 * @brief Initialize the keyboard device
 * @return 0 on success, -1 if device is not ready
 */
int kbd_init(void);

#endif /* MAIN_H */
