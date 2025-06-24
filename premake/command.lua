-- Adds a postbuildcommand to make directory `dest`.
function PostMakeDirectory(dest)
	postbuildcommands {
		"{MKDIR} " .. dest
	}
end


-- Adds a postbuildcommand to copy `src` into `dest`, ensuring `dest` indeed exists.
function PostCopyDir(src, dest)
	PostMakeDirectory(dest)
	postbuildcommands {
		"{COPYDIR} %[" .. src .. "] %[" .. dest .. "]"
	}
end


function PostCopyFile(filename, srcdir, destdir)
	PostMakeDirectory(destdir)

	local extensions
	if os.target() == "windows" then
		extensions = { ".lib", ".dll" }
	end

	for i = 1, #(extensions) do
		local cmd = "{COPYFILE} %[" .. srcdir .. "/" .. filename .. extensions[i] .. "] %[" .. destdir .. "/" .. filename .. extensions[i] .. "]"
		postbuildcommands {
			cmd
		}
	end
end