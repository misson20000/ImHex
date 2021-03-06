#include "views/view_help.hpp"

namespace hex {

    ViewHelp::ViewHelp() {

    }

    ViewHelp::~ViewHelp() {

    }

    void ViewHelp::drawAboutPopup() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(450, 300), ImVec2(450, 300));
        if (ImGui::BeginPopupModal("About", &this->m_aboutWindowOpen, ImGuiWindowFlags_NoResize)) {

            ImGui::Text("ImHex Hex Editor");
            ImGui::Text("by WerWolv");
            ImGui::Separator();
            ImGui::NewLine();

            ImGui::Text("Source code found at"); ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.4F, 0.4F, 0.8F, 1.0F), "https://github.com/WerWolv/ImHex");
            ImGui::NewLine();

            ImGui::Separator();
            ImGui::Text("Libraries used");
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2F, 0.2F, 0.2F, 0.3F));
            ImGui::BeginChild("##scroll");
            ImGui::NewLine();
            ImGui::BulletText("ImGui by ocornut");
            ImGui::BulletText("imgui_club by ocornut");
            ImGui::BulletText("ImGui-Addons by gallickgunner");
            ImGui::BulletText("ImGuiColorTextEdit by BalazsJako");
            ImGui::BulletText("capstone by aquynh");
            ImGui::NewLine();
            ImGui::BulletText("GNU libmagic");
            ImGui::BulletText("OpenSSL libcrypto");
            ImGui::BulletText("GLFW3");

            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }
    }

    void ViewHelp::drawPatternHelpPopup() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(450, 300), ImVec2(450, 300));

        constexpr static auto DrawTitle = [](const std::string &title) {
            ImGui::TextColored(ImVec4(0.6F, 0.6F, 1.0F, 1.0F), title.c_str());
        };

        constexpr static auto DrawCodeSegment = [](const std::string &id, size_t height, const std::string &code) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2F, 0.2F, 0.2F, 0.3F));
            ImGui::NewLine();
            ImGui::BeginChild(id.c_str(), ImVec2(-1, height));

            ImGui::Text("%s", code.c_str());

            ImGui::EndChild();
            ImGui::NewLine();
            ImGui::PopStyleColor();
        };

        ImGui::SetNextWindowSizeConstraints(ImVec2(450, 300), ImVec2(2000, 1000));
        if (ImGui::BeginPopupModal("Cheat Sheet", &this->m_patternHelpWindowOpen)) {
            ImGui::Text("ImHex Pattern Language Cheat Sheet");
            ImGui::Separator();
            ImGui::NewLine();

            DrawTitle("Preprocessor directives");
            ImGui::TextWrapped(
                    "Preprocessor directives can be used to alter the code before it's being parsed. Supported are "
                    "#define to replace one string with another and #include to include a separate file");
            DrawCodeSegment("preprocessor", 40,
                            "#define HEADER_OFFSET 0x100\n"
                            "#include <cstdint.hexpat>\n"
                            "#include \"mypattern.hexpat\""
            );

            DrawTitle("Pragma directives");
            ImGui::TextWrapped(
                    "Pragma directives are used to give ImHex additional information about the code being read. Currently "
                    "the following directives are supported.");
            DrawCodeSegment("pragma", 30,
                            "// Allow this file to be loaded as pattern when a file identified as application/x-executable gets loaded"
                            "#pragma MIME application/x-executable\n"
            );

            DrawTitle("Built-in types");
            ImGui::TextWrapped(
                    "The following built-in types are available for use");
            DrawCodeSegment("built-in", 80,
                    "u8, s8\n"
                    "u16, s16\n"
                    "u32, s32\n"
                    "u64, s64\n"
                    "u128, s128\n"
                    "float, double"
                    );

            DrawTitle("Variables and Arrays");
            ImGui::TextWrapped(
                    "Normal variables as well as arrays are used to highlight and display values. "
                    "It is possible to create arrays within structs and unions that use the value of a previously "
                    "declared variable as size.");
            DrawCodeSegment("vars arrays", 45,
                    "u32 variable;\n"
                    "s8 string[16];\n"
                    "u8 customSizedArray[variable];"
            );

            DrawTitle("Structs");
            ImGui::TextWrapped(
                    "To bundle multiple variables together, a struct can be used. To insert padding bytes which won't show "
                    "up in the pattern data view or be highlighted, use the padding[size] syntax.");
            DrawCodeSegment("struct", 85,
                "struct Header {\n"
                "   u32 magic;\n"
                "   u8 version;\n"
                "   padding[4];\n"
                "   Flags flags;\n"
                "};"
            );

            DrawTitle("Unions");
            ImGui::TextWrapped(
                    "A union is used to make two or more variables occupy the same region of memory. "
                    "The union will have the size of the biggest contained variable.");
            DrawCodeSegment("union", 55,
                            "union Color {\n"
                            "   u32 rgba;\n"
                            "   Components components;\n"
                            "};"
            );

            DrawTitle("Pointers");
            ImGui::TextWrapped(
                    "\"Another possible type of member in structs and unions are pointers. They are variables"
                    "whose value is used as an offset from the start of the file to locate the actual offset. "
                    "The leading type is treated as the data being pointed to and the trailing type as the size of the pointer.");
            DrawCodeSegment("pointer", 55,
                            "Data *data : u16;"
            );

            DrawTitle("Bitfields");
            ImGui::TextWrapped(
                    "To decode values that are stored in fields that don't follow the typical 8 bit alignment, bitfields can be used. "
                    "The size of these fields get specified in numbers of bits.");
            DrawCodeSegment("bitfield", 70,
                            "bitfield Permission {\n"
                            "   r : 1;\n"
                            "   w : 1;\n"
                            "   x : 1;\n"
                            "};"
            );

            DrawTitle("Enum");
            ImGui::TextWrapped(
                    "If a value can only be a few specific values with special meaning, an enum can be used. "
                    "The underlying type has to be specified using a unsigned, built-in type after the name. "
                    "Entries can be listed with or without a value. The values start counting at zero and increase by one "
                    "for every next entry");
            DrawCodeSegment("enum", 70,
                    "enum OperatingSystem : u8 {\n"
                    "   Windows = 0x10,\n"
                    "   MacOSX,\n"
                    "   Linux\n"
                    "};"
            );

            DrawTitle("Using declarations");
            ImGui::TextWrapped(
                    "A using declaration can be used to create type aliases for already existing types. This can be "
                    "a built-in type, a struct, enum or another alias type.");
            DrawCodeSegment("using", 15,
                            "using magic_t = u32;"
            );

            DrawTitle("Comments");
            ImGui::TextWrapped(
                    "To create a comment the C // or /*  */ syntax can be used. //-style comments end at the next new line "
                    "and /*-style comments only end when at the next */.");
            DrawCodeSegment("comment", 55,
                            "// This is a single line comment\n\n"
                            "/* This is a\n"
                            "multiline comment */"
            );

            DrawTitle("Variable placement");
            ImGui::TextWrapped(
                    "In order to highlight bytes and displaying their value in the pattern data window, "
                    "a variable needs to be created and placed in memory. The following line of code creates"
                    "a unsigned 32 bit variable named data and places it at offset 0x100."
                    );
            DrawCodeSegment("var placement", 15, "u32 data @ 0x100;");
            ImGui::EndPopup();
        }
    }

    void ViewHelp::createView() {
        this->drawAboutPopup();
        this->drawPatternHelpPopup();
    }

    void ViewHelp::createMenu() {
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About", ""))
                View::doLater([this]{
                    ImGui::OpenPopup("About");
                    this->m_aboutWindowOpen = true;
                });
            ImGui::Separator();
            if (ImGui::MenuItem("Cheat Sheet", ""))
                View::doLater([this]{
                    ImGui::OpenPopup("Cheat Sheet");
                    this->m_patternHelpWindowOpen = true;
                });
            ImGui::EndMenu();
        }
    }

}