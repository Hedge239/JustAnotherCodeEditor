list(APPEND APP_SRC
    "src/setup.cpp"
    "src/common/global.cpp"
    "src/common/logHandeler.cpp"
    "src/common/fileHandeler.cpp"
    "src/common/localesHandeler.cpp"
    "src/common/sessionManager.cpp"
    "src/plugins/pluginLoader.cpp"
    "src/plugins/pluginManager.cpp"
    "src/GUI/themeManager.cpp"
    "src/GUI/windowManager.cpp"
)

set(WINDOWS_SRC "src/platforms/Windows/windows.cpp" "app_ms.rc")
set(LINUX_SRC "src/platforms/Linux/linux.cpp")