import os
import sys
import typer
from typing_extensions import Annotated, List, Union, Literal


def main(
    build_system: Annotated[Union[Literal["gmake", "vs2022"]], typer.Argument(help="Which build system you want to use")],
    targets: Annotated[List[str], typer.Argument(help="Specify build targets for make")] = ["all"],
    use_premake: Annotated[bool, typer.Option(help="Will run premake build script beforehand")] = True,
    use_bear: Annotated[bool, typer.Option(help="Will add bear as a prefix to the make command")] = True,
    gmake_fix_precompiled_header_paths: Annotated[bool, typer.Option(help="Will replace incorrectly resolved pch paths leading to Builds folder, with an absolute path leading to the original pch")] = True,
    run: Annotated[bool, typer.Option(help="Will execute the built program afterwards")] = False
):
    if use_premake:
        os.system(f"premake/premake5 {build_system}")

    if build_system == "gmake":
        os.system(f"{"bear -- " if use_bear else ""} make {str.join("", [f"{t} " for t in targets])}")

        if gmake_fix_precompiled_header_paths:
            os.system(r"""perl -i -0pe 's|("-include",\s*")[^"]*tepch\.hpp"|$1'"$(pwd)"'/TankEngine/tepch.hpp"|g' compile_commands.json""")

    if run:
        os.system(f"./Builds/bin/TankEditor/debug/TankEditor")
    
if __name__ == "__main__":
    typer.run(main)
