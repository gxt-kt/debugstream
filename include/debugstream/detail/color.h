#pragma once
#include "../stdc++.h"

// clang-format off

namespace gxt {
namespace detail {

//--------------------------------------------------
static const char* DEBUG_STREAM_COLOR_FG_NORMAL = "\x1B[0m";
static const char* DEBUG_STREAM_COLOR_FG_BLACK  = "\x1B[30m";
static const char* DEBUG_STREAM_COLOR_FG_RED    = "\x1B[31m";
static const char* DEBUG_STREAM_COLOR_FG_GREEN  = "\x1B[32m";
static const char* DEBUG_STREAM_COLOR_FG_YELLOW = "\x1B[33m";
static const char* DEBUG_STREAM_COLOR_FG_BLUE   = "\x1B[34m";
static const char* DEBUG_STREAM_COLOR_FG_MAGENTA= "\x1B[35m";
static const char* DEBUG_STREAM_COLOR_FG_CYAN   = "\x1B[36m";
static const char* DEBUG_STREAM_COLOR_FG_WHITE  = "\x1B[37m";
//--------------------------------------------------
static const char* DEBUG_STREAM_COLOR_BG_NORMAL = "\x1B[49m";
static const char* DEBUG_STREAM_COLOR_BG_BLACK  = "\x1b[40m";
static const char* DEBUG_STREAM_COLOR_BG_RED    = "\x1b[41m";
static const char* DEBUG_STREAM_COLOR_BG_GREEN  = "\x1B[42m";
static const char* DEBUG_STREAM_COLOR_BG_YELLOW = "\x1B[43m";
static const char* DEBUG_STREAM_COLOR_BG_BLUE   = "\x1B[44m";
static const char* DEBUG_STREAM_COLOR_BG_MAGENTA= "\x1B[45m";
static const char* DEBUG_STREAM_COLOR_BG_CYAN   = "\x1B[46m";
static const char* DEBUG_STREAM_COLOR_BG_WHITE  = "\x1B[47m";
//--------------------------------------------------
class normal_fg_t {}; 
class black_fg_t  {}; 
class red_fg_t    {}; 
class green_fg_t  {}; 
class yellow_fg_t {}; 
class blue_fg_t   {}; 
class magenta_fg_t{}; 
class cyan_fg_t   {}; 
class white_fg_t  {}; 
//--------------------------------------------------
class normal_bg_t {}; 
class black_bg_t  {}; 
class red_bg_t    {}; 
class green_bg_t  {}; 
class yellow_bg_t {}; 
class blue_bg_t   {}; 
class magenta_bg_t{}; 
class cyan_bg_t   {}; 
class white_bg_t  {}; 
//--------------------------------------------------
class debug_general_t     {};
class debug_status_t      {};
class debug_warning_t     {};
class debug_error_t       {};
class debug_fatal_error_t {};
//--------------------------------------------------
inline void normal_fg (detail::normal_fg_t ) {}
inline void black_fg  (detail::black_fg_t  ) {}
inline void red_fg    (detail::red_fg_t    ) {}
inline void green_fg  (detail::green_fg_t  ) {}
inline void yellow_fg (detail::yellow_fg_t ) {}
inline void blue_fg   (detail::blue_fg_t   ) {}
inline void magenta_fg(detail::magenta_fg_t) {}
inline void cyan_fg   (detail::cyan_fg_t   ) {}
inline void white_fg  (detail::white_fg_t  ) {}
//--------------------------------------------------
inline void normal_bg (detail::normal_bg_t ) {}
inline void black_bg  (detail::black_bg_t  ) {}
inline void red_bg    (detail::red_bg_t    ) {}
inline void green_bg  (detail::green_bg_t  ) {}
inline void yellow_bg (detail::yellow_bg_t ) {}
inline void blue_bg   (detail::blue_bg_t   ) {}
inline void magenta_bg(detail::magenta_bg_t) {}
inline void cyan_bg   (detail::cyan_bg_t   ) {}
inline void white_bg  (detail::white_bg_t  ) {}
//--------------------------------------------------
// set the debug color
inline void GENERAL     (detail::debug_general_t)     {}
inline void STATUS      (detail::debug_status_t)      {}
inline void WARNING     (detail::debug_warning_t)     {}
inline void ERROR       (detail::debug_error_t)       {}
inline void FATAL_ERROR (detail::debug_fatal_error_t) {}
}  // namespace detail
}  // namespace gxt

// clang-format on
