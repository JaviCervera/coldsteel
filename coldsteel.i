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

%include "src/common.h"
%include "src/camera.h"
%include "src/channel.h"
%include "src/core.h"
%include "src/dir.h"
%include "src/entity.h"
%include "src/font.h"
%include "src/graphics.h"
%include "src/gui.h"
%include "src/input.h"
%include "src/light.h"
%include "src/listener.h"
%include "src/log.h"
%include "src/material.h"
%include "src/math.h"
%include "src/memblock.h"
%include "src/mesh.h"
%include "src/mesh_entity.h"
%include "src/music.h"
%include "src/screen.h"
%include "src/sound.h"
%include "src/sprite.h"
%include "src/string.h"
%include "src/surface.h"
%include "src/texture.h"
%include "src/world.h"
%include "src/xml.h"
