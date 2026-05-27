#include <irrlicht.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum GUIId
{
    ID_SAVE = 101,
    ID_ABOUT,
    ID_BROWSE,
    ID_FILE_DIALOG,
    ID_FONT_PATH,
    ID_SIZE_COMBO,
    ID_BITMAP_SIZE_COMBO,
    ID_PREVIEW_IMAGE
};

static IrrlichtDevice *g_device = NULL;
static IVideoDriver *g_driver = NULL;
static IGUIEnvironment *g_gui = NULL;
static IGUIEditBox *g_font_path = NULL;
static IGUIComboBox *g_size_combo = NULL;
static IGUIComboBox *g_bitmap_combo = NULL;
static IGUIImage *g_preview = NULL;
static ITexture *g_preview_tex = NULL;

static const rect<s32> PREVIEW_CONTAINER(8, 64, 522, 410);

static const wchar_t *toWide(const char *s)
{
    static wchar_t buf[1024];
    mbstowcs(buf, s, 1023);
    buf[1023] = 0;
    return buf;
}

static const char *toNarrow(const wchar_t *ws)
{
    static char buf[1024];
    wcstombs(buf, ws, 1023);
    buf[1023] = 0;
    return buf;
}

static std::string getBaseName(const std::string &path)
{
    size_t last = path.find_last_of("/\\");
    std::string base = (last != std::string::npos) ? path.substr(last + 1) : path;
    size_t dot = base.find_last_of('.');
    if (dot != std::string::npos)
        base = base.substr(0, dot);
    return base;
}

static std::string getDirName(const std::string &path)
{
    size_t last = path.find_last_of("/\\");
    if (last != std::string::npos)
        return path.substr(0, last + 1);
    return std::string("./");
}

struct CharInfo
{
    int x0, y0, x1, y1;
    int underhang, overhang;
};

static bool packFontAtlas(
    const char *ttf_path, int font_size, int atlas_w, int atlas_h,
    unsigned char **alpha_out, CharInfo chars_out[94], int *line_height_out)
{
    FILE *f = fopen(ttf_path, "rb");
    if (!f)
        return false;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *ttf_data = new unsigned char[fsize];
    fread(ttf_data, 1, fsize, f);
    fclose(f);

    stbtt_fontinfo fi;
    if (!stbtt_InitFont(&fi, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0)))
    {
        delete[] ttf_data;
        return false;
    }

    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&fi, &ascent, &descent, &line_gap);
    float scale = stbtt_ScaleForPixelHeight(&fi, (float)font_size);

    int baseline = (int)ceil(ascent * scale);
    int line_h = (int)ceil((ascent - descent) * scale);

    /* First pass: ensure line_h actually fits all glyph pixel extents */
    {
        int c;
        for (c = 0; c < 94; c++)
        {
            int ix0, iy0, ix1, iy1;
            stbtt_GetCodepointBitmapBox(&fi, c + 32, scale, scale, &ix0, &iy0, &ix1, &iy1);
            int needed = baseline + iy1;
            if (needed > line_h) line_h = needed;
        }
    }
    *line_height_out = line_h;

    unsigned char *alpha = new unsigned char[atlas_w * atlas_h];
    memset(alpha, 0, atlas_w * atlas_h);
    *alpha_out = alpha;

    int cur_x = 0, cur_y = 0;
    bool all_fit = true;
    int c;
    for (c = 0; c < 94; c++)
    {
        int ch = c + 32;
        int ix0, iy0, ix1, iy1;
        stbtt_GetCodepointBitmapBox(&fi, ch, scale, scale, &ix0, &iy0, &ix1, &iy1);
        int gw = ix1 - ix0;
        int gh = iy1 - iy0;

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fi, ch, &advance, &lsb);
        int xadvance = (int)ceil(advance * scale);

        if (cur_x + gw + 1 >= atlas_w)
        {
            cur_x = 0;
            cur_y += line_h + 1;
        }
        if (cur_y + line_h > atlas_h)
            all_fit = false;

        if (gw > 0 && gh > 0)
        {
            int dst_y = cur_y + baseline + iy0;
            if (dst_y >= 0 && dst_y + gh <= atlas_h && cur_x >= 0 && cur_x + gw <= atlas_w)
            {
                stbtt_MakeCodepointBitmap(&fi,
                    alpha + dst_y * atlas_w + cur_x,
                    gw, gh, atlas_w,
                    scale, scale, ch);
            }
        }

        chars_out[c].x0 = cur_x;
        chars_out[c].y0 = cur_y;
        chars_out[c].x1 = cur_x + gw;
        chars_out[c].y1 = cur_y + line_h;
        chars_out[c].underhang = ix0;
        chars_out[c].overhang = xadvance - ix0 - gw;

        cur_x += gw + 1;
    }

    delete[] ttf_data;
    return all_fit;
}

