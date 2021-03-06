#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "views/view.hpp"

struct GLFWwindow;

namespace hex {

    class Window {
    public:
        Window();
        ~Window();

        void loop();

        template<std::derived_from<View> T, typename ... Args>
        T* addView(Args&& ... args) {
            this->m_views.emplace_back(new T(std::forward<Args>(args)...));

            return static_cast<T*>(this->m_views.back());
        }

    public:
        float m_globalScale = 1.0f, m_fontScale = 1.0f;

    private:
        void frameBegin();
        void frameEnd();

        void initGLFW();
        void initImGui();
        void deinitGLFW();
        void deinitImGui();

        GLFWwindow* m_window;
        std::vector<View*> m_views;
        bool m_fpsVisible = false;

        static inline std::tuple<int, int> s_currShortcut = { -1, -1 };
    };

}