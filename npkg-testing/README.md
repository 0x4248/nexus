# NPKG Testing Workspace

Flat, dev-first workspace for package migration experiments.

The package CLI is `./npkg`, with package build artifacts in `./npkg-build/`.
Implementation modules live in `tools/npkg/*.py`.
Code and package folders live directly here so development stays simple.

## Category Layout

- `bin/` - user-facing binaries
- `sbin/` - admin/service binaries
- `toolkits/` - toolkit code and utilities
- `lib/public/` - public/installable libraries
- `lib/private/` - personal/internal libraries
- `lab/` - unstable prototypes
- `systems/` - system/platform-specific things (configs, patches, scripts)

## Demo Package

`bin/hello_world` builds a C binary and packages it as `/usr/bin/hello-world`.

## Commands

From repository root:

```bash
./npkg list
./npkg installed
./npkg build hello-world
./npkg package hello-world
./npkg install hello-world
./npkg clean hello-world
./npkg uninstall hello-world
```

`build` compiles package sources only.
`package` creates `npkg-build/packages/<name>-<version>.tar.gz`.

Default install root is `/opt/npkg`, so installs land under `/opt/npkg/usr/...`.
If `/opt/npkg` is not writable for your user, run install/uninstall with `sudo`.

To run as `npkg ...` directly, symlink once:

```bash
mkdir -p ~/.local/bin
ln -sf "$PWD/npkg" ~/.local/bin/npkg
```
