# TODO
- [ ] MenuBar / MenuItems
- [ ] Extended color selection 
- [ ] Histogram -> add width and height as parameter
- [ ] Diagram -> add width and height as parameter
- [ ] extract colors into GUISettings
- [ ] add Enumeration for Colors
- [ ] PushStyle(GuiStyle style, int value);
- [ ] PushStyle(GuiStyle style, float value);
- [ ] PushStyle(GuiStyle style, ds::vec2 value);
- [ ] PushStyle(GuiStyle style, ds::vec3 value);
- [ ] PushStyle(GuiStyle style, ds::vec4 value);
- [ ] PushStyle(GuiStyle style, ds::Color value);
- [ ] PopStyle(GuiStyle style);
- [ ] implement "hot" selection
- [ ] overall slider on the right when dialog too long for screen
- [ ] Slider Angle (shows degress but returns radians)

## 14.03
- [x] Slider
- [x] List Box
- [x] Open/Close boxes
- [x] DropDown Box

# Playground

## Style handling 

StyleBuffer {
char* data;
int size;
}

StyleBuffer currentBuffer;
StyleBuffer orgBuffer;

int styleOffsets[32]

addStyle(int index,void* data,int size);

void* getStyle(int index);



enum GuiStyle {
    GS_BACKGROUND_COLOR
};

guiCtx:
    GuiStyle styles[32];
    int styleElementSize[32];


class MenuModel {

public:
    void addMenu(const char* label);
    void addMenuItem(int menuId, int id, const char* label);
    int numMenus();
    const char* getMenuLabel(int index);
    int numItems(int menuId);
    const char* getMenuItemLabel(int menuId, int index);
    void selectMenu(int menuId);
    int getActiveMenu();
        
}


if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
}