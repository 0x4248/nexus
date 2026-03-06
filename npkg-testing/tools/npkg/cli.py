import argparse

from .commands import (
    cmd_build,
    cmd_clean,
    cmd_install,
    cmd_installed,
    cmd_list,
    cmd_package,
    cmd_uninstall,
)
from .console import fail


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="npkg",
        description="Simple package helper for the Nexus monorepo",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Examples:\n"
            "  npkg list\n"
            "  npkg build hello-world\n"
            "  npkg build '*'\n"
            "  npkg package hello-world\n"
            "  npkg install hello-world\n"
            "  npkg clean '*'\n"
            "  npkg installed\n"
            "  npkg uninstall hello-world"
        ),
    )

    sub = parser.add_subparsers(dest="command", required=True)

    list_parser = sub.add_parser("list", help="List available packages")
    list_parser.set_defaults(func=cmd_list)

    build_parser = sub.add_parser("build", help="Build package(s) (supports wildcards)")
    build_parser.add_argument(
        "packages",
        nargs="+",
        help="Package selectors (name/path or wildcards like '*', 'hello-*', 'bin/*')",
    )
    build_parser.set_defaults(func=cmd_build)

    package_parser = sub.add_parser("package", help="Create package archive(s) (.tar.gz, supports wildcards)")
    package_parser.add_argument(
        "packages",
        nargs="+",
        help="Package selectors (name/path or wildcards like '*', 'hello-*', 'bin/*')",
    )
    package_parser.set_defaults(func=cmd_package)

    install_parser = sub.add_parser("install", help="Install a package")
    install_parser.add_argument("package", help="Package name or package directory path")
    install_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    install_parser.set_defaults(func=cmd_install)

    clean_parser = sub.add_parser("clean", help="Run package clean command (supports wildcards)")
    clean_parser.add_argument(
        "packages",
        nargs="+",
        help="Package selectors (name/path or wildcards like '*', 'hello-*', 'bin/*')",
    )
    clean_parser.set_defaults(func=cmd_clean)

    installed_parser = sub.add_parser("installed", help="List installed packages from manifest database")
    installed_parser.add_argument(
        "--root",
        default="/opt/npkg",
        help="Installation root directory (default: /opt/npkg)",
    )
    installed_parser.set_defaults(func=cmd_installed)

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
