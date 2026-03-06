import configparser
import json
import os
from pathlib import Path
from typing import Any, Dict, Optional

try:
    import tomllib
except ModuleNotFoundError:
    tomllib = None

from .paths import workspace_root
from .types import Package


def normalize_command(raw: Optional[str]) -> Optional[str]:
    if raw is None:
        return None
    command = raw.strip()
    if not command:
        return None
    return command


def as_dict(value: Any) -> Dict[str, Any]:
    return value if isinstance(value, dict) else {}


def parse_bool(value: Any, default: bool) -> bool:
    if value is None:
        return default
    if isinstance(value, bool):
        return value
    if isinstance(value, (int, float)):
        return bool(value)
    raw = str(value).strip().lower()
    if raw in {"1", "true", "yes", "on"}:
        return True
    if raw in {"0", "false", "no", "off"}:
        return False
    return default


def load_json_package(meta_path: Path) -> Dict[str, Any]:
    with meta_path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def load_toml_package(meta_path: Path) -> Dict[str, Any]:
    if tomllib is None:
        raise ValueError("TOML metadata requires Python 3.11+ (tomllib not available)")
    with meta_path.open("rb") as handle:
        parsed = tomllib.load(handle)
    if not isinstance(parsed, dict):
        raise ValueError(f"Invalid metadata in {meta_path}: expected top-level table")
    return parsed


def load_ini_package(meta_path: Path) -> Dict[str, Any]:
    parser = configparser.ConfigParser()
    parser.optionxform = str
    read_ok = parser.read(meta_path, encoding="utf-8")
    if not read_ok:
        raise ValueError(f"Unable to read metadata: {meta_path}")

    package: Dict[str, Any] = {}
    build: Dict[str, Any] = {}
    stage: Dict[str, Any] = {}
    capabilities: Dict[str, Any] = {}

    if parser.has_section("package"):
        section = parser["package"]
        package["name"] = section.get("name")
        package["version"] = section.get("version")
        package["description"] = section.get("description")

    if parser.has_section("build"):
        build["command"] = parser["build"].get("command")

    if parser.has_section("stage"):
        stage["command"] = parser["stage"].get("command")

    if parser.has_section("capabilities"):
        capabilities["installable"] = parser["capabilities"].get("installable")

    package["build"] = build
    package["stage"] = stage
    package["capabilities"] = capabilities
    return package


def load_raw_package(meta_path: Path) -> Dict[str, Any]:
    name = meta_path.name.lower()
    suffix = meta_path.suffix.lower()
    if name == "npkg.conf":
        try:
            data = load_toml_package(meta_path)
        except ValueError:
            data = load_ini_package(meta_path)
    elif suffix == ".json":
        data = load_json_package(meta_path)
    elif suffix == ".toml":
        data = load_toml_package(meta_path)
    elif suffix == ".ini":
        data = load_ini_package(meta_path)
    else:
        raise ValueError(f"Unsupported metadata format: {meta_path.name}")

    if not isinstance(data, dict):
        raise ValueError(f"Invalid metadata in {meta_path}: expected object/table")
    return data


def load_package(meta_path: Path) -> Package:
    data = load_raw_package(meta_path)

    default_name = meta_path.parent.name.replace("_", "-")
    name = str(data.get("name") or default_name).strip()
    version = str(data.get("version") or "0.1.0").strip()
    description = str(data.get("description") or "").strip()
    build = as_dict(data.get("build", {}))
    stage = as_dict(data.get("stage", {}))
    capabilities = as_dict(data.get("capabilities", {}))

    if not name or not version:
        raise ValueError(f"Invalid metadata in {meta_path}: missing name/version")

    build_command = normalize_command(build.get("command"))
    stage_command = normalize_command(stage.get("command"))
    installable_default = stage_command is not None
    installable = parse_bool(capabilities.get("installable"), default=installable_default)

    return Package(
        name=name,
        version=version,
        description=description,
        package_dir=meta_path.parent,
        build_command=build_command,
        stage_command=stage_command,
        installable=installable,
    )


def metadata_file_in_dir(package_dir: Path) -> Optional[Path]:
    candidates = [
        package_dir / "npkg.conf",
        package_dir / "npkg.toml",
        package_dir / "npkg.ini",
        package_dir / "npkg.json",
    ]
    found = [path for path in candidates if path.exists()]
    if len(found) > 1:
        names = ", ".join(path.name for path in found)
        raise ValueError(f"Multiple metadata files in {package_dir}: {names}")
    return found[0] if found else None


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
            filename_set = set(filenames)
            if not ({"npkg.conf", "npkg.toml", "npkg.ini", "npkg.json"} & filename_set):
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
