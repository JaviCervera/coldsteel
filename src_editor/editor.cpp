#include "../_build/sdk/sdk.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <irrString.h>
#include <irrArray.h>

#include "../src/screen.h"
#include "../src/gui.h"
#undef EXPORT
#undef CALL

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define CALL __stdcall
#else
#define EXPORT
#define CALL
#endif

#define KEY_TAB 0x09
#define KEY_ESC 0x1B

#define COLOR_GREEN -16711936

static ColdSteelSDK *g_sdk = NULL;
static Font *g_fixedsys = NULL;
static irr::core::stringc g_bindir;

static bool isEmpty(const char *s)
{
  return !s || !s[0];
}

static bool isDirty(Control *tabbar, const irr::core::array<irr::core::stringc> &saved, int idx)
{
  if (idx <= 0 || idx > (int)saved.size())
    return false;
  Control *tab = g_sdk->ControlChild(tabbar, idx);
  if (!tab)
    return false;
  Control *eb = g_sdk->ControlChild(tab, 1);
  if (!eb)
    return false;
  const char *text = g_sdk->ControlText(eb);
  if (saved[idx - 1] == "")
    return text && text[0];
  return saved[idx - 1] != text;
}

static void updateTabLabel(Control *tabbar, const irr::core::array<irr::core::stringc> &names,
                           const irr::core::array<irr::core::stringc> &saved, int idx)
{
  if (idx <= 0)
    return;
  Control *tab = g_sdk->ControlChild(tabbar, idx);
  if (!tab)
    return;
  irr::core::stringc label;
  if (idx <= (int)names.size() && names[idx - 1] != "")
    label = names[idx - 1];
  else
    label = "<untitled>";
  if (isDirty(tabbar, saved, idx))
    label = irr::core::stringc("* ") + label;
  g_sdk->SetControlText(tab, label.c_str());
}

static int newTab(Control *tabbar, irr::core::array<irr::core::stringc> &names,
                  irr::core::array<irr::core::stringc> &saved,
                  int headerHeight, const char *filename, const char *content)
{
  int idx = g_sdk->AddControlItem(tabbar, "<untitled>", 0);
  Control *tab = g_sdk->ControlChild(tabbar, idx);
  int w = g_sdk->ScreenWidth();
  int h = g_sdk->ScreenHeight() - headerHeight - 20;
  Control *eb = g_sdk->CreateEditBox(0, 0, w, h, EDITBOX_MULTILINE, tab);
  g_sdk->SetControlFont(eb, g_fixedsys);
  g_sdk->SetControlColor(eb, g_sdk->MultiplyColor(COLOR_GREEN, 0.5f));

  while (idx > (int)names.size())
    names.push_back(irr::core::stringc());
  while (idx > (int)saved.size())
    saved.push_back(irr::core::stringc());

  if (filename)
    names[idx - 1] = filename;
  else
    names[idx - 1] = "";
  if (content)
    saved[idx - 1] = content;
  else
    saved[idx - 1] = "";

  if (content)
    g_sdk->SetControlText(eb, content);

  updateTabLabel(tabbar, names, saved, idx);
  g_sdk->SelectControlItem(tabbar, idx);
  g_sdk->SetFocusedControl(eb);

  if (filename)
  {
    irr::core::stringc title = irr::core::stringc("ColdSteel - ") + filename;
    g_sdk->SetScreenTitle(title.c_str());
  }
  return idx;
}

static void saveTab(Control *tabbar, irr::core::array<irr::core::stringc> &names,
                    irr::core::array<irr::core::stringc> &saved, int idx, const char *path)
{
  if (idx <= 0)
    return;
  Control *tab = g_sdk->ControlChild(tabbar, idx);
  if (!tab)
    return;
  Control *eb = g_sdk->ControlChild(tab, 1);
  if (!eb)
    return;

  FILE *file = fopen(path, "w");
  if (!file)
    return;
  const char *text = g_sdk->ControlText(eb);
  if (text)
    fprintf(file, "%s", text);
  fclose(file);

  if (idx <= (int)names.size())
    names[idx - 1] = path;
  if (idx <= (int)saved.size())
    saved[idx - 1] = text ? text : "";

  updateTabLabel(tabbar, names, saved, idx);
  irr::core::stringc title = irr::core::stringc("ColdSteel - ") + path;
  g_sdk->SetScreenTitle(title.c_str());
}

