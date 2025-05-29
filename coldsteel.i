%module coldsteel
%{
    #include "engine.h"
%}

typedef bool bool_t;
class ISceneNode { protected: ISceneNode(); };
class ICameraSceneNode : public ISceneNode { protected: ICameraSceneNode(); };
class ILightSceneNode : public ISceneNode { protected: ILightSceneNode(); };
class IMeshSceneNode : public ISceneNode { protected: IMeshSceneNode(); };
class IBillboardSceneNode : public ISceneNode { protected: IBillboardSceneNode(); };
class ITerrainSceneNode : public ISceneNode { protected: ITerrainSceneNode(); };

%include "src/common.h"
%include "src/bitwise.h"
%include "src/camera.h"
%include "src/channel.h"
%include "src/clipboard.h"
%include "src/color.h"
%include "src/console.h"
%include "src/core.h"
%include "src/dialogs.h"
%include "src/dir.h"
%include "src/entity.h"
%include "src/font.h"
%include "src/graphics.h"
%include "src/input.h"
%include "src/light.h"
%include "src/listener.h"
%include "src/log.h"
%include "src/material.h"
%include "src/math.h"
%include "src/memblock.h"
%include "src/memblock_files.h"
%include "src/mesh.h"
%include "src/model.h"
%include "src/music.h"
%include "src/pixmap.h"
%include "src/screen.h"
%include "src/sound.h"
%include "src/sprite.h"
%include "src/string.h"
%include "src/string_files.h"
%include "src/surface.h"
%include "src/terrain.h"
%include "src/texture.h"
%include "src/world.h"
%include "src/xml.h"
