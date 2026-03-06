from pathlib import Path
from typing import List

from .types import Package


def workspace_root() -> Path:
    return Path(__file__).resolve().parents[2]


def npkg_build_root() -> Path:
    return workspace_root() / "npkg-build"


def out_root() -> Path:
    return npkg_build_root()


def package_stage_dir(pkg: Package) -> Path:
    return out_root() / "work" / pkg.name / "stage"


def package_archive_path(pkg: Package) -> Path:
    return out_root() / "packages" / f"{pkg.name}-{pkg.version}.tar.gz"


def manifest_root(install_root: Path) -> Path:
    return install_root / ".npkg-db"


def package_manifest_path(pkg_name: str, install_root: Path) -> Path:
    return manifest_root(install_root) / f"{pkg_name}.json"


def list_manifest_paths(install_root: Path) -> List[Path]:
    db_dir = manifest_root(install_root)
    if not db_dir.exists() or not db_dir.is_dir():
        return []
    return sorted(db_dir.glob("*.json"))