static void performClose(Control *tabbar, irr::core::array<irr::core::stringc> &names,
                         irr::core::array<irr::core::stringc> &saved, int idx)
{
  if (idx <= 0 || idx > g_sdk->ControlNumItems(tabbar))
    return;
  g_sdk->RemoveControlItem(tabbar, idx);
  if (idx <= (int)names.size())
    names.erase(idx - 1);
  if (idx <= (int)saved.size())
    saved.erase(idx - 1);
  if (g_sdk->ControlNumItems(tabbar) == 0)
  {
    newTab(tabbar, names, saved, 0, NULL, NULL);
  }
  else
  {
    int active = g_sdk->SelectedControlItem(tabbar);
    if (active == 0)
    {
      active = 1;
      g_sdk->SelectControlItem(tabbar, 1);
    }
    Control *tab = g_sdk->ControlChild(tabbar, active);
    if (tab)
    {
      Control *eb = g_sdk->ControlChild(tab, 1);
      if (eb)
        g_sdk->SetFocusedControl(eb);
    }
  }
}

static void closeAllTabs(Control *tabbar, irr::core::array<irr::core::stringc> &names,
                         irr::core::array<irr::core::stringc> &saved)
{
  g_sdk->RemoveControlItems(tabbar);
  names.clear();
  saved.clear();
  newTab(tabbar, names, saved, 0, NULL, NULL);
}

static void focusActiveTab(Control *tabbar)
{
  int active = g_sdk->SelectedControlItem(tabbar);
  if (active > 0)
  {
    Control *tab = g_sdk->ControlChild(tabbar, active);
    if (tab)
    {
      Control *eb = g_sdk->ControlChild(tab, 1);
      if (eb)
        g_sdk->SetFocusedControl(eb);
    }
  }
}

static void resizeTabs(Control *tabbar, int headerHeight)
{
  int w = g_sdk->ScreenWidth();
  int h = g_sdk->ScreenHeight() - headerHeight - 20;
  g_sdk->SetControlShape(tabbar, 0, headerHeight, w, h);
  int n = g_sdk->ControlNumItems(tabbar);
  for (int i = 1; i <= n; i++)
  {
    Control *tab = g_sdk->ControlChild(tabbar, i);
    Control *eb = g_sdk->ControlChild(tab, 1);
    if (eb)
      g_sdk->SetControlShape(eb, 0, 0, w, h);
  }
}

