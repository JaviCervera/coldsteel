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
class IParticleSystemSceneNode : public ISceneNode { protected: IParticleSystemSceneNode(); };
class ITerrainSceneNode : public ISceneNode { protected: ITerrainSceneNode(); };
class IGUIElement { protected: IGUIElement(); };
class Shader { protected: Shader(); };

%include "src/common.h"
%include "src/bitwise.h"
%include "src/camera.h"
%include "src/channel.h"
%include "src/color.h"
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
%include "src/model.h"
%include "src/music.h"
%include "src/pixmap.h"
%include "src/screen.h"
%include "src/shader.h"
%include "src/sound.h"
%include "src/sprite.h"
%include "src/particle.h"
%include "src/string.h"
%include "src/surface.h"
%include "src/terrain.h"
%include "src/texture.h"
%include "src/water.h"
%include "src/world.h"