static void updatePreview(const char *ttf_path, int font_size, int atlas_w, int atlas_h)
{
    unsigned char *alpha = NULL;
    CharInfo chars[94];
    int line_h = 0;
    if (!packFontAtlas(ttf_path, font_size, atlas_w, atlas_h, &alpha, chars, &line_h))
    {
        if (alpha) delete[] alpha;
        return;
    }

    unsigned char *rgba = new unsigned char[atlas_w * atlas_h * 4];
    int i;
    for (i = 0; i < atlas_w * atlas_h; i++)
    {
        rgba[i * 4 + 0] = 255;
        rgba[i * 4 + 1] = 255;
        rgba[i * 4 + 2] = 255;
        rgba[i * 4 + 3] = alpha[i];
    }
    delete[] alpha;

    IImage *img = g_driver->createImageFromData(ECF_A8R8G8B8, dimension2du(atlas_w, atlas_h), rgba, false, false);
    delete[] rgba;
    if (!img)
        return;

    if (g_preview_tex)
    {
        g_driver->removeTexture(g_preview_tex);
        g_preview_tex = NULL;
    }
    g_preview_tex = g_driver->addTexture("__preview_atlas", img);
    img->drop();

    if (g_preview && g_preview_tex)
    {
        int cw = PREVIEW_CONTAINER.getWidth();
        int ch = PREVIEW_CONTAINER.getHeight();
        int fit = cw < ch ? cw : ch;
        int display = atlas_w < fit ? atlas_w : fit;
        int cx = PREVIEW_CONTAINER.UpperLeftCorner.X + (cw - display) / 2;
        int cy = PREVIEW_CONTAINER.UpperLeftCorner.Y + (ch - display) / 2;
        g_preview->setScaleImage(display < atlas_w);
        g_preview->setRelativePosition(rect<s32>(cx, cy, cx + display, cy + display));
        g_preview->setImage(g_preview_tex);
    }
}

static bool saveFont(const char *ttf_path, int font_size, int atlas_w, int atlas_h)
{
    unsigned char *alpha = NULL;
    CharInfo chars[94];
    int line_h = 0;
    bool all_fit = packFontAtlas(ttf_path, font_size, atlas_w, atlas_h, &alpha, chars, &line_h);
    if (!alpha)
        return false;

    unsigned char *rgba = new unsigned char[atlas_w * atlas_h * 4];
    int i;
    for (i = 0; i < atlas_w * atlas_h; i++)
    {
        rgba[i * 4 + 0] = 255;
        rgba[i * 4 + 1] = 255;
        rgba[i * 4 + 2] = 255;
        rgba[i * 4 + 3] = alpha[i];
    }
    delete[] alpha;

    std::string base_name = getBaseName(std::string(ttf_path));
    std::string dir_name = getDirName(std::string(ttf_path));

    char size_suffix[32];
    sprintf(size_suffix, "_%d_%d", font_size, atlas_w);
    std::string out_base = dir_name + base_name + size_suffix;

    char png_path[1024];
    sprintf(png_path, "%s.png", out_base.c_str());
    IImage *img_save = g_driver->createImageFromData(
        ECF_A8R8G8B8, dimension2du(atlas_w, atlas_h), rgba, false, false);
    delete[] rgba;
    if (img_save)
    {
        g_driver->writeImageToFile(img_save, toWide(png_path));
        img_save->drop();
    }

    const char *png_filename = strrchr(png_path, '/');
    if (!png_filename)
        png_filename = strrchr(png_path, '\\');
    if (!png_filename)
        png_filename = png_path;
    else
        png_filename++;

    char xml_path[1024];
    sprintf(xml_path, "%s.xml", out_base.c_str());
    FILE *xml = fopen(xml_path, "w");
    if (!xml)
        return false;

    fprintf(xml, "<?xml version=\"1.0\"?>\n<font type=\"bitmap\">\n");
    fprintf(xml, "<Texture index=\"0\" filename=\"%s\" hasAlpha=\"true\" />\n", png_filename);

    int c;
    for (c = 0; c < 94; c++)
    {
        int ch = c + 32;

        char c_attr[16];
        if (ch == '"')
            strcpy(c_attr, "&quot;");
        else if (ch == '&')
            strcpy(c_attr, "&amp;");
        else if (ch == '<')
            strcpy(c_attr, "&lt;");
        else if (ch == '>')
            strcpy(c_attr, "&gt;");
        else
        {
            c_attr[0] = (char)ch;
            c_attr[1] = '\0';
        }

        fprintf(xml, "<c c=\"%s\" u=\"%d\" o=\"%d\" i=\"0\" r=\"%d, %d, %d, %d\" />\n",
                c_attr, chars[c].underhang, chars[c].overhang,
                chars[c].x0, chars[c].y0, chars[c].x1, chars[c].y1);
    }

    fprintf(xml, "</font>\n");
    fclose(xml);

    return all_fit;
}

class EventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(const SEvent &e)
    {
        if (e.EventType == EET_GUI_EVENT)
        {
            s32 id = e.GUIEvent.Caller->getID();
            if (e.GUIEvent.EventType == EGET_BUTTON_CLICKED)
            {
                if (id == ID_SAVE)
                {
                    doSave();
                    return true;
                }
                if (id == ID_ABOUT)
                {
                    doAbout();
                    return true;
                }
                if (id == ID_BROWSE)
                {
                    g_gui->addFileOpenDialog(L"Select TTF font", true, NULL, ID_FILE_DIALOG);
                    return true;
                }
            }
            if (e.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED)
            {
                if (id == ID_SIZE_COMBO || id == ID_BITMAP_SIZE_COMBO)
                {
                    doPreview();
                    return true;
                }
            }
            if (e.GUIEvent.EventType == EGET_FILE_SELECTED)
            {
                IGUIFileOpenDialog *dlg = (IGUIFileOpenDialog *)e.GUIEvent.Caller;
                const wchar_t *fn = dlg->getFileName();
                if (fn && g_font_path)
                {
                    g_font_path->setText(fn);
                    doPreview();
                }
                return true;
            }
            if (e.GUIEvent.EventType == EGET_EDITBOX_ENTER)
            {
                if (id == ID_FONT_PATH)
                {
                    doPreview();
                    return true;
                }
            }
        }
        return false;
    }

private:
    void doPreview()
    {
        const char *path = toNarrow(g_font_path->getText());
        if (!path || path[0] == '\0')
            return;
        int font_size = getSelectedSize();
        int atlas_size = getSelectedAtlasSize();
        updatePreview(path, font_size, atlas_size, atlas_size);
    }

    void doSave()
    {
        const char *path = toNarrow(g_font_path->getText());
        if (!path || path[0] == '\0')
            return;
        int font_size = getSelectedSize();
        int atlas_size = getSelectedAtlasSize();
        bool ok = saveFont(path, font_size, atlas_size, atlas_size);
        if (ok)
            g_gui->addMessageBox(L"Font Tool", L"Font saved successfully.");
        else
            g_gui->addMessageBox(L"Font Tool", L"Warning: not all characters fit in the atlas. Try a larger atlas size.");
    }

    void doAbout()
    {
        g_gui->addMessageBox(L"About Font Tool", L"Font Tool for ColdSteel\nConverts TTF files to Irrlicht bitmap fonts.\nUses stb_truetype for font rasterization.");
    }

    int getSelectedSize()
    {
        static const int sizes[] = {8, 10, 12, 14, 16, 18, 20, 22, 24, 28, 32, 36, 48, 64};
        s32 idx = g_size_combo->getSelected();
        if (idx < 0 || idx >= (s32)(sizeof(sizes) / sizeof(sizes[0])))
            return 16;
        return sizes[idx];
    }

    int getSelectedAtlasSize()
    {
        static const int atlas_sizes[] = {128, 256, 512};
        s32 idx = g_bitmap_combo->getSelected();
        if (idx < 0 || idx >= (s32)(sizeof(atlas_sizes) / sizeof(atlas_sizes[0])))
            return 256;
        return atlas_sizes[idx];
    }
};

