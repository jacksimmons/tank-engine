import os
import typer
from typing_extensions import Annotated, List, Union, Literal


def command(cmd: str):
    print(f"$ > {cmd}")
    os.system(cmd)


def main(
    build_system: Annotated[str, typer.Argument(help="Which build system you want to use")],
    targets: Annotated[List[str], typer.Argument(help="Specify build targets for make")] = ["all"],
    use_premake: Annotated[bool, typer.Option(help="Will run premake build script beforehand")] = True,
    use_bear: Annotated[bool, typer.Option(help="Will add bear as a prefix to the make command")] = True,
    gmake_fix_precompiled_header_paths: Annotated[bool, typer.Option(help="Will replace incorrectly resolved pch paths leading to Builds folder, with an absolute path leading to the original pch")] = True,
    run: Annotated[bool, typer.Option(help="Will execute the built program afterwards")] = False
):
    if use_premake:
        if os.name == "nt":
            command(fr".\premake\premake5 {build_system}")
        else:
            command(f"premake/premake5 {build_system}")

    if build_system == "gmake":
        # Make NFD
        command(f"make -C include/nativefiledialog/build/gmake_linux nfd config=debug_x64")
        command(f"{"bear --append -- " if use_bear else ""} make {str.join("", [f"{t} " for t in targets])}")

        if gmake_fix_precompiled_header_paths:
            command(r"""perl -i -0pe 's|("-include",\s*")[^"]*tepch\.hpp"|$1'"$(pwd)"'/TankEngine/tepch.hpp"|g' compile_commands.json""")

    if run:
        if os.name == "nt":
            command(r".\Builds\bin\TankEditor\debug\TankEditor")
        else:
            command("./Builds/bin/TankEditor/debug/TankEditor")
    
if __name__ == "__main__":
    typer.run(main)
