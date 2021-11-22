#ImGui

set(IMGUI_SOURCE_FILES
	imgui_draw.cpp
	imgui_widgets.cpp
	imgui_impl_opengl3.cpp
	imgui_impl_sfml.cpp
	imgui.cpp
)

set(IMGUI_INCLUDE_FILES
	"include/imconfig.h"
	"include/imgui_internal.h"
	"include/imgui.h"
	"include/imstb_rectpack.h"
	"include/imstb_textedit.h"
	"include/imstb_truetype.h"
	"include/imgui_impl_sfml.h"
	"include/imgui_impl_opengl3.h"
)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
target_include_directories(imgui PUBLIC include)
target_include_directories(imgui PUBLIC ../SFML-2.5.1/include)
target_include_directories(imgui PUBLIC ../glew-2.1.0/include)
target_link_libraries(imgui sfml-window sfml-system)