static void buildGUI()
{
    int y = 8;
    g_gui->addStaticText(L"Font file:", rect<s32>(8, y + 2, 76, y + 18), false, false, NULL, -1, false);
    g_font_path = g_gui->addEditBox(L"", rect<s32>(78, y, 424, y + 20), true, NULL, ID_FONT_PATH);
    g_gui->addButton(rect<s32>(426, y, 510, y + 20), NULL, ID_BROWSE, L"Browse", L"Browse for a TTF font file");

    y += 28;
    g_gui->addStaticText(L"Size (px):", rect<s32>(8, y + 2, 72, y + 18), false, false, NULL, -1, false);
    g_size_combo = g_gui->addComboBox(rect<s32>(74, y, 162, y + 20), NULL, ID_SIZE_COMBO);
    g_size_combo->addItem(L"8");
    g_size_combo->addItem(L"10");
    g_size_combo->addItem(L"12");
    g_size_combo->addItem(L"14");
    g_size_combo->addItem(L"16");
    g_size_combo->addItem(L"18");
    g_size_combo->addItem(L"20");
    g_size_combo->addItem(L"22");
    g_size_combo->addItem(L"24");
    g_size_combo->addItem(L"28");
    g_size_combo->addItem(L"32");
    g_size_combo->addItem(L"36");
    g_size_combo->addItem(L"48");
    g_size_combo->addItem(L"64");
    g_size_combo->setSelected(4); /* default: 16px */

    g_gui->addStaticText(L"Atlas:", rect<s32>(168, y + 2, 210, y + 18), false, false, NULL, -1, false);
    g_bitmap_combo = g_gui->addComboBox(rect<s32>(212, y, 300, y + 20), NULL, ID_BITMAP_SIZE_COMBO);
    g_bitmap_combo->addItem(L"128");
    g_bitmap_combo->addItem(L"256");
    g_bitmap_combo->addItem(L"512");
    g_bitmap_combo->setSelected(1); /* default: 256 */

    g_gui->addButton(rect<s32>(308, y, 378, y + 20), NULL, ID_SAVE, L"Save", L"Save font files");
    g_gui->addButton(rect<s32>(382, y, 452, y + 20), NULL, ID_ABOUT, L"About", L"About Font Tool");

    g_preview = g_gui->addImage(PREVIEW_CONTAINER, NULL, ID_PREVIEW_IMAGE, L"", true);
    g_preview->setScaleImage(false);
}

int main(int argc, char *argv[])
{
    EventReceiver receiver;
    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
    params.WindowSize = dimension2du(530, 420);
    params.Bits = 32;
    params.Fullscreen = false;
    params.Vsync = false;
    params.EventReceiver = &receiver;

    g_device = createDeviceEx(params);
    if (!g_device)
    {
        params.DriverType = EDT_SOFTWARE;
        g_device = createDeviceEx(params);
    }
    if (!g_device)
        return 1;

    g_device->setWindowCaption(L"Font Tool");
    g_driver = g_device->getVideoDriver();
    g_gui = g_device->getGUIEnvironment();

    IGUISkin *skin = g_gui->createSkin(EGST_WINDOWS_CLASSIC);
    if (skin)
    {
        int i;
        for (i = 0; i < EGDC_COUNT; ++i)
        {
            SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
            col.setAlpha(255);
            skin->setColor((EGUI_DEFAULT_COLOR)i, col);
        }
        g_gui->setSkin(skin);
        skin->drop();
    }

    buildGUI();

    while (g_device->run())
    {
        g_driver->beginScene(true, true, SColor(255, 180, 180, 180));
        g_driver->draw2DRectangle(SColor(255, 0, 0, 0), PREVIEW_CONTAINER);
        g_gui->drawAll();
        g_driver->endScene();
    }

    g_device->drop();
    return 0;
}
