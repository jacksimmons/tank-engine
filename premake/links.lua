require "premake.command"


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


-- Links Assimp, and adds a postbuildcommand to copy after building.
function LinkAssimpPostCopy(srcdir, destdir)
	local assimpFile
	if os.target() == "windows" then
		assimpFile = "assimp-vc143-mt"
	else
		assimpFile = "assimp5"
	end
	
	PostCopyFile(assimpFile, srcdir, destdir)
	links { assimpFile }
end


-- Links freetype, and adds a postbuildcommand to copy after building.
function LinkFreetypePostCopy(srcdir, destdir)
	local freetypeFile
	if os.target() == "windows" then
		freetypeFile = "freetype"
	else
		freetypeFile = "TODO"

	PostCopyFile(freetypeFile, srcdir, destdir)
	links { "freetype" }
	end
end