extern "C"
{

  EXPORT int CALL editor_load(ColdSteelSDK *sdk, const char *bindir)
  {
    g_sdk = sdk;
    g_bindir = bindir ? bindir : "";
    return COLDSTEEL_SDK_VERSION;
  }

  EXPORT void CALL editor_run()
  {
    g_sdk->SetDriver(DRIVER_SOFTWAREFAST);
    g_sdk->OpenScreen(640, 480, g_sdk->DesktopDepth(), SCREEN_RESIZABLE);
    g_sdk->SetScreenFPS(60);
    g_sdk->SetScreenTitle("ColdSteel - <untitled>");

    Font *liberationSans = g_sdk->LoadFont((g_bindir + "/editor/segoe_ui.xml").c_str());
    g_fixedsys = g_sdk->LoadFont((g_bindir + "/editor/fixedsys.xml").c_str());
    g_sdk->SetControlFont(NULL, liberationSans);

    g_sdk->SetSkinColor(SKINCOLOR_WINDOW, g_sdk->RGB(60, 60, 60));
    g_sdk->SetSkinColor(SKINCOLOR_3D_FACE, g_sdk->RGB(50, 50, 50));
    g_sdk->SetSkinColor(SKINCOLOR_3D_DARK_SHADOW, g_sdk->RGB(30, 30, 30));
    g_sdk->SetSkinColor(SKINCOLOR_3D_SHADOW, g_sdk->RGB(40, 40, 40));
    g_sdk->SetSkinColor(SKINCOLOR_3D_HIGH_LIGHT, g_sdk->RGB(72, 72, 72));
    g_sdk->SetSkinColor(SKINCOLOR_3D_LIGHT, g_sdk->RGB(80, 80, 80));
    g_sdk->SetSkinColor(SKINCOLOR_BUTTON_TEXT, g_sdk->RGB(210, 210, 210));
    g_sdk->SetSkinColor(SKINCOLOR_GRAY_TEXT, g_sdk->RGB(120, 120, 120));
    g_sdk->SetSkinColor(SKINCOLOR_HIGH_LIGHT, g_sdk->RGB(80, 120, 180));
    g_sdk->SetSkinColor(SKINCOLOR_HIGH_LIGHT_TEXT, g_sdk->RGB(255, 255, 255));
    g_sdk->SetSkinColor(SKINCOLOR_WINDOW_SYMBOL, g_sdk->RGB(200, 200, 200));
    g_sdk->SetSkinColor(SKINCOLOR_GRAY_WINDOW_SYMBOL, g_sdk->RGB(100, 100, 100));
    g_sdk->SetSkinColor(SKINCOLOR_EDITABLE, g_sdk->RGB(0, 0, 0));
    g_sdk->SetSkinColor(SKINCOLOR_FOCUSED_EDITABLE, g_sdk->RGB(10, 10, 10));
    g_sdk->SetSkinColor(SKINCOLOR_GRAY_EDITABLE, g_sdk->RGB(20, 20, 20));
    g_sdk->SetSkinColor(SKINCOLOR_SCROLLBAR, g_sdk->RGB(40, 40, 40));
    g_sdk->SetSkinColor(SKINCOLOR_ACTIVE_CAPTION, g_sdk->RGB(210, 210, 210));
    g_sdk->SetSkinColor(SKINCOLOR_INACTIVE_CAPTION, g_sdk->RGB(140, 140, 140));
    g_sdk->SetSkinColor(SKINCOLOR_TOOLTIP_BACKGROUND, g_sdk->RGB(70, 70, 70));
    g_sdk->SetSkinColor(SKINCOLOR_TOOLTIP, g_sdk->RGB(220, 220, 220));
    g_sdk->SetSkinColor(SKINCOLOR_ACTIVE_BORDER, g_sdk->RGB(100, 100, 100));
    g_sdk->SetSkinColor(SKINCOLOR_INACTIVE_BORDER, g_sdk->RGB(50, 50, 50));
    g_sdk->SetSkinColor(SKINCOLOR_APP_WORKSPACE, g_sdk->RGB(40, 40, 40));
    g_sdk->SetSkinColor(SKINCOLOR_ICON, g_sdk->RGB(200, 200, 200));
    g_sdk->SetSkinColor(SKINCOLOR_ICON_HIGH_LIGHT, g_sdk->RGB(255, 255, 255));

    Texture *icons[4];
    icons[0] = g_sdk->LoadTexture((g_bindir + "/editor/new.png").c_str());
    icons[1] = g_sdk->LoadTexture((g_bindir + "/editor/open.png").c_str());
    icons[2] = g_sdk->LoadTexture((g_bindir + "/editor/save.png").c_str());
    icons[3] = g_sdk->LoadTexture((g_bindir + "/editor/close.png").c_str());

    Control *mainMenu = g_sdk->CreateMenu("", NULL);

    Control *fileMenu = g_sdk->CreateMenu("File", mainMenu);
    g_sdk->AddControlItem(fileMenu, "New", 101);
    g_sdk->AddControlItem(fileMenu, "Open", 102);
    g_sdk->AddControlItem(fileMenu, "Save", 103);
    g_sdk->AddControlItem(fileMenu, "Save As...", 104);
    g_sdk->AddControlItem(fileMenu, "Save All", 105);
    g_sdk->AddMenuSeparator(fileMenu);
    g_sdk->AddControlItem(fileMenu, "Close", 106);
    g_sdk->AddControlItem(fileMenu, "Close All", 107);
    g_sdk->AddMenuSeparator(fileMenu);
    g_sdk->AddControlItem(fileMenu, "Next File", 108);
    g_sdk->AddControlItem(fileMenu, "Previous File", 109);
    g_sdk->AddMenuSeparator(fileMenu);
    g_sdk->AddControlItem(fileMenu, "Exit", 110);

    g_sdk->CreateMenu("Edit", mainMenu);
    g_sdk->CreateMenu("Program", mainMenu);
    g_sdk->CreateMenu("Help", mainMenu);

    Control *toolbar = g_sdk->CreateToolbar(NULL);
    for (int i = 0; i < 4; i++)
    {
      g_sdk->AddControlItem(toolbar, "   ", 101 + i);
      Control *btn = g_sdk->ControlChild(toolbar, i + 1);
      if (btn && icons[i])
        g_sdk->SetControlTexture(btn, icons[i], 1);
    }

    int headerHeight = g_sdk->ControlHeight(mainMenu) + g_sdk->ControlHeight(toolbar);

    Control *tabbar = g_sdk->CreateTabbar(
        0, headerHeight,
        g_sdk->ScreenWidth(), g_sdk->ScreenHeight() - headerHeight - 20,
        NULL);
    g_sdk->SetTabBarHeight(tabbar, 20);
    g_sdk->SetTabFocusNavigation(0);

    irr::core::array<irr::core::stringc> tabNames;
    irr::core::array<irr::core::stringc> tabSaved;

    int dialogState = 0; // 0=none, 1=SAVE_AS, 2=CLOSE_PROMPT, 3=OVERWRITE
    int dialogTabIdx = 0;
    irr::core::stringc dialogFilename;
    bool dialogCloseAfter = false;

    newTab(tabbar, tabNames, tabSaved, headerHeight, NULL, NULL);

    bool shouldExit = false;

    while (!g_sdk->ScreenShouldClose() && !g_sdk->KeyHit(KEY_ESC) && !shouldExit)
    {
      while (g_sdk->PrepareNextGUIEvent())
      {
        int eventType = g_sdk->GUIEventType();
        Control *eventCtrl = g_sdk->GUIEventControl();
        int menuId = g_sdk->GUIEventMenuId();

        if (eventType == CONTROL_ACTION)
        {
          if (menuId != -1)
          {
            if (menuId == 101)
            {
              newTab(tabbar, tabNames, tabSaved, headerHeight, NULL, NULL);
            }
            else if (menuId == 102)
            {
              g_sdk->CreateFileOpenDialog("Open File", NULL);
            }
            else if (menuId == 103)
            {
              int active = g_sdk->SelectedControlItem(tabbar);
              if (active > 0)
              {
                if (active <= (int)tabNames.size() && !tabNames[active - 1].empty())
                  saveTab(tabbar, tabNames, tabSaved, active, tabNames[active - 1].c_str());
                else
                {
                  dialogState = 1;
                  dialogTabIdx = active;
                  dialogCloseAfter = false;
                  g_sdk->CreateFileOpenDialog("Save As...", NULL);
                }
              }
            }
            else if (menuId == 104)
            {
              int active = g_sdk->SelectedControlItem(tabbar);
              if (active > 0)
              {
                dialogState = 1;
                dialogTabIdx = active;
                dialogCloseAfter = false;
                g_sdk->CreateFileOpenDialog("Save As...", NULL);
              }
            }
            else if (menuId == 105)
            {
              int n = g_sdk->ControlNumItems(tabbar);
              for (int i = 1; i <= n; i++)
              {
                if (isDirty(tabbar, tabSaved, i) && i <= (int)tabNames.size() && !tabNames[i - 1].empty())
                  saveTab(tabbar, tabNames, tabSaved, i, tabNames[i - 1].c_str());
              }
            }
            else if (menuId == 106)
            {
              int active = g_sdk->SelectedControlItem(tabbar);
              if (active > 0 && isDirty(tabbar, tabSaved, active))
              {
                dialogState = 2;
                dialogTabIdx = active;
                g_sdk->CreateMessageBox("ColdSteel", "Save changes?",
                                        EMBF_YES | EMBF_NO | EMBF_CANCEL);
              }
              else if (active > 0)
              {
                performClose(tabbar, tabNames, tabSaved, active);
              }
            }
            else if (menuId == 107)
            {
              closeAllTabs(tabbar, tabNames, tabSaved);
            }
            else if (menuId == 108)
            {
              int n = g_sdk->ControlNumItems(tabbar);
              if (n > 0)
              {
                int active = g_sdk->SelectedControlItem(tabbar);
                active = (active < n) ? active + 1 : 1;
                g_sdk->SelectControlItem(tabbar, active);
                focusActiveTab(tabbar);
              }
            }
            else if (menuId == 109)
            {
              int n = g_sdk->ControlNumItems(tabbar);
              if (n > 0)
              {
                int active = g_sdk->SelectedControlItem(tabbar);
                active = (active > 1) ? active - 1 : n;
                g_sdk->SelectControlItem(tabbar, active);
                focusActiveTab(tabbar);
              }
            }
            else if (menuId == 110)
            {
              shouldExit = true;
            }
          }
          else
          {
            if (g_sdk->ControlType(eventCtrl) == CONTROL_TABBAR)
            {
              focusActiveTab(tabbar);
            }
            else if (g_sdk->ControlType(eventCtrl) == CONTROL_BUTTON)
            {
              int btnId = g_sdk->ControlId(eventCtrl);
              if (btnId == 101)
                newTab(tabbar, tabNames, tabSaved, headerHeight, NULL, NULL);
              else if (btnId == 102)
                g_sdk->CreateFileOpenDialog("Open File", NULL);
              else if (btnId == 103)
              {
                int active = g_sdk->SelectedControlItem(tabbar);
                if (active > 0)
                {
                  if (active <= (int)tabNames.size() && !tabNames[active - 1].empty())
                    saveTab(tabbar, tabNames, tabSaved, active, tabNames[active - 1].c_str());
                  else
                  {
                    dialogState = 1;
                    dialogTabIdx = active;
                    dialogCloseAfter = false;
                    g_sdk->CreateFileOpenDialog("Save As...", NULL);
                  }
                }
              }
              else if (btnId == 104)
              {
                int active = g_sdk->SelectedControlItem(tabbar);
                if (active > 0 && isDirty(tabbar, tabSaved, active))
                {
                  dialogState = 2;
                  dialogTabIdx = active;
                  g_sdk->CreateMessageBox("ColdSteel", "Save changes?",
                                          EMBF_YES | EMBF_NO | EMBF_CANCEL);
                }
                else if (active > 0)
                {
                  performClose(tabbar, tabNames, tabSaved, active);
                }
              }

              Control *parent = g_sdk->ControlParent(eventCtrl);
              if (parent)
              {
                const char *parentTitle = g_sdk->ControlText(parent);
                if (parentTitle && strcmp(parentTitle, "Save As...") == 0)
                {
                  Control *nameBox = g_sdk->ControlChild(parent, 8);
                  if (nameBox)
                  {
                    const char *editText = g_sdk->ControlText(nameBox);
                    const char *cwd = g_sdk->CurrentDir();
                    if (editText && editText[0] && strcmp(editText, cwd) != 0)
                    {
                      irr::core::stringc fullPath;
                      if (editText[0] == '/')
                        fullPath = editText;
                      else
                        fullPath = irr::core::stringc(cwd) + "/" + editText;

                      int tabIdx = dialogTabIdx;
                      if (tabIdx > 0)
                      {
                        FILE *test = fopen(fullPath.c_str(), "r");
                        if (test)
                        {
                          fclose(test);
                          dialogState = 3;
                          dialogFilename = fullPath;
                          g_sdk->CreateMessageBox("Save As", "File exists. Overwrite?",
                                                  EMBF_YES | EMBF_NO);
                        }
                        else
                        {
                          saveTab(tabbar, tabNames, tabSaved, tabIdx, fullPath.c_str());
                          if (dialogCloseAfter)
                            performClose(tabbar, tabNames, tabSaved, tabIdx);
                          dialogState = 0;
                        }
                        g_sdk->FreeControl(parent);
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if (eventType == CONTROL_FILESELECTED)
        {
          const char *path = g_sdk->DialogResult();
          if (path && path[0])
          {
            const char *title = g_sdk->ControlText(eventCtrl);
            if (title && strcmp(title, "Save As...") == 0)
            {
              int tabIdx = dialogTabIdx;
              FILE *test = fopen(path, "r");
              if (test)
              {
                fclose(test);
                dialogState = 3;
                dialogFilename = path;
                g_sdk->CreateMessageBox("Save As", "File exists. Overwrite?",
                                        EMBF_YES | EMBF_NO);
              }
              else
              {
                saveTab(tabbar, tabNames, tabSaved, tabIdx, path);
                if (dialogCloseAfter)
                  performClose(tabbar, tabNames, tabSaved, tabIdx);
                dialogState = 0;
              }
            }
            else
            {
              FILE *file = fopen(path, "r");
              if (file)
              {
                fseek(file, 0, SEEK_END);
                long len = ftell(file);
                fseek(file, 0, SEEK_SET);
                char *buf = (char *)malloc(len + 1);
                if (buf)
                {
                  fread(buf, 1, len, file);
                  buf[len] = '\0';
                }
                fclose(file);
                newTab(tabbar, tabNames, tabSaved, headerHeight,
                       path, buf);
                free(buf);
              }
            }
          }
        }
        else if (eventType == CONTROL_FILECANCELLED)
        {
          dialogState = 0;
        }
        else if (eventType == CONTROL_MESSAGEBOX_YES)
        {
          if (dialogState == 3)
          {
            int tabIdx = dialogTabIdx;
            saveTab(tabbar, tabNames, tabSaved, tabIdx, dialogFilename.c_str());
            if (dialogCloseAfter)
              performClose(tabbar, tabNames, tabSaved, tabIdx);
            dialogState = 0;
          }
          else if (dialogState == 2)
          {
            int tabIdx = dialogTabIdx;
            if (tabIdx <= (int)tabNames.size() && tabNames[tabIdx - 1] != "")
            {
              saveTab(tabbar, tabNames, tabSaved, tabIdx,
                      tabNames[tabIdx - 1].c_str());
              performClose(tabbar, tabNames, tabSaved, tabIdx);
            }
            else
            {
              dialogState = 1;
              dialogTabIdx = tabIdx;
              dialogCloseAfter = true;
              g_sdk->CreateFileOpenDialog("Save As...", NULL);
            }
            dialogState = 0;
          }
        }
        else if (eventType == CONTROL_MESSAGEBOX_NO)
        {
          if (dialogState == 3)
          {
            dialogState = 1;
            g_sdk->CreateFileOpenDialog("Save As...", NULL);
          }
          else if (dialogState == 2)
          {
            performClose(tabbar, tabNames, tabSaved, dialogTabIdx);
            dialogState = 0;
          }
        }
        else if (eventType == CONTROL_MESSAGEBOX_CANCEL)
        {
          if (dialogState != 0)
            dialogState = 0;
        }
      }

      if (g_sdk->KeyHit(KEY_TAB))
      {
        int active = g_sdk->SelectedControlItem(tabbar);
        if (active > 0)
        {
          Control *tab = g_sdk->ControlChild(tabbar, active);
          if (tab)
          {
            Control *eb = g_sdk->ControlChild(tab, 1);
            if (eb)
            {
              g_sdk->SetFocusedControl(eb);
              const char *text = g_sdk->ControlText(eb);
              int pos = g_sdk->EditBoxCursorPos(eb);
              irr::core::stringc newText;
              if (text)
              {
                irr::core::stringc s(text);
                newText = s.subString(0, pos) + "    " + s.subString(pos, s.size() - pos);
              }
              else
              {
                newText = "    ";
              }
              g_sdk->SetControlText(eb, newText.c_str());
              g_sdk->SetEditBoxCursorPos(eb, pos + 4);
            }
          }
        }
      }

      resizeTabs(tabbar, headerHeight);
      g_sdk->ClearScreen(g_sdk->RGB(40, 40, 40));
      g_sdk->DrawGUI();
      g_sdk->RefreshScreen();
    }

    g_sdk->CloseScreen();
  }

} // extern "C"
