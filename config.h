/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Source Code Pro:size=10" };
static const char dmenufont[]       = "Source Code Pro:size=10";

static const char base03[] = "#002b36";
static const char base02[] = "#073642";
static const char base01[] = "#586e75";
static const char base00[] = "#657b83";
static const char base0[] = "#839496";
static const char base1[] = "#93a1a1";
static const char base2[] = "#eee8d5";
static const char base3[] = "#fdf6e3";
static const char yellow[] = "#b58900";
static const char orange[] = "#cb4b16";
static const char red[] = "#dc322f";
static const char magenta[] = "#d33682";
static const char violet[] = "#6c71c4";
static const char blue[] = "#268bd2";
static const char cyan[] = "#2aa198";
static const char green[] = "#859900";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { base00, base03,  base02 },
	[SchemeSel]  = { base00, base02,  yellow  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "firefox",  NULL,       NULL,       1 << 1,       0,           -1 },
	{ "discord",  NULL,       NULL,       1 << 7,       0,           1 },
	{ "TelegramDesktop",  NULL,       NULL,       1 << 8,       0,           1 },
	{ "Slack",  NULL,       NULL,       1 << 6,       0,           1 },
	{ "eow-debug",  NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Steam",  NULL,       NULL,       1 << 2,       0,           -1 },
	{ "battle.net.exe",  NULL,       NULL,       1 << 2,       0,           -1 },
	{ "starcraft.exe",  NULL,       NULL,       1 << 0,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static const char *dmenucmd[] = { "rofi", "-show", "drun"};
static const char *druncmd[] = { "rofi", "-show", "run"};
static const char *sshcmd[] = { "rofi", "-show", "ssh"};
static const char *termcmd[]  = { "urxvt" };

static const char *volup[] = { "amixer", "set", "'Master'", "10%+" };
static const char *voldwn[] = { "amixer", "set", "'Master'", "10%-" };
static const char *monup[] = { "xbacklight" "-inc" "5" };
static const char *mondown[] = { "xbacklight" "-dec" "5" };
static const char *volmute[] = { "~/.bin/mute" };

static const char *grabmouse[] = {"xrandr", "--output", "DP-2", "--same-as", "HDMI-0"};
static const char *ungrabmouse[] = {"xrandr", "--output", "--primary", "DP-2", "--right-of", "HDMI-0"};


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_e,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_p,       spawn,          {.v = druncmd } },
	{ MODKEY,                       XK_s,       spawn,          {.v = sshcmd } },
	{ MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = termcmd } },

	{ 0,              XF86XK_AudioLowerVolume,  spawn,          {.v = voldwn } },
	{ 0,              XF86XK_AudioRaiseVolume,  spawn,          {.v = volup } },
	{ 0,              XF86XK_AudioMute,         spawn,          {.v = volmute } },
	{ 0,              XF86XK_MonBrightnessUp,   spawn,          {.v = monup } },
	{ 0,              XF86XK_MonBrightnessDown, spawn,          {.v = mondown } },

	{ MODKEY,                       XK_n,      spawn,           {.v = grabmouse } },
	{ MODKEY|ShiftMask,             XK_n,      spawn,           {.v = ungrabmouse } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_ampersand,              0)
	TAGKEYS(                        XK_bracketleft,            1)
	TAGKEYS(                        XK_braceleft,              2)
	TAGKEYS(                        XK_braceright,             3)
	TAGKEYS(                        XK_parenleft,              4)
	TAGKEYS(                        XK_equal,                  5)
	TAGKEYS(                        XK_asterisk,               6)
	TAGKEYS(                        XK_parenright,             7)
	TAGKEYS( 			XK_plus, 		   8)
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

