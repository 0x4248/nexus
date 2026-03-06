# NPKG tools

Package tooling lives here.

Implementation modules are in `tools/npkg/*.py` and loaded by the root `./npkg` entrypoint.

- `../npkg` - CLI entrypoint (`list`, `build`, `install`, `uninstall`)
- `work/` - staging work dirs
- `packages/` - built package archives

## Quick start

```bash
./npkg list
./npkg installed
./npkg build hello-world
./npkg install hello-world
./npkg install-prebuilt --file ./npkg-build/packages/hello-world-0.1.0.tar.gz
./npkg uninstall hello-world
```

Default install root is `/opt/npkg` and default prefix is `/` (so binaries land under `/opt/npkg/bin`, etc).
If `/opt/npkg` is not writable for your user, run install/uninstall with `sudo`.

## Package metadata

Packages are discovered by scanning these directories for one metadata file per package directory:

- `npkg.conf` (preferred)
- `npkg.ini`
- `npkg.toml` (compatibility)
- `npkg.json` (legacy compatibility)

If more than one metadata file exists in a package directory, `npkg` errors to avoid ambiguity.

Scan roots:

- `bin/`
- `sbin/`
- `toolkits/`
- `lib/public/`
- `lib/private/`
- `lab/`
- `systems/`

Minimal `npkg.conf` example:

```toml
name = "hello-world"
version = "0.1.0"
description = "Tiny demo C package"

[build]
command = "make -C {package_dir} all"

[stage]
command = "make -C {package_dir} install DESTDIR={stage_dir} PREFIX={prefix}"

[capabilities]
installable = true
```

INI equivalent:

```ini
[package]
name = hello-world
version = 0.1.0
description = Tiny demo C package

[build]
command = make -C {package_dir} all

[stage]
command = make -C {package_dir} install DESTDIR={stage_dir} PREFIX={prefix}

[capabilities]
installable = true
```

Notes:

- Optional defaults:
  - `name` defaults to folder name (with `_` converted to `-`)
  - `version` defaults to `0.1.0`
  - `description` defaults to empty
  - `capabilities.installable` defaults to `true` if `stage.command` is set, else `false`
- `build.command` remains optional.
- `stage.command` is required only for installable packages.
