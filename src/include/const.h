#ifndef _FOS_CONST_H_
#define _FOS_CONST_H_

#define NR_IRQ          (16)
#define NR_SYS_CALL     (2)

#define TRUE            (1)
#define FALSE           (0)

#define NULL            (0)

#define BRIGHT          (0x80)
#define BLACK           (0x00)
#define MAZARINE        (0x01)          // 深蓝色
#define GREEN           (0x02)
#define CYAN            (0x03)          // 青色
#define RED             (0x04)
#define PINK            (0x05)
#define BROWN           (0x06)
#define INCANUS         (0x07)          // 灰白
#define GRAY            (0x08)
#define SAPPHIRINE      (0x09)          // 亮蓝
#define BRIGHT_GREEN    (0x0a)          // 亮绿
#define LIGHTCYAN       (0x0b)          // 亮青
#define BRIGHT_RED      (0x0c)          // 亮红
#define BRIGHT_PINK     (0x0d)          // 亮粉红
#define YELLOW          (0x0e)
#define GLOSS_WHITE     (0x0f)          // 亮白

#define MAKE_COLOR(background, foreground) ((background << 4) | foreground)
#define DEFAULT_CHAR_COLOR MAKE_COLOR(BLACK, INCANUS)

#define CRTC_ADDR_REG                   (0x3d4)
#define CRTC_DATA_REG                   (0x3d5)
#define CRTC_DATA_IDX_HORIZONTAL_TOTAL  (0x00)
#define CRTC_DATA_IDX_START_ADDR_H      (0x0c)
#define CRTC_DATA_IDX_START_ADDR_L      (0x0d)
#define CRTC_DATA_IDX_CURSOR_H          (0x0e)
#define CRTC_DATA_IDX_CURSOR_L          (0x0f)

#define DISABLE_INT __asm__ __volatile__("cli");
#define ENABLE_INT  __asm__ __volatile__("sti");

#define NR_CONSOLES (3)
#define V_MEM_BASE (0xB8000)
#define V_MEM_SIZE (0x7fff)

#define SCREEN_WIDTH (80)
#define SCREEN_HEIGHT (25)
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define SCROLL_SCREEN_UP (0)
#define SCROLL_SCREEN_DOWN (1)

#define PROC_NAME_LEN (32)

#endif /* _FOS_CONST_H_ */
