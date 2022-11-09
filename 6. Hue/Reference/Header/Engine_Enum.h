#ifndef Engine_Enum_h__
#define Engine_Enum_h__

BEGIN(Engine)

enum CHANNELID { BGM, PLAYER, MONSTER, SKULL, INTERACT, INTERACT2, ROCK, EFFECT, RAIN,
	KEYEFFECT, UI, CAMERA, BALLOON, TABLE, BLACKHOLE, FIRE, FIREWORK, FIREEXPLODE, MAXCHANNEL };

enum RESOURCETYPE { RESOURCE_STATIC, RESOURCE_DYNAMIC, RESOURCE_END };
enum BUFFERID {
	BUFFER_TRICOL, BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_RCYTEX, BUFFER_TERRAINCOL, BUFFER_TERRAINTEX, BUFFER_CIRCLECOL,
	BUFFER_CUBECOL, BUFFER_CUBE_NORMAL, BUFFER_CUBE_FRONT, BUFFER_CUBE_CullB, BUFFER_CUBE_CullL, BUFFER_CUBE_CullR, BUFFER_CUBE_CullLR, BUFFER_CUBE_CullBackAndB,
	BUFFER_CUBE_Player, BUFFER_CUBE_SkyBox, BUFFER_UVTEX, BUFFER_RCMOUSE, BUFFER_END
};
enum OBJDECOID {
	OBJDECO_TOPFIRE, OBJDECO_FIREPOT, OBJDECO_LEFTLANTERN, OBJDECO_RIGHTLANTERN, OBJDECOID_END
};

/************************ (SongSong) ************************/
/*  Ŭ����Ÿ���� �߰��� �� ������ �˷��ּ���~ Tool���� �����ϱ� (��ü������ ����������)  */
enum CUBE_CLIPPING_TYPE { CUBE_NORMAL, CUBE_FRONT, CUBE_CullB, CUBE_CullL, CUBE_CullR, CUBE_CullLR, CUBE_CullBackAndB, CUBE_Player, CUBE_SkyBox, CUBE_TYPE_END };

enum CUBE_COLOR { CUBE_RED, CUBE_ORANGE, CUBE_GREEN, CUBE_YELLOW, CUBE_BLUE, CUBE_SKYBLUE, CUBE_PINK, CUBE_PURPLE, CUBE_COLOR_END };

enum CUBE_MOVING_DIR { CUBE_UP, CUBE_DOWN, CUBE_LEFT, CUBE_RIGHT, CUBE_MOVING_DIR_END};

enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };

enum ANGLETYPE { ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END };
END
#endif // Engine_Enum_h__