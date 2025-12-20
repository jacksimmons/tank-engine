require("command")
local os_type = require("os_type")


-- Adds a libdir which premake will search through for each "links {}" operation.
-- On Windows, copies the libdir into target's bindir.
function LibDirWithPostCopy(dir, dest)
    libdirs { dir }

	-- Only need to copy DLL/LIBs into exe folder on Windows.
	-- Linux bakes the path of the shared libs into the executable file.
	if os_type.windows() then
	    PostCopyDir(dir, dest)
	end
end


-- Adds GLFW's libdir, and copies the directory after building.
-- Windows exclusive.
function LibDirGLFWPostCopy(action, dest)
	if os_type.windows() then
		LibDirWithPostCopy("lib/glfw/" .. action, dest)
	end
end


-- Links OpenGL.
function LinkOpenGL()
    filter { "system:windows" }
		links { "OpenGL32" }
	filter { "system:not windows" }
		links { "GL" }
	filter {} -- Reset the filter to prevent side effects
end

function LinkGLFW()
	links { "glfw3" }
end

function LinkLua()
	links { "Lua" }
end

function LinkMiniaudio()
	links { "miniaudio" }
end

-- Links Assimp, copies library on Windows.
function LinkAssimp(srcdir, destdir)
	local assimpFile

	if os_type.windows() == true then
		assimpFile = "assimp-vc143-mt"
		links { assimpFile }
		PostCopyFile(assimpFile, srcdir, destdir, false, true)
	else
		assimpFile = "libassimp5"
		links { "assimp5" }
	end
end