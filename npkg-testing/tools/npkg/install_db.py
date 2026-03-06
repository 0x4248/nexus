import json
from pathlib import Path
from typing import Any, Dict, List

from .paths import list_manifest_paths, manifest_root, package_manifest_path
from .types import Package


def write_install_manifest(pkg: Package, install_root: Path, members: List[str]) -> None:
    db_dir = manifest_root(install_root)
    db_dir.mkdir(parents=True, exist_ok=True)
    manifest = {
        "package": pkg.name,
        "version": pkg.version,
        "install_root": str(install_root),
        "paths": members,
    }
    manifest_path = package_manifest_path(pkg.name, install_root)
    with manifest_path.open("w", encoding="utf-8") as handle:
        json.dump(manifest, handle, indent=2)


def read_install_manifest(pkg_name: str, install_root: Path) -> Dict[str, Any]:
    manifest_path = package_manifest_path(pkg_name, install_root)
    if not manifest_path.exists():
        raise RuntimeError(f"package '{pkg_name}' is not installed in {install_root}")
    with manifest_path.open("r", encoding="utf-8") as handle:
        data = json.load(handle)
    if not isinstance(data, dict):
        raise RuntimeError(f"invalid manifest for package '{pkg_name}'")
    return data


def uninstall_from_manifest(pkg_name: str, install_root: Path) -> None:
    data = read_install_manifest(pkg_name, install_root)
    raw_paths = data.get("paths", [])
    if not isinstance(raw_paths, list):
        raise RuntimeError(f"invalid manifest path list for package '{pkg_name}'")

    unique_paths = []
    seen = set()
    for item in raw_paths:
        if not isinstance(item, str):
            continue
        normalized = item.strip().lstrip("/")
        if not normalized or normalized in seen:
            continue
        seen.add(normalized)
        unique_paths.append(normalized)

    for rel_path in sorted(unique_paths, key=lambda value: (value.count("/"), len(value)), reverse=True):
        target = install_root / rel_path
        if target.is_symlink() or target.is_file():
            target.unlink(missing_ok=True)
        elif target.is_dir():
            try:
                target.rmdir()
            except OSError:
                pass

    for rel_path in sorted(unique_paths, key=lambda value: value.count("/"), reverse=True):
        current = (install_root / rel_path).parent
        while current != install_root and current.exists():
            try:
                current.rmdir()
            except OSError:
                break
            current = current.parent

    manifest_path = package_manifest_path(pkg_name, install_root)
    manifest_path.unlink(missing_ok=True)

    db_dir = manifest_root(install_root)
    try:
        db_dir.rmdir()
    except OSError:
        pass


def load_installed_rows(install_root: Path) -> List[Dict[str, Any]]:
    rows: List[Dict[str, Any]] = []
    for manifest_path in list_manifest_paths(install_root):
        try:
            with manifest_path.open("r", encoding="utf-8") as handle:
                data = json.load(handle)
        except (OSError, json.JSONDecodeError) as error:
            rows.append(
                {
                    "manifest": manifest_path,
                    "error": str(error),
                }
            )
            continue

        rows.append(
            {
                "manifest": manifest_path,
                "package": str(data.get("package") or manifest_path.stem),
                "version": str(data.get("version") or "unknown"),
                "path_count": len(data.get("paths", [])) if isinstance(data.get("paths", []), list) else 0,
                "error": None,
            }
        )

    return rows
