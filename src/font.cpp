#define STB_TRUETYPE_IMPLEMENTATION
#include "../lib/stb/stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION
#include "color.h"
#include "core.h"
#include "font.h"
#include "math.h"
#include "memblock.h"
#include "string.h"
#include "texture.h"
#include "pixmap.h"

extern "C"
{
  struct Font
  {
    ITexture *tex;
    stbtt_bakedchar glyphs[94];
    float height;
    float max_height;

    Font(const u8 *ttf, float height) : height(height)
    {
      // Bake font into alpha buffer
      int w = 256, h = 256;
      u8 *alpha_buffer = new u8[w * h];
      while (stbtt_BakeFontBitmap(ttf, 0, height, alpha_buffer, w, h, 32, sizeof(glyphs) / sizeof(glyphs[0]), glyphs) <= 0)
      {
        if (w == h)
          w *= 2;
        else
          h *= 2;
        delete[] alpha_buffer;
        alpha_buffer = new u8[w * h];
      }

      // Copy into pixels
      int *pixels = new int[w * h];
      for (size_t i = 0; i < w * h; ++i)
        pixels[i] = FadeColor(COLOR_WHITE, alpha_buffer[i]);
      delete[] alpha_buffer;

      // Create texture
      IImage *pixmap = CreatePixmap(w, h);
      for (int y = 0; y < h; ++y)
      {
        for (int x = 0; x < w; ++x)
        {
          WritePixel(pixmap, x, y, pixels[y * w + x]);
        }
      }
      delete[] pixels;
      tex = CreateTextureFromPixmap(pixmap);
      FreePixmap(pixmap);

      // Get max char height
      float x = 0, y = 0;
      float miny = 999999, maxy = -999999;
      stbtt_aligned_quad q;
      max_height = -999999;
      const size_t len = sizeof(glyphs) / sizeof(glyphs[0]);
      for (size_t i = 0; i < len; ++i)
      {
        stbtt_GetBakedQuad(glyphs, TextureWidth(tex), TextureHeight(tex), i, &x, &y, &q, true);
        miny = Min(miny, q.y0);
        maxy = Max(maxy, q.y1);
      }
      max_height = maxy - miny;
    }

    ~Font()
    {
      if (tex)
        _Device()->getVideoDriver()->removeTexture(tex);
    }
  };

  EXPORT Font *CALL LoadFont(const char *filename, int height)
  {
    if (stringc(Lower(ExtractExt(filename))) != "ttf")
      return NULL;
    Memblock *memblock = LoadMemblock(filename);
    if (!memblock)
      return NULL;
    Font *font = new Font((const u8 *)memblock, height);
    FreeMemblock(memblock);
    return font;
  }

  EXPORT void CALL FreeFont(Font *font)
  {
    delete font;
  }

  EXPORT void CALL DrawText(Font *font, const char *text, int x, int y, int color)
  {
    if (!font)
    {
      IGUIFont *irr_font = _Device()->getGUIEnvironment()->getBuiltInFont();
      const wchar_t *wtext = _WSTR(text);
      recti rect(vector2di(x, y), irr_font->getDimension(wtext));
      irr_font->draw(wtext, rect, _Color(color));
      return;
    }
    float fx = x;
    float fy = y + font->max_height;
    const size_t len = strlen(text);
    for (size_t i = 0; i < len; ++i)
    {
      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(
          font->glyphs,
          TextureWidth(font->tex),
          TextureHeight(font->tex),
          int(Min(text[i] - 32, 94)),
          &fx,
          &fy,
          &q,
          true);
      const float src_x = q.s0 * TextureWidth(font->tex);
      const float src_y = q.t0 * TextureHeight(font->tex);
      const float src_width = (q.s1 - q.s0) * TextureWidth(font->tex);
      const float src_height = (q.t1 - q.t0) * TextureHeight(font->tex);
      DrawTextureEx2(font->tex, src_x, src_y, src_width, src_height, q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0, color);
    }
  }

  EXPORT int CALL TextWidth(Font *font, const char *text)
  {
    if (!font)
      return _Device()->getGUIEnvironment()->getBuiltInFont()->getDimension(_WSTR(text)).Width;
    float x = 0, y = 0;
    stbtt_aligned_quad q;
    memset(&q, 0, sizeof(q));
    const size_t len = strlen(text);
    for (size_t i = 0; i < len; ++i)
    {
      stbtt_GetBakedQuad(font->glyphs, TextureWidth(font->tex), TextureHeight(font->tex), int(Min(text[i] - 32, 94)), &x, &y, &q, true);
    }
    return q.x1;
  }

  EXPORT int CALL TextHeight(Font *font, const char *text)
  {
    if (!font)
      return _Device()->getGUIEnvironment()->getBuiltInFont()->getDimension(_WSTR(text)).Height;
    float x = 0, y = 0, miny = 999999, maxy = -999999;
    stbtt_aligned_quad q;
    memset(&q, 0, sizeof(q));
    const size_t len = strlen(text);
    for (size_t i = 0; i < len; ++i)
    {
      stbtt_GetBakedQuad(font->glyphs, TextureWidth(font->tex), TextureHeight(font->tex), int(Min(text[i] - 32, 94)), &x, &y, &q, true);
      miny = Min(miny, q.y0);
      maxy = Max(maxy, q.y1);
    }
    return maxy - miny;
  }

} // extern "C"
