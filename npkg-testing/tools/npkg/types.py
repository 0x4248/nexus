from dataclasses import dataclass
from pathlib import Path
from typing import Optional


@dataclass
class Package:
    name: str
    version: str
    description: str
    package_dir: Path
    build_command: Optional[str]
    stage_command: Optional[str]
    installable: bool
