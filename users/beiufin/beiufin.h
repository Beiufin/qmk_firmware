#include "quantum.h"

#ifdef TAP_REPEAT_RGB
#    include "tap_repeat.h"
#endif

#ifdef KEY_LOCK_TOGGLE_RGB
#    include "key_lock_toggle.h"
#endif

#define MILLISECONDS_IN_SECOND 1000

#if !defined(RGB_MATRIX_MAXIMUM_BRIGHTNESS) || RGB_MATRIX_MAXIMUM_BRIGHTNESS > UINT8_MAX
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS UINT8_MAX
#endif

#if !defined(RGB_MATRIX_HUE_STEP)
#    define RGB_MATRIX_HUE_STEP 8
#endif

#if !defined(RGB_MATRIX_SAT_STEP)
#    define RGB_MATRIX_SAT_STEP 16
#endif

#if !defined(RGB_MATRIX_VAL_STEP)
#    define RGB_MATRIX_VAL_STEP 16
#endif

#if !defined(RGB_MATRIX_SPD_STEP)
#    define RGB_MATRIX_SPD_STEP 16
#endif

// These are just to make it neater to use builtin HSV values in the keymap
// clang-format off
#define WHITE {HSV_WHITE}
#define RED {HSV_RED}
#define CORAL {HSV_CORAL}
#define ORANGE {HSV_ORANGE}
#define GOLDEN {HSV_GOLDENROD}
#define GOLD {HSV_GOLD}
#define YELLOW {HSV_YELLOW}
#define CHART {HSV_CHARTREUSE}
#define GREEN {HSV_GREEN}
#define SPRING {HSV_SPRINGGREEN}
#define TURQ {HSV_TURQUOISE}
#define TEAL {HSV_TEAL}
#define CYAN {HSV_CYAN}
#define AZURE {HSV_AZURE}
#define BLUE {HSV_BLUE}
#define PURPLE {HSV_PURPLE}
#define MAGENT {HSV_MAGENTA}
#define PINK {HSV_PINK}
#define HSV_CREAM 24, 240, 255
#define CREAM {HSV_CREAM}
#define HSV_9B59B5 208, 192, 255
#define M9B59B5 {HSV_9B59B5}

enum beiufin_keycodes {
    BEI_CRM = SAFE_RANGE,
    BEI_PRP,
    BEI_RED,
    BEI_GRN,
    BEI_BLU,
    BEI_CYN,
    BEI_MGT,
    BEI_YEL,
    BEI_KEY,
    BEI_WHT,
    NEW_SAFE_RANGE
};

// clang-format on

#define MODS_SHIFT (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT (get_mods() & MOD_MASK_ALT)

enum beiufin_layout_names {
    _KL = 0, // Keys Layout: The main keyboard layout that has all the characters
    _FL,     // Function Layout: The function key activated layout with default functions and some added ones
    SAFE_LAYER_RANGE,
};
