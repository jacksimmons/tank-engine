require("command")
local os_type = require("os_type")


function LibDirWithPostCopy(dir, dest)
    libdirs { dir }
    PostCopyDir(dir, dest)
end


-- Adds GLFW's libdir, and copies the directory after building.
function LibDirGLFWPostCopy(action, dest)
	LibDirWithPostCopy("lib/glfw/lib-" .. action, dest)
end


-- Links OpenGL.
function LinkOpenGL()
    filter { "system:windows" }
		links { "OpenGL32" }
	filter { "system:not windows" }
		links { "GL" }
	filter {} -- Reset the filter to prevent side effects
end

-- Links GLFW.
function LinkGLFW()
	links { "glfw3" }
end

-- Links Lua.
function LinkLua()
	links { "Lua" }
end

function LinkMiniaudio()
	links { "miniaudio" }
end

-- Links Assimp, and adds a postbuildcommand to copy after building.
function LinkAssimpPostCopy(srcdir, destdir)
	local assimpFile

	if os_type.windows() == true then
		assimpFile = "assimp-vc143-mt"
	else
		assimpFile = "libassimp5"
	end

	PostCopyFile(assimpFile, srcdir, destdir, false, true)
	links { assimpFile }
end