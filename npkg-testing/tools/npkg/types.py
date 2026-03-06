from dataclasses import dataclass
from pathlib import Path
from typing import Optional


@dataclass
class Package:
    name: str
    version: str
    description: str
    package_dir: Path
    install_path: str
    build_command: Optional[str]
    package_command: Optional[str]
    clean_command: Optional[str]
