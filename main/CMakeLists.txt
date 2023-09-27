
set(SOURCES main.cpp
            MainWindow.cpp
            FractalWidget.cpp
            FractalImageCreator.cpp
            Mandelbrot.cpp
            ColorPalette.cpp
)
if(WIN32)
    #add_executable(${MAIN} WIN32 ${SOURCES}) # WIN32 prevents console opening
    add_executable(${MAIN} ${SOURCES})
else()
    add_executable(${MAIN} ${SOURCES})
endif()

target_link_libraries(${MAIN} Qt6::Widgets
                  )
