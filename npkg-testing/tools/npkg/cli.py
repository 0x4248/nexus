import argparse

from .commands import (
    cmd_build,
    cmd_install,
    cmd_install_prebuilt,
    cmd_installed,
    cmd_list,
    cmd_uninstall,
)
from .console import fail


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="npkg",
        description="Nexus package helper",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Examples:\n"
            "  npkg list\n"
            "  npkg installed\n"
            "  npkg build hello-world\n"
            "  npkg install hello-world\n"
            "  npkg install-prebuilt --file ./npkg-build/packages/hello-world-0.1.0.tar.gz\n"
            "  npkg uninstall hello-world"
        ),
    )

    sub = parser.add_subparsers(dest="command", required=True)

    list_parser = sub.add_parser("list", help="List available packages")
    list_parser.set_defaults(func=cmd_list)

    installed_parser = sub.add_parser("installed", help="List installed packages from manifest database")
    installed_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    installed_parser.set_defaults(func=cmd_installed)

    build_parser = sub.add_parser("build", help="Build a package")
    build_parser.add_argument("package", help="Package name or package directory path")
    build_parser.set_defaults(func=cmd_build)

    install_parser = sub.add_parser("install", help="Install a package")
    install_parser.add_argument("package", help="Package name or package directory path")
    install_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    install_parser.add_argument(
        "--prefix",
        default="/",
        help="Prefix passed to package stage command (default: /)",
    )
    install_parser.set_defaults(func=cmd_install)

    install_prebuilt_parser = sub.add_parser("install-prebuilt", help="Install from a prebuilt tarball")
    install_prebuilt_parser.add_argument(
        "--file",
        required=True,
        help="Path to .tar.gz or .tgz package archive",
    )
    install_prebuilt_parser.add_argument(
        "--package",
        help="Package name override (defaults to name inferred from archive filename)",
    )
    install_prebuilt_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    install_prebuilt_parser.set_defaults(func=cmd_install_prebuilt)

    uninstall_parser = sub.add_parser("uninstall", help="Uninstall a package")
    uninstall_parser.add_argument("package", help="Package name")
    uninstall_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    uninstall_parser.set_defaults(func=cmd_uninstall)

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        return int(args.func(args))
    except ValueError as error:
        fail(str(error))
        return 2
