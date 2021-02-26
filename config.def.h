/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char startup[] = "$HOME/.local/bin/startup.sh";
static char *fonts[]          = { "SpaceMono Nerd Font:style=Regular:size=10", "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#282c34";
static const char col_gray2[]       = "#353b45";
static const char col_gray3[]       = "#3e4451";
static const char col_white[]       = "#abb2bf";
static const char col_blue[]        = "#61afef";
static const char *colors[][3]      = {
	/*               fg          bg         border   */
	[SchemeNorm]  = { col_white, col_gray1, col_gray3 },
	[SchemeSel]   = { col_white, col_gray2, col_blue  },
};

/* tagging */
static const char *tags[] = { "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι", "κ"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    	      instance       title      tags mask      iscentered     isfloating  monitor */
	{ "vimus",	      NULL,	     NULL,	1 << 0,	       0,	      0,	  -1 },
	{ "newsboat",	      NULL,	     NULL,	1 << 9,	       0,	      0,	  -1 },
	{ "calcurse",	      NULL,	     NULL,	1 << 8,	       0,	      0,	  -1 },
	{ "Gimp",  	      NULL,	     NULL,	1 << 7,        0,             0,          -1 },
	{ "discord",          NULL,          NULL,      1 << 1,        0,             0,          -1 },
	{ "Sxiv",	      NULL,	     NULL,	0,	       0,             1,	  -1 },
	{ "floating",         NULL,	     NULL,	0,	       1,             1,	  -1 },
	{ "Gpick",            NULL,          NULL,      0,  	       0,             1,          -1 },
	{ "St",               NULL,          NULL,      0,             0,             0,          -1 },
	{ "mpv",              NULL,          NULL, 	0,             0,             1,          -1 },
	{ "Pavucontrol",      NULL,          NULL,      0,     	       0,             1,          -1 },
	{ "Pdf2png",          NULL,          NULL,      0,             0,             1,          -1 },
	{ "hakuneko-desktop", NULL,          NULL,      0,             0,             1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "gaplessgrid.c"
#include "tatami.c"
#include "tcl.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      gaplessgrid},
	{ "|+|",      tatami},
	{ "|||",      tcl},
	{ NULL,       NULL},
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
static const char *ftermcmd[] = { "st", "-c", "floating", NULL};
static const char *dmenucmd[] = { "dmenu_run", NULL};
static const char *pscrot[] = {"scrot", "'%Y-%m-%d-%s_$wx$h.png'", "-e", "'mv $f ~/Images/screenshots/'", "&&", "notify-send", "--icon=itmages-information", "\"Screenshot\"" "\"Screenshot saved to /home/luc/Images/screenshots\""};
static const char *layoutmenu_cmd = "layoutmenu";

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        			function        argument */
	{ MODKEY,                       XK_d,      			spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return, 			spawn,          SHCMD("st") },
	{ MODKEY|ShiftMask,		XK_Return, 			spawn,	   	{.v = ftermcmd } }, 
	{ MODKEY|ShiftMask,		XK_d,	   			spawn,	   	SHCMD("networkmanager_dmenu")}, 
	{ MODKEY|ShiftMask,		XK_m,	   			spawn,	   	SHCMD("clipmenu")}, 
	{ MODKEY|ShiftMask,		XK_p,	   			spawn,	   	SHCMD("dmenuunicode")}, 
	{ MODKEY,			XK_space,  			spawn,	   	SHCMD("pmenu.sh")}, 
	{ MODKEY,			XK_p,	   			spawn,	   	SHCMD("thunar")}, 
	{ MODKEY,			XK_t,	   			spawn,	   	SHCMD("firefox")}, 
	{ MODKEY|ShiftMask,		XK_t,	   			spawn,	   	SHCMD("qutebrowser")}, 
	{ 0,				XK_Print,  			spawn,	   	SHCMD("perscrot")}, 
	{ MODKEY|ShiftMask,		XK_Print,  			spawn,	   	{.v = pscrot } }, 
	{ 0,				XF86XK_AudioLowerVolume,  	spawn,	   	SHCMD("amixer set Master 1%-, NULL")}, 
	{ 0,				XF86XK_AudioRaiseVolume,  	spawn,	   	SHCMD("amixer set Master 1%+")}, 
	{ 0,				XF86XK_AudioMute,  		spawn,		SHCMD("amixer -q set Master toggle")}, 
	{ MODKEY|ShiftMask,		XK_e,				spawn,		SHCMD("powermenu")}, 
	{ MODKEY|ShiftMask,		XK_z,				spawn,		SHCMD("lock.sh")}, 
	{ MODKEY|ShiftMask,		XK_i,				spawn,		SHCMD("current-wal")}, 
	{ MODKEY,			XK_i,				spawn,		SHCMD("random-wall")}, 
	{ MODKEY,			XK_w,				spawn,		SHCMD("walc")}, 
	{ MODKEY|ShiftMask,		XK_w,				spawn,		SHCMD("livewalc")},
	{ 0,				XF86XK_AudioPlay,		spawn,		SHCMD("mpc toggle")}, 
	{ 0,				XF86XK_AudioPause,		spawn,		SHCMD("mpc toggle")}, 
	{ 0,				XF86XK_AudioNext,		spawn,		SHCMD("mpc next")}, 
	{ 0,				XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev")}, 
	{ 0,				XF86XK_AudioStop,		spawn,		SHCMD("mpc stop")}, 
	{ 0,				XF86XK_MonBrightnessDown,	spawn,		SHCMD("backlight-control dec")}, 
	{ 0,				XF86XK_MonBrightnessUp,		spawn,		SHCMD("backlight-control inc")}, 
	{ MODKEY,			XK_b,				spawn,		SHCMD("toggle-screenkey")}, 
	{ MODKEY,			XK_F1,				spawn,		SHCMD("dmenufm /home/luc/.local/share/documents/")}, 
	{ MODKEY,			XK_F1,				spawn,		SHCMD("dmenufm /home/luc/work/")}, 
	{ MODKEY,                       XK_q,      			togglebar,      {0} },
	{ MODKEY,                       XK_Left,   			focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,  			focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Left,   			movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Right,  			movestack,      {.i = -1 } },
	{ MODKEY,                       XK_o,      			incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      			incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      			setmfact,       {.f = -0.05} },
	{ MODKEY,		        XK_l,      			setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, 			zoom,           {0} },
	{ MODKEY,                       XK_Tab,    			view,           {0} },
	{ MODKEY|ShiftMask,             XK_a,      			killclient,     {0} },
	{ MODKEY|ShiftMask,		XK_z,	   			cyclelayout,	{.i = -1 } },
	{ MODKEY,			XK_z,	   			cyclelayout,	{.i = +1 } },
	{ MODKEY,			XK_f,	   			fullscreen,	{0} },
	{ MODKEY|ShiftMask,             XK_space,  			togglefloating, {0} },
	{ MODKEY,                       XK_0,      			view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      			tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_ampersand,              			0)
	TAGKEYS(                        XK_eacute,                 			1)
	TAGKEYS(                        XK_quotedbl,               			2)
	TAGKEYS(                        XK_apostrophe,             			3)
	TAGKEYS(                        XK_parenleft,              			4)
	TAGKEYS(                        XK_minus,                  			5)
	TAGKEYS(                        XK_egrave,                 			6)
	TAGKEYS(                        XK_underscore,             			7)
	TAGKEYS(                        XK_ccedilla,               			8)
	TAGKEYS(			XK_agrave,		   			9)
	{ MODKEY|ShiftMask,             XK_q,      			quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD("st") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

