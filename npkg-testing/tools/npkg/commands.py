from pathlib import Path

from .archive import command_context, ensure_package_archive, extract_archive_into_root, infer_package_from_archive, render_command, run_shell
from .console import fail, info, ok, style, warn
from .install_db import load_installed_rows, uninstall_from_manifest, write_install_manifest, write_prebuilt_manifest
from .metadata import discover_packages, select_package
from .paths import workspace_root


def cmd_list(_args) -> int:
    packages = discover_packages()
    print(style("NPKG packages", bold=True))
    if not packages:
        warn("no packages found")
        return 0

    for pkg in packages.values():
        flags = []
        flags.append("build" if pkg.build_command else "no-build")
        if pkg.installable and pkg.stage_command:
            flags.append("install")
        elif pkg.installable and not pkg.stage_command:
            flags.append("no-stage")
        else:
            flags.append("build-only")

        rel_dir = pkg.package_dir.relative_to(workspace_root())
        print(
            f"  {style(pkg.name, bold=True)} {pkg.version}  "
            f"[{', '.join(flags)}]  {rel_dir}"
        )
        if pkg.description:
            print(f"    {pkg.description}")

    return 0


def cmd_build(args) -> int:
    packages = discover_packages()
    try:
        pkg = select_package(packages, args.package)
    except KeyError:
        fail(f"package not found: {args.package}")
        return 2

    if not pkg.build_command:
        fail(f"package '{pkg.name}' does not define build.command")
        return 2

    command = render_command(pkg.build_command, command_context(pkg))
    info(f"building {pkg.name} with: {command}")
    try:
        run_shell(command, cwd=workspace_root())
    except RuntimeError as error:
        fail(str(error))
        return 1

    ok(f"built {pkg.name}")
    return 0


def cmd_install(args) -> int:
    packages = discover_packages()
    try:
        pkg = select_package(packages, args.package)
    except KeyError:
        fail(f"package not found: {args.package}")
        return 2

    install_root = Path(args.root).expanduser().resolve()
    prefix = args.prefix

    try:
        archive_path = ensure_package_archive(pkg, prefix=prefix)
    except RuntimeError as error:
        fail(str(error))
        return 2

    install_root.mkdir(parents=True, exist_ok=True)
    info(f"installing {pkg.name} into {install_root}")
    try:
        member_names = extract_archive_into_root(archive_path, install_root)
    except RuntimeError as error:
        fail(str(error))
        return 1

    write_install_manifest(pkg, install_root, member_names)

    ok(f"installed {pkg.name} -> {install_root}")
    return 0


def cmd_install_prebuilt(args) -> int:
    archive_path = Path(args.file).expanduser().resolve()
    if not archive_path.exists() or not archive_path.is_file():
        fail(f"archive not found: {archive_path}")
        return 2

    install_root = Path(args.root).expanduser().resolve()
    install_root.mkdir(parents=True, exist_ok=True)

    inferred_name, inferred_version = infer_package_from_archive(archive_path)
    package_name = (args.package or inferred_name).strip()
    package_version = inferred_version

    if not package_name:
        fail("unable to determine package name; pass --package")
        return 2

    info(f"installing prebuilt {package_name} from {archive_path} into {install_root}")
    try:
        member_names = extract_archive_into_root(archive_path, install_root)
    except RuntimeError as error:
        fail(str(error))
        return 1

    write_prebuilt_manifest(
        package_name=package_name,
        version=package_version,
        install_root=install_root,
        members=member_names,
        source_archive=archive_path,
    )

    ok(f"installed prebuilt {package_name} -> {install_root}")
    return 0


def cmd_uninstall(args) -> int:
    packages = discover_packages()
    pkg_name = args.package
    if pkg_name in packages:
        pkg_name = packages[pkg_name].name
    else:
        try:
            pkg_name = select_package(packages, args.package).name
        except KeyError:
            pkg_name = args.package

    install_root = Path(args.root).expanduser().resolve()
    info(f"uninstalling {pkg_name} from {install_root}")
    try:
        uninstall_from_manifest(pkg_name, install_root)
    except RuntimeError as error:
        fail(str(error))
        return 2

    ok(f"uninstalled {pkg_name} from {install_root}")
    return 0


def cmd_installed(args) -> int:
    install_root = Path(args.root).expanduser().resolve()
    rows = load_installed_rows(install_root)

    print(style(f"Installed packages in {install_root}", bold=True))
    if not rows:
        warn("no installed packages found")
        return 0

    for row in rows:
        if row.get("error"):
            warn(f"skipping invalid manifest {row['manifest'].name}: {row['error']}")
            continue

        print(
            f"  {style(row['package'], bold=True)} {row['version']}  "
            f"({row['path_count']} paths)"
        )

    return 0
