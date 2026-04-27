Title: UNIX Configuration Archiving Structure
Publisher: 0x4248
Design Proposal Exchange: 25-234545
Category: Design and Protocol
Version: 1.0
Copyrights: © 2026 0x4248. CC0. All rights are given
Usage rights: All rights given
License: CC0
                                                                          0x4248
                                                                  8 January 2026
                                                        Design Proposal Exchange
                UNIX Configuration Archiving Structure
                            DPX25-234545

A design for sharing and archiving UNIX based system configuration such as 
dotfiles, application settings, and system preferences in a standardized
structure.

BRIEF

When archiving and sharing UNIX configuration files that is system wide it can 
be confusing to know what requirements are needed or where a folder may belong.

This design proposal aims to create a standardized structure for archiving and
sharing UNIX configuration files to make it easier for users to backup, restore,
and share their system settings.

This DPX also applies to Linux and other UNIX-like operating systems.

SPECIFICATION

There are three main components to a repository that follows this design:
1. Filesystem and System Files
2. User Configuration Files
3. Documentation, Metadata, and Install Scripts


1. Filesystem and System Files

In the repository root, there should be a folder named either:
- fs (preferred)
- rootfs
- root

This folder is basically the root (/) of the UNIX system. It does not however
need to contain all the system files (and doing so is significantly discouraged).
Only include configuration files that are relevant to the system setup and
configuration. For example if i want to share my NTP configuration, I would
include /etc/ntp.conf in this folder structure. This file would be located at
<repository-root>/fs/etc/ntp.conf.

2. User Configuration Files

User specific configuration files such as dotfiles and application settings
should be placed in a folder named either:
- home (preferred)
- user

Now why? Why not put it in /home/username? The reason is simple, this way it
is user agnostic and can be used by any user.

Now Jane, Mike and Alice can all use the same repository to setup their user
configuration without needing to change anything or even type in their username,
since ~ is a standard symlink to the home directory of the current user.

3. Documentation, Metadata, and Install Scripts

In the repository root, there should be these main files:
- README.txt OR README.md : A documentation file that explains the configuration
  files included, any special instructions, and any other relevant information.
- INSTALL.sh OR INSTALL.md : An optional installation script or instructions
  for applying the configuration files to a system.
- LICENSE : A file that outlines the usage rights and license for the repository.
- REQUIREMENTS.txt : An optional file that lists any dependencies or prerequisites
  needed for the configuration files to work properly.
- METADATA.json OR METADATA.txt : An optional metadata file that includes
  information about the repository such as author, version, date created,
  and any other relevant details. However this is optional and not required.

