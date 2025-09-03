/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int gappx = 10; /* gaps between windows */
static const unsigned int snap = 32; /* snap pixel */
static const int swallowfloating = 0; /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1; /* 0 means bottom bar */
static const char* fonts[] = {"JetBrainsMono Nerd Font:size=12"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=12";

static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#2d3149";

static const char fg[] = "#c6c8d1",
                  bg[] = "#161821",
                  hl[] = "#cccccc",
                  rhl[] = "#555555";

static const char* colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {fg, bg, bg},
    [SchemeSel] = {hl, col_cyan, bg},
};

static const unsigned int baralpha = 229.5;
static const unsigned int borderalpha = OPAQUE;

static const unsigned int alphas[][3] = {
    /*               fg      bg        border*/
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char* tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class            instance    title           tags mask   isfloating  isterminal  noswallow   monitor */
    {"Gimp",            NULL,       NULL,           0,          1,          0,          0,          -1},
    {"Firefox",         NULL,       NULL,           1 << 8,     0,          0,          -1,         -1},
    {"st-256color",     NULL,       NULL,           0,          0,          1,          0,          -1},
    {NULL,              NULL,       "Event Tester", 0,          0,          0,          1,          -1}, /* xev */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol       arrange function */
    {"[]=",         tile}, /* first entry is default */
    {"><>",         NULL}, /* no layout function means floating behavior */
    {"[M]",         monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
// move across different workspaces
// visualize one workspace into another workspaces
// move window in another workspace
// duplicate one window across multiple workspaces

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                          \
    {                                                       \
        .v = (const char*[]) { "/bin/sh", "-c", cmd, NULL } \
    }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char* dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", bg, "-nf", fg, "-sb", rhl, "-sf", hl, NULL};
static const char* passmenucmd[] = {"passmenu", "-fn", dmenufont, "-nb", bg, "-nf", fg, "-sb", rhl, "-sf", hl, NULL};
static const char* termcmd[] = {"st", NULL};
static const char* lfcmd[] = {"st", "-e", "lfrun", NULL};
static const char* neomuttcmd[] = {"st", "-e", "neomutt", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *screenshootcmd[] = {"/bin/sh", "-c", "screenshoot", NULL};
static const char* bookmarkscmd[] = {"bookmarks", NULL};

/* monitor brightness */
static const char* mon_light_up[] = {"xbacklight", "-inc", "10", NULL};
static const char* mon_light_down[] = {"xbacklight", "-dec", "10", NULL};

/* volume control */
static const char* audio_up[] = {"amixer", "set", "Master", "10%+", NULL};
static const char* audio_down[] = {"amixer", "set", "Master", "10%-", NULL};
static const char* audio_mute[] = {"amixer", "-D", "pulse", "set", "Master", "1+", "toogle", NULL};
static const char* audio_play[] = {"mpc", "play-pausse", NULL};
static const char* audio_stop[] = {"mpc", "stop", NULL};
static const char* audio_next[] = {"mpc", "next", NULL};
static const char* audio_prev[] = {"mpc", "prev", NULL};

/* keyboard brightness */
static const char* kbd_light_up[] = {"brightnessctl", "--device=smc::kbd_backlight", "set", "10%+", NULL};
static const char* kbd_light_down[] = {"brightnessctl", "--device=smc::kbd_backlight", "set", "10%-", NULL};

static const Key keys[] = {
	/* modifier                     key                         function        argument */
        { MODKEY,                       XK_space,                   spawn,          { .v = dmenucmd } },            // open dmenu bar
        { MODKEY,                       XK_Return,                  spawn,          { .v = termcmd } },             // open terminal
	{ MODKEY,                       XK_s,                       togglescratch,  { .v = scratchpadcmd } },       // open scratchpad
        { MODKEY,                       XK_p,                       spawn,          { .v = passmenucmd } },         // open passmenu bar
        { MODKEY,                       XK_e,                       spawn,          { .v = lfcmd } },               // open file explorer
        { MODKEY,                       XK_n,                       spawn,          { .v = neomuttcmd } },          // open email client
        { MODKEY,                       XK_w,                       spawn,          { .v = screenshootcmd } },      // take screenshoot 
        { MODKEY,                       XK_q,                       killclient,     { 0 } },                        // close window
        { MODKEY,                       XK_t,                       setlayout,      { .v = &layouts[0] } },         // standard stack layout
        { MODKEY,                       XK_m,                       setlayout,      { .v = &layouts[2] } },         // focus on particular window
        { MODKEY,                       XK_f,                       setlayout,      { .v = &layouts[1] } },         // all windows floating
        { MODKEY | ShiftMask,           XK_Return,                  zoom,           { 0 } },                        // promote to master
        { MODKEY | ShiftMask,           XK_f,                       togglefloating, { 0 } },                        // enter/exit floating mode for a single window
        { MODKEY,                       XK_j,                       focusstack,     { .i = +1 } },                  // move focus to next window
        { MODKEY,                       XK_k,                       focusstack,     { .i = -1 } },                  // move focus to previous window
        { MODKEY,                       XK_r,                       rotatestack,    { .i = -1 } },                  // rotate stack
        { MODKEY | ShiftMask,           XK_0,                       tag,            { .ui = ~0 } },                 // active window followig across workspaces
        { MODKEY,                       XK_h,                       setmfact,       { .f = -0.05 } },               // resize layout horizontally
        { MODKEY,                       XK_l,                       setmfact,       { .f = +0.05 } },
        { MODKEY,                       XK_i,                       incnmaster,     { .i = +1 } },                  // increase number of windows in master
        { MODKEY,                       XK_d,                       incnmaster,     { .i = -1 } },                  // decrease number of windows in master
        { MODKEY,                       XK_b,                       spawn,          { .v = bookmarkscmd } },
        { MODKEY,                       XK_0,                       view,           { .ui = ~0 } },                 // all windows in one workspace
        { MODKEY,                       XK_comma,                   focusmon,       { .i = -1 } },                  // multi screen options
        { MODKEY,                       XK_period,                  focusmon,       { .i = +1 } },
        { MODKEY | ShiftMask,           XK_comma,                   tagmon,         { .i = -1 } },
        { MODKEY | ShiftMask,           XK_period,                  tagmon,         { .i = +1 } },
        { 0,                            XF86XK_MonBrightnessUp,     spawn,          { .v = mon_light_up } },
        { 0,                            XF86XK_MonBrightnessDown,   spawn,          { .v = mon_light_down } },
        { 0,                            XF86XK_KbdBrightnessUp,     spawn,          { .v = kbd_light_up } },
        { 0,                            XF86XK_KbdBrightnessDown,   spawn,          { .v = kbd_light_down } },
        { 0,                            XF86XK_AudioRaiseVolume,    spawn,          { .v = audio_up } },
        { 0,                            XF86XK_AudioLowerVolume,    spawn,          { .v = audio_down } },
        { 0,                            XF86XK_AudioMute,           spawn,          { .v = audio_mute } },
        { 0,                            XF86XK_AudioPlay,           spawn,          { .v = audio_play } },
        { 0,                            XF86XK_AudioStop,           spawn,          { .v = audio_stop } },
        { 0,                            XF86XK_AudioNext,           spawn,          { .v = audio_next } },
        { 0,                            XF86XK_AudioPrev,           spawn,          { .v = audio_prev } },
        TAGKEYS(                        XK_1,                                       0)
        TAGKEYS(                        XK_2,                                       1)
        TAGKEYS(                        XK_3,                                       2)
        TAGKEYS(                        XK_4,                                       3)
        TAGKEYS(                        XK_5,                                       4)
        TAGKEYS(                        XK_6,                                       5)
        TAGKEYS(                        XK_7,                                       6)
        TAGKEYS(                        XK_8,                                       7)
        TAGKEYS(                        XK_9,                                       8)
        { MODKEY | ShiftMask,           XK_q,                       quit,           { 0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask                  button          function        argument */
	{ ClkLtSymbol,          0,                          Button1,        setlayout,      { 0 } },
        { ClkLtSymbol,          0,                          Button3,        setlayout,      { .v = &layouts[2] } },
        { ClkWinTitle,          0,                          Button2,        zoom,           { 0 } },
        { ClkStatusText,        0,                          Button2,        spawn,          { .v = termcmd } },
        { ClkClientWin,         MODKEY,                     Button1,        movemouse,      { 0 } },
        { ClkClientWin,         MODKEY,                     Button2,        togglefloating, { 0 } },
        { ClkClientWin,         MODKEY | ShiftMask,         Button1,        resizemouse,    { 0 } },
        { ClkTagBar,            0,                          Button1,        view,           { 0 } },
        { ClkTagBar,            0,                          Button3,        toggleview,     { 0 } },
        { ClkTagBar,            MODKEY,                     Button1,        tag,            { 0 } },
        { ClkTagBar,            MODKEY,                     Button3,        toggletag,      { 0 } },
};
