import os
from pathlib import Path
from typing import Any, Dict, Optional

try:
    import tomllib
except ModuleNotFoundError:  # pragma: no cover
    tomllib = None

from .paths import workspace_root
from .types import Package


def normalize_command(raw: Optional[str]) -> Optional[str]:
    if raw is None:
        return None
    command = raw.strip()
    return command or None


def as_dict(value: Any) -> Dict[str, Any]:
    return value if isinstance(value, dict) else {}


def load_raw_package(meta_path: Path) -> Dict[str, Any]:
    if tomllib is None:
        raise ValueError("TOML metadata requires Python 3.11+ (tomllib not available)")
    with meta_path.open("rb") as handle:
        parsed = tomllib.load(handle)
    if not isinstance(parsed, dict):
        raise ValueError(f"Invalid metadata in {meta_path}: expected top-level table")
    return parsed


def load_package(meta_path: Path) -> Package:
    data = load_raw_package(meta_path)

    default_name = meta_path.parent.name.replace("_", "-")
    name = str(data.get("name") or default_name).strip()
    version = str(data.get("version") or "0.1.0").strip()
    description = str(data.get("description") or "").strip()
    install_path = str(data.get("install_path") or "/usr/bin/").strip()

    build = as_dict(data.get("build", {}))
    package_section = as_dict(data.get("package", {}))
    clean = as_dict(data.get("clean", {}))

    if not name or not version:
        raise ValueError(f"Invalid metadata in {meta_path}: missing name/version")

    if install_path and not install_path.startswith("/"):
        install_path = f"/{install_path}"

    return Package(
        name=name,
        version=version,
        description=description,
        package_dir=meta_path.parent,
        install_path=install_path,
        build_command=normalize_command(build.get("command")),
        package_command=normalize_command(package_section.get("command")),
        clean_command=normalize_command(clean.get("command")),
    )


def metadata_file_in_dir(package_dir: Path) -> Optional[Path]:
    meta = package_dir / "npkg.conf"
    return meta if meta.exists() else None


def discover_packages() -> Dict[str, Package]:
    root = workspace_root()
    packages: Dict[str, Package] = {}
    search_roots = [
        root / "bin",
        root / "sbin",
        root / "toolkits",
        root / "lib" / "public",
        root / "lib" / "private",
        root / "lab",
        root / "systems",
    ]

    for top in search_roots:
        if not top.exists():
            continue
        for dirpath, _, filenames in os.walk(top):
            if "npkg.conf" not in set(filenames):
                continue
            package_dir = Path(dirpath)
            meta = metadata_file_in_dir(package_dir)
            if meta is None:
                continue
            package = load_package(meta)
            if package.name in packages:
                first = packages[package.name].package_dir
                raise ValueError(
                    f"Duplicate package name '{package.name}' in {first} and {package.package_dir}"
                )
            packages[package.name] = package

    return dict(sorted(packages.items(), key=lambda item: item[0]))


def select_package(packages: Dict[str, Package], selector: str) -> Package:
    if selector in packages:
        return packages[selector]

    root = workspace_root()
    normalized = selector.strip().strip("/")
    if normalized:
        selector_path = (root / normalized).resolve()
        for pkg in packages.values():
            if pkg.package_dir.resolve() == selector_path:
                return pkg

    raise KeyError(selector)
