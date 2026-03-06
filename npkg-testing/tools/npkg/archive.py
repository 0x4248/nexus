import shutil
import subprocess
import tarfile
from pathlib import Path
from typing import Dict, List, Optional

from .console import info, ok
from .paths import package_archive_path, package_stage_dir, workspace_root
from .types import Package


def command_context(pkg: Package, stage_dir: Optional[Path] = None) -> Dict[str, str]:
    context = {
        "workspace_root": str(workspace_root()),
        "npkg_root": str(workspace_root()),
        "npkg_build_root": str(workspace_root() / "npkg-build"),
        "package_dir": str(pkg.package_dir),
        "install_path": pkg.install_path,
    }
    if stage_dir is not None:
        context["stage_dir"] = str(stage_dir)
    return context


def render_command(template: str, context: Dict[str, str]) -> str:
    return template.format(**context)


def run_shell(command: str, cwd: Path) -> None:
    result = subprocess.run(command, cwd=str(cwd), shell=True)
    if result.returncode != 0:
        raise RuntimeError(f"Command failed with exit code {result.returncode}: {command}")


def ensure_package_archive(pkg: Package) -> Path:
    if not pkg.package_command:
        raise RuntimeError(f"Package '{pkg.name}' does not define package.command")

    stage_dir = package_stage_dir(pkg)
    archive_path = package_archive_path(pkg)

    shutil.rmtree(stage_dir, ignore_errors=True)
    stage_dir.mkdir(parents=True, exist_ok=True)

    context = command_context(pkg, stage_dir=stage_dir)
    package_command = render_command(pkg.package_command, context)

    info(f"staging {pkg.name} with: {package_command}")
    run_shell(package_command, cwd=workspace_root())

    archive_path.parent.mkdir(parents=True, exist_ok=True)
    with tarfile.open(archive_path, "w:gz") as tar:
        for item in sorted(stage_dir.iterdir()):
            tar.add(item, arcname=item.name)

    ok(f"packaged {pkg.name} -> {archive_path}")
    return archive_path


def extract_archive_into_root(archive_path: Path, install_root: Path) -> List[str]:
    try:
        with tarfile.open(archive_path, "r:gz") as tar:
            member_names = [member.name for member in tar.getmembers() if member.name and member.name != "."]
            try:
                tar.extractall(path=install_root, filter="data")
            except TypeError:
                tar.extractall(path=install_root)
    except (tarfile.TarError, OSError) as error:
        raise RuntimeError(f"failed to install archive: {error}") from error
    return member_names
