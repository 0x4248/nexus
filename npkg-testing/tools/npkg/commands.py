from pathlib import Path
import fnmatch

from .archive import command_context, ensure_package_archive, extract_archive_into_root, render_command, run_shell
from .console import fail, info, ok, style, warn
from .install_db import load_installed_rows, uninstall_from_manifest, write_install_manifest
from .metadata import discover_packages, select_package
from .paths import workspace_root


def resolve_package_selectors(packages, selectors):
    root = workspace_root()
    matched: list = []
    seen = set()
    had_selector_miss = False

    for selector in selectors:
        selector = selector.strip()
        if not selector:
            continue

        wildcard = any(token in selector for token in "*?[")
        local_matches = []

        if wildcard:
            for pkg in packages.values():
                rel_dir = str(pkg.package_dir.relative_to(root))
                if fnmatch.fnmatch(pkg.name, selector) or fnmatch.fnmatch(rel_dir, selector):
                    local_matches.append(pkg)
        else:
            try:
                local_matches.append(select_package(packages, selector))
            except KeyError:
                normalized_selector = selector.strip().strip("/")
                for pkg in packages.values():
                    rel_dir = str(pkg.package_dir.relative_to(root))
                    if rel_dir == normalized_selector or rel_dir.startswith(f"{normalized_selector}/"):
                        local_matches.append(pkg)

        if not local_matches:
            normalized_selector = selector.strip().strip("/")
            selector_exists = (root / normalized_selector).exists() if normalized_selector else False
            if wildcard or not selector_exists:
                warn(f"no packages matched selector: {selector}")
                had_selector_miss = True
            continue

        for pkg in local_matches:
            if pkg.name in seen:
                continue
            seen.add(pkg.name)
            matched.append(pkg)

    return matched, had_selector_miss


def cmd_list(_args) -> int:
    packages = discover_packages()
    print(style("NPKG packages", bold=True))
    if not packages:
        warn("no packages found")
        return 0

    for pkg in packages.values():
        flags = []
        if pkg.build_command:
            flags.append("build")
        if pkg.package_command:
            flags.append("install")
        if pkg.clean_command:
            flags.append("clean")
        if not flags:
            flags.append("meta-only")

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
    matched, had_selector_miss = resolve_package_selectors(packages, args.packages)
    if not matched:
        return 2

    had_failures = False
    for pkg in matched:
        if not pkg.build_command:
            warn(f"skipping {pkg.name}: build.command is not defined")
            had_failures = True
            continue

        command = render_command(pkg.build_command, command_context(pkg))
        info(f"building {pkg.name} with: {command}")
        try:
            run_shell(command, cwd=workspace_root())
        except RuntimeError as error:
            warn(f"failed to build {pkg.name}: {error}")
            had_failures = True
            continue

        ok(f"built {pkg.name}")

    return 1 if (had_failures or had_selector_miss) else 0


def cmd_package(args) -> int:
    packages = discover_packages()
    matched, had_selector_miss = resolve_package_selectors(packages, args.packages)
    if not matched:
        return 2

    had_failures = False
    for pkg in matched:
        try:
            archive_path = ensure_package_archive(pkg)
        except RuntimeError as error:
            warn(f"failed to package {pkg.name}: {error}")
            had_failures = True
            continue

        ok(f"package archive ready: {archive_path}")

    return 1 if (had_failures or had_selector_miss) else 0


def cmd_install(args) -> int:
    packages = discover_packages()
    try:
        pkg = select_package(packages, args.package)
    except KeyError:
        fail(f"package not found: {args.package}")
        return 2

    install_root = Path(args.root).expanduser().resolve()
    try:
        archive_path = ensure_package_archive(pkg)
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


def cmd_clean(args) -> int:
    packages = discover_packages()
    root = workspace_root()
    matched, had_selector_miss = resolve_package_selectors(packages, args.packages)
    if not matched:
        return 2

    had_failures = False
    for pkg in matched:
        if not pkg.clean_command:
            warn(f"skipping {pkg.name}: clean.command is not defined")
            had_failures = True
            continue

        command = render_command(pkg.clean_command, command_context(pkg))
        info(f"cleaning {pkg.name} with: {command}")
        try:
            run_shell(command, cwd=root)
        except RuntimeError as error:
            warn(f"failed to clean {pkg.name}: {error}")
            had_failures = True
            continue

        ok(f"cleaned {pkg.name}")

    return 1 if (had_failures or had_selector_miss) else 0


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
