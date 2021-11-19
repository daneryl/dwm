/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { "monospace:size=12", "MesloLGM Nerd Font:size=12" }; */
static const char *fonts[]          = { "MesloLGM Nerd Font:size=12" };
static const char col_black[]       = "#282c34";
static const char col_red[]         = "#E06C75";
static const char col_green[]       = "#98C379";
static const char col_blue[]        = "#61AFEF";
static const char col_magenta[]     = "#C678DD";
static const char col_gray[]        = "#ABB2BF";
static const char col_cyan[]        = "#282c34";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_blue, col_black, col_black },
	[SchemeSel]  = { col_gray, col_cyan,  col_magenta},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance            title           tags mask       isfloating   monitor    float x,y,w,h         floatborderpx*/
	{ NULL,       "whatsapp",         NULL,            1,              0,           1 },
	{ NULL,       "nest",             NULL,            1,              0,           1 },
	{ NULL,       "huridata",         NULL,            1,              0,           1 },
	{ NULL,       NULL,              "quick terminal", 1,              1,           0,        -1,-1,1300,1265,        5  },
	{ NULL,       "calendar",         NULL,            2,              0,           1 },
	{ NULL,       NULL,              "Helper-Window",  2,              0,           1 },
};

static const MonitorRule monrules[] = {
	/* monitor layout  mfact  nmaster  showbar  topbar */
	{  1,      3,      -1,     0,      -1,      -1     }, // use a different layout for the second monitor
	{  -1,     0,      -1,    -1,      -1,      -1     }, // default
};

/* layout(s) */
static const float mfact     = 0.75; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ ">M>",      centeredfloatingmaster },
	{ "|M|",      centeredmaster },
	{ "[M]",      monocle },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-l", "10", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *passmenucmd[]  = { "passmenu", "-l", "10", NULL };
static const char *clipmenucmd[]  = { "clipmenu", NULL };
static const char *quickterm[]  = { "quick_terminal", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = passmenucmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = clipmenucmd } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer --allow-boost --increase 5; dwmblocks_signal.sh 10") },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer --allow-boost --decrease 5; dwmblocks_signal.sh 10") },
	{ 0,                            XF86XK_AudioMute, spawn, SHCMD("pamixer --toggle-mute; dwmblocks_signal.sh 10") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	/* { MODKEY,                       XK_Tab,    view,           {0} }, */
	{ MODKEY,                       XK_semicolon,  spawn,          {.v = quickterm } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	/* { MODKEY,                       XK_space,  setlayout,      {0} }, */
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

